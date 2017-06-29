/* --
 *
 * MIT License
 * 
 * Copyright (c) 2017 Abe Takafumi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _SHARAKU_PLIST_H
#define _SHARAKU_PLIST_H

#include <sharaku/linux/list.h>

#if defined(__cplusplus)
extern "C" {
#endif

// 構造を合わせるため、plist_headを定義する。
// plist_head, plist_nodeをlist_headでキャストしてlist_for_eachできるように
// node_listを頭に持ってくる。
struct plist_head {
	struct list_head	node_list;
};

struct plist_node {
	struct list_head	node_list;
	struct list_head	prio_list;
	int64_t			prio;
};

#define PLIST_HEAD_INIT(head)			\
	{						\
		LIST_HEAD_INIT((head).node_list)	\
	}						\

#define PLIST_NODE_INIT(node, _prio)	\
	{						\
		LIST_HEAD_INIT((node).node_list),	\
		LIST_HEAD_INIT((node).prio_list),	\
		_prio					\
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

#define plist_for_each_entry_safe(pos, n, head, type, member)	\
	 list_for_each_entry_safe(pos, n, &(head)->node_list, type, member.node_list)

#if defined(__cplusplus)
}
#endif

#endif
