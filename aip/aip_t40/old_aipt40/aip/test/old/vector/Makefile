# Makefile
CC              = gcc
CXX             = g++
CROSS_COMPILE	= mips-linux-gnu-

SRCS_DIR        = $(PWD)
INC             = -I$(SRCS_DIR)/../../include/model -I$(SRCS_DIR)/../../include/api -I$(SRCS_DIR)/../../include/hw_api -I$(SRCS_DIR) -I../../../../../common/lib/eyer/drv 
CFLAGS          = -m32 -lm -O4 -Wall -g -DEYER_SIM_ENV

EYER_CFLAGS  	= $(CFLAGS) -DEYER_SIM_ENV -ffast-math \
		  -DHAVE_MALLOC_H -DARCH_X86 -DSYS_LINUX -s -fomit-frame-pointer -s -lpthread
EYER_CFLAGS 	+= -I./ -I../../../../../common/lib/eyer/drv 

CSE_CFLAGS 	= -DCSE_SIM_ENV -g
CSE_CFLAGS 	+= -Wall -g -static -EL -mfp64 -pthread
CSE_CFLAGS 	+= -I./driver_cse -I./

OBJS_DIR = ./objs

MDL_DIR  = $(SRCS_DIR)/../../source/model
MDL_SRCS = $(wildcard $(MDL_DIR)/*.c)
MDL_OBJS = $(patsubst %.c, ${OBJS_DIR}/%.o, $(notdir $(MDL_SRCS)))
MDL_CFLAGS = -m32 -lm -O4 -Wall -std=c99 -g -DEYER_SIM_ENV

EYER_DIR  = $(SRCS_DIR)/../../../../../common/lib/eyer/drv
EYER_SRCS = $(wildcard $(EYER_DIR)/*.c)
EYER_OBJS = $(patsubst %.c, ${OBJS_DIR}/%.o, $(notdir $(EYER_SRCS)))

API_DIR  = $(SRCS_DIR)/../../source/api
API_SRCS = $(wildcard $(API_DIR)/*.cpp)
API_OBJS = $(patsubst %.cpp, ${OBJS_DIR}/%.o, $(notdir $(API_SRCS)))
API_CFLAGS = -m32 -lm -O4 -Wall -std=gnu++0x -DEYER_SIM_ENV

HW_API_DIR  = $(SRCS_DIR)/../../source/hw_api
HW_API_SRCS = $(wildcard $(HW_API_DIR)/*.c)
HW_API_OBJS = $(patsubst %.c, ${OBJS_DIR}/%.o, $(notdir $(HW_API_SRCS)))
HW_API_CFLAGS = -m32 -lm -O4 -Wall -std=c99 -DEYER_SIM_ENV

TARGET = t_bscaler_c t_bscaler_t

all: $(TARGET)

test:
	@echo $(HW_API_OBJS) $(API_OBJS)

$(OBJS_DIR)/%.o: $(EYER_DIR)/%.c
	$(CC) $(EYER_CFLAGS) $(INC) -c -o $@ $<

$(OBJS_DIR)/%.o: $(MDL_DIR)/%.c
	$(CC) $(MDL_CFLAGS) $(INC) -c $< -o $@

$(OBJS_DIR)/%.o: $(API_DIR)/%.cpp
	$(CXX) $(API_CFLAGS) $(INC) -c $< -o $@

$(OBJS_DIR)/%.o: $(HW_API_DIR)/%.c
	$(CC) $(HW_API_CFLAGS) $(INC) -c $< -o $@

t_bscaler_c: $(MDL_OBJS) $(EYER_OBJS) $(HW_API_OBJS) t_bscaler_c.c  t_vector_c.h
	@$(CC) -std=c99 $(EYER_CFLAGS) $(INC) -o $@ $^

t_bscaler_t: $(MDL_OBJS) $(EYER_OBJS) $(HW_API_OBJS) t_bscaler_t.c  t_vector_t.h
	@$(CC) -std=c99 $(EYER_CFLAGS) $(INC) -o $@ $^


#t_bscaler_resize_line_nv12: $(MDL_OBJS) t_bscaler_resize.c
#	@echo "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#	@$(CC) $(CFLAGS) $(INC) -o $@ $^


#eyer_driver.o: 	../../../common/lib/eyer/drv/*.c
# 	       	$(CC) -I../../../common/lib/eyer/drv -pipe -fPIC -O2 -DPERMIT_ANY_WORD_WIDTH -m32 -c -o $@ $<
# 
#bscaler_frmt_eyer:	eyer_driver.o bscaler_frmt.c bscaler_mdl.c
# 			$(CC) -o $@ $+ $(EYER_CFLAGS)
# 
#bscaler_frmc_eyer_affine_sim:	eyer_driver.o bscaler_frmc.c bscaler_mdl.c
# 				$(CC) -o $@ $+ $(EYER_CFLAGS)
# 
#bscaler_frmc_chain_eyer:	eyer_driver.o bscaler_chain_frmc.c bscaler_mdl.c
# 			$(CC) -o $@ $+ $(EYER_CFLAGS)
# 
#bscaler_frmt_chain_eyer:	eyer_driver.o bscaler_chain_frmt.c bscaler_mdl.c
# 			$(CC) -o $@ $+ $(EYER_CFLAGS)
# 
#bscaler_frmt_cse: 	bscaler_frmt.c driver_cse/*.c bscaler_mdl.c 
# 			$(CROSS_COMPILE)$(CC) $(CSE_CFLAGS) -o $@ $^ 
# 
#bscaler_frmc_cse: 	bscaler_frmc.c driver_cse/*.c bscaler_mdl.c
# 			$(CROSS_COMPILE)$(CC) $(CSE_CFLAGS) -o $@ $^ 
# 
#bscaler_frmt_fpga:
# 
#bscaler_frmc_fpga:
# 
##bscaler_frmc_eyer_test:	eyer_driver.o bscaler_frmc_test.c
#		$(CC) -o $@ $+ $(EYER_CFLAGS)



clean:
	rm -f $(OBJS_DIR)/*.o
