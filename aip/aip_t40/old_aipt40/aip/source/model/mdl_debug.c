/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : mdl_debug.c
 * Authors    : jmqi@taurus
 * Create Time: 2020-05-28:11:21:02
 * Description:
 *
 */

int debug_dx = -1;
int debug_dy = -1;

void debug_point(int dx, int dy)
{
    debug_dx = dx;
    debug_dy = dy;
}
