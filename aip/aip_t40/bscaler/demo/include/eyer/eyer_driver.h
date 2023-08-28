#ifndef EYER_DRIVER_H
#define EYER_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "EYE_LIB_DEF.h"
#include "stdarg.h"
#include "inttypes.h"

#define __place_k0_data__
#define __place_k1_data__
//#include "getpid.h"
#define AUX_EYER_SIM
#define EYER_PADDR_BASE 0x20000000
#define EYER_PADDR_HIGH 0x7FFFFFFF
#define EYER_VADDR_BASE 0x00000000
#define EYER_VADDR_HIGH 0xEFFFFFFF

#define EYER_TLB_VBASE EYER_VADDR_BASE
#define EYER_TLB_VHADDR (EYER_VADDR_BASE+1024*1024*4)

#define EYER_TLB_PBASE EYER_PADDR_BASE
#define EYER_TLB_PHADDR (EYER_PADDR_BASE+1024*1024*4)
extern uint32_t *tlb_tab;
extern uint32_t *tlb_tba;

#if 0
#define AP_TLB_BASE tlb_tab
#else
#define AP_TLB_BASE tlb_tba
#endif

//#include "eyer_util.h"
void eyer_wave_enable();
void *eyer_malloc(uint32_t *addr, uint32_t size);
void *eyer_malloc_aliagn(uint32_t *addr, uint32_t size, uint32_t align);
uint32_t *do_get_world_addr(uint32_t addr);
void show_vmm_list();
typedef struct vmm_nod
{
  uint8_t *word_base_addr;
  uint8_t *word_high_addr;

  uint32_t eyer_base_vaddr;
  uint32_t eyer_high_vaddr;

  uint32_t eyer_base_paddr;
  uint32_t eyer_high_paddr;

  struct vmm_nod *next;
}vmm_nod_t, *vmm_nod_p;
extern vmm_nod_p vmm_head;

typedef struct cmd_core_cmd{
  uint32_t addr;
  uint32_t data;
  uint32_t pread_id;
}cmd_core_cmd, *cmd_core_cmd_p;

typedef struct thread_inact{
  struct sembuf sem_b;
  int32_t shmid;
  int32_t semid;
  void *shm_p;
  int32_t max_size;
}thread_inact, *thread_inact_p;

typedef struct addr_context{
  uint32_t base_addr;
  uint32_t high_addr;
}addr_context, *addr_context_p;

typedef struct thread_addr_tab{
  uint32_t tab_dep;
  addr_context addr_context_buf[HDEC_ADDR_TAB_DEP];
}thread_addr_tab, *thread_addr_tab_p;

typedef struct eyer_sys{
  thread_inact cmd_core_thread;
  thread_inact aux_thread;
  thread_inact hdec_thread;
  thread_inact ddr_thread;
  thread_inact tcsm_thread;
  thread_inact tcsm1_thread;
  thread_inact sram_thread;
  thread_inact reset_thread;
  thread_inact wave_thread;
  thread_inact score_thread;
  thread_inact intc_thread;
  thread_addr_tab thread_addr_tab_buf;
  __pid_t cur_pid;
  __pid_t eyer_pid ;
  uint32_t *tcsm_base;
  uint32_t *tcsm1_base;
  uint32_t *sram_base;
  pthread_t thread_id_aux;
  pthread_attr_t thread_attr_aux;
  struct sched_param scheduling_value;
  uint32_t aux_run;
  pthread_mutex_t lock;
  pthread_cond_t notempty;
  pthread_cond_t notfull;
  pthread_cond_t aux_end;
  pthread_cond_t aux_end_resp;
  pthread_cond_t aux_start;
  pthread_cond_t aux_start_resp;
  int32_t aux_go_now;
  int32_t safe_mod;
  //int32_t eyer_hw_running;
  //int32_t eyer_sw_stop;
}eyer_sys, *eyer_sys_p;

typedef struct eyer_t{
  int eyer_sys_en  ;
  int debug_frame ;
  int debug_mb_x ;
  int debug_mb_y ;
  int decoder_frame_num_cur  ;
  int eyer_debug  ;
  int eyer_random;
  int fm_go_now;
  int eyer_wave;
  int eyer_run;
}eyer_t;

void x86_stop(int sig);

void init_eyer_thread(
              thread_inact_p thread_inact_ptr,
              key_t sem_id_seed,
              key_t shm_id_seed,
              uint32_t shm_mem_size,
              uint32_t set_sem_ini,
              int32_t sem_flag
              );

void thread_send(thread_inact_p thread_inact_ptr,
         uint32_t *buf_ptr,
         uint32_t  buf_size
         );
uint32_t cmd_core_read (thread_inact_p thread_inact_ptr,
                uint32_t addr);
void cmd_core_write( thread_inact_p thread_inact_ptr,
             uint32_t addr,
             uint32_t data);
void eyer_addr_map( thread_inact_p thread_inact_ptr,
            thread_addr_tab_p thread_addr_tab_ptr);
void eyer_read(int32_t sig);
void eyer_write(int32_t sig);
void send_pid_to_eyer(int32_t need_wave);
void eyer_system_ini(int32_t);
extern eyer_sys_p eyer_sys_ptr;

#define TCSM0_BASE 0x132B0000
#define TCSM1_BASE 0x132C0000
#define SRAM_BASE 0x132F0000

