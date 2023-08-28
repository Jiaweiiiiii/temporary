#include <stdio.h>
#include <stdlib.h>
#include "image_process.h"//not necessary
#include "bscaler_api.h"
#include "bscaler_hal.h"
#include "aie_mmap.h"

char perf_array[48] = {0};
int fperf;

#define ALIGN_BYTE  64
#define ADDRS_ALIGN(addrs, bytes) (((size_t)(addrs) + ((bytes) - 1)) & (~((bytes) - 1)))
#define NUM_ALIGN(val, T) ((val + T - 1) & (~(T-1)))

void print_data_info(data_info_s* src)
{
    printf("///////////////////////\n");
    printf("base %p\n", src->base);
    printf("format %d\n", src->format);
    printf("chn %d\n", src->chn);
    printf("width %d\n", src->width);
    printf("height %d\n", src->height);
    printf("line_stride %d\n", src->line_stride);
    printf("locate %d\n", src->locate);
    printf("///////////////////////\n");
}
void set_alpha(char* filename, void* src_, int x, int y, int w, int h, int c, int stride)
{
    int i,j, k;
    int osize = w*h*c;
    uint8_t* dst = (uint8_t*)malloc(osize);
    if(!dst)
        return;
    int dstride = w*c;

    uint8_t* src = (uint8_t*)src_;

    for(i=0; i<h; i++){
        for(j=0; j<w; j++){
            uint8_t* sptr = src + (i+y)*stride + (j+x)*c;
            uint8_t* dptr = dst + i*dstride + j*c;
            for(k=0; k<c-1; k++){
                dptr[k] = sptr[k];
            }
            dptr[k] = 255;
        }
    }

    stbi_write_png(filename, w, h, c, dst, dstride);

    free(dst);
    dst = NULL;
}

