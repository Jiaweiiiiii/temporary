/*
 * IMP alloc interface.
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Aeolus <weiqing.yan@ingenic.com>
 */

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include "LocalMemMgr.h"
#include "alloc_manager.h"
#define IMP_SUCCESS 1
#define IMP_FAILED 0

/* the manager part of user memory */
static inline void *imp_user_alloc(Alloc *alloc, int size, const char *name) {
    AllocInfo *info;
    int32_t tsize = size + sizeof(*info);
    info = (AllocInfo *)malloc(tsize);
    if (info == NULL) {
        return NULL;
    }

    /*pthread_mutex_lock(&(alloc->user_mutex));*/
    /*strncpy(info->name, name, MAX_NAME_LEN);*/
    info->length = size;
    duplex_list_add_tail(&info->list, &alloc->user_root);
    /*pthread_mutex_unlock(&(alloc->user_mutex));*/

    return info->data;
}

static inline void imp_user_free(Alloc *alloc, void *addr) {
    AllocInfo *info = NULL;
    info = container_of(addr, AllocInfo, data);

    /*pthread_mutex_lock(&(alloc->user_mutex));*/
    info->length = 0;
    duplex_list_del(&info->list);
    free(info);
    /*pthread_mutex_unlock(&(alloc->user_mutex));*/
}

/* the manager part of buffer memory */
#define BUF_MANAGER_NODE_CNT 256
struct memery_header {
    struct list_head list;
    char data[0];
};

struct unit_manager {
    char name[MAX_NAME_LEN];
    pthread_mutex_t mutex;
    struct list_head memlist;
    struct list_head free;
    unsigned int unitsize;
    unsigned int inc_units;
    char mem[0];
};

static void *IMP_create_units(Alloc *alloc, uint32_t unitsize, uint32_t incunits,
                              const char *name) {
    unsigned int i = 0;
    struct list_head *list = NULL;
    unsigned int memsize = 0;
    struct unit_manager *manager = NULL;

    if (!name) {
        printf("%s: name is NULL\n", __func__);
        return NULL;
    }

    unitsize = unitsize > sizeof(struct memery_header)
                   ? unitsize
                   : sizeof(struct memery_header); /*最少申请的大小*/
    memsize = unitsize * incunits;

    manager =
        (struct unit_manager *)imp_user_alloc(alloc, sizeof(struct unit_manager) + memsize, name);
    if (!manager)
        return NULL;

    memset(manager, 0, sizeof(*manager) + memsize);
    memcpy(manager->name, name, sizeof(manager->name));
    pthread_mutex_init(&manager->mutex, NULL);
    INIT_DUPLEX_LIST_HEAD(&(manager->free));
    INIT_DUPLEX_LIST_HEAD(&(manager->memlist));
    manager->unitsize = unitsize;
    manager->inc_units = incunits;

    for (i = 0; i < manager->inc_units; i++) { // 256
        list = (struct list_head *)((char *)(manager->mem) + i * unitsize);
        //    printf("%d %s list->next = %p, list->prev\n", __LINE__,__func__
        //    ,list->next,
        // list->prev);
        duplex_list_add_tail(list, &(manager->free));
    }
    return (void *)manager;
}

static void IMP_destroy_units(Alloc *alloc, void *handle) {
    struct memery_header *mem = NULL;
    struct unit_manager *manager = (struct unit_manager *)handle;
    if (!manager)
        return;
    pthread_mutex_lock(&(manager->mutex));
    while (!duplex_list_empty(&manager->memlist)) {
        mem = duplex_list_first_entry(&(manager->memlist), struct memery_header, list);
        duplex_list_del(&(mem->list));
        imp_user_free(alloc, mem);
    }
    pthread_mutex_unlock(&(manager->mutex));
    pthread_mutex_destroy(&(manager->mutex));
    imp_user_free(alloc, manager);
}

static void *IMP_alloc_unit(Alloc *alloc, void *handle) {
    struct unit_manager *manager = (struct unit_manager *)handle;
    struct list_head *list = NULL;
    struct memery_header *mem = NULL;
    if (!manager)
        return NULL;

    pthread_mutex_lock(&(manager->mutex));
    if (duplex_list_empty(&manager->free)) {
        unsigned int memsize =
            manager->unitsize * manager->inc_units + sizeof(struct memery_header);
        unsigned int i = 0;
        mem = (struct memery_header *)imp_user_alloc(alloc, memsize,
                                                     manager->name); // manager user memory
        if (!mem) {
            printf("units malloc failed\n");
            pthread_mutex_unlock(&(manager->mutex));
            return NULL;
        }
        memset(mem, 0, memsize);

        duplex_list_add_tail(&(mem->list), &(manager->memlist));

        for (i = 0; i < manager->inc_units; i++) {
            list = (struct list_head *)((char *)(mem->data) + i * manager->unitsize);
            duplex_list_add_tail(list, &(manager->free));
        }
    }

    list = manager->free.next;
    duplex_list_del(list);
    pthread_mutex_unlock(&(manager->mutex));
    //    memset(list, 0, manager->unitsize);
    return (void *)list;
}

