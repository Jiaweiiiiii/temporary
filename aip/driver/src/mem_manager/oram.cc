#include "oram.h"
#include "buf_list.h"
#include "data.h"
#include "drivers/aie_mmap.h"
#include "LocalMemMgr.h"
#include "alloc_manager.h"
#include <stdio.h>

static void *phy_memory = NULL;
static void *vir_memory = NULL;
static unsigned int mp_memory_size = 0;
static unsigned int mp_memory_used = 0;
static void *list = NULL;

#define CHECK_NODE_TRUE(node, ret)                                                                 \
    if (node == NULL) {                                                                            \
        printf("node is nullptr\n");                                                                \
        return ret;                                                                                \
    }

int oram_memory_init() {
    soc_mem_buf_t buf = __aie_get_oram_info();
    vir_memory = buf.vaddr;
    phy_memory = buf.paddr;
    unsigned int mp_size = buf.size;
    mp_memory_size = mp_size;
    mp_memory_used = 0;

    list = malloc(sizeof(BufList));
    if (list == NULL) {
        printf("list malloc failed\n");
        return -1;
    }
    BufList *buf_list = (BufList *)list;
    BufNode *node = (BufNode *)create_bufnode(); /*maybe malloc together is better???*/
    CHECK_NODE_TRUE(node, -1);
    node->va = POINTER_TO_ADDR(buf.vaddr);
    node->pa = POINTER_TO_ADDR(buf.paddr);
    node->usr_va = POINTER_TO_ADDR(buf.vaddr);
    node->usr_pa = POINTER_TO_ADDR(buf.paddr);
    node->offset = 0;
    node->length = buf.size;
    node->status = BufStatus::BUF_FREE;
    node->prev = NULL;
    node->next = NULL;
    buflist_init(buf_list, node);
    return 0;
}

void oram_memory_deinit(void) {
    if (list) {
        buflist_destory((BufList *)list);
        free(list);
        list = NULL;
    }
    vir_memory = NULL;
    mp_memory_size = 0;
    mp_memory_used = 0;
}

MemoryInfo get_oram_memory_info() {
    struct MemoryInfo info;
    info.phy_addr = phy_memory;
    info.vir_addr = vir_memory;
    info.total_size = mp_memory_size;
    info.used_size = mp_memory_used;
    return info;
}

/*
 *----------------------
 *      free
 *----------------------
 *>>>
 *----------|-----------
 *   busy   |  free
 *----------|-----------
 */
void *oram_malloc(unsigned int size) {
    if (list != NULL) {
        BufList *buf_list = (BufList *)list;
        for (BufNode *cur = buf_list->head; cur != NULL; cur = cur->next) {
            if ((cur->status == BufStatus::BUF_FREE) && (cur->length >= size)) {
                BufNode *node = (BufNode *)create_bufnode();
                CHECK_NODE_TRUE(node, 0);
                int cur_ofs = 0;
                int cur_length = size + cur_ofs;
                /*new*/
                node->va = cur->va + cur_length;
                node->pa = cur->pa + cur_length;
                node->usr_va = 0;
                node->usr_pa = 0;
                node->offset = 0;
                node->length = cur->length - cur_length;
                node->status = BufStatus::BUF_FREE;

                /*old*/
                cur->status = BufStatus::BUF_BUSY;
                cur->length = cur_length;
                cur->offset = cur_ofs;
                cur->usr_va = cur->va + cur->offset;
                cur->usr_pa = cur->pa + cur->offset;

                buflist_insert_after(buf_list, cur, node);
                mp_memory_used += cur_length;
                return (void *)(cur->usr_va);
            } // if
        }     // for
    }         // list
    return NULL;
}

void *oram_memalign(unsigned int align, unsigned int size) {
    if (list != NULL) {
        BufList *buf_list = (BufList *)list;
        for (BufNode *cur = buf_list->head; cur != NULL; cur = cur->next) {
            if ((cur->status == BufStatus::BUF_FREE) && (cur->length >= size)) {
                int cur_ofs = 0;
                uint32_t pa = cur->pa;
                int rest = pa % align;
                if (rest != 0) {
                    cur_ofs = align - rest;
                }
                unsigned int cur_length = size + cur_ofs;
                if (cur->length < cur_length)
                    continue;

                rest = cur->length - cur_length;
                if (rest < 1024) { /*too small*/
                    cur->status = BufStatus::BUF_BUSY;
                    cur->offset = cur_ofs;
                    cur->usr_va = cur->va + cur->offset;
                    cur->usr_pa = cur->pa + cur->offset;
                } else {
                    BufNode *node = (BufNode *)create_bufnode();
                    CHECK_NODE_TRUE(node, 0);
                    /*new*/
                    node->va = cur->va + cur_length;
                    node->pa = cur->pa + cur_length;
                    node->usr_va = 0;
                    node->usr_pa = 0;
                    node->offset = 0;
                    node->length = cur->length - cur_length;
                    node->status = BufStatus::BUF_FREE;

                    /*old*/
                    cur->status = BufStatus::BUF_BUSY;
                    cur->length = cur_length;
                    cur->offset = cur_ofs;
                    cur->usr_va = cur->va + cur->offset;
                    cur->usr_pa = cur->pa + cur->offset;

                    buflist_insert_after(buf_list, cur, node);
                }
                // printf("%s: %d: cur->usr_va = 0x%x, size %d\n", __func__, __LINE__, cur->usr_va,
                // cur->length);
                mp_memory_used += cur->length;
                return (void *)(cur->usr_va);
            }
        }
    }
    return NULL;
}

void oram_free(void *addr) {
    BufList *buf_list = (BufList *)list;
    BufNode *cur = buflist_find_node(buf_list, POINTER_TO_ADDR(addr));
    if (cur == NULL)
        return;
    mp_memory_used -= cur->length;
    // printf("%s: %d, cur->usr_va = 0x%x, cur->length = %d, mp_memory_used = %d\n", __func__,
    // __LINE__, cur->usr_va, cur->length, mp_memory_used);

    cur->status = BufStatus::BUF_FREE;
    buflist_merge_node(buf_list, cur);
}
