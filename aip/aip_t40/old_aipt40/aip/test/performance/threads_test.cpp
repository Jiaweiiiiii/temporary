/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : resize_demo.cpp
 * Authors    : jmqi@taurus
 * Create Time: 2020-04-20:11:04:27
 * Description:
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/time.h>
#include "Matrix.h"//not necessary
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_hal.h"
#include "aie_mmap.h"
#include "pthread.h"
#include "semaphore.h"
static pthread_mutex_t mem_lock;

//extern sem_t sem_bsc;
nna_cache_attr_t desram_cache_attr = NNA_UNCACHED_ACCELERATED;
nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
nna_cache_attr_t ddr_cache_attr = NNA_CACHED;

typedef struct thread_info{
    char * file_name;
    int pid;
    int box_num;
    int box_t[33][4];
} thread_info;

int box[33][4] = {
    { 970,  502, 36, 40},
    {  92,  642, 52, 62},
    { 744,  628, 52, 64},
    {1554,  560, 48, 60},
    {1750,  630, 48, 58},
    { 400,  622, 50, 62},
    {1222,  456, 46, 56},
    {1834,  484, 46, 58},
    {1262,  616, 46, 58},
    { 510,  446, 48, 60},
    { 760,  484, 48, 60},
    { 274,  494, 48, 60},
    {1602,  640, 48, 60},
    {1470,  634, 48, 60},
    {1906,  646, 52, 64},
    { 548,  632, 46, 58},
    {1122,  636, 50, 62},
    { 448,  476, 44, 56},
    { 622,  478, 48, 60},
    { 174,  510, 48, 60},
    {1408,  496, 48, 60},
    { 894,  508, 48, 60},
    {1554,  474, 46, 56},
    {1714,  566, 48, 60},
    {1042,  542, 46, 58},
    { 354,  478, 44, 56},
    {1280,  506, 46, 58},
    { 270,  632, 48, 60},
    {1686,  510, 42, 52},
    { 990,  634, 48, 60},
    {1168,  524, 44, 56},
    {1092,  498, 44, 54},
    {1858,  548, 46, 56},
    };

