#ifdef __cplusplus
extern "C"
{
#endif

#include "eyer_driver.h"
#include "errno.h"
#undef printf
//#define AUX_EYER_SIM
eyer_sys eyer_sys_buf;
eyer_sys_p eyer_sys_ptr;
vmm_nod_p vmm_head = NULL;
uint32_t *tlb_tab, *tlb_tba, tlb_stba[1024];
unsigned char vremap[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
unsigned char premap[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint32_t *dump_mem;
int vmm_init_tlb_tab()
{
  tlb_tab = (uint32_t *)eyer_malloc(NULL, 1024*1024*4);
  int seed = (int)time(NULL);
  unsigned int i;
  for ( i = 0 ; i<1024*1024; i++)
    {
      tlb_tab[i] = EYER_PADDR_BASE + i*4096 + 1;
    }

  seed = seed%256;
  //seed = 0x6d;
  printf("EYER: dump mem seed %x\n", seed);
  dump_mem = (uint32_t *)eyer_malloc(NULL, 4096*seed);
  dump_mem = (uint32_t *)eyer_malloc(NULL, seed);
}

int vmm_list_add(uint8_t* waddr_low, uint8_t* waddr_hight, uint32_t align_size) //add an nod into vmm list, maybe here need add some align
{
  vmm_nod_p cur_p;
  vmm_nod_p next_p;
  uint32_t align_msk = align_size - 1;
  //printf("align_msk:%x-%x\n", align_msk, ~align_msk);
  if ( vmm_head == NULL )
    {
      vmm_head = (vmm_nod_p)malloc(sizeof(vmm_nod_t));
      vmm_head->word_base_addr = waddr_low;
      vmm_head->word_high_addr = waddr_hight;
      vmm_head->eyer_base_paddr = EYER_PADDR_BASE ;
      vmm_head->eyer_high_paddr = vmm_head->eyer_base_paddr + (uint32_t)((uint32_t)waddr_hight - (uint32_t)waddr_low);
      vmm_head->eyer_base_vaddr = EYER_VADDR_BASE;
      vmm_head->eyer_high_vaddr = vmm_head->eyer_base_vaddr + (uint32_t)((uint32_t)waddr_hight - (uint32_t)waddr_low);
      vmm_head->next = NULL;
      return 1;
    }
  else
    {
      cur_p = vmm_head;
      next_p = vmm_head->next;
      while(next_p != NULL )
    {
      cur_p = next_p;
      next_p = cur_p->next;
    }
      next_p = (vmm_nod_p)malloc(sizeof(vmm_nod_t));
      cur_p->next = next_p;
      next_p->word_base_addr = waddr_low;
      next_p->word_high_addr = waddr_hight;

      next_p->eyer_base_paddr = ((cur_p->eyer_high_paddr + align_msk) &  (~align_msk));
      next_p->eyer_high_paddr = next_p->eyer_base_paddr + ((uint32_t)waddr_hight - (uint32_t)waddr_low);
      next_p->next = NULL;
      if ( next_p->eyer_high_paddr >= EYER_PADDR_HIGH )
    {
      printf("EERROR: --------eyer malloc phy addr error----------\n");
      return 0;
    }

      next_p->eyer_base_vaddr = ((cur_p->eyer_high_vaddr + align_msk) & (~align_msk));
      next_p->eyer_high_vaddr = next_p->eyer_base_vaddr + ((uint32_t)waddr_hight - (uint32_t)waddr_low);
      if ( next_p->eyer_high_vaddr >= EYER_VADDR_HIGH )
    {
      printf("EERROR:--------eyer malloc virtual addr error----------\n");
      return 0;
    }
    }
};

int vmm_list_del_bywaddr(uint8_t *waddr)
{
  vmm_nod_p cur_p;
  vmm_nod_p next_p;
  vmm_nod_p last_p;
  cur_p = vmm_head;
  if ( vmm_head == NULL)
    return 1;
  else if ( vmm_head->word_base_addr == waddr )
    {
      next_p = vmm_head->next;
      free(vmm_head);
      vmm_head = next_p;
      return 1;
    }
  else {
    cur_p = vmm_head;
    next_p = vmm_head->next;
    last_p = vmm_head;
    while(cur_p != NULL)
      {
    //printf("DDDD: %llx --%xllx\n", cur_p->word_base_addr, waddr);
    if( cur_p->word_base_addr == waddr )
      {
        last_p->next = cur_p->next;
        //free(cur_p->word_base_addr);
        free(cur_p);
        return 1;
      }
    last_p = cur_p;
    cur_p = cur_p->next;
    next_p = cur_p->next;
      }
  }
  return 0;
}

vmm_nod_p vmm_list_find_byvaddr(uint32_t addr) //find an nod by vaddr
{
  vmm_nod_p cur_p;
  vmm_nod_p next_p;
  cur_p = vmm_head;
  if ( vmm_head == NULL )
    return 0;
  else
    {
      while( cur_p != NULL )
    {
      next_p = cur_p->next;
      if ( cur_p->eyer_base_vaddr <= addr && cur_p->eyer_high_vaddr > addr )
        return cur_p;
      cur_p = next_p;
    }
    }
  return 0;
}

vmm_nod_p vmm_list_find_bypaddr(uint32_t addr) //find an nod by paddr
{
  vmm_nod_p cur_p;
  vmm_nod_p next_p;
  cur_p = vmm_head;
  if ( vmm_head == NULL )
    return NULL;
  else
    {
      while( cur_p != NULL )
    {
      next_p = cur_p->next;
      if ( cur_p->eyer_base_paddr <= addr && cur_p->eyer_high_paddr > addr )
        return cur_p;
      cur_p = next_p;
    }
    }
  return NULL;
}

vmm_nod_p vmm_list_find_bywaddr(uint8_t *addr) //find an nod by world addr
{
  vmm_nod_p cur_p;
  vmm_nod_p next_p;
  cur_p = vmm_head;
  if ( vmm_head == NULL )
    return 0;
  else
    {
      while( cur_p != NULL )
    {
      next_p = cur_p->next;
      if ( cur_p->word_base_addr <= addr && cur_p->word_high_addr > addr )
        return cur_p;
      cur_p = next_p;
    }
    }
  return 0;
}

void list_all_vmm()
{
  vmm_nod_p cur_p;
  vmm_nod_p next_p;
  cur_p = vmm_head;
  if ( vmm_head == NULL )
    return ;
  else{
    while(cur_p != NULL )
      {
    next_p = cur_p->next;
    printf("base waddr: 0x%x --0x%x\n", cur_p->word_base_addr, cur_p->word_high_addr);
    printf("base vaddr: 0x%x --0x%x\n", cur_p->eyer_base_vaddr, cur_p->eyer_high_vaddr);
    printf("base paddr: 0x%x --0x%x\n", cur_p->eyer_base_paddr, cur_p->eyer_high_paddr);
    printf("tlb  paddr: 0x%x --0x%x-0x%x--0x%x\n", ((unsigned int)cur_p->eyer_base_vaddr>>12),
           tlb_tab[(unsigned int)cur_p->eyer_base_vaddr>>12],
           do_get_phy_addr((uint8_t *)&tlb_tab[(unsigned int)cur_p->eyer_base_vaddr>>12]
                   ),
           &tlb_tab[(unsigned int)cur_p->eyer_base_vaddr>>12]
           );
    printf("next:0x%x\n", cur_p->next);
    cur_p = next_p;
      }
  }
};

void kill_eyer_shm_sem(thread_inact_p thread_inact_ptr, int32_t shm_flag)
{
  if ( shm_flag ){
  shmdt(thread_inact_ptr->shm_p);
  if ( shmctl(thread_inact_ptr->shmid, IPC_RMID, 0) != 0 )//remove share mem
    {
    perror("kill eyer shm sem shmctl RMID\n");
    exit(EXIT_FAILURE);
      }
  }
    if ( semctl(thread_inact_ptr->semid, 0, IPC_RMID, 0) != 0) // remove signal
      {
    perror("kill eyer shm sem");
    exit(EXIT_FAILURE);
      }
}

void init_eyer_thread(
              thread_inact_p thread_inact_ptr,
              key_t sem_id_seed,
              key_t shm_id_seed,
              uint32_t shm_mem_size,
              uint32_t set_sem_ini,
              int32_t sem_flag
              )
{
  thread_inact_ptr->max_size = shm_mem_size;
  thread_inact_ptr->sem_b.sem_num = 0 ;
  thread_inact_ptr->sem_b.sem_flg = sem_flag;//SEM_UNDO;
  //printf("key_t: %x\n", sem_id_seed);
  if ( ( thread_inact_ptr->semid = semget((key_t)sem_id_seed, 1, 0666|IPC_CREAT))==-1)// creat signal ( or read ID)
    {
      perror("semget");
      exit(EXIT_FAILURE);
    }
  //printf("semid: %x\n", thread_inact_ptr->semid);
  if ( set_sem_ini ){
  if ( semctl(thread_inact_ptr->semid, 0, SETVAL, 0) == -1 ) //set initial value to 0
    {
      printf("sem init error");
      if ( semctl(thread_inact_ptr->semid, 0, IPC_RMID, 0) != 0 )// int32_t initial fail, remove it
    {
      perror("semctl");
      exit(EXIT_FAILURE);
    }
      exit(EXIT_FAILURE);
    }
  }

  if ( shm_mem_size != 0 ){
  thread_inact_ptr->shmid = shmget((key_t)shm_id_seed, (size_t)shm_mem_size, 0600|IPC_CREAT); // creat share mem
  //printf("shmid: %x\n", thread_inact_ptr->shmid);

  if ( thread_inact_ptr->shmid == -1 )
    {
      perror("shmget");
      exit(EXIT_FAILURE);
    }

  thread_inact_ptr->shm_p = shmat(thread_inact_ptr->shmid, NULL, 0);//hand share mem to current id
  if ( thread_inact_ptr->shm_p == NULL )
    {
      perror("shmat");
      exit(EXIT_FAILURE);
    }
  }
}

void thread_send(thread_inact_p thread_inact_ptr,
         uint32_t *buf_ptr,
         uint32_t  buf_size
         )
{
  int32_t value;
  if ( buf_size != 0xFFFFF ){
  if ( buf_size > thread_inact_ptr->max_size )
    {
      fprintf(stderr, "ERROR: thread_send size(%x) > max size(%x)\n", buf_size, thread_inact_ptr->max_size);
      exit(EXIT_FAILURE);
    }
  }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  int32_t i ;
  if ( buf_size == 0xFFFFF )
    {
      ptr[0] = buf_ptr[0];
    }
  else{
    for ( i = 0 ; i < buf_size; i ++){
      ptr[i] = buf_ptr[i];
    }
  }
  thread_inact_ptr->sem_b.sem_op = 1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "thread_send semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
}

void eyer_delay (long wait) {
#define SEC_UNIT 1000000
  long goal = wait*SEC_UNIT + clock();
  while(goal > clock());
}

int eyer_intc_stat;
int eyer_intc_bsfull;
//#define ICM_ISP_SIM
#ifdef ICM_ISP_SIM
void intc_handler(int32_t sig)
{
  int value;
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->intc_thread;
  volatile uint32_t *ptr = (volatile uint32_t *)thread_inact_ptr->shm_p;
  printf("[EYER] INTC handler\n");

  //handshake only
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );
  *ptr = 1;   //mask req
  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  value=semctl(thread_inact_ptr->semid, 0, GETVAL);
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      value=semctl(thread_inact_ptr->semid, 0, GETVAL);
      printf("addr: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      fprintf(stderr, "intc_handler semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  //status check
  {
#define REG_ISP_STAT      0x13200048
#define REG_ISP_EVPN      0x1320005C
#define REG_ISP_TLBRTY    0x13200050
#define ISP_STAT_TLBERR   (0x1<<31)
#define ISP_STAT_AXIEND   (0x1<<0)
    int isp_stat = read_reg(REG_ISP_STAT, 0);
    if(isp_stat & ISP_STAT_TLBERR){
      uint32_t vpn = read_reg(REG_ISP_EVPN, 0) & 0xFFFFF000;
      printf("[EYER] INTC: TLB re-build | VPN: %08x\n", vpn);
      uint32_t pidx = ((vpn & 0xFFC00000)>>22) & 0x3FF;
      uint32_t sidx = ((vpn & 0x003FF000)>>12) & 0x3FF;
      if(!(tlb_tba[pidx] & 0x1)){
    tlb_tba[pidx] = (tlb_stba[pidx] & 0xFFFFF000) | 0x1;
    printf("[EYER] INTC: TLB re-build | PEntry[%d]: %08x\n", pidx, tlb_tba[pidx]);
      }
      uint32_t *ptr = (uint32_t *)(tlb_stba[pidx]);
      if(!(ptr[sidx] & 0x1)){
    ptr[sidx] = (vpn & 0xFFFFF000) | 0x1;
    printf("[EYER] INTC: TLB re-build | STBA %d, SEntry[%d]: %08x\n", pidx, sidx, ptr[sidx]);
    write_reg(REG_ISP_TLBRTY, 0x1);
      } else {
    printf("[EYER] INTC: ISP TLB error!\n");
    eyer_stop();
    exit(EXIT_FAILURE);
      }
    } else if(isp_stat & ISP_STAT_AXIEND){
      eyer_intc_stat = 1;
      printf("[EYER] INTC: ISP done!\n");
      write_reg(REG_ISP_STAT, 0);
    } else {
      printf("[EYER] INTC: illegal interrupt happened!\n");
      eyer_stop();
      exit(EXIT_FAILURE);
    }
  }

  eyer_delay(1);
#if 0
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
#endif
  *ptr = 0;   //enable req, need not sync
}

void eyer_intc_init(){
  eyer_intc_stat = 0;
  eyer_intc_bsfull = 0;
}

void eyer_intc_wait(){
  while(!eyer_intc_stat){
    int i;
    for(i=0; i<1000; i++)
      read_reg(0x132D0000, i*4);
  }
}

#else
void intc_handler(int32_t sig)
{
  int value;
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->intc_thread;
  volatile uint32_t *ptr = (volatile uint32_t *)thread_inact_ptr->shm_p;
  printf("[EYER] INTC handler\n");

  //handshake only
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );
  *ptr = 1;   //mask req
  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  value=semctl(thread_inact_ptr->semid, 0, GETVAL);
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      value=semctl(thread_inact_ptr->semid, 0, GETVAL);
      printf("addr: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      fprintf(stderr, "intc_handler semaphore_p failed\n");
      eyer_stop();
      exit(EXIT_FAILURE);
    }
  if (eyer_intc_stat)
  return;
  //status check
  {
#define REG_VPU_STAT      0x13200034
#define VPU_STAT_ENDF    (0x1<<0)
#define VPU_STAT_BPF     (0x1<<1)
#define VPU_STAT_ACFGERR (0x1<<2)
#define VPU_STAT_TIMEOUT (0x1<<3)
#define VPU_STAT_JPGEND  (0x1<<4)
#define VPU_STAT_BSERR   (0x1<<7)
#define VPU_STAT_BSFULL  (0x1<<20)
#define VPU_STAT_TLBERR  (0x3F<<10)
#define VPU_STAT_SLDERR  (0x1<<16)

#define REG_VPU_TLBC      0x13200050
#define VPU_TLBC_VPN      (0xFFFFF000)
#define VPU_TLBC_INVLD    (0x1<<1)
#define VPU_TLBC_RETRY    (0x1<<0)

#define REG_VPU_JPGC_STAT 0x132E0008
#define JPGC_STAT_ENDF   (0x1<<31)

#define REG_VPU_SDE_STAT  0x13290000
#define SDE_STAT_BSEND   (0x1<<1)

#define REG_VPU_DBLK_STAT 0x13270070
#define DBLK_STAT_DOEND  (0x1<<0)

#define REG_VPU_EMC_STAT   0x13230044
#define EMC_BS_STAT      (0x1<<1)

#define REG_VPU_AUX_STAT  0x132A0010
#define AUX_STAT_MIRQP   (0x1<<0)

    int vpu_stat = read_reg(REG_VPU_STAT, 0);
    if(vpu_stat & VPU_STAT_SLDERR) {
      printf("[EYER] INTC: VPU SHLD error!\n");
      eyer_stop();
      exit(EXIT_FAILURE);
    } else if(vpu_stat & VPU_STAT_TLBERR) {
#if 0
      printf("[EYER] INTC: VPU TLB error!\n");
      exit(EXIT_FAILURE);
#else
      uint32_t vpn = read_reg(REG_VPU_TLBC, 0) & VPU_TLBC_VPN;
      printf("[EYER] INTC: TLB re-build | VPN: %08x\n", vpn);
      uint32_t pidx = ((vpn & 0xFFC00000)>>22) & 0x3FF;
      uint32_t sidx = ((vpn & 0x003FF000)>>12) & 0x3FF;
      if(!(tlb_tba[pidx] & 0x1)){
    tlb_tba[pidx] = (tlb_stba[pidx] & 0xFFFFF000) | 0x1;
    printf("[EYER] INTC: TLB re-build | PEntry[%d]: %08x\n", pidx, tlb_tba[pidx]);
      }
      uint32_t *ptr = (uint32_t *)(tlb_stba[pidx]);
      if(!(ptr[sidx] & 0x1)){
    ptr[sidx] = (vpn & 0xFFFFF000) | 0x1;
    //ptr[sidx] = (premap[((vpn & 0xF0000000)>>28) & 0xF]<<28) | (ptr[sidx] & 0x0FFFFFFF);
    printf("[EYER] INTC: TLB re-build | STBA %d, SEntry[%d]: %08x\n", pidx, sidx, ptr[sidx]);
    write_reg(REG_VPU_TLBC, VPU_TLBC_RETRY);
      } else {
    printf("[EYER] INTC: VPU TLB error!\n");
    eyer_stop();
    exit(EXIT_FAILURE);
      }
#endif
    } else if(vpu_stat & VPU_STAT_BSERR) {
      printf("[EYER] INTC: VPU BS error!\n");
      eyer_stop();
      exit(EXIT_FAILURE);
    } else if(vpu_stat & VPU_STAT_ACFGERR) {
      printf("[EYER] INTC: VPU ACFG error!\n");
      eyer_stop();
      exit(EXIT_FAILURE);
    } else if(vpu_stat & VPU_STAT_TIMEOUT) {
      printf("[EYER] INTC: VPU timed out!\n");
      eyer_stop();
      exit(EXIT_FAILURE);
    } else if(vpu_stat & VPU_STAT_BSFULL) {
      printf("[EYER] INTC: VPU(EMC) bitstream buffer full!\n");
      eyer_intc_bsfull = 1;
      write_reg(REG_VPU_EMC_STAT, EMC_BS_STAT);
    } else if(vpu_stat & VPU_STAT_ENDF) {
      eyer_intc_stat = 1;
      if(vpu_stat & VPU_STAT_JPGEND) {
    printf("[EYER] INTC: VPU(JPG) successfully done!\n");
    write_reg(REG_VPU_JPGC_STAT, read_reg(REG_VPU_JPGC_STAT, 0) & ~JPGC_STAT_ENDF);
      } else {
    printf("[EYER] INTC: VPU(SCH) successfully done!\n");
    write_reg(REG_VPU_SDE_STAT, read_reg(REG_VPU_SDE_STAT, 0) & ~SDE_STAT_BSEND);
    write_reg(REG_VPU_DBLK_STAT, read_reg(REG_VPU_DBLK_STAT, 0) & ~DBLK_STAT_DOEND);
      }
    } else {
      if(read_reg(REG_VPU_AUX_STAT, 0) & AUX_STAT_MIRQP) {
    eyer_intc_stat = 1;
    printf("[EYER] INTC: VPU(AUX) successfully done!\n");
    write_reg(REG_VPU_AUX_STAT, read_reg(REG_VPU_AUX_STAT, 0) & ~AUX_STAT_MIRQP);
      } else {
    printf("[EYER] INTC: illegal interrupt happened!\n");
    eyer_stop();
    exit(EXIT_FAILURE);
      }
    }
  }
  read_reg(0x132F0000, 0); //just for delay! why sleep() not work?
  *ptr = 0;   //enable req, need not sync
}

void eyer_intc_init(){
  eyer_intc_stat = 0;
  eyer_intc_bsfull = 0;
}

int eyer_intc_wait(int id){

#define INTC_WAIT_ID_SDE  0
#define INTC_WAIT_ID_EFE  1
#define INTC_WAIT_ID_JPG  2
#define INTC_WAIT_ID_AUX  3
//#define REG_VPU_SDE_STAT  0x13290000
#define SDE_STAT_MBX(a)   (((a)>>16) & 0xFF)
#define SDE_STAT_MBY(a)   (((a)>>24) & 0xFF)
#define REG_VPU_EFE_STAT  0x13240110
#define EFE_STAT_G0X(a)   (((a)>>0 ) & 0xFF)
#define EFE_STAT_G0Y(a)   (((a)>>8 ) & 0xFF)
#define EFE_STAT_G1X(a)   (((a)>>16) & 0xFF)
#define EFE_STAT_G1Y(a)   (((a)>>24) & 0xFF)
#define REG_VPU_JPG_NMCU  0x132E0064

  int reg_val;
  while(!eyer_intc_stat){
    eyer_delay(5);
    //sleep(3);
#if 0
    int i;
    for(i=0; i<1000; i++)
      read_reg(0x132C0000, i*4);
#endif
    if(eyer_intc_bsfull){
      eyer_intc_bsfull = 0;
      return 1;
    }

    switch (id){
    case INTC_WAIT_ID_SDE:
      reg_val = read_reg(REG_VPU_SDE_STAT, 0);
      fprintf(stderr, "[SDE] |MBX| %3d |MBY| %3d\n",
          SDE_STAT_MBX(reg_val), SDE_STAT_MBY(reg_val) );
      break;
    case INTC_WAIT_ID_EFE:
      reg_val = read_reg(REG_VPU_EFE_STAT, 0);
      fprintf(stderr, "[EFE] |G0X| %3d |G0Y| %3d |G1X| %3d |G1Y| %3d\n",
          EFE_STAT_G0X(reg_val), EFE_STAT_G0Y(reg_val),
          EFE_STAT_G1X(reg_val), EFE_STAT_G1Y(reg_val));
      break;
    case INTC_WAIT_ID_JPG:
      reg_val = read_reg(REG_VPU_JPG_NMCU, 0);
      fprintf(stderr, "[JPG] |MCU| %5d\n", reg_val);
      break;
    default:
      fprintf(stderr, "...\n");
      break;
    }
  }
  return 0;
}
#endif //!ICM_ISP_SIM

void eyer_tlb_book(char *addr, int size)
{
  uint32_t i, tmp, *ptr;
  printf("[EYER] TLB booking: %08x %d\n", addr, size);
  for(tmp=(uint32_t)addr; tmp<(uint32_t)addr+size; tmp+= 0x1000){
    uint32_t pidx = ((tmp & 0xFFC00000)>>22) & 0x3FF;
    uint32_t sidx = ((tmp & 0x003FF000)>>12) & 0x3FF;
    if(!(tlb_tba[pidx] & 0x1)){
      tlb_tba[pidx] = (tlb_stba[pidx] & 0xFFFFF000) | 0x1;
      printf("[EYER] TLB PEntry[%d]: %08x\n", pidx, tlb_tba[pidx]);
    }
    ptr = (uint32_t *)(tlb_stba[pidx]);
    if(!(ptr[sidx] & 0x1)){
      ptr[sidx] = (tmp & 0xFFFFF000) | 0x1;
      printf("[EYER] TLB STBA %d, SEntry[%d]: %08x\n", pidx, sidx, ptr[sidx]);
    }
  }
}

void score_write (thread_inact_p thread_inact_ptr, int32_t cmd)
{
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = cmd;
  thread_inact_ptr->sem_b.sem_op = 1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  //printf("score write wait ...\n");
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  //printf("score write wait finish\n");
}

uint32_t cmd_core_read (thread_inact_p thread_inact_ptr,
             uint32_t addr)
{
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  int32_t i ;
  ptr[0] = addr;
  //printf("---cmd_core_read--%x-%x\n", ptr[0], ptr[1]);
  thread_inact_ptr->sem_b.sem_op = 2;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  //printf("---++cmd_core_read--%x-%x\n", ptr[0], ptr[1]);
  return ptr[1];

}
void cmd_core_write( thread_inact_p thread_inact_ptr,
             uint32_t addr,
             uint32_t data)
{
  cmd_core_cmd cmd_core_cmd_buf;
  //printf("Write0:%x-%x\n", addr, data);
  cmd_core_cmd_buf.addr = addr;
  cmd_core_cmd_buf.data = data;
  //cmd_core_cmd_buf.pread_id = getpid();
  //printf("Write:%x-%x\n", addr, data);
  thread_send(thread_inact_ptr,
          (uint32_t *)(&cmd_core_cmd_buf),
          3
          );
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
}

uint32_t read_aux_irq()
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->cmd_core_thread;//&eyer_sys_ptr->aux_thread;
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = 4;
  thread_inact_ptr->sem_b.sem_op = 3;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  return ptr[1];
}

uint32_t read_sys_cycle()
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->cmd_core_thread;//&eyer_sys_ptr->aux_thread;
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = 5;
  thread_inact_ptr->sem_b.sem_op = 3;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  return ptr[1];
}

