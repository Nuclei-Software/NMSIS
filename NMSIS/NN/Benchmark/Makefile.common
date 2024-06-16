TARGET ?= nn_benchmark

NUCLEI_SDK_ROOT ?= ../../$(BUILD_ROOT_DIR)
NUCLEI_SDK_NMSIS ?= $(NUCLEI_SDK_ROOT)/NMSIS

ARCH_EXT ?=
BENCH_UNIT ?=
RISCV_ALIGN ?= OFF
COMMON_FLAGS ?= -O2
STDCLIB ?= newlib_full
NMSIS_LIB := nmsis_nn

SUBDIR:=$(wildcard *)
SRCDIRS += . $(SUBDIR)
INCDIRS += . Utils


ifeq ($(BENCH_UNIT),instret)
$(warning "Benchmark by reading cpu instret")
COMMON_FLAGS += -DREAD_CYCLE=read_cpu_instret
else
$(warning "Benchmark by reading cpu cycle")
endif

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
