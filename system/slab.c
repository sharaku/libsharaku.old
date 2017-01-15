/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#include <stdlib.h>
#include <errno.h>
#include <sharaku/slab.h>

#define SHARKAU_SLAB_MAGIC	0xF324ABE3
// メモリバッファのヘッダ。
// 利用者からは参照できない領域
typedef struct smem_header {
	uint32_t			h_magic;
	uint32_t			h_line;
	const char			*h_src;
	struct sharaku_slab_node	*h_node;
	struct list_head		h_list;
} smem_header_t;

// メモリバッファのフッタ。
// 利用者からは参照できない領域
typedef struct sharaku_mem_footer {
	uint32_t			f_magic;
} smem_footer_t;

// slab獲得の優先度を計算する。
static inline int64_t
__sharaku_get_slab_prio(struct sharaku_slab_node *node)
{
	return (node->sn_max_cnt - node->sn_alloc_cnt)
			 * SHARAKU_SLAB_PRIO / node->sn_max_cnt;
}

// ヘッダからフッタを取得する
static inline smem_footer_t*
__sharaku_slab_h2f(smem_header_t *h)
{
	return (smem_footer_t*)
			((char *)(h)
				 + h->h_node->sn_slab->s_size
				 + sizeof(smem_header_t));
}

// ヘッダからバッファを取得する
static inline void*
__sharaku_slab_h2b(smem_header_t *h)
{
	return (void*)((char *)(h) + sizeof(smem_header_t));
}

// バッファからヘッダを取得する
static inline smem_header_t*
__sharaku_slab_b2h(void *buf)
{
	return (smem_header_t *)(((void *)buf) - sizeof(smem_header_t));
}

// slab獲得の優先度キューの再登録を行う。
static inline void
__sharaku_slab_resched(struct sharaku_slab *slab,
			struct sharaku_slab_node *node)
{
	int64_t	prio;

	plist_del(node, &slab->s_list);
	// 空きがある場合は空きリストに入れる
	// fullの場合はfullリストに入れる
	// リストは同一優先度の末端に挿入されるため、抜き差しが頻発する
	// 可能性は少ない。
	prio = __sharaku_get_slab_prio(node);
	node->sn_plist.prio = prio;
	if (prio) {
		plist_add(node, &slab->s_list);
	} else {
		plist_add(node, &slab->s_flist);
	}
}

// nodeからメモリを獲得する
static inline void *
__sharaku_slab_alloc(struct sharaku_slab_node *node,
			const char *src, uint32_t line)
{
	smem_header_t *h;
	smem_footer_t *f;

	h = list_first_entry_or_null(&node->sn_flist, smem_header_t, h_list);
	if (!h) {
		return NULL;
	}
	list_del_init(&h->h_list);
	list_add_tail(&h->h_list, &node->sn_alist);
	h->h_magic = SHARKAU_SLAB_MAGIC;
	h->h_src = src;
	h->h_line = line;
	h->h_node = node;

	f = __sharaku_slab_h2f(h);
	f->f_magic = SHARKAU_SLAB_MAGIC;

	node->sn_alloc_cnt++;
	return __sharaku_slab_h2b(h);
}

// nodへメモリを返却する
static inline int
__sharaku_slab_free_nochk(smem_header_t *h, struct sharaku_slab_node *node)
{
	list_del_init(&h->h_list);
	list_add_tail(&h->h_list, &node->sn_flist);
	node->sn_alloc_cnt--;

	return 0;
}

// nodeへメモリを返却する
static inline int
__sharaku_slab_free(void *buf)
{
	struct sharaku_slab_node *node;
	smem_header_t *h;
	smem_footer_t *f;

	h = __sharaku_slab_b2h(buf);
	f = __sharaku_slab_h2f(h);
	node = h->h_node;

	if (h->h_magic != SHARKAU_SLAB_MAGIC ||
	    f->f_magic != SHARKAU_SLAB_MAGIC) {
		// 不正アクセス。
		return -EFAULT;
	}

	__sharaku_slab_free_nochk(h, node);
	return 0;
}

