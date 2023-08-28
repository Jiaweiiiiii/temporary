#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "iaic.h"
#include "iaic_list.h"

int iaic_ioctl(int fd, unsigned long request, void *arg)
{
    int ret;

    do {
        ret = ioctl(fd, request, arg);
    } while (ret == -1 && (errno == EINTR || errno == EAGAIN));
    return ret;
}

int iaic_set_proc_nice(int aip_fd, int32_t value)
{
	int ret;

	aip_v20_ioctl_action_set_proc_nice_t set_proc_nice;
	set_proc_nice.op = AIP_ACTION_SET_PROC_NICE;
	set_proc_nice.value = value;

	ret = IAIC_IOCTL(aip_fd, IOCTL_AIP_V20_ACTION, &set_proc_nice);

        return ret;
}
