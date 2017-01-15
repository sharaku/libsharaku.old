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
// 密度は(max_cnt - alloc_cnt) * N / max_cntの値で求められる。
// 先にNを掛けることで、整数計算のみで0 ～ N値へ変換する。
// この密度を優先度としてplistを使用して優先度順に並べる。
// Nを大きくすると、細かく段階を分けられるが抜き差しが大きくなり性能が
// 劣化する原因となる。
// 小さくすると、SLABが開放されにくくなる。
//
// 指定により、nodeサイズ、bufの最大数を変更できる。
//
// SLABの作成方法
//  - グローバル変数としてstruct sharaku_slabを作成し、SLAB_INITを使用して作成
//  - sharaku_slab_create*を使用して動的に作成。
//  - mallocでメモリを獲得、INIT_SLAB*を使用して初期化。(非推奨)
//    （この方法の場合、slabから獲得したメモリが残るかもしれません）

// SLABのサイズは1MB単位とする
#define SHARAKU_SLAB_PRIO	10
#define SHARAKU_SLAB_SZ		1048576

struct sharaku_slab {
	struct plist_head	s_list;
	struct plist_head	s_flist;
	uint32_t		s_node_cnt;
	size_t			s_size;
	size_t			s_node_size;
	uint32_t		s_max_buf_cnt;
};

struct sharaku_slab_node {
	struct plist_node		sn_plist;
	struct list_head		sn_alist;
	struct list_head		sn_flist;
	uint32_t			sn_alloc_cnt;
	uint32_t			sn_max_cnt;
	struct sharaku_slab		*sn_slab;
};

// スラブを初期化する。（静的初期化）
// SLAB_NODE_INIT	Nodeのサイズ、bufの数を指定する。
// SLAB_NODE_INIT_SZ	Nodeのサイズのみ指定する。bufの数は無限。
// SLAB_NODE_INIT_DEF	Nodeのサイズは1MB、bufの数は無限。
#define SLAB_INIT(slab, size, node_size, max_cnt)	\
	{						\
		PLIST_HEAD_INIT((slab).s_list),		\
		PLIST_HEAD_INIT((slab).s_flist),	\
		0,					\
		size,					\
		node_size,				\
		max_cnt,				\
	}

#define SLAB_INIT_SZ(slab, size, node_size)	\
	SLAB_INIT(slab, size, node_size, 0)

#define SLAB_INIT_DEF(slab, size)	\
	SLAB_INIT(slab, size, SHARAKU_SLAB_SZ, 0)

// slabを初期化する。
#define INIT_SLAB(slab, size, node_size, max_cnt)	\
	{						\
		INIT_PLIST_HEAD(&(slab)->s_list);	\
		INIT_PLIST_HEAD(&(slab)->s_flist);	\
		(slab)->s_node_cnt = 0;			\
		(slab)->s_size = size;			\
		(slab)->s_node_size = node_size;	\
		(slab)->s_max_buf_cnt = max_cnt;	\
	}

#define INIT_SLAB_SZ(slab, size, node_size)	\
	INIT_SLAB(slab, size, node_size, 0)

#define INIT_SLAB_DEF(slab, size)	\
	INIT_SLAB(slab, size, SHARAKU_SLAB_SZ, 0)

// スラブを動的に作成する。
extern struct sharaku_slab* sharaku_slab_create(size_t size,
					size_t node_size, uint32_t max_cnt);
static inline struct sharaku_slab*
sharaku_slab_create_sz(size_t size, size_t node_size)
{
	return sharaku_slab_create(size, node_size, 0);
}
static inline struct sharaku_slab*
sharaku_slab_create_def(size_t size)
{
	return sharaku_slab_create(size, SHARAKU_SLAB_SZ, 0);
}

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

