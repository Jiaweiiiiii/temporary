/*
 *        (C) COPYRIGHT Ingenic Limited.
 *             ALL RIGHTS RESERVED
 *
 * File       : dump_t_vector.c
 * Authors    : jmqi@joshua
 * Create Time: 2020-06-26:16:17:21
 * Description:
 *
 */

#include <stdio.h>
#include "dump_t_vector.h"

void dump_t_once_vector(FILE *fpo, uint32_t frame, bst_hw_once_cfg_s *cfg)
{
    fprintf(fpo, "bst_hw_once_cfg_s t_cfg%d[] = {\n", frame);
    fprintf(fpo, "    %d,//src_format,fixme:eg BST_HW_DATA_FM_BGRA\n", cfg->src_format);
    fprintf(fpo, "    %d,//width\n", cfg->src_w);
    fprintf(fpo, "    %d,//height\n", cfg->src_h);
    fprintf(fpo, "    NULL,//src_base0\n");
    fprintf(fpo, "    NULL,//src_base1\n");
    fprintf(fpo, "    %d,//src_line_stride\n", cfg->src_line_stride);
    fprintf(fpo, "    %d,//dst_format,fixme:eg BST_HW_DATA_FM_BGRA\n", cfg->dst_format);
    fprintf(fpo, "    %d,//dst_w\n", cfg->dst_w);
    fprintf(fpo, "    %d,//dst_h\n", cfg->dst_h);
    fprintf(fpo, "    NULL,//dst_base0\n");
    fprintf(fpo, "    NULL,//dst_base1\n");
    fprintf(fpo, "    %d,//dst_line_stride\n", cfg->dst_line_stride);
    fprintf(fpo, "    %d,//dst_plane_stride\n", cfg->dst_plane_stride);

    fprintf(fpo, "    %d,//kernel_size\n", cfg->kernel_size);
    fprintf(fpo, "    %d,//kernel_xstride\n", cfg->kernel_xstride);
    fprintf(fpo, "    %d,//kernel_ystride\n", cfg->kernel_ystride);
    fprintf(fpo, "    %d,//zero_point\n", cfg->zero_point);
    fprintf(fpo, "    %d,//pad_left\n", cfg->pad_left);
    fprintf(fpo, "    %d,//pad_right\n", cfg->pad_right);
    fprintf(fpo, "    %d,//pad_top\n", cfg->pad_top);
    fprintf(fpo, "    %d,//pad_bottom\n", cfg->pad_bottom);
    fprintf(fpo, "    %d,//task_len\n", cfg->task_len);
    fprintf(fpo, "    %d,//chain_bus\n", cfg->chain_bus);
    fprintf(fpo, "    %d,//ibuft_bus\n", cfg->ibuft_bus);
    fprintf(fpo, "    %d,//obuft_bus\n", cfg->obuft_bus);
    fprintf(fpo, "    %d, //nv2bgr_alpha\n", cfg->nv2bgr_alpha);
    fprintf(fpo, "    {");
    for (int i = 0; i < 9; i++) {
        fprintf(fpo, "%d, ", cfg->nv2bgr_coef[i]);
    }
    fprintf(fpo, "}, //nv2bgr_coef\n");

    fprintf(fpo, "    {");
    for (int i = 0; i < 2; i++) {
        fprintf(fpo, "%d, ", cfg->nv2bgr_ofst[i]);
    }
    fprintf(fpo, "}, //nv2bgr_ofst\n");

    fprintf(fpo, "    %d,//isum\n", cfg->isum);
    fprintf(fpo, "    %d,//osum\n", cfg->osum);
    fprintf(fpo, "    %d,//irq_mask\n", cfg->irq_mask);
    fprintf(fpo, "};\n\n");

    // src data
#if 1
    if (cfg->src_format == 0) {//nv12
        fprintf(fpo, "__place_k0_data__ uint8_t t_src%d0[] = {\n", frame);
        for (int j = 0; j < cfg->src_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_line_stride; i++) {
                fprintf(fpo, "0x%02x,", cfg->src_base0[j * cfg->src_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");

        fprintf(fpo, "__place_k0_data__ uint8_t t_src%d1[] = {\n", frame);
        for (int j = 0; j < cfg->src_h/2; j++) {
            for (int i = 0; i < cfg->src_line_stride; i++) {
                fprintf(fpo, "0x%02x,", cfg->src_base1[j * cfg->src_line_stride + i]);
            }
        }
        fprintf(fpo, "};\n\n");
    } else if (cfg->src_format == 1) {//bgr
        uint32_t bpp = 4;
        fprintf(fpo, "__place_k0_data__ uint8_t t_src%d0[] = {\n", frame);
        printf("cfg->src_line_stride=%d, cfg->src_h=%d\n", cfg->src_line_stride, cfg->src_h);
        for (int j = 0; j < cfg->src_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_line_stride; i++) {
                fprintf(fpo, "0x%02x,",
                        cfg->src_base0[j * cfg->src_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");
        fprintf(fpo, "__place_k0_data__ uint8_t *t_src%d1 = NULL;\n\n", frame);
    } else {
        printf("[Error]:[%s]\n", __func__);
    }

    // golden data
    if (cfg->dst_format == 0) {//nv12
        fprintf(fpo, "uint8_t t_gld_dst%d[] = {\n", frame);
        fprintf(fpo, "/**** dst_base 0 ****/\n");
        for (int j = 0; j < cfg->src_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_w; i++) {
                fprintf(fpo, "0x%02x,", (cfg->dst_base0)[j * cfg->dst_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "\n/**** dst_base 1 ****/\n");
        for (int j = 0; j < cfg->src_h/2; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_w; i++) {
                fprintf(fpo, "0x%02x,", (cfg->dst_base1)[j * cfg->dst_line_stride + i]);
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");
    } else if (cfg->dst_format == 1) {//bgr
        uint32_t bpp_mode = (cfg->dst_format >> 5) & 0x3;
        uint32_t bpp = 1 << (bpp_mode + 2);
        fprintf(fpo, "uint8_t t_gld_dst%d[] = {\n", frame);
        fprintf(fpo, "/**** dst_base 0 ****/\n");
        for (int j = 0; j < cfg->src_h; j++) {
            fprintf(fpo, "/*%3d*/ ", j);
            for (int i = 0; i < cfg->src_w; i++) {
                for (int k = 0; k < bpp; k++) {
                    fprintf(fpo, "0x%02x,",
                            (cfg->dst_base0)[j * cfg->dst_line_stride + i * bpp + k]);
                }
            }
            fprintf(fpo, "\n");
        }
        fprintf(fpo, "};\n\n");
    } else if (cfg->dst_format == 3) {//virchn
        int chn_group = ((cfg->kernel_size == 3) ? 5 :
                         (cfg->kernel_size == 2) ? 3 : 1);
        int kw = ((cfg->kernel_size == 3) ? 7 :
                  (cfg->kernel_size == 2) ? 5 :
                  (cfg->kernel_size == 1) ? 3 : 1);
        int kh = kw;
        int ow = (cfg->src_w + cfg->pad_left + cfg->pad_right - kw) / cfg->kernel_xstride + 1;
        int oh = (cfg->src_h + cfg->pad_top + cfg->pad_bottom - kh) / cfg->kernel_ystride + 1;
        fprintf(fpo, "uint8_t t_gld_dst%d[] = {\n", frame);
        for (int n = 0; n < chn_group; n++) {
            fprintf(fpo, "/**** group 0 ****/\n");
            for (int j = 0; j < oh; j++) {
                for (int i = 0; i < ow; i++) {
                    fprintf(fpo, "/*%3d*/ ", j);
                    int idx = n * cfg->dst_plane_stride + j * cfg->dst_line_stride +
                        i * 32;
                    for (int k = 0; k < 32; k++) {
                        fprintf(fpo, "0x%02x,",    (cfg->dst_base0)[idx + k]);
                    }
                    fprintf(fpo, "\n");
                }
            }
        }
        fprintf(fpo, "};\n\n");
    } else {
        printf("dst format Unkown\n");
    }
#endif
}

void dump_t_vector(bst_hw_once_cfg_s *cfg, uint32_t seed, char* note)
{
    FILE *fpo;
    fpo = fopen("t_vector_t.h", "w+");
    if (fpo == NULL) {
        fprintf(stderr, "open t_vector_t.h failed!\n");
        exit(1);
    }

    fprintf(fpo, "/* %s */\n", note);
    fprintf(fpo, "/* seed = 0x%08x */\n\n", seed);

    fprintf(fpo, "#ifndef __T_VECTOR_T_H__\n");
    fprintf(fpo, "#define __T_VECTOR_T_H__\n\n");
    fprintf(fpo, "#include \"../common/bscaler_hal.h\"\n\n");

        dump_t_once_vector(fpo, 0, cfg);

    fprintf(fpo, "#endif /* __T_VECTOR_T_H__ */\n");

}

void dump_t_chain_vector(uint32_t frame, bst_hw_once_cfg_s *cfg,
                                                 bs_chain_ret_s *chain_info, uint32_t seed, char* note)
{
        FILE *fpo;
    fpo = fopen("t_vector_t_chain.h", "w+");
    if (fpo == NULL) {
        fprintf(stderr, "open t_vector_t_chain.h failed!\n");
        exit(1);
    }
    fprintf(fpo, "/* %s */\n", note);
    fprintf(fpo, "/* seed = 0x%08x */\n\n", seed);

    fprintf(fpo, "#ifndef __T_VECTOR_T_CHAIN_H__\n");
    fprintf(fpo, "#define __T_VECTOR_T_CHAIN_H__\n\n");
    fprintf(fpo, "#include \"../common/bscaler_hal.h\"\n\n");

        fprintf(fpo, "__place_k0_data__ uint32_t t_chain_base[] = {\n");
        for (int j = 0; j < chain_info->bs_ret_len/4; j ++) {
                fprintf(fpo, "0x%02x,",
                                chain_info->bs_ret_base[j]);
                if((chain_info->bs_ret_base[j] & 0x80000000)>>31){
                        fprintf(fpo, "\n/*  next chain*/");
                }
        }
        fprintf(fpo, "\n};\n\n");

        for (uint32_t n = 0; n < frame; n ++) {
                dump_t_once_vector(fpo, n, &cfg[n]);
        }

        fprintf(fpo, "bst_hw_once_cfg_s* t_cfg[] = {\n");
        for (uint32_t n = 0; n < frame; n ++) {
                fprintf(fpo, "t_cfg%d,", n);
        }
        fprintf(fpo, "\n};\n\n");

        fprintf(fpo, "uint8_t* t_src[] = {\n");
        for (uint32_t n = 0; n < frame; n ++) {
                fprintf(fpo, "t_src%d0, t_src%d1, ", n, n);
        }
        fprintf(fpo, "\n};\n\n");

        fprintf(fpo, "uint8_t* t_gld_dst[] = {\n");
        for (uint32_t n = 0; n < frame; n ++) {
                fprintf(fpo, "t_gld_dst%d, ", n);
        }
        fprintf(fpo, "\n};\n\n");

    fprintf(fpo, "#endif /* __T_VECTOR_T_CHAIN_H__ */\n");

}