static void IMP_free_unit(void *handle, void *addr) {
    struct unit_manager *manager = (struct unit_manager *)handle;
    struct list_head *list = (struct list_head *)addr;
    if (!manager || !addr)
        return;
    pthread_mutex_lock(&(manager->mutex));
    memset(list, 0, manager->unitsize);
    duplex_list_add_tail(list, &(manager->free));
    pthread_mutex_unlock(&(manager->mutex));
}

static int32_t imp_buf_alloc_init(Alloc *alloc) {
    Bufunit_s *root = NULL;
    Bufunit_s *unit = NULL;
    alloc->buf_handle =
        IMP_create_units(alloc, sizeof(Bufunit_s), BUF_MANAGER_NODE_CNT, "buffalloc");
    if (alloc->buf_handle == NULL) {
        printf("Failed to init buf_handle!\n");
        return IMP_FAILED;
    }
    root = IMP_alloc_unit(alloc, alloc->buf_handle);
    /*strncpy(root->name, "buf_root", MAX_NAME_LEN);*/
    root->status = ALLOC_BUF_BUSY; // busy
    root->vaddr = (uint32_t)alloc->start;
    root->length = alloc->size;
    INIT_DUPLEX_LIST_HEAD(&(root->brothers));
    INIT_DUPLEX_LIST_HEAD(&(root->children));

    unit = IMP_alloc_unit(alloc, alloc->buf_handle);
    unit->status = ALLOC_BUF_FREE; // free
    unit->vaddr = root->vaddr;
    unit->length = root->length;
    unit->parents = root;
    INIT_DUPLEX_LIST_HEAD(&(unit->brothers));
    INIT_DUPLEX_LIST_HEAD(&(unit->children));
    duplex_list_add_tail(&(unit->brothers), &(root->children));

    alloc->buf_root = root;

    return IMP_SUCCESS;
}

static int imp_buf_alloc_Deinit(Alloc *alloc) {
    IMP_destroy_units(alloc, alloc->buf_handle);
    alloc->buf_handle = NULL;
    alloc->buf_root = NULL;

    return IMP_SUCCESS;
}

static Bufunit_s *alloc_bufunit(Alloc *alloc, Bufunit_s *root, int size, uint32_t alignsize) {
    Bufunit_s *unit = NULL;
    Bufunit_s *freeunit = NULL;
    Bufunit_s *newunit = NULL;
    //Bufunit_s *preunit = NULL;
    uint32_t offset = 0;

    duplex_list_for_each_entry(unit, &root->children, brothers) {
        if (unit->status == ALLOC_BUF_FREE && unit->length >= size) {
            if (unit->vaddr % alignsize) {
                offset = alignsize - (unit->vaddr % alignsize);
                if (unit->length >= size + offset) {
                    freeunit = unit;
                    break;
                }
            } else {
                freeunit = unit;
                offset = 0;
                break;
            }
            //preunit = unit;
        }
    }
    if (freeunit) {
        /*
         * a case: free->vaddr % alignsize == 0
         *
         * |---------------------|            |--------------------------------|
         * | preunit | free unit | -------->| preunit | free unit | new unit |
         * |---------------------|            |--------------------------------|
         *
         * other case :
         * |---------------------| |---------------------------------------------|
         * | preunit | free unit | -------->| preunit len + offset | free unit | new
         * unit |
         * |---------------------| |---------------------------------------------|
         * */
        if (freeunit->length > size + offset) {
            newunit = IMP_alloc_unit(alloc, alloc->buf_handle);
            if (!newunit) {
                printf("Failed to alloc bufunit!\n");
                return NULL;
            }
            INIT_DUPLEX_LIST_HEAD(&(newunit->brothers));
            INIT_DUPLEX_LIST_HEAD(&(newunit->children));
            newunit->status = ALLOC_BUF_FREE;
            newunit->vaddr = freeunit->vaddr + size + offset;
            newunit->length = freeunit->length - size - offset;
            newunit->parents = root;
            duplex_list_next_insert(&(freeunit->brothers), &(newunit->brothers));
        }
        /* if(alignsize && (freeunit->vaddr % alignsize) && preunit) */
        /*    preunit->length += offset; */

        freeunit->vaddr += offset;
        freeunit->length = size + offset;
        freeunit->offset = offset;
        freeunit->status = ALLOC_BUF_BUSY;
    }
    return freeunit;
}