uint32_t clr_sys_cycle()
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->cmd_core_thread;//&eyer_sys_ptr->aux_thread;
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = 6;
  thread_inact_ptr->sem_b.sem_op = 3;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  return ptr[1];
}

uint32_t read_aux_irq_msk()
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->cmd_core_thread;//&eyer_sys_ptr->aux_thread;
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = 2;
  thread_inact_ptr->sem_b.sem_op = 3;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  return ptr[1];
}

uint32_t write_aux_irq(uint32_t irq)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->cmd_core_thread;//&eyer_sys_ptr->aux_thread;
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = 3;
  ptr[1] = irq;
  thread_inact_ptr->sem_b.sem_op = 3;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  return ptr[1];
}

uint32_t write_aux_irq_msk(uint32_t irq_msk)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->cmd_core_thread;//&eyer_sys_ptr->aux_thread;
  uint32_t value;
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  ptr[0] = 1;
  ptr[1] = irq_msk;
  thread_inact_ptr->sem_b.sem_op = 3;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr,"vale: %d\n", value);
      fprintf(stderr, "cmd_core_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  return ptr[1];
}

void dump_aux_irq()
{
  printf("-------dump_aux_irq-------\n");
}

void init_aux_irq(void (*fun)())
{
#if 0
  if ( signal(EYER_IRQ_SIG, *fun) == SIG_ERR )
    {
      printf("Cannot catch aux_irq func\n");
    }
#endif
}