static inline void
__sharaku_slab_node_alloc(struct sharaku_slab *slab)
{
	struct sharaku_slab_node *node;
	smem_header_t *h;
	char *buf;
	int buf_sz;
	int i;

	buf_sz = slab->s_size + sizeof(smem_header_t) + sizeof(smem_footer_t);
	node = (struct sharaku_slab_node *)malloc(SHARAKU_SLAB_SZ);

	INIT_PLIST_NODE(&node->sn_plist, 0);
	INIT_LIST_HEAD(&node->sn_alist);
	INIT_LIST_HEAD(&node->sn_flist);
	node->sn_max_cnt
		 = (SHARAKU_SLAB_SZ - sizeof(struct sharaku_slab_node))
		 						 / buf_sz;
	node->sn_slab = slab;

	// 最初にすべての領域を獲得しているものとして初期化。
	// その後、全領域をfreeすることでリストにつなげつつallocカウンタを
	// 適切に集計する。
	node->sn_alloc_cnt	= node->sn_max_cnt;
	buf = (char *)(node + 1);
	for (i = 0; i < node->sn_max_cnt; i++) {
		h = (smem_header_t *)buf;
		INIT_LIST_HEAD(&h->h_list);
		__sharaku_slab_free_nochk(h, node);
		buf += buf_sz;
	}
	__sharaku_slab_resched(slab, node);

	slab->s_node_cnt++;
}

// node1枚を開放する。
// ただし、1つでもallocされている場合は開放しない。
static int
__sharaku_slab_node_free(struct sharaku_slab_node *node)
{
	struct sharaku_slab *slab;

	// allocされている場合は開放してはならない。
	// -EBUSYを応答する。
	if (node->sn_alloc_cnt) {
		return -EBUSY;
	}
	slab = node->sn_slab;
	plist_del(node, &slab->s_list);
	list_del_init(&node->sn_plist);
	free(node);

	slab->s_node_cnt--;
	return 0;
}

// slabからメモリを獲得する。
// 空きがない場合は新しいslabを獲得する。
void*
_sharaku_salloc(struct sharaku_slab *slab,
		   const char *src, uint32_t line)
{
	struct sharaku_slab_node *node;
	char *buf;
	int prio = -1;

	if (plist_empty(&slab->s_list)) {
		__sharaku_slab_node_alloc(slab);
	}

	node = list_entry(slab->s_list.node_list.next,
				struct sharaku_slab_node,
				sn_plist.node_list);
	if (node) {
		prio = __sharaku_get_slab_prio(node);
		buf = __sharaku_slab_alloc(node, src, line);
	} else {
		// このルートを通ることはない。もし通過する場合バグである。
	}
	// もしslabの獲得により優先度に変化が発生した時は、nodeを入れなおす。
	if (prio != __sharaku_get_slab_prio(node)) {
		__sharaku_slab_resched(slab, node);
	}
	return buf;
}

int
sharaku_sfree(void *buf)
{
	struct sharaku_slab_node *node;
	smem_header_t *h;
	int prio;
	int rc;

	h = __sharaku_slab_b2h(buf);
	node = h->h_node;
	prio = __sharaku_get_slab_prio(node);
	rc = __sharaku_slab_free(buf);
	if (rc) {
		return rc;
	}
	// もしslabの獲得により優先度に変化が発生した時は、nodeを入れなおす。
	if (prio != __sharaku_get_slab_prio(node)) {
		__sharaku_slab_resched(node->sn_slab, node);
	}
	return 0;
}

struct sharaku_slab*
sharaku_slab_create(size_t size, size_t node_size, uint32_t max_cnt)
{
	struct sharaku_slab *slab;
	int buf_sz;

	slab = (struct sharaku_slab *)malloc(sizeof(struct sharaku_slab));
	INIT_SLAB(slab, size, node_size, max_cnt);
	return slab;
}

int
sharaku_slab_destroy(struct sharaku_slab *slab)
{
	if (slab->s_node_cnt) {
		return -EBUSY;
	}
	free(slab);
	return 0;
}

