/*
 * IMP alloc manager function header file.
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Aeolus <weiqing.yan@ingenic.com>
 */

#ifndef __ALLOC_MANAGER_H__
#define __ALLOC_MANAGER_H__

#include <stdint.h>
#include <stdlib.h>

#include "duplex_list.h"

//#include <constraints.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct Alloc;
typedef struct Alloc Alloc;

struct buffer_unit;
typedef struct buffer_unit Bufunit_s;
#define MAX_NAME_LEN 16
struct alloc_info {
    struct list_head list;
    /*char name[MAX_NAME_LEN];*/
    int32_t length;
    int8_t data[0];
};
typedef struct alloc_info AllocInfo;

typedef enum {
    ALLOC_BUF_FREE,
    ALLOC_BUF_BUSY,
} AllocBufStatus_e;

struct buffer_unit {
    /*char name[16];*/
    AllocBufStatus_e status;
    uint32_t vaddr;
    uint32_t length;
    uint32_t offset;
    Bufunit_s *parents;
    struct list_head brothers;
    struct list_head children;
};

typedef enum {
    ALLOC_NOT_INITIALIZED = 0x12345678,
    ALLOC_IS_INITIALIZED,
    ALLOC_ALREADY_INITIALIZED,
} AllocInitStatus;

struct Alloc {
    AllocInitStatus init_status;

    struct list_head user_root;
    /*pthread_mutex_t user_mutex;*/

    void *start;
    unsigned int size;
    void *buf_handle;
    Bufunit_s *buf_root;
    pthread_mutex_t buf_mutex;
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALLOC_MANAGER_H__ */