void *convert(void *convert_info)
{

    thread_info *info_t = (thread_info *)convert_info;
    float time_use = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);

    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};
    const uint32_t offset[2] = {16, 128};

    uint32_t zero_point = 0x00807060;
    int task_len = rand()%1024+1;
    int plane_stride = 0;
    int img_w = 1024;
    int img_h = 1024;
    bs_data_format_e src_format = BS_DATA_NV12;
    bs_data_format_e dst_format = BS_DATA_BGRA;
    data_info_s src, dst_task;
    task_info_s task_info;
    src.base   = NULL;
    src.base1   = NULL;
    src.format = src_format;
    src.chn    = 1;
    src.width  = img_w;
    src.height = img_h;
    src.line_stride = img_w;
    src.locate = BS_DATA_NMEM;

    dst_task.base   = NULL;
    dst_task.base1   = NULL;
    dst_task.format = dst_format;
    dst_task.chn    = 4;
    dst_task.width  = img_w;
    dst_task.height = task_len;
    dst_task.line_stride = img_w * 4;
    dst_task.locate = BS_DATA_NMEM;

    task_info.zero_point = zero_point;
    task_info.task_len = task_len;
    task_info.plane_stride = task_info.task_len * dst_task.line_stride;

    //alloc space
    int src_size = img_w * img_h * 3 / 2;
    src.base = ddr_memalign(64, src_size);
    if (src.base == NULL) {
        printf("malloc src_base failed!\n");
    }
    FILE *fpi;
    fpi = fopen(info_t->file_name, "rb+");
    if (fpi == NULL) {
        fprintf(stderr, "Open %s failed!\n", info_t->file_name);

    }
    fread(src.base, 1, src_size, fpi);

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    // printf("1 -- %.3fms\n", time_use);

    __aie_flushcache(src.base, src_size);

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    //printf("2 -- %.3fms\n", time_use);
    int dst_task_size = task_info.task_len * dst_task.line_stride;
    void *dst_task_ping = NULL;
    void *dst_task_pong = NULL;
    if (dst_task.locate == BS_DATA_NMEM) {
        dst_task_ping = ddr_memalign(64, dst_task_size);
        dst_task_pong = ddr_memalign(64, dst_task_size);
    } else {
        dst_task_ping = oram_memalign(64, dst_task_size);
        dst_task_pong = oram_memalign(64, dst_task_size);
    }

    if ((dst_task_ping == NULL) || (dst_task_pong == NULL)) {
        printf("[Error]: alloc task ping-pong buffer failed!\n");
    }

    int dst_all_size = img_w * img_h * 4;
    uint8_t *dst_all = (uint8_t *)malloc(dst_all_size);
    memset(dst_all, 0, dst_all_size);
    uint8_t *dst_ptr = dst_all;

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    //printf("3 -- %.3fms\n", time_use);

    bs_covert_cfg(&src, &dst_task, coef, offset, &task_info);
    int times = (img_h + task_info.task_len - 1) / task_info.task_len;
    static int cnt = 0;
    uint32_t phy_dst_base;

    printf("[BSCALER]: bscaler convert start\n");
    for (int i = 0; i < times; i++) {
        //gettimeofday(&end, NULL);
        //time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
        //printf("times:%d -- %.3fms\n", i, time_use);

        int cur_task_len = (i == times - 1) ? ((img_h - 1) % task_len) + 1 : task_len;
        task_info.task_len = cur_task_len;
        int cur_task_size = cur_task_len * dst_task.line_stride;
        if (i%2) {
            bs_covert_step_start(&task_info, dst_task_pong, BS_DATA_NMEM);
        } else {
            bs_covert_step_start(&task_info, dst_task_ping, BS_DATA_NMEM);
        }

        bs_covert_step_wait();

        if (i%2) {
            if (ddr_cache_attr & NNA_CACHED) {
                __aie_flushcache(dst_task_pong, dst_task_size);
            }
            memcpy(dst_ptr, dst_task_pong, cur_task_size);
        } else {
            if (ddr_cache_attr & NNA_CACHED) {
                __aie_flushcache(dst_task_ping, dst_task_size);
            }
            memcpy(dst_ptr, dst_task_ping, cur_task_size);
        }
        dst_ptr += cur_task_size;
    }

    printf("[BSCALER]: bscaler convert  finish...\n");
    //gettimeofday(&end, NULL);
    //time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;//ms
    //printf("4 -- %.3fms\n", time_use);

    char str[128];
    sprintf(str,"result_out/convert_pthread:%d.jpg",info_t->pid);
    stbi_write_jpg(str, img_w, img_h, 4, dst_all, 99);

     if (dst_task.locate == BS_DATA_NMEM) {
         ddr_free(dst_task_ping);
         ddr_free(dst_task_pong);
    }

    ddr_free(dst_task.base);
    ddr_free(src.base);
    free(dst_all);
    fclose(fpi);

}
void *affine(void *affine_info)
{
    //bscaler_frmc_soft_reset();
    thread_info *info_t = (thread_info *)affine_info;
    uint32_t seed = (uint32_t)time(NULL);
    srand(seed);
    printf("[BSCALER]: seed = 0x%08x\n", seed);
    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};//opencv

    const uint32_t offset[2] = {16, 128};

    /*if (argc < 2) {
        fprintf(stderr, "%s [image_path]/entertainers.jpg\n", argv[1]);
        exit(1);
        }*/

    int img_w, img_h, img_chn;
    //argv[1] = ../image/entertainers.jpg
    printf("[BSCALER]: load image ...\n");
    // fprintf(stderr,"%s\n",info_t->file_name);
    uint8_t *img = stbi_load(info_t->file_name, &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        fprintf(stderr, "Open input image failed!\n");
        exit(1);
    }
    printf("[BSCALER]: load image success\n");

    int box_num = info_t->box_num;//33;
    int dst_w = 96;
    int dst_h = 96;
    int dst_size = dst_w * dst_h * 4 * box_num;
    uint8_t *dst_base = (uint8_t *)ddr_memalign(1, dst_size);
    if (dst_base == NULL) {
        fprintf(stderr, "[BSCALER]: dst alloc failed!\n");
    }

    memset(dst_base, 255, dst_size);

    uint8_t *src_base = (uint8_t *)ddr_memalign(1, img_w * img_h * 4);
    if (src_base == NULL) {
        fprintf(stderr, "[BSCALER]: src alloc failed!\n");
    }

    memset(src_base, 255, img_w * img_h * 4);
    memcpy(src_base, img, img_w * img_h * 4);

    data_info_s src;
    src.base = src_base;
    src.base1 = NULL;
    src.format = BS_DATA_BGRA;
    src.chn = 4;
    src.width = img_w;
    src.height = img_h;
    src.line_stride =img_w * 4;
    src.locate = BS_DATA_NMEM;

    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i * dst_w * dst_h * 4;
        dst[i].base1 = NULL;
        dst[i].format = BS_DATA_BGRA;
        dst[i].chn = 4;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_w * 4;
        dst[i].locate = BS_DATA_NMEM;
    }

    box_affine_info_s *infos = (box_affine_info_s *)malloc(sizeof(box_affine_info_s) * box_num);

    for (int i = 0; i < box_num; i++) {
        int src_x = info_t->box_t[i][0];
        int src_y =  info_t->box_t[i][1];
        int src_w =  info_t->box_t[i][2];
        int src_h =  info_t->box_t[i][3];

        CV::Matrix trans;
        float angle = rand() % 360;
        //float angle = (rand() % 3) * 30;
        //float angle = 30;
        //printf("angle = %f\n", angle);
        trans.setRotate(angle, (float)(src_w - 1)/2, (float)(src_h - 1)/2);
        trans.postScale((float)dst_w / (float)src_w,
                        (float)dst_h / (float)src_h);
        float matrix[9];
        trans.get9(matrix);

        infos[i].box.x = src_x;
        infos[i].box.y = src_y;
        infos[i].box.w = src_w;
        infos[i].box.h = src_h;
        infos[i].matrix[0] = matrix[0];
        infos[i].matrix[1] = matrix[1];
        infos[i].matrix[2] = matrix[2];
        infos[i].matrix[3] = matrix[3];
        infos[i].matrix[4] = matrix[4];
        infos[i].matrix[5] = matrix[5];
        infos[i].matrix[6] = matrix[6];
        infos[i].matrix[7] = matrix[7];
        infos[i].matrix[8] = matrix[8];
        infos[i].wrap = 0;
        infos[i].zero_point = 0;
    }
    printf("[BSCALER]: bscaler affine start\n");
    bs_affine_start(&src, box_num, dst, infos, coef, offset);
    printf("[BSCALER]: bscaler wait finish ...\n");
    bs_affine_wait();
    printf("[BSCALER]: bscaler affine finish ..\n");

    char str[128];
    sprintf(str,"result_out/affine_pthread:%lu.jpg",info_t->pid);
    stbi_write_jpg(str, dst_w, dst_h * box_num, 4, dst_base, 99);

    ddr_free(dst_base);
    ddr_free(src_base);
    free(dst);
    free(infos);
    stbi_image_free(img);
}

