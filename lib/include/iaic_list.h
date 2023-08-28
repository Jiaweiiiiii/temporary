/*
 * =====================================================================================
 *       Filename:  aipv20_pool.h
 *
 *    Description:  .
 *
 *        Version:  1.0
 *        Created:  07/18/2023 11:18:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:  .
 * =====================================================================================
 */
#ifndef AIPV20_POOL_H
#define AIPV20_POOL_H
#include <stddef.h>

#define AIP_LIST_INIT(name) __list_head_init(name);

// init
static inline void __list_head_init(struct aip_list *list)
{
	list->prev = list;
	list->next = list;
}


// insert
static inline void __list_add(struct aip_list *new,
		struct aip_list *prev, struct aip_list *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}
static inline void aip_list_add(struct aip_list *new, struct aip_list *head)
{
	__list_add(new, head, head->next);
}
static inline void aip_list_add_tail(struct aip_list *new, struct aip_list *head)
{
	__list_add(new, head->prev, head);
}


// remove
static inline void __list_del(struct aip_list * prev, struct aip_list * next)
{
	next->prev = prev;
	prev->next = next;
}
static inline void __list_del_entry(struct aip_list *entry)
{
	__list_del(entry->prev, entry->next);
}
static inline void aip_list_del(struct aip_list *entry)
{
	__list_del(entry->prev, entry->next);
	entry->prev = NULL;
	entry->next = NULL;
}
static inline void aip_list_del_init(struct aip_list *entry)
{
	__list_del(entry->prev, entry->next);
	AIP_LIST_INIT(entry);
}

// replace
static inline void aip_list_replace(struct aip_list *old,
		struct aip_list *new)  
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}
static inline void aip_list_replace_init(struct aip_list *old,
		struct aip_list *new)  
{  
	aip_list_replace(old, new);
	AIP_LIST_INIT(old);
}

// move
static inline void list_move(struct aip_list *list,
		struct aip_list *head)
{
	__list_del_entry(list);
	aip_list_add(list, head);
}
static inline void list_move_tail(struct aip_list *list,
		struct aip_list *head)
{
	__list_del_entry(list);
	aip_list_add_tail(list, head);
}

// Status Detection
static inline int aip_list_is_last(const struct aip_list *list,
		const struct aip_list *head)
{
	return list->next == head;
}
static inline int aip_list_empty(const struct aip_list *head)
{
	return head->next == head;
}
static inline int aip_list_empty_careful(const struct aip_list *head)
{
	struct aip_list *next = head->next;
	return (next == head) && (next == head->prev);
}
static inline int aip_list_is_singular(const struct aip_list *head)
{
	return !aip_list_empty(head) && (head->next == head->prev);
}


// scan
#define list_entry(ptr, type, member) ({\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define list_first_entry(head, type, member) \
	list_entry((head)->next, type, member)
#define aip_list_next_entry(head, type, member) \
	list_entry((head)->next, type, member)

#define list_last_entry(head, type, member) \
	list_entry((head)->prev, type, member)
#define aip_list_prev_entry(head, type, member) \
	list_entry((head)->prev, type, member)

#define aip_list_for_each(pos, head, type, member) \
	for (pos = (type *)(head)->next; \
	pos != (type *)(head); \
	pos = (type *)(pos->member.next))

#endif

