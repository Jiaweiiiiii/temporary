#ifndef __BUF_LIST_H__
#define __BUF_LIST_H__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BufNode BufNode;
typedef struct BufList BufList;
typedef enum BufStatus { BUF_FREE, BUF_BUSY } BufStatus_t;

struct BufNode {
    uintptr_t va;
    uintptr_t pa;
    uintptr_t usr_va;
    uintptr_t usr_pa;
    int32_t offset;
    uint32_t length;
    BufStatus status;

    BufNode *prev;
    BufNode *next;
};

struct BufList {
    BufNode *head;
    BufNode *tail;
};

inline void *create_bufnode() { return malloc(sizeof(BufNode)); }
inline void delete_bufnode(void *node) {
    if (node) {
        free(node);
        node = NULL;
    }
}

void buflist_init(BufList *list, BufNode *head);
void buflist_insert_after(BufList *list, BufNode *current, BufNode *node);
BufNode *buflist_find_node(BufList *list, uint32_t usr_va);
void buflist_remove_node(BufList *list, BufNode *node);
void buflist_destory(BufList *list);
void buflist_merge_node(BufList *list, BufNode *node);

#endif //__BUF_LIST_H__
