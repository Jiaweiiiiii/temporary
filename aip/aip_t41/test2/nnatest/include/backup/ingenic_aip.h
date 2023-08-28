
#ifndef __INGENIC_AIP_H__
#define __INGENIC_AIP_H__

typedef enum {
	// do not modify this value
	// bit2~bit0 is AIP f config register bw
        BS_DATA_Y       = 0, 
        BS_DATA_UV      = 1,                                      
        BS_DATA_BGRA    = 2 | (0  << 16), 
        BS_DATA_GBRA    = 2 | (1  << 16), 
        BS_DATA_RBGA    = 2 | (2  << 16), 
        BS_DATA_BRGA    = 2 | (3  << 16), 
        BS_DATA_GRBA    = 2 | (4  << 16), 
        BS_DATA_RGBA    = 2 | (5  << 16),
        BS_DATA_ABGR    = 2 | (6  << 16),
        BS_DATA_AGBR    = 2 | (7  << 16),
        BS_DATA_ARBG    = 2 | (8  << 16),
        BS_DATA_ABRG    = 2 | (9  << 16),
        BS_DATA_AGRB    = 2 | (10 << 16),
        BS_DATA_ARGB    = 2 | (11 << 16),                           
        BS_DATA_FMU2    = 3,                           
        BS_DATA_FMU4    = 4,                           
        BS_DATA_FMU8    = 5,                           
        BS_DATA_FMU8_H  = 6,                                       
        BS_DATA_NV12    = 7,                                      
} bs_data_format_e;   

typedef enum {
	// do not modify this value
	// bit0: 0 is DDR
	// bit0: 1 is ORAM
        BS_DATA_NMEM    = 0,//data in nmem
        BS_DATA_ORAM    = 1,//data in oram
        BS_DATA_RMEM    = 2,//data in rmem
} bs_data_locate_e;

typedef struct {                       
        int                     x;
        int                     y;
        int                     w;
        int                     h;
} box_info_s;

typedef struct {
        box_info_s              box;//source box
        float                   matrix[9];//fixme
        uint32_t                wrap;
        uint32_t                zero_point;
} box_affine_info_s;

typedef struct {
        uint32_t                base;
        uint32_t                base1;
        bs_data_format_e        format;
        int                     chn;
        int                     width;
        int                     height;
        int                     line_stride;
        bs_data_locate_e        locate;
} data_info_s;

extern
int ingenic_aip_resize_process(data_info_s *src,
			       int box_num,
			       const data_info_s *dst,
                	       const box_affine_info_s *boxes,
                	       const uint32_t *coef,
			       const uint32_t *offset);

#endif
