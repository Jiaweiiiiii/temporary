#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int get_wafer_status() {
    int fd = -1;
    void *map_base, *virt_addr;

    if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
        printf("open /dev/mem failed, errno = %d(%s)\n", errno, strerror(errno));
        return 0;
    }

    map_base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x13540000);
    if (map_base == MAP_FAILED) {
        printf("mmap CPM error, errno = %d(%s)\n", errno, strerror(errno));
        close(fd);
        return 0.f;
    }

    virt_addr = map_base + 0x22c;
    unsigned long status = (*(unsigned long *)virt_addr) & 0x80; /*bit7*/
    close(fd);
    return status;
}

/*get cpu frequency (GHz)*/
float get_cpu_frequency(void) {

    int fd = -1;
    void *map_base, *virt_addr;
    unsigned long apll_cpapcr;
    int apllm, aplln, apllod0, apllod1;
    float cpu_frequency;

    if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
        printf("open /dev/mem failed, errno = %d(%s)\n", errno, strerror(errno));
        return 0.f;
    }

    map_base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x10000000);
    if (map_base == MAP_FAILED) {
        printf("mmap CPM error, errno = %d(%s)\n", errno, strerror(errno));
        close(fd);
        return 0.f;
    }

    virt_addr = map_base + 0x10;
    apll_cpapcr = *(unsigned long *)virt_addr;

    // T40/A1:(24*M/N)/OD1/OD0
    // T41:（24*(M+1)*2/(N+1)）/2^OD1/(OD0+1)
    apllm = (apll_cpapcr >> 20) & 0xfff;
    aplln = (apll_cpapcr >> 14) & 0x3f;
    apllod1 = (apll_cpapcr >> 11) & 0x7;
    apllod0 = (apll_cpapcr >> 7) & 0xf;
    cpu_frequency = (24 * (apllm + 1) * 2 / (aplln + 1)) / pow(2, apllod1) / (apllod0 + 1);

    if (munmap(map_base, 0x1000) == -1) {
        printf("munmap CPM error, errno = %d(%s)\n", errno, strerror(errno));
        close(fd);
        return 0.f;
    }

    close(fd);

    // Ghz
    return (cpu_frequency / 1000.f);
}
