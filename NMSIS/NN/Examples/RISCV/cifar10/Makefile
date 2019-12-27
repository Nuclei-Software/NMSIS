ROOT_DIR = ../../../..
DSP_ROOT_DIR = ${ROOT_DIR}/DSP
NN_ROOT_DIR = ${ROOT_DIR}/NN
CORE_ROOT_DIR = ${ROOT_DIR}/Core

COMPILER = riscv-nuclei-elf
CC      =  $(COMPILER)-gcc
LD      =  $(COMPILER)-ld
OBJDUMP =  $(COMPILER)-objdump
OBJCOPY =  $(COMPILER)-objcopy

TOOLCHAIN := GCC
INC_DIR = . $(CORE_ROOT_DIR)/Include $(DSP_ROOT_DIR)/Include $(NN_ROOT_DIR)/Include
LIB_DIR = $(ROOT_DIR)/Library/DSP/$(TOOLCHAIN) \
		  $(ROOT_DIR)/Library/NN/$(TOOLCHAIN)

DSP_ENABLE ?= ON
CORE ?= n307fd
NANO ?= OFF
TEST_IMAGE ?= cat1

CORE_UPPER = $(shell echo $(CORE) | tr a-z A-Z)
include ../../../../Makefile.core

CORE_ARCH_ABI = $($(CORE_UPPER)_CORE_ARCH_ABI)
ifneq ($(words $(CORE_ARCH_ABI)), 2)
$(error There is no coresponding ARCH_ABI setting for CORE $(CORE), please check Makefile.core)
endif

RISCV_ARCH = $(word 1, $(CORE_ARCH_ABI))
RISCV_ABI = $(word 2, $(CORE_ARCH_ABI))

RISCV_CORE_OPT=-march=$(RISCV_ARCH) -mabi=$(RISCV_ABI)

ifeq ($(DSP_ENABLE),ON)
$(warning "enable dsp")
lib = -lnmsis_nn_$(RISCV_ARCH)p -lnmsis_dsp_$(RISCV_ARCH)p -lm
DEFINES += -D__RISCV_FEATURE_DSP=1
else
$(warning "disable dsp")
lib = -lnmsis_nn_$(RISCV_ARCH) -lnmsis_dsp_$(RISCV_ARCH) -lm
endif

ifeq ($(NANO),ON)
LDFLAGS = --specs=nano.specs --specs=nosys.specs
endif

ifdef TEST_IMAGE
DEFINES += -DTEST_IMAGE=$(TEST_IMAGE)
endif

SRC_C = $(wildcard *.c)
SPIKE ?= spike
PK ?= /home/share/tools/riscv/32/riscv32-unknown-elf/bin/pk

INCS_OPT = $(foreach dir,$(INC_DIR),-I$(dir))
LIBS_OPT = $(foreach dir,$(LIB_DIR),-L$(dir)) $(lib)

all: run.elf

run.elf: $(SRC_C)
	$(CC) $(RISCV_CORE_OPT) -O2 -g $(DEFINES) $(LDFLAGS) $< $(LIBS_OPT) $(INCS_OPT) -o $@
	$(OBJCOPY) -O binary $@ run.bin

test: $(SRC_C)
	$(CC) $(RISCV_CORE_OPT) -g -flto $(DEFINES) $< $(LIBS_OPT) $(INCS_OPT) -o run.elf -Xlinker -verbose

run: run.elf
	$(SPIKE) --isa=rv32gct $(PK) $<

debug: run.elf
	$(SPIKE) --isa=rv32gct -d $(PK) $<

diss:
	$(OBJDUMP) -S -d run.elf > run.diss

clean:
	rm run.elf *.diss *.bin *.hex -f
