Ingenic artificial intelligence preprocess(AIP) application programming interface
=====================================

overview
-------------------
function:
`void* aip_version(void)`

`int ingenic_aip_init(void);`

`int ingenic_aip_deinit(void);`

`int ingenic_aip_affine_process(data_info_s              *src,
                                const int                box_num,
                                const data_info_s        *dst,
                                const box_affine_info_s  *boxes,
                                const uint32_t           *coef,
                                const uint32_t           *offset);`

`int ingenic_aip_perspective_process(data_info_s             *src,
                                     const int               box_num,
                                     const data_info_s       *dst,
                                     const box_affine_info_s *boxes,
                                     const uint32_t          *coef,
                                     const uint32_t          *offset);`

`int ingenic_aip_resize_process(data_info_s             *src,
                                const int               box_num,
                                const data_info_s       *dst,
                                const box_resize_info_s *boxes,
                                const uint32_t          *coef,
                                const uint32_t          *offset);`

`int ingenic_aip_max_cnt(int aip_type);`

`int bs_covert_cfg(data_info_s       *src,
                   const data_info_s *dst,
                   const uint32_t    *coef,
                   const uint32_t    *offset,
                   const task_info_s *task_info);`

`int bs_covert_step_start(const task_info_s      *task_info,
                          uint32_t               dst_ptr,
                          const bs_data_locate_e locate);`

`int bs_covert_step_wait();`

`int bs_covert_step_exit();`
