TARGET ?= cifar10
TEST_IMAGE ?= cat1

NUCLEI_SDK_ROOT ?= ../../$(BUILD_ROOT_DIR)
NUCLEI_SDK_NMSIS ?= $(NUCLEI_SDK_ROOT)/NMSIS

SRCDIRS = .

INCDIRS = . $(NUCLEI_SDK_NMSIS)/DSP/Include $(NUCLEI_SDK_NMSIS)/NN/Include

LIBDIRS = $(NUCLEI_SDK_NMSIS)/Library/DSP/GCC $(NUCLEI_SDK_NMSIS)/Library/NN/GCC

DSP_ENABLE ?= ON
RISCV_ALIGN ?= OFF
COMMON_FLAGS ?=
NEWLIB ?=
BENCH_UNIT ?=

ifdef TEST_IMAGE
DEFINES += -DTEST_IMAGE=$(TEST_IMAGE)
endif

ifeq ($(BENCH_UNIT),instret)
$(warning "Benchmark by reading cpu instret")
COMMON_FLAGS += -DREAD_CYCLE=read_cpu_instret
else
$(warning "Benchmark by reading cpu cycle")
endif

ifeq ($(DSP_ENABLE),ON)
$(warning "Use and link with RISC-V DSP enabled library")
COMMON_FLAGS += -D__RISCV_FEATURE_DSP=1 -D__RISCV_FEATURE_DSP=1
NMSIS_LIB_SUFFIX=$(RISCV_ARCH)p
else
$(warning "Use and link with RISC-V DSP disabled library")
ifeq ($(RISCV_ALIGN),ON)
COMMON_FLAGS += -DRISCV_ALIGN_ACCESS
endif
NMSIS_LIB_SUFFIX=$(RISCV_ARCH)
endif
LDLIBS = -lnmsis_nn_$(NMSIS_LIB_SUFFIX) -lnmsis_dsp_$(NMSIS_LIB_SUFFIX) -lm


include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
