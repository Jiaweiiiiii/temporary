
编译规则
mips-linux-gnu-g++ -muclibc -static -O2 -o nmem_test nmem_test.cc -I../../7.2.0/include -L ../../../aip_memmgr    -L ../../7.2.0/lib/uclibc -ldrivers -lpthread -lrt