void *resize(void *resize_info)
{
    thread_info *info_t = (thread_info *)resize_info;

    // printf("info.file:%s,info.box_num:%d\n",info_t->file_name,info_t->box_num);

    float time_use = 0;
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);

    const uint32_t coef[9] = {1220542, 0, 1673527,
                              1220542, 409993, 852492,
                              1220542, 2116026, 0};//opencv
    const uint32_t offset[2] = {16, 128};

    int img_w, img_h, img_chn;
    printf("[BSCALER]: load image ...\n");
    // fprintf(stderr,"%s\n",info_t->file_name);
    uint8_t *img = stbi_load(info_t->file_name, &img_w, &img_h, &img_chn, 4);
    if (img == NULL) {
        fprintf(stderr, "[BSCALER]: Open input image failed!\n");
        exit(1);
    }
    printf("[BSCALER]: load image success\n");
    int box_num = info_t->box_num;

    int dst_w = 96;//atoi(argv[4]);
    int dst_h = 96;//atoi(argv[5]);
    int dst_size = dst_w * dst_h * 4 * box_num;

    uint8_t *dst_base = (uint8_t *)ddr_memalign(8, dst_size);
    // printf("pid:%2lu,dst_base:%d\n",pthread_self(),dst_base);

    if (dst_base == NULL) {
        fprintf(stderr, "[BSCALER]: dst alloc failed!\n");
    }
    memset(dst_base, 255, dst_size);

    int src_size = img_w * img_h * 4;
    uint8_t *src_base = (uint8_t *)ddr_memalign(8, src_size);
    if (src_base == NULL) {
        fprintf(stderr, "[BSCALER]: src alloc failed!\n");
    }

    memset(src_base, 0, src_size);
    memcpy(src_base, img, src_size);

    data_info_s src;
    src.base = src_base;
    src.base1 = NULL;
    src.format = BS_DATA_BGRA;
    src.chn = 4;
    src.width = img_w;
    src.height = img_h;
    src.line_stride = img_w * 4;
    src.locate = BS_DATA_NMEM;

    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
    // printf("pid:%2lu,dst:%d\n",pthread_self(),dst);
    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i * dst_w * dst_h * 4;
        dst[i].base1 = NULL;
        dst[i].format = BS_DATA_BGRA;
        dst[i].chn = 4;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_w * 4;
        dst[i].locate = BS_DATA_NMEM;
    }

    box_resize_info_s *infos = (box_resize_info_s *)malloc(sizeof(box_affine_info_s) * box_num);
    // printf("pid:%2lu,info:%d\n",pthread_self(),infos);
    for (int i = 0; i < box_num; i++) {
        int src_x = info_t->box_t[i][0];
        int src_y = info_t->box_t[i][1];
        int src_w = info_t->box_t[i][2];
        int src_h = info_t->box_t[i][3];

        infos[i].box.x = src_x;
        infos[i].box.y = src_y;
        infos[i].box.w = src_w;
        infos[i].box.h = src_h;
        infos[i].wrap = 0;
        infos[i].zero_point = 0;
    }

    if (ddr_cache_attr & NNA_CACHED) {
        __aie_flushcache((void *)src_base, src_size);
    }
    //pthread_mutex_lock(&mem_lock);
    printf("[BSCALER]: bscaler resize start\n");
    bs_resize_start(&src, box_num, dst, infos, coef, offset);
    printf("[BSCALER]: bscaler wait finish ...\n");
    bs_resize_wait();
    printf("[BSCALER]: bscaler resize finish ..\n");
    //pthread_mutex_unlock(&mem_lock);
    //printf("pid:%lu sem release\n",pthread_self());
    if (ddr_cache_attr & NNA_CACHED) {
        __aie_flushcache((void *)dst_base, dst_size);
    }

    gettimeofday(&end, NULL);
    time_use = (float)((end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)) / 1000.0f;
    // printf("resize time cost:%3fms\n",time_use);

    char str[128];
    sprintf(str,"result_out/resize_pthread:%d.jpg",info_t->pid);
    stbi_write_jpg(str, dst_w, dst_h * box_num, 4, dst_base, 90);
    ddr_free(dst_base);
    ddr_free(src_base);
    free(dst);
    free(infos);
    stbi_image_free(img);

}

