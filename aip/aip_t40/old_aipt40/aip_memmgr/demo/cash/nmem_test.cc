#include "drivers/aie_mmap.h"
#include "mem_manager/ddr_mem.h"
#include <fstream>
#include <iostream>
#include <memory.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <errno.h>
/*mips-linux-gnu-g++ -muclibc -static -O2 -o nmem_test nmem_test.cc -I/home/lzwang/work/venus/ivse_new/proj/magik/Magik/InferenceKit/venus/external/DRIVERS/7.2.0/include/ -L /home/lzwang/work/venus/ivse_new/proj/magik/Magik/InferenceKit/venus/external/DRIVERS/7.2.0/lib/uclibc/ -ldrivers -lpthread -lrt*/
int main(int argc, char **argv) {
    /* set cpu affinity */
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (-1 == sched_setaffinity(0, sizeof(mask), &mask)) {
	fprintf(stderr, "set cpu affinity failed, %s\n", strerror(errno));
	return -1;
    }

    int ret = __aie_mmap(0, 1, NNA_UNCACHED_ACCELERATED, NNA_UNCACHED_ACCELERATED, NNA_CACHED);
    if (0 != ret) {
      printf("venus init failed.\n");
      return ret;
    }

    int iw = 64;
    int ih = 64;

    //for(int i=0; i<1000; i++){
	while(1){
      int size = iw*ih*4*sizeof(float);
      uint8_t* data = (uint8_t*)ddr_malloc(size);
      if(!data){
	printf("malloc %d failed\n", size);
	return -1;
      }
     // printf("size = %d\n", size);
      memset(data, 0, size);
      iw += 32;

      if(data){
	ddr_free(data);
	data = nullptr;
      }
    }
    return 0;
}