int main(int argc, char** argv)
{
    //bscaler_initt();
    bscaler_mem_init();
    int ret = 0;
    int b_use_rmem = 1;
    nna_cache_attr_t desram_cache_attr = NNA_UNCACHED;
    //nna_cache_attr_t oram_cache_attr = NNA_UNCACHED_ACCELERATED;
    //nna_cache_attr_t ddr_cache_attr = NNA_CACHED;
    nna_cache_attr_t oram_cache_attr = NNA_UNCACHED;
    nna_cache_attr_t ddr_cache_attr = NNA_UNCACHED;
    if (__aie_mmap(0x4000000, b_use_rmem, desram_cache_attr, oram_cache_attr, ddr_cache_attr) == 0)
    {
        printf ("DDR malloc init faild\n");
        return -1;
    }

    if (argc < 5) {
        printf("./main img_h img_w img_c scale\n");
        return -1;
    }

    int img_h = atoi(argv[1]);
    int img_w = atoi(argv[2]);
    int img_chn = atoi(argv[3]);
    float scale = atof(argv[4]);
    int img_bit = atoi(argv[5]);//8;input bitwidth
    int dst_bit = atoi(argv[6]);//8;output bitwidth
    int dst_chn = img_chn;//4;
    int box_num = 64;
    int dst_w, dst_h;
    if(scale==0){
        if (argc <10 ) {
            printf("./main img_h img_w img_c scale dst_h dstw dstc\n");
            return -1;
        }
        dst_h = atoi(argv[7]);
        dst_w = atoi(argv[8]);
        dst_chn = atoi(argv[9]);
    }else{
        dst_w = (int)((float)img_w/scale);
        dst_h = (int)((float)img_h/scale);
        dst_chn = img_chn;
    }
    bs_data_format_e img_format = (img_bit == 2) ? BS_DATA_FMU2 : (img_bit == 4) ? BS_DATA_FMU4 : BS_DATA_FMU8;
    bs_data_format_e dst_format = (dst_bit == 2) ? BS_DATA_FMU2 : (dst_bit == 4) ? BS_DATA_FMU4 : BS_DATA_FMU8;

    printf("IN: %d, %d, %d, %d\n", img_h, img_w, img_chn, img_bit);
    printf("scale %f\n", scale);
    printf("OUT: %d, %d, %d, %d\n", dst_h, dst_w, dst_chn, dst_bit);
    if(((img_bit!=2)&&(img_bit!=4)&&(img_bit!=8))||((dst_bit!=2)&&(dst_bit!=4)&&(dst_bit!=8)))
    {
        printf("ERROR %s: %d Bad param !!!!!!\n",__func__,__LINE__);
        printf("Support Input biwidt: 2, 4, 8\n");
        return -1;
    }

    /****************************************************/
    int src_line_stride, src_size;//FMU2,FMU4,FMU8
    int fp_num = img_chn/32;
    src_line_stride = (img_w*32*img_bit)/8;
    src_line_stride = NUM_ALIGN(src_line_stride,ALIGN_BYTE);
    src_size = img_h*src_line_stride*fp_num;

    int dst_line_stride, dst_size;
    dst_line_stride = (dst_w*32*dst_bit)/8;
    dst_line_stride = NUM_ALIGN(dst_line_stride,ALIGN_BYTE);
    dst_size = dst_h*dst_line_stride*box_num*fp_num;

    printf("%d  %d  %d\n",dst_w,dst_line_stride,dst_size);
    //malloc
    printf("malooc src_base and dst_base:   %s, %d\n",__func__,__LINE__);
    uint8_t *src_base = (uint8_t *)bscaler_malloc(64, src_size);
    if (src_base == NULL) {
        printf("Error %s: %d alloc failed!    src_size:%d\n", __func__, __LINE__,src_size);
        return -1;
    }
    uint8_t *dst_base = (uint8_t *)bscaler_malloc(64, dst_size);
    if (dst_base == NULL) {
        printf("Error %s: %d alloc failed!   dst_size:%d\n", __func__, __LINE__,dst_size);
        return -1;
    }
    //---------------load output data-------------------//
    uint8_t *compare_base = (uint8_t *)bscaler_malloc(64, dst_size);
   /*if (compare_base == NULL) {
        printf("Error %s: %d alloc failed!\n", __func__, __LINE__);
        return -1;
    }
     int handle_out = open("./output.data", O_RDONLY);
     if(handle_out==-1)
    {
         printf("Error: %s:%d open failed\n",__func__,__LINE__);
         return -1;
     }
     if(dst_size != read(handle_out,compare_base, dst_size))
    {
         printf("Error %s:%d read failed\n", __func__,__LINE__);
         return -1;
     }
     close(handle_out);*/

    //-----------init src_base--------------//
    //memset(dst_base, 255, dst_size);//init dst_base
    //memset(compare_base,0 , dst_size);//init compare_base

    //char input_name[2048];
    //sprintf(input_name, "./input_feature_%dbit.data",img_bit);
    //printf("Load input_data: %s\n",input_name);
    int handle = open("./input_feature_8bit.data", O_RDONLY);
    if(handle==-1){
        printf("Error: %s:%d open failed\n",__func__,__LINE__);
        return -1;
    }
    if(src_size != read(handle, src_base, src_size)){
        printf("Error %s:%d read failed\n", __func__,__LINE__);
        return -1;
    }
    close(handle);

    //--------------load box data--------------//
    int box_size = box_num * 4 * (32/8);
    int *box_base = (int *)bscaler_malloc(64, box_size);
    if (box_base == NULL)
    {
        printf("Error %s: %d alloc failed!\n", __func__, __LINE__);
        return -1;
    }

    //char box_name[2048];
    //sprintf(box_name, "./boxes_%dbit.data",img_bit);
   // printf("Load box_data: %s\n",box_name);
     int handle_box = open("./boxes.data", O_RDONLY);
     if(handle_box==-1)
    {
         printf("Error: %s:%d open failed\n",__func__,__LINE__);
         return -1;
     }
     if(box_size != read(handle_box,box_base, box_size))
    {
         printf("Error %s:%d read failed    box_size:%d\n", __func__,__LINE__,box_size);
         return -1;
     }
     close(handle_box);
    //-------------load box end--------------------//

    /*----------------------------------------*/
    printf("set parm:  %s, %d\n",__func__,__LINE__);
    data_info_s *dst = (data_info_s *)malloc(sizeof(data_info_s) * box_num);
    if(!dst){
        printf("Error %s: %d alloc failed!\n", __func__, __LINE__);
        return -1;
    }
    box_resize_info_s *infos = (box_resize_info_s *)malloc(sizeof(box_resize_info_s) * box_num);
    if(!infos){
        printf("Error %s: %d alloc failed!\n", __func__, __LINE__);
        return -1;
    }
    data_info_s src;
    src.base = src_base;
    src.base1 = NULL;
    src.format = img_format;
    src.chn = img_chn;
    src.width = img_w;
    src.height = img_h;
    src.line_stride = src_line_stride;
    src.locate = 0;//0-ddr, 1-oram
    print_data_info(&src);

    for (int i = 0; i < box_num; i++) {
        dst[i].base = dst_base + i*(dst_h*dst_line_stride*fp_num);
        dst[i].base1 = NULL;
        dst[i].chn = dst_chn;
        dst[i].format = dst_format;
        dst[i].height = dst_h;
        dst[i].width = dst_w;
        dst[i].line_stride = dst_line_stride;
        dst[i].locate = 0;
        //print_data_info(&dst[i]);
    }

    for (int i = 0; i < box_num; i++) {
        infos[i].box.x = round(box_base[4*i]/16);
        infos[i].box.y = round(box_base[4*i+1]/16);
        infos[i].box.w = round(box_base[4*i+2]/16);
        infos[i].box.h = round(box_base[4*i+3]/16);

        printf("src: %d %d %d %d\n",infos[i].box.x, infos[i].box.y, infos[i].box.w, infos[i].box.h);

        //int xy_00 = (infos[i].box.x + 0)*32 + (infos[i].box.y)*src_line_stride;
        //int xy_01 = ((infos[i].box.x)+0)*32 + (infos[i].box.y + 1)*src_line_stride;
        //int xy_10 = (infos[i].box.x + 1)*32 + ((infos[i].box.y)+0)*src_line_stride;
        //int xy_11 = ((infos[i].box.x) + 1)*32 + ((infos[i].box.y) + 1)*src_line_stride;
        //printf("index: %d %d %d %d\n",xy_00,xy_01,xy_10,xy_11);
        //printf("box_0: %d %d %d %d\n",infos[0].box.x,infos[0].box.y,infos[0].box.w,infos[0].box.h);
        //printf("(0,0):%d  (0,1):%d  (1,0):%d  (1,1):%d\n",src_base[xy_00],src_base[xy_01],src_base[xy_10],src_base[xy_11]);

        //for(int j=0;j<10;j++)
        //{
        //    printf("%d\n",src_base[xy_00 + j]);
        //}
    }

    /*BGRA*/
    const uint32_t coef_bgra[9] = {0x129fbe, 0x198937, 0,
                                   0x129fbe, 0x64189, 0xd020c,
                                   0x129fbe, 0, 0x2049ba};

    const uint32_t offset[2] = {16, 128};
    const uint32_t *coef;
    coef = coef_bgra;
    printf("----------------start.....\n");
    bscaler_write_reg(BSCALER_FRMC_TIMEOUT, 0);
    //OPENPMON;
    int iter_num = 1;
    while(iter_num--)
    {
        printf("============ start ===============\n");
        bs_resize_start(&src, box_num, dst, infos, coef, offset);
        printf("============ wait ===============\n");
        START0;
        bs_resize_wait();
        printf("============ wait finish ===============\n");
        __aie_flushcache_dir(dst_base, dst_size, NNA_DMA_FROM_DEVICE);
        STOP();
    }
    CLOSEFS;
    uint32_t time = bscaler_read_reg(BSCALER_FRMC_TIMEOUT, 0);
    printf("total cycle: %d -- %d/box\n", (0xFFFFFFFF - time), (0xFFFFFFFF - time) / box_num);

    //return 0;
    //data_info_s *dst_mdl = dst;
    //dst_mdl[0].base = compare_base;
    //printf("%p  %p  %p\n",dst_base,dst_mdl[0].base,compare_base);
    // bs_resize_mdl(&src,box_num,dst,infos,coef,offset);
    //__aie_flushcache_dir(dst_base, dst_size, NNA_DMA_FROM_DEVICE);
    //stbi_write_png("resize_out.png", dst_w, dst_h * box_num, dst_chn, dst_base, dst_w*dst_chn);
    //set_alpha("resize_out.png", dst_base, 0, 0, dst_w, dst_h, dst_chn, dst_line_stride);
    //stbi_image_free(img);
    //---------------compare with ndl--------------//

    for(int i =0;i<16;i++)
    {
        printf("%d   %d\n",dst_base[i],compare_base[i]);
    }

    int flag = 0;
    for(int i=0;i<dst_size;i++)
    {
        if(dst_base[i]!=compare_base[i])
        {
            flag =1;
            printf("ERROR: index:%d   dst_base(%d) != compare_base(%d)\n",i,dst_base[i],compare_base[i]);
            return -1;
        }
    }
    if(!flag)
    {
        printf("-----------^_^------------\n");
    }

    ddr_free(dst_base);
    ddr_free(src_base);
    free(dst);
    free(infos);
    return 0;
}