void eyer_addr_map( thread_inact_p thread_inact_ptr,
            thread_addr_tab_p thread_addr_tab_ptr
)
{
  thread_send(thread_inact_ptr,
          (uint32_t *)thread_addr_tab_ptr,
          thread_addr_tab_ptr->tab_dep*2+1
          );
}

int32_t write_cnt = 0 ;
int32_t read_cnt = 0 ;
void eyer_read(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->ddr_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  read_cnt ++;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );
  //printf("eyer_read 0\n ");
  //printf("addr: %x-%x\n", ptr[0], ptr[1]);
  if ( eyer_sys_ptr->safe_mod == 0 )
    {
      eyer_read_ptr = (uint32_t *)ptr[0];
      ptr[1] = *eyer_read_ptr;
    }
  else{
    eyer_read_ptr = do_get_world_addr(ptr[0]);
  if ( eyer_read_ptr == NULL )
    {
      ptr[1] = 0;
      printf(" eyer read read exceed boundary. err addr: 0x%x\n", ptr[0]);
    }else{
      //eyer_read_ptr = (uint32_t *)ptr[0];

#if 1
  ptr[1] = *eyer_read_ptr;
#else
  ptr[1] = 0;
#endif
    }
  }

  //printf("eyer_read ");
  //printf("addr: %x-%x\n", ptr[0], ptr[1]);

  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  value=semctl(thread_inact_ptr->semid, 0, GETVAL);
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      value=semctl(thread_inact_ptr->semid, 0, GETVAL);
      printf("addr: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      printf("read_cnt: %d, write_cnt: %d\n", read_cnt, write_cnt);
      fprintf(stderr, "eyer_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }

}

void eyer_write(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->ddr_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  write_cnt++;
  //printf("eyer_write\n");
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );

  //printf("eyer_write ");
  if ( eyer_sys_ptr->safe_mod == 0 )
    {
      eyer_read_ptr = (uint32_t *)ptr[0];
      *eyer_read_ptr = ptr[1];
    }
  else{
    eyer_read_ptr = do_get_world_addr(ptr[0]);

  if ( eyer_read_ptr == NULL )
    {
      printf(" eyer write exceed boundary. err addr: 0x%x\n", ptr[0]);
    }else{
      //eyer_read_ptr = (uint32_t *)ptr[0];
      //printf("ddrw-addr: a %x-d %x\n", ptr[0], ptr[1]);
#if 1
      *eyer_read_ptr = ptr[1];
#endif
    }
  }
  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_write semaphore_p failed\n");
      printf("addr: %x-%x\n", ptr, ptr[0]);
      exit(EXIT_FAILURE);
    }
}