static Bufunit_s *find_bufunit_byaddr(Bufunit_s *root, uint32_t addr) {
    Bufunit_s *tmp = NULL;
    Bufunit_s *find = NULL;
    if (root == NULL)
        return NULL;

    duplex_list_for_each_entry(tmp, &root->children, brothers) {
        if (tmp->vaddr == addr) {
            find = tmp;
            break;
        }
        if ((find = find_bufunit_byaddr(tmp, addr)) != NULL)
            break;
    }

    return find;
}

static int32_t free_bufunit(Alloc *alloc, Bufunit_s *self) {
    Bufunit_s *child = NULL;
    Bufunit_s *brother = NULL;
    /* free children */
    if (self->status == ALLOC_BUF_BUSY) {
        while (!duplex_list_empty(&self->children)) {
            child = duplex_list_first_entry(&(self->children), Bufunit_s, brothers);
            duplex_list_del(&(child->brothers));
            if (child->status == ALLOC_BUF_BUSY) {
                printf("addr(0x%08x) is busy!\n", child->vaddr);
            }
            IMP_free_unit(alloc->buf_handle, child);
        }
    }

    self->status = ALLOC_BUF_FREE;
    self->vaddr = self->vaddr - self->offset;
    /* has next brother */
    if (self->brothers.next != &(self->parents->children)) {
        brother = duplex_list_entry(self->brothers.next, Bufunit_s, brothers);
        if (brother->status == ALLOC_BUF_FREE) {
            self->length += brother->length;
            duplex_list_del(&(brother->brothers));
            IMP_free_unit(alloc->buf_handle, brother);
        }
    }
    /* has prev brother */
    if (self->brothers.prev != &(self->parents->children)) {
        brother = duplex_list_entry(self->brothers.prev, Bufunit_s, brothers);
        if (brother->status == ALLOC_BUF_FREE) {
            brother->length += self->length;
            duplex_list_del(&(self->brothers));
            IMP_free_unit(alloc->buf_handle, self);
        }
    }
    return IMP_SUCCESS;
}

/* new **/
int32_t Local_HeapInit(void *heap, unsigned int size, void **list) {
    uint32_t ret = 0;
    Alloc *alloc = NULL;
    if (heap == NULL) {
        return IMP_FAILED;
    }
    if ((unsigned int)heap % sizeof(void *))
        return IMP_FAILED;

    alloc = (Alloc *)malloc(sizeof(Alloc));

    if (alloc == NULL) {
        return IMP_FAILED;
    }

    if (size < sizeof(alloc))
        return IMP_FAILED;

    INIT_DUPLEX_LIST_HEAD(&alloc->user_root);
    /*pthread_mutex_init(&alloc->user_mutex, NULL);*/
    pthread_mutex_init(&alloc->buf_mutex, NULL);

    alloc->start = heap + sizeof(alloc);
    alloc->size = size - sizeof(alloc);
    ret = imp_buf_alloc_init(alloc);
    if (ret != IMP_SUCCESS)
        return ret;

    alloc->init_status = ALLOC_ALREADY_INITIALIZED;
    *list = alloc;
    //*(unsigned int*)heap = (unsigned int)alloc;
    /*printf("%s; alloc = %p\n",__func__, alloc);*/
    return IMP_SUCCESS;
}

int32_t Local_HeapDeInit(void **list) {
    Alloc *alloc = NULL;
    AllocInfo *info = NULL;
    int ret = IMP_SUCCESS;
    if (list == NULL) {
        return IMP_FAILED;
    }

    alloc = (Alloc *)(*(unsigned int *)list);
    /*printf("%s; alloc = %p\n",__func__, alloc);*/
    if (alloc->init_status != ALLOC_ALREADY_INITIALIZED)
        return IMP_SUCCESS;

    pthread_mutex_lock(&(alloc->buf_mutex));
    imp_buf_alloc_Deinit(alloc);
    pthread_mutex_unlock(&(alloc->buf_mutex));

    /*pthread_mutex_lock(&(alloc->user_mutex));*/
    while (!duplex_list_empty(&alloc->user_root)) {
        info = duplex_list_first_entry(&(alloc->user_root), AllocInfo, list);
        duplex_list_del(&(info->list));
        free(info);
    }
    /*pthread_mutex_unlock(&(alloc->user_mutex));*/

    /*pthread_mutex_destroy(&(alloc->user_mutex));*/
    pthread_mutex_destroy(&(alloc->buf_mutex));
    free(alloc);
    *(unsigned int *)list = 0;
    return ret;
}

