/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : bscaler_wrap.h
 * Authors    : jmqi@ingenic.st.jz.com
 * Create Time: 2020-08-03:09:37:01
 * Description:
 *
 */

#ifndef __BSCALER_WRAP_H__
#define __BSCALER_WRAP_H__
#ifdef __cplusplus
extern "C" {
#endif

void bscaler_mem_init();

void *bscaler_malloc(size_t align, size_t size);

void bscaler_free(void *ptr);

void *bscaler_malloc_oram(size_t align, size_t size);

void bscaler_free_oram(void *ptr);

void bscaler_write_reg(uint32_t reg, uint32_t val);

uint32_t bscaler_read_reg(uint32_t reg, uint32_t val);

void bscaler_init();

#ifdef __cplusplus
}
#endif
#endif /* __BSCALER_WRAP_H__ */

