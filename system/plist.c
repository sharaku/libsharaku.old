/*
 * Copyright Abe Takafumi All Rights Reserved, 2017
 * Author Abe Takafumi
 *
 */

//  lh:list_head
//  pl:prio_list
//  nl:node_list
//  
//  < 初期状態 >
//      +->|lh|<-+
//      |        |
//      +--------+
// 
//  < 最初の登録時点の状態 >
//             +--------+
//             |        |
//             +->|pl|<-+
//                |02|   (prioメンバ)
//      +->|lh|<->|nl|<-+
//      |               |
//      +---------------+
// 
//  < 複数登録の状態 >
//             +----------------------------------------+
//             |                                        |
//             +->|pl|<->|pl|<------------------->|pl|<-+
//                |  |   |  |  +------+ +------+  |  |
//                |  |   |  |  |      | |      |  |  |
//                |  |   |  |  +>|pl|<+ +>|pl|<+  |  |
//                |01|   |02|    |02|     |02|    |10|   (prioメンバ)
//      +->|lh|<->|nl|<->|nl|<-->|nl|<--->|nl|<-->|nl|<-+
//      |                                               |
//      +-----------------------------------------------+

#include <sharaku/plist.h>

void
plist_add(struct plist_node *node, struct plist_head *head)
{
	struct plist_node *first = NULL;
	struct plist_node *iter = NULL;
	struct plist_node *prev = NULL;
	struct list_head *next = &head->node_list;

	if (list_empty(&head->node_list)) {
		// 最初の登録であれば無条件に登録する
		// 
		// < 初期登録状態 >
		//         +--------+
		//         |        |
		//         +->|pl|<-+
		//            |02|   (prio)
		//            |  |
		//            |  |
		//  +->|lh|<->|nl|<-+
		//  |               |
		//  +---------------+
		goto insert;
	}

	// 2回目以降の登録であれば挿入する位置を検索する
	// 挿入は同一prioの最後に行う。
	// これにより、prio順に昇順でソートされる。かつ、
	// 新規登録はprio内の最後に入る
	// prevがNULLの場合は先頭に挿入する。
	// iter == firstの場合は末尾に挿入する。
	// それ以外の場合はiterの直前に登録する。
	first = list_entry(head->node_list.next,
			struct plist_node, node_list);
	iter = first;
	do {
		if (node->prio < iter->prio) {
			next = &iter->node_list;
			break;
		}
		prev = iter;
		iter = list_entry(iter->prio_list.next,
					struct plist_node, prio_list);
	} while (iter != first);

	// prio_listを構築する。
	// iterは挿入位置のprioのlist_head。
	// つまり1つ前が今登録したprioでなければならない。
	// prioが一致しない場合は、prio_listの登録を行う。
	if (!prev || prev->prio != node->prio) {
		list_add_tail(&node->prio_list, &iter->prio_list);
	}
insert:
	list_add_tail(&node->node_list, next);
}

void
plist_del(struct plist_node *node, struct plist_head *head)
{
	struct plist_node *next;
	// prioの先頭である場合、次のnodeに先頭をバトンタッチする。
	// ただし、末尾の場合（nodeのnextがhead）は抜くだけでよい。
	if (!list_empty(&node->prio_list)) {
		if (node->node_list.next != &head->node_list) {
			next = list_entry(node->node_list.next,
					struct plist_node, node_list);
			if (node->prio == next->prio) {
				list_add(&next->prio_list, &node->prio_list);
			}
		}
		list_del_init(&node->prio_list);
	}
	list_del_init(&node->node_list);
}



