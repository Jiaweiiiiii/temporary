/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : mdl_debug.h
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-28:15:11:23
 * Description:
 *
 */

#ifndef __MDL_DEBUG_H__
#define __MDL_DEBUG_H__
#ifdef __cplusplus
extern "C" {
#endif

extern int debug_dx;
extern int debug_dy;

void debug_point(int dx, int dy);

#ifdef __cplusplus
}
#endif
#endif /* __MDL_DEBUG_H__ */

