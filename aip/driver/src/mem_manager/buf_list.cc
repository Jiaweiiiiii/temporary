#include "buf_list.h"

void buflist_init(BufList *list, BufNode *head) {
    list->head = head;
    list->tail = head;
}
void buflist_add_head(BufList *list, BufNode *node) {
    BufNode *head = list->head;
    node->prev = NULL;
    if (head == NULL) {
        buflist_init(list, head);
    } else {
        node->next = head;
        head->prev = node;
        list->head = node;
    }
}
void buflist_add_tail(BufList *list, BufNode *node) {
    BufNode *tail = list->tail;
    tail->next = node;

    node->prev = tail;
    node->next = NULL;
    list->tail = node;
}

void buflist_insert_after(BufList *list, BufNode *current, BufNode *node) {
    BufNode *next = current->next;
    if (next == NULL) {
        buflist_add_tail(list, node);
    } else {
        current->next = node;
        node->prev = current;
        node->next = next;
        next->prev = node;
    }
}

void buflist_insert_before(BufList *list, BufNode *current, BufNode *node) {
    BufNode *prev = current->prev;
    if (prev == NULL) {
        buflist_add_head(list, node);
    } else {
        prev->next = node;
        node->prev = prev;
        node->next = current;
        current->prev = node;
    }
}

BufNode *buflist_find_node(BufList *list, uint32_t usr_va) {
    if (list != NULL) {
        BufNode *node = list->head;
        while (node != NULL) {
            if (node->usr_va == usr_va) {
                return node;
            }
            node = node->next;
        }
    }
    return NULL;
}

void buflist_remove_head(BufList *list) {
    BufNode *head = list->head;
    if (head != NULL) {
        BufNode *next = head->next;
        next->prev = NULL;
        list->head = next;
    }
}
void buflist_remove_tail(BufList *list) {
    BufNode *tail = list->tail;
    if (tail != NULL) {
        BufNode *prev = tail->prev;
        prev->next = NULL;
        list->tail = prev;
    }
}
void buflist_remove_node(BufList *list, BufNode *node) {
    if (node == list->head) {
        buflist_remove_head(list);
    } else if (node == list->tail) {
        buflist_remove_tail(list);
    } else {
        BufNode *prev = node->prev;
        BufNode *next = node->next;
        if (prev)
            prev->next = next;
        if (next)
            next->prev = prev;
    }
}
void buflist_destory(BufList *list) {
    if (list != NULL) {
        BufNode *node = list->head;
        while (node != NULL) {
            BufNode *cur = node;
            node = node->next;
            delete_bufnode(cur);
        }
    }
}

/*when buf status is FREE, merge into one buf*/
void buflist_merge_node(BufList *list, BufNode *node) {
    if ((node == NULL) || (node->status != BufStatus::BUF_FREE)) {
        printf("node==NULL or node->status!=BufStatus::BUF_FREE\n");
        return;
    }
    /*backward*/
    BufNode *start = node;
    BufNode *cur = node->prev;
    while ((cur != NULL) && (cur->status == BufStatus::BUF_FREE)) {
        start = cur;
        cur = cur->prev;
    }
    /*forward*/
    /*keep first*/
    BufNode *end = NULL;
    int length = start->length;
    cur = start->next;
    while ((cur != NULL) && (cur->status == BufStatus::BUF_FREE)) {
        length += cur->length;
        end = cur;
        cur = cur->next;

        /*delete node*/
        buflist_remove_node(list, end);
        delete_bufnode(end);
    }
    start->length = length;
}
