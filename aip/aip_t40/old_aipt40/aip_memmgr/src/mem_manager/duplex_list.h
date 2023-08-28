#ifndef __SVAC_COMMON_LIST_H__
#define __SVAC_COMMON_LIST_H__

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

struct list_head {
	struct list_head *prev;
	struct list_head *next;
};

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define DUPLEX_LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_DUPLEX_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a newnode entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __duplex_list_add(struct list_head *newnode,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = newnode;
	newnode->next = next;
	newnode->prev = prev;
	prev->next = newnode;
}
/**
 * list_add - add a newnode entry
 * @newnode: newnode entry to be added
 * @head: list head to add it after
 *
 * Insert a newnode entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void duplex_list_add(struct list_head *newnode, struct list_head *head)
{
	__duplex_list_add(newnode, head, head->next);
}


/**
 * list_add_tail - add a newnode entry
 * @newnode: newnode entry to be added
 * @head: list head to add it before
 *
 * Insert a newnode entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void duplex_list_add_tail(struct list_head *newnode, struct list_head *head)
{
	__duplex_list_add(newnode, head->prev, head);//add list
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __duplex_list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __duplex_list_del_entry(struct list_head *entry)
{
	__duplex_list_del(entry->prev, entry->next);
}

static inline void duplex_list_del(struct list_head *entry)
{
	__duplex_list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/**
 * list_replace - replace old entry by newnode one
 * @old : the element to be replaced
 * @newnode : the newnode element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static inline void duplex_list_replace(struct list_head *old,
				struct list_head *newnode)
{
	newnode->next = old->next;
	newnode->next->prev = newnode;
	newnode->prev = old->prev;
	newnode->prev->next = newnode;
}

static inline void duplex_list_replace_init(struct list_head *old,
					struct list_head *newnode)
{
	duplex_list_replace(old, newnode);
	INIT_DUPLEX_LIST_HEAD(old);
}

static inline void duplex_list_prev_insert(struct list_head *list,
				struct list_head *newnode)
{
	list->prev->next = newnode;
	newnode->prev = list->prev;
	newnode->next = list;
	list->prev = newnode;
}

static inline void duplex_list_next_insert(struct list_head *list,
				struct list_head *newnode)
{
	list->next->prev = newnode;
	newnode->next = list->next;
	newnode->prev = list;
	list->next = newnode;
}

/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static inline void duplex_list_del_init(struct list_head *entry)
{
	__duplex_list_del_entry(entry);
	INIT_DUPLEX_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void duplex_list_move(struct list_head *list, struct list_head *head)
{
	__duplex_list_del_entry(list);
	duplex_list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static inline void duplex_list_move_tail(struct list_head *list,
				  struct list_head *head)
{
	__duplex_list_del_entry(list);
	duplex_list_add_tail(list, head);
}

/**
 * list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static inline int duplex_list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int duplex_list_empty(const struct list_head *head)
{
	return head->next == head;
}

/**
 * list_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 */
static inline int duplex_list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * list_del_head_entry - del entry from list head to a newnode
 * @newnode: newnode entry to be output
 * @head: list head to del
 */
static inline void duplex_list_del_head_entry(struct list_head **newnode, struct list_head *head)
{
	if (head->next == head) {
		*newnode = NULL;
	} else {
		*newnode = head->next;
		head->next = head->next->next;
		head->next->prev = head;
		(*newnode)->next = (*newnode)->prev = *newnode;
	}
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define duplex_list_entry(ptr, type, member) \
		((type *)(((char *)(ptr)) - offsetof(type, member)))

/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define duplex_list_first_entry(ptr, type, member) \
	duplex_list_entry((ptr)->next, type, member)

/**
 * list_last_entry - get the last element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define duplex_list_last_entry(ptr, type, member) \
	duplex_list_entry((ptr)->prev, type, member)

/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define duplex_list_first_entry_or_null(ptr, type, member) \
	(!duplex_list_empty(ptr) ? duplex_list_first_entry(ptr, type, member) : NULL)

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define duplex_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * __list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 *
 * This variant doesn't differ from list_for_each() any more.
 * We don't do prefetching in either case.
 */
#define __duplex_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_prev	-	iterate over a list backwards
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define duplex_list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define duplex_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - iterate over a list backwards safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
#define duplex_list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define duplex_list_for_each_entry(pos, head, member)				\
	for (pos = duplex_list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = duplex_list_entry(pos->member.next, typeof(*pos), member))

#endif /*  __SVAC_COMMON_LIST_H__*/