void eyer_tcsm_read(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->tcsm_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  uint32_t err_tmp = 0x5a5a5a5a;
  read_cnt ++;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );

  if ( (unsigned int)ptr[0] < TCSM0_SIZE ){
    if ( eyer_sys_ptr->tcsm_base == NULL ){
      eyer_sys_ptr->tcsm_base = (uint32_t *)malloc(TCSM0_SIZE*sizeof(int));
      printf("WARNING: TCSM0 generate %x\n", eyer_sys_ptr->tcsm_base);
    }
    eyer_read_ptr = (uint32_t *)((uint32_t)eyer_sys_ptr->tcsm_base + ((unsigned int)ptr[0])*4);
  }
  else
    {
      printf("tcsm0 read address access error: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      //exit(EXIT_FAILURE);
      eyer_read_ptr = &err_tmp;
    }
#if 1
  ptr[1] = *eyer_read_ptr;
#else
  ptr[1] = 0;
#endif

  //printf("eyer_read ");
  //printf("addr: %x-%x\n", ptr[0], ptr[1]);

  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  value=semctl(thread_inact_ptr->semid, 0, GETVAL);
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      value=semctl(thread_inact_ptr->semid, 0, GETVAL);
      printf("addr: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      printf("read_cnt: %d, write_cnt: %d\n", read_cnt, write_cnt);
      fprintf(stderr, "eyer_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }

}

void eyer_tcsm_write(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->tcsm_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  uint32_t err_tmp = 0x5a5a5a5a;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );
  //printf("tcsm0 write size: %x\n", ptr[0]);
  if ( (unsigned int)ptr[0] < TCSM0_SIZE ){
    if ( eyer_sys_ptr->tcsm_base == NULL ){
      eyer_sys_ptr->tcsm_base = (uint32_t *)malloc(TCSM0_SIZE*sizeof(int));
      printf("WARNING: TCSM0 generate %x\n", eyer_sys_ptr->tcsm_base);
    }
    eyer_read_ptr = (uint32_t *)((uint32_t)eyer_sys_ptr->tcsm_base + ((unsigned int)ptr[0])*4);
  }
  else
    {
      printf("tcsm0 write address access error: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      //exit(EXIT_FAILURE);
      eyer_read_ptr = &err_tmp;

    }
  //printf("tcsm0-addr: a %x-d %x\n", eyer_read_ptr, ptr[1]);
#if 1
  *eyer_read_ptr = ptr[1];
#endif
  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_write semaphore_p failed\n");
      printf("addr: %x-%x\n", ptr, ptr[0]);
      exit(EXIT_FAILURE);
    }
}

void eyer_tcsm1_read(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->tcsm1_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  uint32_t err_tmp = 0x5a5a5a5a ;
  read_cnt ++;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );

  if ( (unsigned int)ptr[0] < TCSM1_SIZE ){
    if ( eyer_sys_ptr->tcsm1_base == NULL ){
      eyer_sys_ptr->tcsm1_base = (uint32_t *)malloc(TCSM1_SIZE*sizeof(int));
      printf("WARNING: TCSM0 generate %x\n", eyer_sys_ptr->tcsm1_base);
    }
    eyer_read_ptr = (uint32_t *)((uint32_t)eyer_sys_ptr->tcsm1_base + ((uint32_t )ptr[0])*4);
  }
  else
    {
      printf("tcsm1 read address access error: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      eyer_read_ptr = &err_tmp;
      //exit(EXIT_FAILURE);
    }

#if 1
  ptr[1] = *eyer_read_ptr;
#else
  ptr[1] = 0;
#endif

  //printf("eyer_read ");
  //printf("addr: %x-%x-%x\n", ptr[0], eyer_read_ptr, ptr[1]);

  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  value=semctl(thread_inact_ptr->semid, 0, GETVAL);
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      value=semctl(thread_inact_ptr->semid, 0, GETVAL);
      printf("addr: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      printf("read_cnt: %d, write_cnt: %d\n", read_cnt, write_cnt);
      fprintf(stderr, "eyer_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }

}

void eyer_tcsm1_write(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->tcsm1_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  uint32_t err_tmp = 0x5a5a5a5a ;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );
  //printf("tcsm1 write size: %x\n", ptr[0]);
  if ( (unsigned int)ptr[0] < TCSM1_SIZE ){
    if ( eyer_sys_ptr->tcsm1_base == NULL ){
      eyer_sys_ptr->tcsm1_base = (uint32_t *)malloc(TCSM1_SIZE*sizeof(int));
      printf("WARNING: TCSM1 generate %x\n", eyer_sys_ptr->tcsm1_base);
    }
    eyer_read_ptr = (uint32_t *)((uint32_t)eyer_sys_ptr->tcsm1_base + ((uint32_t )ptr[0])*4);
  }
  else
    {
      printf("tcsm1 write address access error: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      //exit(EXIT_FAILURE);
      eyer_read_ptr = &err_tmp;
    }
  //printf("tcsm1_write-addr: a %x-%x-d %x\n", ptr[0], eyer_read_ptr, ptr[1]);
#if 1
  *eyer_read_ptr = ptr[1];
#endif
  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_write semaphore_p failed\n");
      printf("addr: %x-%x\n", ptr, ptr[0]);
      exit(EXIT_FAILURE);
    }
}

void eyer_sram_read(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->sram_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  uint32_t err_tmp = 0x5a5a5a5a ;
  read_cnt ++;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );

  if ( (unsigned int)ptr[0] < SRAM_SIZE ){
    if ( eyer_sys_ptr->sram_base == NULL ){
      eyer_sys_ptr->sram_base = (uint32_t *)malloc(SRAM_SIZE*sizeof(int));
      printf("WARNING: TCSM0 generate %x\n", eyer_sys_ptr->sram_base);
    }
    eyer_read_ptr = (uint32_t *)((uint32_t)eyer_sys_ptr->sram_base + ((uint32_t )ptr[0])*4);
  }
  else
    {
      printf("sram read address access error: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      //exit(EXIT_FAILURE);
      eyer_read_ptr = &err_tmp;
    }

#if 1
  ptr[1] = *eyer_read_ptr;
#else
  ptr[1] = 0;
#endif

  //printf("eyer_read ");
  //printf("addr: %x-%x\n", ptr[0], ptr[1]);

  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  value=semctl(thread_inact_ptr->semid, 0, GETVAL);
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      value=semctl(thread_inact_ptr->semid, 0, GETVAL);
      printf("addr: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      printf("read_cnt: %d, write_cnt: %d\n", read_cnt, write_cnt);
      fprintf(stderr, "eyer_read semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }

}

void eyer_sram_write(int32_t sig)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->sram_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t *eyer_read_ptr;
  uint32_t value;
  uint32_t err_tmp = 0x5a5a5a5a ;
  while ( (value=semctl(thread_inact_ptr->semid, 0, GETVAL)) == 0 );
  if ( (unsigned int)ptr[0] < SRAM_SIZE ){
    if ( eyer_sys_ptr->sram_base == NULL ){
      eyer_sys_ptr->sram_base = (uint32_t *)malloc(SRAM_SIZE*sizeof(int));
      printf("WARNING: SRAM generate %x\n", eyer_sys_ptr->sram_base);
    }
    eyer_read_ptr = (uint32_t *)((uint32_t)eyer_sys_ptr->sram_base + ((uint32_t )ptr[0])*4);
  }
  else
    {
      printf("sram write address access error: %x-%x, %d, %d\n", ptr, ptr[0], value, getpid());
      printf("--errno:%d\n", errno);
      //exit(EXIT_FAILURE);
      eyer_read_ptr = &err_tmp;
    }
  //printf("sramw-addr: a %x-s %d-d %x\n", eyer_read_ptr, ptr[0], ptr[1]);
#if 1
  *eyer_read_ptr = ptr[1];
#endif
  thread_inact_ptr->sem_b.sem_op = -1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_write semaphore_p failed\n");
      printf("addr: %x-%x\n", ptr, ptr[0]);
      exit(EXIT_FAILURE);
    }
}

void send_pid_to_eyer(int32_t need_wave)
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->ddr_thread;
  uint32_t *ptr = (uint32_t *)thread_inact_ptr->shm_p;
  uint32_t value;
  while((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 8); //wait util eyer need pid
  ptr[0] = eyer_sys_ptr->cur_pid;
  ptr[1] = need_wave;
  thread_inact_ptr->sem_b.sem_op = -8;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "send_pid_to_eyer semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while((value=semctl(thread_inact_ptr->semid, 0, GETVAL)) != 7); //wait util eyer get pid
  eyer_sys_ptr->eyer_pid = ptr[0];
  //printf("eyer id: %x\n", eyer_sys_ptr->eyer_pid);
  thread_inact_ptr->sem_b.sem_op = -7;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "send_pid_to_eyer semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
}

int32_t get_sys_key(int32_t key)
{
  char *p;
  char path[256];
  p=getcwd(NULL,256);
  //sprintf(path, "%s/my_t.vmem",p);
#ifdef EYER_EVA
  sprintf(path, "%s",p);
#else
  sprintf(path, "%s/my_t.vmem",p);
#endif

  int32_t key_id = ftok(path,key);
  //printf("--%s: %d\n", path, key_id);
  //printf("=>key_id: %d\n", key_id);
  free(p);
  return (key_id);
}

void eyer_system_ini(int32_t need_wave)
{
  eyer_sys_ptr = &eyer_sys_buf;
  eyer_sys_ptr->cur_pid = getpid();
  eyer_sys_ptr->eyer_pid = -1;
  eyer_sys_ptr->thread_addr_tab_buf.tab_dep = 0;
  eyer_sys_ptr->aux_run = 0 ;
  eyer_sys_ptr->aux_go_now = 0 ;
  //vmm_init_tlb_tab();
  eyer_intc_stat = 1;//add
  //ICMedia V2.1 TLB
#if __x86_64__
#define EYER_ALN4K(a)   (((uint64_t)(a) + 0xFFF) & ~0xFFF)
#else
#define EYER_ALN4K(a)   (((uint32_t)(a) + 0xFFF) & ~0xFFF)
#endif
  int tlb_i;
  tlb_tba = (uint32_t *)malloc(1024*4*2);
  if(!tlb_tba){
    printf("[EYER] Error: TLB Primary Table malloc failed!\n");
    exit(1);
  }
  tlb_tba = (uint32_t *)EYER_ALN4K(tlb_tba);
  printf("[EYER] TLB TBA: %08x\n", (uint32_t)tlb_tba);
  memset(tlb_tba, 0, 1024*4);

  for(tlb_i=0; tlb_i<1024; tlb_i++){
    char *tmp = (char *)malloc(1024*4*2);
    if(!tmp){
      printf("[EYER] Error: TLB Secondary Table malloc failed!\n");
      exit(1);
    }
    memset(tmp, 0, 1024*4);
    tlb_stba[tlb_i] = (uint32_t)EYER_ALN4K(tmp);
  }

  int32_t cc;
  init_eyer_thread(
           &eyer_sys_ptr->score_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(SCORE_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(SCORE_SHM_KEY), //key_t shm_id_seed,
           CMDCORE_SHM_SIZE, //uint32_t shm_mem_size
           1, //sender
           0//SEM_UNDO
           );

  /*initial reset thread*/
  init_eyer_thread(
           &eyer_sys_ptr->wave_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(WAVE_SEM_KEY), //key_t sem_id_seed,
           0, //key_t shm_id_seed,
           0, //uint32_t shm_mem_size
           1, // no initial id receiver
           0//SEM_UNDO
           );
#ifdef IPCS_BUG
  system("ipcs -m");
  printf("\nwave_now----\n");
#endif
  /*initial reset thread*/
  init_eyer_thread(
           &eyer_sys_ptr->reset_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(RESET_SEM_KEY), //key_t sem_id_seed,
           0, //key_t shm_id_seed,
           0, //uint32_t shm_mem_size
           1, // no initial id receiver
           0//SEM_UNDO
           );

#ifdef IPCS_BUG
  system("ipcs -m");
  printf("\nreset----\n");
#endif
  /*initial intc thread*/
  init_eyer_thread(
           &eyer_sys_ptr->intc_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(INTC_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(INTC_SHM_KEY), //key_t shm_id_seed,
           INTC_SHM_SIZE, //uint32_t shm_mem_size
           0, // receiver
           0
           );
  /*initial ddr thread*/
  init_eyer_thread(
           &eyer_sys_ptr->ddr_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(DDR_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(DDR_SHM_KEY), //key_t shm_id_seed,
           DDR_SHM_SIZE, //uint32_t shm_mem_size
           0, // receiver
           0
           );
#ifdef IPCS_BUG
  system("ipcs -m");
  printf("\nddr----\n");
#endif
  /*initial cmd core thread*/
  init_eyer_thread(
           &eyer_sys_ptr->cmd_core_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(CMDCORE_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(CMDCORE_SHM_KEY), //key_t shm_id_seed,
           CMDCORE_SHM_SIZE, //uint32_t shm_mem_size
           1, //sender
           0//SEM_UNDO
           );

  init_eyer_thread(
           &eyer_sys_ptr->aux_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(CMDCORE1_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(CMDCORE1_SHM_KEY), //key_t shm_id_seed,
           CMDCORE1_SHM_SIZE, //uint32_t shm_mem_size
           1, //sender
           0//SEM_UNDO
           );

#ifdef IPCS_BUG
  system("ipcs -m");
  printf("\ncmd_core----\n");
#endif
  /*initial hdec thread*/
  init_eyer_thread(
           &eyer_sys_ptr->hdec_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(HDEC_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(HDEC_SHM_KEY), //key_t shm_id_seed,
           HDEC_SHM_SIZE, //uint32_t shm_mem_size
           1, //sender
           0//SEM_UNDO
           );
#ifdef IPCS_BUG
  system("ipcs -m");
  printf("\nhdec----\n");
#endif
  /*----------------- tcsm0 ------------------*/
  //printf("TCSM0 %d, %d\n", get_sys_key(TCSM0_SEM_KEY), get_sys_key(TCSM0_SHM_KEY));
  init_eyer_thread(
           &eyer_sys_ptr->tcsm_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(TCSM0_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(TCSM0_SHM_KEY), //key_t shm_id_seed,
           TCSM0_SHM_SIZE, //uint32_t shm_mem_size
           0, // receiver
           0
           );
  eyer_sys_ptr->tcsm_base = (uint32_t *)malloc(TCSM0_SIZE*sizeof(int));
  printf("TCSM0 generate %x\n", eyer_sys_ptr->tcsm_base);
  //printf("TCSM rsign: %d, wsign: %d\n",EYER_TCSM0_RD_SIG, EYER_TCSM0_WR_SIG);

  if ( signal(EYER_TCSM0_RD_SIG, eyer_tcsm_read) == SIG_ERR )
    {
      printf("Cannot catch EYER_TCSM0_RD_SIG\n");
    }
  if ( signal(EYER_TCSM0_WR_SIG, eyer_tcsm_write) == SIG_ERR )
    {
      printf("Cannot catch EYER_TCSM0_WR_SIG\n");
    }

  /*----------------- tcsm1 ------------------*/
  init_eyer_thread(
           &eyer_sys_ptr->tcsm1_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(TCSM1_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(TCSM1_SHM_KEY), //key_t shm_id_seed,
           TCSM0_SHM_SIZE, //uint32_t shm_mem_size
           0, // receiver
           0
           );
  eyer_sys_ptr->tcsm1_base = (uint32_t *)malloc(TCSM1_SIZE*sizeof(int));
  printf("TCSM1 generate %x\n", eyer_sys_ptr->tcsm1_base);

  if ( signal(EYER_TCSM1_RD_SIG, eyer_tcsm1_read) == SIG_ERR )
    {
      printf("Cannot catch EYER_TCSM0_RD_SIG\n");
    }
  if ( signal(EYER_TCSM1_WR_SIG, eyer_tcsm1_write) == SIG_ERR )
    {
      printf("Cannot catch EYER_TCSM0_WR_SIG\n");
    }

  /*----------------- sram ------------------*/
  init_eyer_thread(
           &eyer_sys_ptr->sram_thread, //thread_inact_p thread_inact_ptr,
           get_sys_key(SRAM_SEM_KEY), //key_t sem_id_seed,
           get_sys_key(SRAM_SHM_KEY), //key_t shm_id_seed,
           TCSM0_SHM_SIZE, //uint32_t shm_mem_size
           0, // receiver
           0
           );
  eyer_sys_ptr->sram_base = (uint32_t *)malloc(SRAM_SIZE*sizeof(int));
  printf("SRAM generate %x\n", eyer_sys_ptr->sram_base);

  init_aux_irq(&dump_aux_irq);

  if ( signal(EYER_SRAM_RD_SIG, eyer_sram_read) == SIG_ERR )
    {
      printf("Cannot catch EYER_TCSM0_RD_SIG\n");
    }
  if ( signal(EYER_SRAM_WR_SIG, eyer_sram_write) == SIG_ERR )
    {
      printf("Cannot catch EYER_TCSM0_WR_SIG\n");
    }

  if ( signal(EYER_INTC_SIG, intc_handler) == SIG_ERR )
    {
      printf("Cannot catch EYER_INTC_SIG\n");
    }
  if ( signal(EYER_RD_SIG, eyer_read) == SIG_ERR )
    {
      printf("Cannot catch EYER_RD_SIG\n");
    }
  if ( signal(EYER_WR_SIG, eyer_write) == SIG_ERR )
    {
      printf("Cannot catch EYER_WR_SIG\n");
    }

  if ( signal(EYER_KILL_SIG, x86_stop) == SIG_ERR )
    {
      printf("Cannot catch EYER_KILL_SIG\n");
    }

  char *cmd = "cd ./my_t.vmem; . ncsim_cmd &";
  //system(cmd);
  fprintf(stderr, "wait reset0\n");
  send_pid_to_eyer(need_wave);

  fprintf(stderr, "wait reset\n");
  while ((semctl(eyer_sys_ptr->reset_thread.semid, 0, GETVAL)) != 1 ); // wait reset end
  eyer_sys_ptr->reset_thread.sem_b.sem_op = -1;  // self-incre
  if ( semop(eyer_sys_ptr->reset_thread.semid, &eyer_sys_ptr->reset_thread.sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_write semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }

  fprintf(stderr, "wait reset finish\n");

  pthread_mutex_init(&eyer_sys_ptr->lock, NULL);
  pthread_cond_init(&eyer_sys_ptr->notempty, NULL);
  pthread_cond_init(&eyer_sys_ptr->notfull, NULL);
}

void eyer_safe_mod_enable()
{
  eyer_sys_ptr->safe_mod = 1;
}

void write_fifo(uint32_t *wptr, uint32_t *rptr)
{
#ifdef AUX_EYER_SIM
  pthread_mutex_lock(&eyer_sys_buf.lock);
  *wptr = *wptr + 1;
  pthread_cond_signal(&eyer_sys_buf.notempty);
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#else
  write_reg(do_get_phy_addr(wptr), (read_reg(do_get_phy_addr(wptr), 0)+1));
#endif
}

void wait_fifo_notfull(uint32_t *wptr, uint32_t *rptr, int32_t fifo_depth)
{
#ifdef AUX_EYER_SIM
  pthread_mutex_lock(&eyer_sys_buf.lock);
  while(*wptr >= (*rptr + fifo_depth))
    {
      pthread_cond_wait(&eyer_sys_buf.notfull, &eyer_sys_buf.lock);
    }
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#else
  while (read_reg(do_get_phy_addr(wptr), 0) >=(read_reg(do_get_phy_addr(rptr), 0)+fifo_depth))
    {
    }
#endif
}

void aux_clr()
{
  pthread_mutex_unlock(&eyer_sys_buf.lock);
  printf("aux clean up ---\n");
}

void wait_fifo_notempty(uint32_t *wptr, uint32_t *rptr)
{
#ifdef AUX_EYER_SIM
  pthread_mutex_lock(&eyer_sys_buf.lock);
  while(*wptr == *rptr )
    {
      pthread_cond_wait(&eyer_sys_buf.notempty, &eyer_sys_buf.lock);
    }
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#else
  while(read_reg(do_get_phy_addr(wptr), 0) == read_reg(do_get_phy_addr(rptr), 0))
    {
      sleep(1);
    }
#endif
}
//#define AUX_RESPOR

void aux_end(uint32_t *end_ptr, int32_t endvalue)
{
  pthread_mutex_lock(&eyer_sys_buf.lock);
  printf("send aux end!!\n");
  *end_ptr = endvalue;
  pthread_cond_signal(&eyer_sys_buf.aux_end);
  printf("wait aux end resp!!\n");
  pthread_mutex_unlock(&eyer_sys_buf.lock);

#ifdef AUX_RESPOR
  pthread_mutex_lock(&eyer_sys_buf.lock);
  while(*end_ptr == endvalue )
    {
      pthread_cond_wait(&eyer_sys_buf.aux_end_resp, &eyer_sys_buf.lock);
    }
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#endif
}

void wait_aux_end(uint32_t *end_ptr, int32_t endvalue)
{
  printf("wait aux end....\n");
#ifdef AUX_EYER_SIM
  pthread_mutex_lock(&eyer_sys_buf.lock);
  while (*end_ptr != endvalue){
    pthread_cond_wait(&eyer_sys_buf.aux_end, &eyer_sys_buf.lock);
  }
  *end_ptr = 0 ;
#ifdef AUX_RESPOR
  pthread_cond_signal(&eyer_sys_buf.aux_end_resp);
#endif
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#else
  while(read_reg(do_get_phy_addr(end_ptr), 0) != endvalue)
    {
    }
  write_reg(do_get_phy_addr(end_ptr), 0);
#endif
  printf("aux end!!\n");
}
void aux_start()
{
  pthread_mutex_lock(&eyer_sys_buf.lock);
  printf("send aux start!!\n");
  eyer_sys_ptr->aux_go_now = 1;
  pthread_cond_signal(&eyer_sys_buf.aux_start);
  pthread_mutex_unlock(&eyer_sys_buf.lock);

#ifdef AUX_RESPOR
  pthread_mutex_lock(&eyer_sys_buf.lock);
  while ( eyer_sys_ptr->aux_go_now == 1)
    {
      pthread_cond_wait(&eyer_sys_buf.aux_start_resp, &eyer_sys_buf.lock);
    }
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#endif
}

void wait_aux_start()
{
  printf("wait aux start....\n");
  pthread_mutex_lock(&eyer_sys_buf.lock);
  while (eyer_sys_ptr->aux_go_now == 0 ){
    pthread_cond_wait(&eyer_sys_buf.aux_start, &eyer_sys_buf.lock);
  }
  eyer_sys_ptr->aux_go_now = 0 ;
#ifdef AUX_RESPOR
  pthread_cond_signal(&eyer_sys_buf.aux_start_resp);
#endif
  pthread_mutex_unlock(&eyer_sys_buf.lock);
  printf("aux start!!\n");

}

void consume_fifo(uint32_t *wptr, uint32_t *rptr)
{
#ifdef AUX_EYER_SIM
  pthread_mutex_lock(&eyer_sys_buf.lock);
  *rptr = *rptr + 1;
  pthread_cond_signal(&eyer_sys_buf.notfull);
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#else
  write_reg(do_get_phy_addr(rptr), (read_reg(do_get_phy_addr(rptr), 0)+1));
#endif
}

void p0_tel_aux_and_wait(uint32_t *wptr, uint32_t *rptr)
{
  pthread_mutex_lock(&eyer_sys_buf.lock);
  *wptr = *wptr + 1;
  pthread_cond_signal(&eyer_sys_buf.notempty);
  while(*wptr != *rptr )
    {
      pthread_cond_wait(&eyer_sys_buf.notfull, &eyer_sys_buf.lock);
    }
  pthread_mutex_unlock(&eyer_sys_buf.lock);
#if 0
  if ( pthread_kill(eyer_sys_buf.thread_id_aux, 51) !=0 )
    {
      perror("pthread_kill error");
      exit(EXIT_FAILURE);
    }
#endif
}

void download_aux()
{
  fprintf(stderr, "----download_aux----\n");
  if ( eyer_sys_buf.aux_run == 0 ){
  }else
    {
      pthread_cancel(eyer_sys_buf.thread_id_aux);
      pthread_join(eyer_sys_buf.thread_id_aux, NULL);
      eyer_sys_ptr->aux_run = 0 ;
    }
}

#define AUX_PILOCY SCHED_OTHER
void aux_main(void (*fun)()){
  pthread_cleanup_push((void (*)(void*))aux_clr, NULL);
  while(1){
    wait_aux_start();
    (*fun)();
  }
  pthread_cleanup_pop(1);
}

void load_aux_pro(void *fun(void *))
{
  fprintf(stderr,"---------load_aux_pro--------\n");
  if ( eyer_sys_buf.aux_run == 0 ){
    int32_t res;
    res = pthread_attr_init(&eyer_sys_buf.thread_attr_aux);
    if ( res != 0 )
      {
    perror("Attribute creation failed");
    exit(EXIT_FAILURE);
      }
    res = pthread_attr_setschedpolicy(&eyer_sys_buf.thread_attr_aux, AUX_PILOCY);
    if ( res != 0 )
      {
    perror("seting schedpolicy failed");
    exit(EXIT_FAILURE);
      }

    res = pthread_attr_setdetachstate(&eyer_sys_buf.thread_attr_aux, PTHREAD_CREATE_DETACHED);
    if ( res != 0 )
      {
    perror("seting detached failed");
    exit(EXIT_FAILURE);
      } else
      printf("set the detachstate to PTHREAD_CREATE_DETACHED\n");

    res = pthread_create(&eyer_sys_buf.thread_id_aux,
             &eyer_sys_buf.thread_attr_aux,
             (void *(*)(void *))&aux_main,
             (void *)fun
             );

    if ( res != 0 )
      {
    perror("creat thread failed");
    exit(EXIT_FAILURE);
      }
#if 0
    int32_t min_pri;
    int32_t max_pri;
    min_pri = sched_get_priority_min(AUX_PILOCY);
    max_pri = sched_get_priority_max(AUX_PILOCY);
    printf("max: %d, min:%d\n", max_pri, min_pri);
    res = pthread_attr_getschedparam(&eyer_sys_buf.thread_attr_aux,
                     &eyer_sys_buf.scheduling_value
                     );

    if ( res != 0 )
      {
    perror("get schedpolicy failed");
    exit(EXIT_FAILURE);
      }

    eyer_sys_buf.scheduling_value.sched_priority = -10;
    res = pthread_attr_setschedparam(&eyer_sys_buf.thread_attr_aux,
                     &eyer_sys_buf.scheduling_value
                                     );
    if ( res != 0 )
      {
    perror("setting schedpolicy failed");
    exit(EXIT_FAILURE);
      }

    res = pthread_attr_getschedparam(&eyer_sys_buf.thread_attr_aux,
                     &eyer_sys_buf.scheduling_value
                     );
    if ( res != 0 )
      {
    perror("get schedpolicy failed");
    exit(EXIT_FAILURE);
      }
    else {
      printf("current thread priority is %d\n", eyer_sys_buf.scheduling_value.sched_priority);
    }
#endif

    eyer_sys_buf.aux_run = 1;
  }else{
    fprintf(stderr,"---------aux is running now--------\n");
  }
}

void load_aux_pro_bin(char * name, uint32_t *addr)
{
  FILE *fp;
  printf("load aux pro bin %s to %x\n", name, do_get_phy_addr((uint8_t*)addr));
  fp = fopen(name, "r+b");
  if (!fp)
    printf(" error while open %s \n", name);
  uint32_t but_ptr[1024];
  int32_t len;
  int32_t total_len;
  while ( len = fread(but_ptr, 4, 1024, fp) ){
    printf(" len of aux task(word) = %d\n",len);
    int32_t i;
    for ( i = 0; i<len; i++)
      {
    write_reg(do_get_phy_addr((uint8_t*)addr), but_ptr[i]);
    addr = addr + 4;
      }
  }
  printf("pro end address: %x\n", do_get_phy_addr((uint8_t*)addr));
  fclose(fp);
}

void eyer_wave_enable()
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->wave_thread;
  while ( (semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  thread_inact_ptr->sem_b.sem_op = 1;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_wave_enable semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ( (semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );

}

void eyer_stop()
{
  fprintf(stderr, "--------eyer stop-------\n");
  if ( eyer_sys_ptr->aux_run == 1 ){
    pthread_join(eyer_sys_ptr->thread_id_aux, NULL);
    eyer_sys_ptr->aux_run = 0 ;
  }

  kill_eyer_shm_sem(&eyer_sys_ptr->score_thread, 1);

  kill_eyer_shm_sem(&eyer_sys_ptr->hdec_thread, 1);
  kill_eyer_shm_sem(&eyer_sys_ptr->sram_thread, 1);
  kill_eyer_shm_sem(&eyer_sys_ptr->tcsm_thread, 1);
  kill_eyer_shm_sem(&eyer_sys_ptr->tcsm1_thread, 1);
  kill_eyer_shm_sem(&eyer_sys_ptr->aux_thread, 1);
  kill_eyer_shm_sem(&eyer_sys_ptr->ddr_thread, 1);
  kill_eyer_shm_sem(&eyer_sys_ptr->reset_thread, 0);
  kill_eyer_shm_sem(&eyer_sys_ptr->cmd_core_thread, 1);
  kill(eyer_sys_ptr->eyer_pid, EYER_KILL_SIG);
  while(1){}
  //puts("INFO: ---eyer_stop---\n");
  //kill_eyer_shm_sem(&eyer_sys_ptr->wave_thread);
}

void x86_stop(int32_t sig)
{
  puts("INFO: ---x86_stop END---\n");
  exit(0);
}

void eyer_wave_disable()
{
  thread_inact_p thread_inact_ptr = &eyer_sys_ptr->wave_thread;
  while ( (semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
  thread_inact_ptr->sem_b.sem_op = 2;  // self-incre
  if ( semop(thread_inact_ptr->semid, &thread_inact_ptr->sem_b, 1) == -1 )//act self-incr
    {
      fprintf(stderr, "eyer_wave_enable semaphore_p failed\n");
      exit(EXIT_FAILURE);
    }
  while ( (semctl(thread_inact_ptr->semid, 0, GETVAL)) != 0 );
}

#if 0
int32_t main(int32_t argc, char *argv[])
{
  int32_t running = 1;
  int32_t value;
  eyer_system_ini();

  int32_t ddr_mem[1000];
  eyer_sys_ptr->thread_addr_tab_buf.tab_dep = 1;
  eyer_sys_ptr->thread_addr_tab_buf.addr_context_buf[0].base_addr = ddr_mem;
  eyer_sys_ptr->thread_addr_tab_buf.addr_context_buf[0].high_addr = ddr_mem + 1000;
  thread_inact_p cmd_core_thread_ptr = &eyer_sys_ptr->cmd_core_thread;
  thread_inact_p hdec_thread_ptr = &eyer_sys_ptr->hdec_thread;
  eyer_addr_map( &eyer_sys_ptr->hdec_thread,//thread_inact_p thread_inact_ptr,
         &eyer_sys_ptr->thread_addr_tab_buf//thread_addr_tab_p thread_addr_tab_ptr
         );

  int32_t i ;
  for ( i = 0 ; i<5000; i++)
    {
      uint32_t ptr;
      cmd_core_write(cmd_core_thread_ptr,
             ddr_mem,
             0x12345678
                     );
      printf("%x\n", ddr_mem[0]);
      uint32_t value;
      value = cmd_core_read(cmd_core_thread_ptr,
                ddr_mem
                );
      printf("--%x\n", value);
    }
  shmdt(cmd_core_thread_ptr->shm_p); //download
  return 0;
}
#endif
#if 1
void *eyer_malloc(uint32_t *addr, uint32_t size)
{
  uint8_t *mem_ptr;
  if ( addr == NULL )
    mem_ptr = (uint8_t *)malloc(size);
  else
    mem_ptr = (uint8_t *)addr;

  if ( eyer_sys_ptr->safe_mod == 0 )
    {
      printf("eyer_malloc no safe mode\n");
    }else{
      uint8_t *haddr;
      haddr = mem_ptr + size;
      vmm_list_add(mem_ptr, haddr, 1);
    }
  return (void *)mem_ptr;
}

void *eyer_malloc_aliagn(uint32_t *addr, uint32_t size, uint32_t align_size)
{
  uint8_t *mem_ptr;
  if ( addr == NULL )
    mem_ptr = (uint8_t *)malloc(size);
  else
    mem_ptr = (uint8_t *)addr;

  if ( eyer_sys_ptr->safe_mod == 0 )
    {
      printf("eyer_malloc no safe mode\n");
    }else{
      uint8_t *haddr;
      haddr = mem_ptr + size;
      vmm_list_add(mem_ptr, haddr, align_size);
    }

  return (void *)mem_ptr;
}

#else
void * eyer_malloc( int32_t ptr, int32_t mem_size)
{
  uint8_t *mem_ptr;
  if ( ptr == NULL )
    mem_ptr = (uint8_t *)malloc(mem_size);
  else
    mem_ptr = (uint8_t *)ptr;
  //  puts("eyer_malloc");
  eyer_sys_ptr->thread_addr_tab_buf.addr_context_buf[eyer_sys_ptr->thread_addr_tab_buf.tab_dep].base_addr = (uint32_t)mem_ptr;
  eyer_sys_ptr->thread_addr_tab_buf.addr_context_buf[eyer_sys_ptr->thread_addr_tab_buf.tab_dep].high_addr = (uint32_t)(mem_ptr + mem_size);
  printf("base: %x-%x-%x\n", eyer_sys_ptr->thread_addr_tab_buf.addr_context_buf[eyer_sys_ptr->thread_addr_tab_buf.tab_dep].base_addr, eyer_sys_ptr->thread_addr_tab_buf.addr_context_buf[eyer_sys_ptr->thread_addr_tab_buf.tab_dep].high_addr, mem_size);
  eyer_sys_ptr->thread_addr_tab_buf.tab_dep = eyer_sys_ptr->thread_addr_tab_buf.tab_dep + 1;
  eyer_addr_map( &eyer_sys_ptr->hdec_thread,
         &eyer_sys_ptr->thread_addr_tab_buf
         );

  return (void *)mem_ptr;
}
#endif

uint32_t eyer_free(uint32_t *addr)
{
  uint8_t *mem_ptr;
  if ( addr == NULL )
    return 0;

  free(addr);
  vmm_list_del_bywaddr((uint8_t *)addr);
  return 1;
}

#if 0
void printf_ek(char *fmt, ...)
{
  va_list args;
  int32_t n;
  char sprintf_buf[1024];
  va_start(args, fmt);
  n = vsprintf(sprintf_buf, fmt, args);
  va_end(args);
  printf("%s", sprintf_buf);
  return n;
}
#else
#define printf_ek printf
#endif

uint32_t do_get_vaddr(uint8_t *addr)
{
  vmm_nod_p vmm_nod_ptr;
  vmm_nod_ptr = vmm_list_find_bywaddr(addr);
  if ( vmm_nod_ptr == NULL)
    {
      return (uint32_t)addr;
    }
  else{
    uint32_t vaddr;
    vaddr = vmm_nod_ptr->eyer_base_vaddr + ((unsigned int)addr - (unsigned int)vmm_nod_ptr->word_base_addr);
    return (uint32_t)vaddr;
  }
}

uint32_t do_get_vaddr_v2tlb(uint8_t *addr)
{
  uint32_t tmp = (uint32_t)addr;
  return ( (vremap[((tmp & 0xF0000000)>>28) & 0xF]<<28) | (tmp & 0x0FFFFFFF) );
}

#if 1
uint32_t do_get_phy_addr(uint8_t *addr)
{
  if ( (unsigned int)addr < TCSM0_SIZE*4+C_TCSM0_BASE &&  (unsigned int)addr >= C_TCSM0_BASE )
    {
      uint32_t ofst = (unsigned int)addr - C_TCSM0_BASE;
      return (ofst+TCSM0_BASE);
    }
  else if ( (unsigned int)addr < TCSM1_SIZE*4+C_TCSM1_BASE &&  (unsigned int)addr >= C_TCSM1_BASE )
    {
      uint32_t ofst = (unsigned int)addr - C_TCSM1_BASE;
      return (ofst+TCSM1_BASE);
    }
  else if ( (unsigned int)addr < SRAM_SIZE*4+C_SRAM_BASE &&  (unsigned int)addr >= C_SRAM_BASE )
    {
      uint32_t ofst = (unsigned int)addr - C_SRAM_BASE;
      return (ofst+SRAM_BASE);
    }else if ( eyer_sys_ptr->safe_mod == 0 )
      {
    return (uint32_t)addr;
      }
  else {
      vmm_nod_p vmm_nod_ptr;
      vmm_nod_ptr = vmm_list_find_bywaddr(addr);
      if ( vmm_nod_ptr == NULL)
    {
      return 0;
    }
      else
    {
      uint32_t phy_addr;
      phy_addr = vmm_nod_ptr->eyer_base_paddr + ((unsigned int)addr - (unsigned int)vmm_nod_ptr->word_base_addr);
      return phy_addr;
    }
      }
}
#else
uint32_t do_get_phy_addr(uint32_t vaddr){
  if ( vaddr < TCSM0_SIZE*4+C_TCSM0_BASE &&  vaddr >= C_TCSM0_BASE )
    {
      uint32_t ofst = vaddr - C_TCSM0_BASE;
      return (ofst+TCSM0_BASE);
    }
  else if ( vaddr < TCSM1_SIZE*4+C_TCSM1_BASE &&  vaddr >= C_TCSM1_BASE )
    {
      uint32_t ofst = vaddr - C_TCSM1_BASE;
      return (ofst+TCSM1_BASE);
    }
  else if ( vaddr < SRAM_SIZE*4+C_SRAM_BASE &&  vaddr >= C_SRAM_BASE )
    {
      uint32_t ofst = vaddr - C_SRAM_BASE;
      return (ofst+SRAM_BASE);
    }
  else
    {
      return vaddr;
    }
}
#endif

uint32_t *do_get_world_addr(uint32_t addr)
{
  vmm_nod_p vmm_nod_ptr;
  vmm_nod_ptr = vmm_list_find_bypaddr(addr);
  if ( vmm_nod_ptr == NULL)
    {
      return 0;
    }
  else
    {
      uint32_t *phy_addr;
      phy_addr = (uint32_t *)(vmm_nod_ptr->word_base_addr + ((uint32_t)addr - (uint32_t)vmm_nod_ptr->eyer_base_paddr));
      return phy_addr;
    }
}

void i_dcache_discard (int32_t c, int32_t d)
{
}

void i_cache (int32_t a, int32_t b, int32_t c){
}
void i_sync()
{
}
#ifdef EYER_EVA
#else
#define SHOW_LONG_OPT
void show_long(uint32_t c){
  printf("%x\n", c);
}
#endif

void show_vmm_list()
{
  printf("===========show_vmm_list============\n");
  list_all_vmm();
}

uint32_t eyer_ddr_cmp(uint32_t* ref_phy_addr, uint32_t ref_str,
          uint32_t* dut_phy_addr, uint32_t dut_str,
          uint32_t w, uint32_t h, uint32_t unit_size
          )
{
  printf("+++++++++++++eyer_ddr_cmp %x-%x++++++++++++\n", ref_phy_addr, dut_phy_addr);
  if ( unit_size == 2 ) //16bit
    {
      uint16_t *ref_addr = (uint16_t *)ref_phy_addr;
      uint16_t *dut_addr = (uint16_t *)dut_phy_addr;
      int i;
      int j;
      for ( i =0; i< h; i++)
    for ( j = 0; j<w; j++){
      if ( ref_addr[i*ref_str+j] != dut_addr[i*dut_str+j] )
        {
          printf("==========ERROR: [%x,%x]======\n", j, i);
          printf("====ref: %x, err:%x=====\n", ref_addr[i*ref_str+j], dut_addr[i*dut_str+j]);
          return 1;
        }
    }
    }
  else if ( unit_size == 4 ) //32bit
    {
      uint32_t *ref_addr = ref_phy_addr;
      uint32_t *dut_addr = dut_phy_addr;
      int i;
      int j;
      //return 1;
      for ( i =0; i<h; i++)
    for ( j = 0; j<w; j++){
      if ( ref_addr[i*ref_str+j] != dut_addr[i*dut_str+j] )
        {
          printf("==========ERROR: [%x,%x]======\n", j, i);
          printf("====ref: %x-%x, err:%x-%x=====\n", &ref_addr[i*ref_str+j], ref_addr[i*ref_str+j], &dut_addr[i*dut_str+j], dut_addr[i*dut_str+j]);
          return 1;
        }
    }
    }
  return 0;
}
#ifdef SHOW_LONG_OPT
#else
void show_long(uint32_t data)
{
  printf("%x-%x\n",&data, data);
}
#endif

void sys_time_wait(unsigned int time_cycle)
{
  int i;
  unsigned int cycle_value;
  clr_sys_cycle();
  do{
    usleep(100);
    cycle_value =   read_sys_cycle();
  }while( cycle_value < time_cycle);
}

void sys_valu_wait(unsigned int addr, int value)
{
  do{
    usleep(100);
  }while( read_reg(addr, 0) != value);
}

void eyer_ctrlc_exit(int a){
  eyer_stop();
  exit(0);
}

void eyer_segv_exit(int a){
  puts("Segmentation Fault!");
  eyer_stop();
  exit(0);
}

void eyer_reg_ctrlc(){
  signal(SIGINT, eyer_ctrlc_exit);
}

void eyer_reg_segv(){
  signal(SIGSEGV, eyer_segv_exit);
}

#ifdef __cplusplus
}
#endif
