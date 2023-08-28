
#ifndef __IAIC_AIP_CORE_H__
#define __IAIC_AIP_CORE_H__

#include "iaic_aip_ioctl.h"

#define M_CURR_CTX ________iaic_curr_context________
#define M_CURR_CTX_FD M_CURR_CTX.fd

typedef struct {
	int fd;
} iaic_context_t;

extern iaic_context_t M_CURR_CTX;

extern 
int aip_submit(int fd, iaic_aip_ioctl_submit_t *args);

extern
int aip_wait(int fd, iaic_aip_ioctl_wait_t *args);

#endif
