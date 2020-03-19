DSP_ENABLE ?= ON
CORE ?= n307fd
NANO ?= OFF

ROOT_DIR = ../../..
DSP_ROOT_DIR = ${ROOT_DIR}/DSP
NN_ROOT_DIR = ${ROOT_DIR}/NN
CORE_ROOT_DIR = ${ROOT_DIR}/Core
REF_NN_ROOT_DIR = ${NN_ROOT_DIR}/NN_Lib_Tests

COMPILER = riscv-nuclei-elf
CC      =  $(COMPILER)-gcc
CXX      =  $(COMPILER)-c++
LD      =  $(COMPILER)-ld
OBJDUMP =  $(COMPILER)-objdump
OBJCOPY =  $(COMPILER)-objcopy

TOOLCHAIN := GCC
INC_DIR = . $(CORE_ROOT_DIR)/Include $(DSP_ROOT_DIR)/Include $(NN_ROOT_DIR)/Include \
		  ${REF_NN_ROOT_DIR}/nn_test/Ref_Implementations
LIB_DIR = $(ROOT_DIR)/Library/DSP/$(TOOLCHAIN) \
		  $(ROOT_DIR)/Library/NN/$(TOOLCHAIN) \
		  $(ROOT_DIR)/Library/NNREF/$(TOOLCHAIN)


CORE_UPPER = $(shell echo $(CORE) | tr a-z A-Z)
include ../../../Makefile.core

CORE_ARCH_ABI = $($(CORE_UPPER)_CORE_ARCH_ABI)
ifneq ($(words $(CORE_ARCH_ABI)), 2)
$(error There is no coresponding ARCH_ABI setting for CORE $(CORE), please check Makefile.core)
endif

RISCV_ARCH = $(word 1, $(CORE_ARCH_ABI))
RISCV_ABI = $(word 2, $(CORE_ARCH_ABI))

RISCV_CORE_OPT=-march=$(RISCV_ARCH) -mabi=$(RISCV_ABI)
SPIKE_ROOT ?= $(shell readlink -f $(shell dirname $(shell which spike))/../../)
ifeq ($(subst 32,64,$(RISCV_ABI)),$(RISCV_ABI))
SPIKE_ISA ?= rv64gct
SPIKE ?= $(SPIKE_ROOT)/64/bin/spike
PK ?= $(SPIKE_ROOT)/64/riscv64-unknown-elf/bin/pk
else
SPIKE_ISA ?= rv32gct
SPIKE ?= $(SPIKE_ROOT)/32/bin/spike
PK ?= $(SPIKE_ROOT)/32/riscv32-unknown-elf/bin/pk
endif

ifeq ($(DSP_ENABLE),ON)
$(warning "enable dsp")
lib = -lnmsis_nn_$(RISCV_ARCH)p -lnmsis_nnref_$(RISCV_ARCH) -lnmsis_dsp_$(RISCV_ARCH)p -lm
DEFINES += -D__RISCV_FEATURE_DSP=1
else
$(warning "disable dsp")
lib = -lnmsis_nn_$(RISCV_ARCH) -lnmsis_nnref_$(RISCV_ARCH) -lnmsis_dsp_$(RISCV_ARCH) -lm
endif

ifeq ($(NANO),ON)
LDFLAGS = --specs=nano.specs --specs=nosys.specs
endif

SRC_C = $(wildcard *.c)
SRC_CPP = $(wildcard *.cpp)

INCS_OPT = $(foreach dir,$(INC_DIR),-I$(dir))
LIBS_OPT = $(foreach dir,$(LIB_DIR),-L$(dir)) $(lib)

all: run.elf

run.elf: $(SRC_CPP)
	$(CXX) $(RISCV_CORE_OPT) $(DEFINES) -O2 -g $(LDFLAGS) $< $(LIBS_OPT) $(INCS_OPT) -o $@
	$(OBJCOPY) -O binary $@ run.bin

test: $(SRC_CPP)
	$(CXX) $(RISCV_CORE_OPT) $(DEFINES) -g -T spike.lds -nostartfiles $< $(LIBS_OPT) $(INCS_OPT) -o run.elf

run: run.elf
	$(SPIKE) --isa=$(SPIKE_ISA) $(PK) $<

debug: run.elf
	$(SPIKE) --isa=$(SPIKE_ISA) -d $(PK) $<

diss:
	$(OBJDUMP) -S -d run.elf > run.diss

clean:
	rm *.elf *.diss *.bin *.hex -f
