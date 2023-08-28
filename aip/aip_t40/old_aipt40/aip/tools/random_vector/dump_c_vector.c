/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : dump_c_vector.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:10:32:53
 * Description:
 *
 */

#include <stdio.h>
#include "dump_c_vector.h"

void dump_c_once_vector(FILE *fpo, uint32_t frame, bsc_hw_once_cfg_s *cfg)
{
    fprintf(fpo, "bsc_hw_once_cfg_s c_cfg%d[] = {\n",frame);
    fprintf(fpo, "    NULL,//src_base[]\n");
    fprintf(fpo, "    NULL,//src_base1[]\n");
    fprintf(fpo, "    %d,//src_format, fixme:eg BSC_HW_DATA_FM_BGRA\n", cfg->src_format);
    fprintf(fpo, "    %d,//src_line_stride\n", cfg->src_line_stride);
    fprintf(fpo, "    %d,//src_box_x\n", cfg->src_box_x);
    fprintf(fpo, "    %d,//src_box_y\n", cfg->src_box_y);
    fprintf(fpo, "    %d,//src_box_w\n", cfg->src_box_w);
    fprintf(fpo, "    %d,//src_box_h\n", cfg->src_box_h);
    fprintf(fpo, "    %d,//dst_format, fixme:eg BSC_HW_DATA_FM_BGRA\n", cfg->dst_format);

    fprintf(fpo, "    %d,//dst_line_stride\n", cfg->dst_line_stride);
    fprintf(fpo, "    %d,//dst_box_x\n", cfg->dst_box_x);
    fprintf(fpo, "    %d,//dst_box_y\n", cfg->dst_box_y);
    fprintf(fpo, "    %d,//dst_box_w\n", cfg->dst_box_w);
    fprintf(fpo, "    %d,//dst_box_h\n", cfg->dst_box_h);
    fprintf(fpo, "    %s,//affine\n", cfg->affine ? "true" : "false");
    fprintf(fpo, "    %s,//box_mode\n", cfg->box_mode ? "true" : "false");
    fprintf(fpo, "    %d,//y_gain_exp\n", cfg->y_gain_exp);
    fprintf(fpo, "    %d,//zero_point\n", cfg->zero_point);
    fprintf(fpo, "    %d, //nv2bgr_alpha\n",cfg->nv2bgr_alpha);

    fprintf(fpo, "    {");
    for (int i = 0; i < 9; i++) {
        fprintf(fpo, "%d, ", cfg->coef[i]);
    }
    fprintf(fpo, "}, //coef\n");

    fprintf(fpo, "    {%d, %d}, //offset\n", cfg->offset[0], cfg->offset[1]);

    fprintf(fpo, "    {");
    for (int i = 0; i < 9; i++) {
        fprintf(fpo, "%d, ", cfg->matrix[i]);
    }
    fprintf(fpo, "}, //matrix\n");

    fprintf(fpo, "    %d,//box_num\n", cfg->box_num);

    fprintf(fpo, "    {");
    for (int i = 0; i < 64; i++) {
        fprintf(fpo, "NULL, ");
    }
    fprintf(fpo, "}, //dst_base\n");

    fprintf(fpo, "    NULL,//boxes_info\n");
    fprintf(fpo, "    0x%02x,//mono_x\n", cfg->mono_x);
    fprintf(fpo, "    0x%02x,//mono_y\n", cfg->mono_y);

    fprintf(fpo, "    {");
    for (int i = 0; i < 64; i++) {
        fprintf(fpo, "0x%02x, ", cfg->extreme_point[i]);
    }
    fprintf(fpo, "}, //extreme_point\n");

    fprintf(fpo, "    0x%08x,//isum\n", cfg->isum);
    fprintf(fpo, "    0x%08x,//osum\n", cfg->osum);

    fprintf(fpo, "    0x%02x,//box_bus\n", cfg->box_bus);
    fprintf(fpo, "    0x%02x,//ibufc_bus\n", cfg->ibufc_bus);
    fprintf(fpo, "    0x%02x,//obufc_bus\n", cfg->obufc_bus);
    fprintf(fpo, "    0x%02x,//irq_mask\n", cfg->irq_mask);
    fprintf(fpo, "};\n\n");

    fprintf(fpo, "__place_k0_data__ uint32_t boxes_info%d[] = {\n", frame);
    if(cfg->box_mode == 0){
        for (int n = 0; n < cfg->box_num; n++) {
            for (int i = 0; i < 6; i++) {
                fprintf(fpo, "0x%08x,", cfg->boxes_info[n * 6 + i]);
            }
            fprintf(fpo, "\n");
        }
    }
    fprintf(fpo, "};\n\n");

    // src data
    if (cfg->src_format == BSC_HW_DATA_FM_NV12) {
        fprintf(fpo, "__place_k0_data__ uint8_t c_src%d0[] = {\n", frame);
        for (int j = 0; j < cfg->src_box_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_line_stride; i++) {
                fprintf(fpo, "0x%02x,", cfg->src_base0[j * cfg->src_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");

        fprintf(fpo, "__place_k0_data__ uint8_t c_src%d1[] = {\n", frame);
        for (int j = 0; j < cfg->src_box_h/2; j++) {
            for (int i = 0; i < cfg->src_line_stride; i++) {
                fprintf(fpo, "0x%02x,", cfg->src_base1[j * cfg->src_line_stride + i]);
            }
        }
        fprintf(fpo, "};\n\n");
    } else {
        fprintf(fpo, "__place_k0_data__ uint8_t c_src%d0[] = {\n", frame);
        for (int j = 0; j < cfg->src_box_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_line_stride; i++) {
                fprintf(fpo, "0x%02x,",
                        cfg->src_base0[j * cfg->src_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");
        fprintf(fpo, "__place_k0_data__ uint8_t *c_src%d1 = NULL;\n\n", frame);
    }

    // golden data
    if (cfg->dst_format == BSC_HW_DATA_FM_NV12) {
        fprintf(fpo, "uint8_t c_gld_dst%d[] = {\n", frame);
        fprintf(fpo, "/**** dst_base 0 ****/\n");
        for (int j = 0; j < cfg->dst_box_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->dst_box_w; i++) {
                fprintf(fpo, "0x%02x,", (cfg->dst_base[0])[j * cfg->dst_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "\n/**** dst_base 1 ****/\n");
        for (int j = 0; j < cfg->dst_box_h/2; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->dst_box_w; i++) {
                fprintf(fpo, "0x%02x,", (cfg->dst_base[1])[j * cfg->dst_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");
    } else {
        uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
        uint32_t bpp = 1 << (bpp_mode + 2);
        if (cfg->box_mode) {
            fprintf(fpo, "uint8_t c_gld_dst%d[] = {\n", frame);
            fprintf(fpo, "/**** dst_base 0 ****/\n");
            for (int j = 0; j < cfg->dst_box_h; j++) {
                fprintf(fpo, "/*%3d*/ ", j);
                for (int i = 0; i < cfg->dst_box_w; i++) {
                    for (int k = 0; k < bpp; k++) {
                        fprintf(fpo, "0x%02x,",
                                (cfg->dst_base[0])[j * cfg->dst_line_stride + i * bpp + k]);
                    }
                }
                fprintf(fpo, "\n");
            }
            fprintf(fpo, "};\n\n");
        } else {//line mode
            fprintf(fpo, "uint8_t c_gld_dst%d[] = {\n", frame);
            for (int n = 0; n < cfg->box_num; n++) {
                fprintf(fpo, "/**** dst_base %d ****/\n", n);
                for (int j = 0; j < cfg->dst_box_h; j++) {
                    fprintf(fpo, "/*%3d*/ ", j);
                    for (int i = 0; i < cfg->dst_box_w; i++) {
                        for (int k = 0; k < bpp; k++) {
                            int idx = j * cfg->dst_line_stride + i * bpp + k;
                            fprintf(fpo, "0x%02x,", (cfg->dst_base[n])[idx]);
                        }
                    }
                    fprintf(fpo, "\n");
                }
            }
            fprintf(fpo, "};\n\n");
        }
    }
}
void dump_c_vector(bsc_hw_once_cfg_s *cfg, uint32_t seed, char* note)
{
    FILE *fpo;
    fpo = fopen("t_vector_c.h", "w+");
    if (fpo == NULL) {
        fprintf(stderr, "open t_vector_c.h failed!\n");
        exit(1);
    }
    fprintf(fpo, "/* %s */\n", note);
    fprintf(fpo, "/* seed = 0x%08x */\n\n", seed);

    fprintf(fpo, "#ifndef __T_VECTOR_C_H__\n");
    fprintf(fpo, "#define __T_VECTOR_C_H__\n\n");
    fprintf(fpo, "#include \"../common/bscaler_hal.h\"\n\n");

    dump_c_once_vector(fpo, 0, cfg);

    fprintf(fpo, "#endif /* __T_VECTOR_C_H__ */\n");

}
void dump_c_chain_vector(uint32_t frame, bsc_hw_once_cfg_s *cfg,
                         bs_chain_ret_s *chain_info, uint32_t seed, char* note)
{
    FILE *fpo;
    fpo = fopen("t_vector_c_chain.h", "w+");
    if (fpo == NULL) {
        fprintf(stderr, "open t_vector_c_chain.h failed!\n");
        exit(1);
    }
    fprintf(fpo, "/* %s */\n", note);
    fprintf(fpo, "/* seed = 0x%08x */\n\n", seed);

    fprintf(fpo, "#ifndef __T_VECTOR_C_CHAIN_H__\n");
    fprintf(fpo, "#define __T_VECTOR_C_CHAIN_H__\n\n");
    fprintf(fpo, "#include \"../common/bscaler_hal.h\"\n\n");

#if 0
    fprintf(fpo, "bs_chain_ret_s chain_info = {\n");
    fprintf(fpo, "    NULL,//chain_base\n");
    fprintf(fpo, "    %d,//chain_len\n",chain_info->bs_ret_len);

    fprintf(fpo, "};\n\n");
#endif

    fprintf(fpo, "__place_k0_data__ uint32_t c_chain_base[] = {\n");
    for (int j = 0; j < chain_info->bs_ret_len/4; j ++) {
        fprintf(fpo, "0x%02x,",
                chain_info->bs_ret_base[j]);
        if((chain_info->bs_ret_base[j] == 0x80000000)){
            fprintf(fpo, "\n/*  next chain*/");
        }
    }
    fprintf(fpo, "\n};\n\n");

    for (uint32_t n = 0; n < frame; n ++) {
        dump_c_once_vector(fpo, n, &cfg[n]);
    }

    fprintf(fpo, "bsc_hw_once_cfg_s* c_cfg[] = {\n");
    for (uint32_t n = 0; n < frame; n ++) {
        fprintf(fpo, "c_cfg%d,", n);
    }
    fprintf(fpo, "\n};\n\n");

    fprintf(fpo, "uint32_t* boxes_info[] = {\n");
    for (uint32_t n = 0; n < frame; n ++) {
        fprintf(fpo, "boxes_info%d, ", n);
    }
    fprintf(fpo, "\n};\n\n");

    fprintf(fpo, "uint8_t* c_src[] = {\n");
    for (uint32_t n = 0; n < frame; n ++) {
        fprintf(fpo, "c_src%d0, c_src%d1, ", n, n);
    }
    fprintf(fpo, "\n};\n\n");

    fprintf(fpo, "uint8_t* c_gld_dst[] = {\n");
    for (uint32_t n = 0; n < frame; n ++) {
        fprintf(fpo, "c_gld_dst%d, ", n);
    }
    fprintf(fpo, "\n};\n\n");

    fprintf(fpo, "#endif /* __T_VECTOR_C_CHAIN_H__ */\n");
}
