#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <string.h>
#define PMON_SIZE 200
#define STR_SIZE 50
#define OPENPMON \
	char* perf_array=(char*)malloc(PMON_SIZE*STR_SIZE);\
	printf("size=%d\n",PMON_SIZE*STR_SIZE);\
	int fperf = open("/proc/jz/pmon/perform",O_RDWR);\
	if(fperf <= 0)\
	{\
		printf("open pmon error!\n");\
		return -1;\
	}\

#define CLOSEFS close(fperf)

#define START0 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8080",8);
#define START1 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8181",8);
#define START2 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8282",8);
#define START3 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8383",8);
#define START4 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8484",8);
#define START5 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8585",8);
#define START6 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8686",8);
#define START7 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8787",8);
#define START8 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8888",8);
#define START9 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8989",8);
#define START10 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8a8a",8);
#define START11 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8b8b",8);
#define START12 memset(perf_array,0,STR_SIZE*PMON_SIZE); write(fperf,"perf8c8c",8);
#define STOP(fp) \
	lseek(fperf,0,SEEK_SET);\
	read(fperf,perf_array,STR_SIZE*PMON_SIZE);\
	write(fperf,"perf0000",8);\
	fprintf(fp,"%s",perf_array);\
	free(perf_array);