#define C_TCSM0_BASE ((unsigned int)eyer_sys_ptr->tcsm_base)
#define C_TCSM1_BASE ((unsigned int)eyer_sys_ptr->tcsm1_base)
#define C_SRAM_BASE ((unsigned int)eyer_sys_ptr->sram_base)

#define __place_k0_data__
#define get_phy_addr(a) (a)
#define i_sw(val, reg, c)            \
({ cmd_core_write((&eyer_sys_ptr->cmd_core_thread), (reg), (val));    \
})

#define i_lw(reg, off) \
({cmd_core_read((&eyer_sys_ptr->cmd_core_thread), (reg+off));\
})

#define aux_i_sw(val, reg, c)            \
({cmd_core_write((&eyer_sys_ptr->aux_thread), (reg), (val));    \
})

#define aux_i_lw(reg, off) \
({cmd_core_read((&eyer_sys_ptr->aux_thread), (reg+off));\
})

#define aux_write_reg(reg, val) \
({ aux_i_sw((val), (reg), 0); \
})
#define aux_read_reg(reg, off) \
({ aux_i_lw((reg), (off)); \
})

#define write_reg(reg, val) \
({ \
i_sw((val), (reg), 0);                \
})
#define read_reg(reg, off) \
({ i_lw((reg), (off)); \
})
uint32_t do_get_phy_addr(uint8_t *addr);
uint32_t eyer_free(uint32_t *addr);

//system(". ncsim_cmd&") ;
extern eyer_sys_p eyer_sys_ptr;

#define TEST_MAIN(function)                        \
  int32_t main(int32_t argc,char* argv[])                \
  {                                    \
    int32_t i;                                \
    int32_t wave_en = 0 ;                        \
    int32_t only_c = 0 ;                        \
    int32_t mode_sel = 0 ;                        \
    printf("--------- wave write now------\n");                \
    for(i=1; i<argc; i++){                        \
      if ( !strcmp(argv[i], "-wave_en") ){                \
    wave_en = 1;                            \
    i++;                                \
    printf("--------- wave write now------\n");            \
      }                                    \
      else if ( !strcmp(argv[i], "-only_c") ){                \
    only_c = 1;                            \
    i++;                                \
    printf("--------- only_c------\n");                \
      }                                    \
      else if ( !strcmp(argv[i], "-safe_mode") ){            \
    mode_sel = 1;                            \
    i++;                                \
    printf("--------- safe_mode------\n");                \
      }                                    \
    }                                    \
    if ( only_c == 0 ){                            \
      eyer_system_ini(wave_en);                        \
    if ( mode_sel == 1 ){                        \
      eyer_safe_mod_enable();                        \
                                      vmm_init_tlb_tab(); \
                                                } \
if (  wave_en == 1)                            \
  eyer_wave_enable();                       \
}                               \
int32_t ret_v = 0;                       \
ret_v = function();                   \
if ( ret_v == 0){                   \
  puts("PASS\n");                   \
 }                           \
 else {                           \
   puts("FAIL\n");                   \
 }                           \
if ( only_c == 0 ){                   \
  eyer_stop();                       \
 }                           \
return 0;                       \
}                           \

#define AUX_START()
#if 0
void eyer_sram_write(int32_t sig);
void eyer_sram_read(int32_t sig);
void eyer_tcsm_write(int32_t sig);
void eyer_tcsm_read(int32_t sig);
void eyer_tcsm1_write(int32_t sig);
void eyer_tcsm1_read(int32_t sig);
#endif

void aux_clr();
#define aux_init()  pthread_cleanup_push(aux_clr, NULL)
#define aux_finish() pthread_cleanup_pop(1)

#define SCORE_RST 0x1001
#define SCORE_RUN 0x1002
#define SCORE_STOP 0x1003
#define SCORE_DISP 0x1004
#define SCORE_CFG_T 0x1005
#define SCORE_DIS_M 0x1006

#define SCORE_SHOW_BUS 0x1
#define SCORE_SHOW_MODULE 0x2
#define SCORE_SHOW_ALL 0x3
#define SCORE_SHOW_BUSW 0x4

#define score_clr()\
({\
  score_write((&eyer_sys_ptr->score_thread), SCORE_RST);    \
})\

#define score_enable()\
({\
  score_write((&eyer_sys_ptr->score_thread), SCORE_RUN);    \
})\

#define score_disable()\
({\
  score_write((&eyer_sys_ptr->score_thread), SCORE_STOP);    \
})\

#define score_disp()\
({\
  score_write((&eyer_sys_ptr->score_thread), SCORE_DISP);    \
})\

#define score_cfg_sample_T(T)\
({\
  score_write((&eyer_sys_ptr->score_thread), ((T<<16) | SCORE_CFG_T));\
})\

#define score_dips_mod(MODE)\
({\
  score_write((&eyer_sys_ptr->score_thread), ((MODE<<16) | SCORE_DIS_M));\
})\

#define AUX_VBASE        0x132A0000
#define aux_hard_start() \
({ write_reg(AUX_VBASE, 2);\
})

#define aux_hard_reset() \
({ write_reg(AUX_VBASE, 1);\
})

void eyer_stop();

void eyer_reg_segv();
void eyer_reg_ctrlc();
int eyer_intc_wait(int id);
void eyer_delay (long wait);
void eyer_intc_init();

#ifdef __cplusplus
}
#endif

#endif

