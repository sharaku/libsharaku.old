/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_SLAB_H
#define _SHARAKU_SLAB_H

#include <sharaku/list.h>
#include <sharaku/plist.h>

// SLABを使用してメモリを確保する。
// 確保するメモリの前後にはヘッダ、フッタが付属する。
// ヘッダにはメモリを確保したソースと行数が記録されるため、デバッグに役に立つ。
// フッタにはmagicが記録される。これにより、確保領域を超えて書き込んだ場合の
// データ破壊検出に役に立つ。
// 
// SLABは密度の濃いものから順に取得される。
// 密度は(max_cnt - alloc_cnt) * 10 / max_cntの値で求められる。
// 先に10を掛けることで、整数計算のみで10%単位の値へ変換する。
// 優先度は0 ～ 9の10段階であり、SLAB獲得は優先度1から順に実施される。
// （優先度0は空きのない管理領域である）
// 10の個所を大きくすると、細かく段階を分けられるが抜き差しが大きくなり性能が
// 劣化する原因となる。
// 小さくすると、SLABが開放されにくくなる。
//

// SLABのサイズは1MB単位とする
#define SHARAKU_SLAB_PRIO	10
#define SHARAKU_SLAB_SZ		1048576

struct sharaku_slab {
	struct plist_head	s_list;
	struct plist_head	s_flist;
	uint32_t		s_node_cnt;
	size_t			s_size;
	uint32_t		s_max_nodes;
};

struct sharaku_slab_node {
	struct plist_node		sn_plist;
	struct list_head		sn_alist;
	struct list_head		sn_flist;
	uint32_t			sn_alloc_cnt;
	uint32_t			sn_max_cnt;
	struct sharaku_slab		*sn_slab;
};

// スラブを初期化する。
#define SLAB_NODE_INIT(node, size)	\
	{						\
		PLIST_HEAD_INIT(&(node).s_list),	\
		PLIST_HEAD_INIT(&(node).s_flist),	\
		0,					\
		size,					\
	}
// スラブを動的に作成する。
extern struct sharaku_slab* sharaku_slab_create(size_t size);
// 動的に作成したスラブを破棄する。
extern int sharaku_slab_destroy(struct sharaku_slab *slab);

// スラブからメモリを獲得する。
extern void* _sharaku_salloc(struct sharaku_slab *slab,
				   const char *src, uint32_t line);
// スラブから獲得したメモリを開放する。
extern int sharaku_sfree(void *buf);
#define sharaku_salloc(slab)	\
		_sharaku_salloc(slab, __FILE__, __LINE__)

#endif /* _SHARAKU_SLAB_H */

