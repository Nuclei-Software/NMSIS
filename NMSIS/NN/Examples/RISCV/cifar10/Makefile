TARGET ?= cifar10
TEST_IMAGE ?= cat1

NUCLEI_SDK_ROOT ?= ../../$(BUILD_ROOT_DIR)
NUCLEI_SDK_NMSIS ?= $(NUCLEI_SDK_ROOT)/NMSIS

SRCDIRS = .
INCDIRS = .

ARCH_EXT ?=
RISCV_ALIGN ?= OFF
COMMON_FLAGS ?=
BENCH_UNIT ?=
STDCLIB ?= newlib_full
NMSIS_LIB := nmsis_nn

ifdef TEST_IMAGE
COMMON_FLAGS += -DTEST_IMAGE=$(TEST_IMAGE)
endif

ifeq ($(BENCH_UNIT),instret)
$(warning "Benchmark by reading cpu instret")
COMMON_FLAGS += -DREAD_CYCLE=read_cpu_instret
else
$(warning "Benchmark by reading cpu cycle")
endif

ifeq ($(RISCV_ALIGN),ON)
COMMON_FLAGS += -DRISCV_ALIGN_ACCESS
endif

LDLIBS = -lm

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
