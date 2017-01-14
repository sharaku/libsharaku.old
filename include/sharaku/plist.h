/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

#ifndef _SHARAKU_PLIST_H
#define _SHARAKU_PLIST_H

#include <sharaku/list.h>

#if defined(__cplusplus)
extern "C" {
#endif

// 構造を合わせるため、plist_headを定義する。
struct plist_head {
	struct list_head	node_list;
};

struct plist_node {
	int64_t			prio;
	struct list_head	prio_list;
	struct list_head	node_list;
};

#define PLIST_HEAD_INIT(head)		\
{							\
	.node_list = LIST_HEAD_INIT((head)->node_list)	\
}

#define PLIST_NODE_INIT(node, _prio)	\
	{						\
		_prio,					\
		LIST_HEAD_INIT((node).prio_list),	\
		LIST_HEAD_INIT((node).node_list)	\
	}


#define INIT_PLIST_HEAD(head) INIT_LIST_HEAD(&(head)->node_list);
static inline void INIT_PLIST_NODE(struct plist_node *node, int prio)
{
	node->prio = prio;
	INIT_LIST_HEAD(&node->prio_list);
	INIT_LIST_HEAD(&node->node_list);
}

static inline int plist_empty(const struct plist_head *head)
{
	return head->node_list.next == &head->node_list;
}

extern void plist_add(struct plist_node *node, struct plist_head *head);
extern void plist_del(struct plist_node *node, struct plist_head *head);

#define plist_entry(ptr, type, member)				\
	container_of(ptr, type, member.node_list)

#define plist_first_entry(ptr, type, member)			\
	plist_entry(&(ptr)->node_list.next, type, member)

#define plist_last_entry(ptr, type, member)			\
	plist_entry(&(ptr)->node_list.prev, type, member)

#define plist_first_entry_or_null(ptr, type, member)		\
	(!(plist_empty(ptr) ? plist_first_entry(ptr, type, member) : NULL))

#define plist_for_each_safe(pos, n, head)		\
	 list_for_each_entry_safe(pos, n, &(head)->node_list, node_list)

#define plist_for_each_entry_safe(pos, n, head, type, member)	\
	 list_for_each_entry_safe(pos, n, &(head)->node_list, type, member.node_list)

#if defined(__cplusplus)
}
#endif

#endif