void *Local_alignAlloc(void *heap, unsigned int alignsize, unsigned int size) {
    Alloc *alloc = NULL;
    Bufunit_s *newunit = NULL;

    alloc = (Alloc *)(*(unsigned int *)heap);
    if (!alloc || alloc->init_status != ALLOC_ALREADY_INITIALIZED) {
        printf("%s:heap is invalid\n", __func__);
        return NULL;
    }

    if (!alignsize) {
        printf("alignsize(%d) is invalid\n", alignsize);
        return NULL;
    }

    pthread_mutex_lock(&(alloc->buf_mutex));
    newunit = alloc_bufunit(alloc, alloc->buf_root, size, alignsize);
    /*if(newunit == NULL)*/
    /*    printf("heap hasn't enough memory!\n");*/

    pthread_mutex_unlock(&(alloc->buf_mutex));
    if (newunit == NULL) {
        return NULL;
    }

    return (void *)newunit->vaddr;
}

int Local_Dealloc(void *heap, void *addr) {
    Alloc *alloc = NULL;
    Bufunit_s *unit = NULL;
    int32_t ret = IMP_SUCCESS;
    int nbytes = 0;
    alloc = (Alloc *)(*(unsigned int *)heap);
    if (!alloc || alloc->init_status != ALLOC_ALREADY_INITIALIZED) {
        printf("%s:heap is invalid\n", __func__);
        ret = IMP_FAILED;
        goto out;
    }

    if (addr == NULL) {
        printf("addr is NULL!\n");
        return IMP_FAILED;
    }

    pthread_mutex_lock(&(alloc->buf_mutex));
    unit = find_bufunit_byaddr(alloc->buf_root, (uint32_t)addr);
    if (unit) {
        nbytes = unit->length - unit->offset;
        ret = free_bufunit(alloc, unit);
    } else {
        ret = IMP_FAILED;
    }
    pthread_mutex_unlock(&(alloc->buf_mutex));
out:
    return ret == IMP_FAILED ? 0 : nbytes;
}

void *Local_Alloc(void *heap, unsigned int nbytes) { return Local_alignAlloc(heap, 1, nbytes); }

void *Local_Calloc(void *heap, unsigned int size, unsigned int n) {
    void *addr = Local_alignAlloc(heap, 1, size * n);
    if (addr) {
        memset(addr, 0, size * n);
    }
    return addr;
}

void Local_Dump_List(void *heap) {
    Alloc *alloc = NULL;
    //int32_t ret = IMP_SUCCESS;
    Bufunit_s *root = NULL;
    Bufunit_s *unit = NULL;
    Bufunit_s *son = NULL;
    int32_t total_free = 0;
    int32_t size = 0, free = 0;
    int idx = 0;
    int son_idx = 0;

    if (heap == NULL)
        return;

    alloc = (Alloc *)(*(unsigned int *)heap);
    if (!alloc || alloc->init_status != ALLOC_ALREADY_INITIALIZED) {
        printf("%s:heap is invalid\n", __func__);
        return;
    }

    root = alloc->buf_root;
    duplex_list_for_each_entry(unit, &root->children, brothers) {
        size = 0;
        free = 0;
        son_idx = 0;
        printf("----------------------------------------------------------------\n");
        printf("%d) dump info start\n", idx);
        if (!duplex_list_empty(&unit->children)) {
            duplex_list_for_each_entry(son, &unit->children, brothers) {
                if (son->status == ALLOC_BUF_FREE)
                    free += son->length;
                size += son->length;
                printf("        %d): state = %s, vaddr = 0x%08x, len = %dBytes\n", son_idx,
                       son->status ? "busy" : "free", son->vaddr, son->length);
                son_idx++;
            }
        } else {
            if (unit->status == ALLOC_BUF_FREE)
                free = unit->length;
        }
        total_free += free;
        printf("   this over: vaddr = 0x%08x, len = %dBytes, free = %dBytes, using "
               "rate = %0.2f%%\n",
               unit->vaddr, unit->length, free, (float)(unit->length - free) / unit->length * 100);
        idx++;
    }
    printf("all over: vaddr = 0x%08x, len = %dBytes, free = %dByptes, using rate "
           "= %0.2f%%\n",
           root->vaddr, root->length, total_free,
           (float)(root->length - total_free) / root->length * 100);
    return;
}