int main(int argc,char** argv)
{

    int ret = 0;

    if ((ret = __aie_mmap(0x4000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr)) == 0) { //64MB
        fprintf(stderr,"nna box_base virtual generate failed !!\n");
        return 0;
    }

    int thread_num;// = atoi(argv[1]);
    int cycle = atoi(argv[1]);
    int mode;

    thread_info* info[100];
    pthread_t id_tb[100];

    // int (*p)[4];
    // p = box;

    char img_jpg[40]  =  "../image/entertainers.jpg";

    char img_nv12[40] = "../image/Meeting_1024x1024.nv12";

    while(cycle>0){
        printf("cycle:%d\n",cycle);
        thread_num = rand()%6 + 1;
        for(int i=0;i<thread_num;i++){

            info[i] = (thread_info *)malloc(sizeof(thread_info));
            info[i]->pid = i;
            info[i]->box_num  = 33;
            mode =  rand()%3;

            if(mode==0){
                info[i]->file_name = img_jpg;
                memcpy(info[i]->box_t,box,info[i]->box_num * 4 * sizeof(int));
                pthread_create(&id_tb[i],NULL,resize,(void *)info[i]);

            }
            else if(mode==1){
                // printf("box:%d, %s\n",rand()%25,argv[index]);
                info[i]->file_name = img_jpg;
                memcpy(info[i]->box_t,box,info[i]->box_num * 4 * sizeof(int));
                pthread_create(&id_tb[i],NULL,affine,(void *)info[i]);

            }
            else if(mode==2){
                info[i]->file_name = img_nv12;
                pthread_create(&id_tb[i],NULL,convert,(void *)info[i]);
            }
            else{
                fprintf(stderr,"fail to this mode\n");
                assert(0) ;
            }

        }
        for(int i=0;i<thread_num;i++)
        {
            pthread_join(id_tb[i],NULL);
        }

        for(int i=0;i<thread_num;i++){
            free(info[i]);
        }

        cycle--;
    }

    __aie_munmap();
   return 0;

}

