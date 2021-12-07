TARGET ?= nn_test

NUCLEI_SDK_ROOT ?= ../../$(BUILD_ROOT_DIR)
NUCLEI_SDK_NMSIS ?= $(NUCLEI_SDK_ROOT)/NMSIS

SRCDIRS = .

INCDIRS = . $(NUCLEI_SDK_NMSIS)/NN/NN_Lib_Tests/nn_test/Ref_Implementations

LIBDIRS = $(NUCLEI_SDK_NMSIS)/Library/NNREF/GCC

ARCH_EXT ?=
BENCH_UNIT ?=
RISCV_ALIGN ?= OFF
COMMON_FLAGS ?= -O2
STDCLIB ?= newlib_full
NMSIS_LIB := nmsis_nn nmsis_dsp

ifeq ($(BENCH_UNIT),instret)
$(warning "Benchmark by reading cpu instret")
COMMON_FLAGS += -DREAD_CYCLE=read_cpu_instret
else
$(warning "Benchmark by reading cpu cycle")
endif

ifeq ($(RISCV_ALIGN),ON)
COMMON_FLAGS += -DRISCV_ALIGN_ACCESS
endif

LDLIBS = -lnmsis_nnref_$(NMSIS_LIB_ARCH) -lm

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