/*
int main(int argc,char** argv)
{

    int ret = 0;

    if ((ret = __aie_mmap(0x4000000, 1, desram_cache_attr, oram_cache_attr, ddr_cache_attr)) == 0) { //64MB
        fprintf(stderr,"nna box_base virtual generate failed !!\n");
        return 0;
    }

    if(argc<3)
       fprintf(stderr,"./thread_test img mode img mode (mode = resize|affine|convert)\n");

    if(argc>20){
        fprintf(stderr,"the max thread num is 10\n");
        assert(0);
      }

    int thread_num = (argc-1)/2;

    thread_info* info[10];

    pthread_t id_tb[10];

    int (*p)[4];
    p = box;

    int index=1;

    for(int i=0;i<thread_num;i++){

        info[i] = (thread_info *)malloc(sizeof(thread_info));
        info[i]->file_name = argv[index++];
        info[i]->pid = i;
        info[i]->box_num  = 8;

        if(strcmp(argv[index],"resize")==0){
           memcpy(info[i]->box_t,p+rand()%25,info[i]->box_num * 4 * sizeof(int));
           pthread_create(&id_tb[i],NULL,resize,(void *)info[i]);

        }
        else if(strcmp(argv[index],"affine")==0){
            // printf("box:%d, %s\n",rand()%25,argv[index]);
            memcpy(info[i]->box_t,p+rand()%25,info[i]->box_num * 4 * sizeof(int));
             pthread_create(&id_tb[i],NULL,affine,(void *)info[i]);

        }
        else if(strcmp(argv[index],"convert")==0){
            pthread_create(&id_tb[i],NULL,convert,(void *)info[i]);
        }
        else{
            fprintf(stderr,"./thread_test img mode img mode (mode = resize|affine|convert)\n");
            assert(0) ;
        }

        index++;
    }

      for(int i=0;i<thread_num;i++)
    {
        pthread_join(id_tb[i],NULL);
    }

    for(int i=0;i<thread_num;i++){
        free(info[i]);
    }

    __aie_munmap();
   return 0;

   }*/
