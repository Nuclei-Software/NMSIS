CORE ?= n307fd
TARGET = NNREF
TOOLCHAIN ?= GCC
LIBIRAY_ROOT ?= ../../Library
PARALLEL ?=
BUILD := build
AR := riscv-nuclei-elf-ar

CORE_UPPER = $(shell echo $(CORE) | tr a-z A-Z)
TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)

MAKE_PARAMS = TOOLCHAIN=$(TOOLCHAIN) PARALLEL=$(PARALLEL) LIBIRAY_ROOT=$(LIBIRAY_ROOT) BUILD=$(BUILD)

include ../../Makefile.core

CORE_ARCH_ABI = $($(CORE_UPPER)_CORE_ARCH_ABI)
ifneq ($(words $(CORE_ARCH_ABI)), 2)
$(error There is no coresponding ARCH_ABI setting for CORE $(CORE), please check ../../Makefile.core)
endif
RISCV_ARCH = $(word 1, $(CORE_ARCH_ABI))
RISCV_ABI = $(word 2, $(CORE_ARCH_ABI))
RISCV_MODEL ?= medany

RISCV_ARCH_DEFINE = -DRISCV_ARCH=$(RISCV_ARCH) -DRISCV_ABI=$(RISCV_ABI) -DRISCV_MODEL=$(RISCV_MODEL)

TARGET_CMAKE_ROOT=nn_test
TARGET_CMAKE_BUILD=$(TARGET_CMAKE_ROOT)/$(BUILD)
TARGET_CMAKEFILE=$(TARGET_CMAKE_ROOT)/CMakeLists.txt
TARGET_LIB_ROOT = $(LIBIRAY_ROOT)/$(TARGET)/$(TOOLCHAIN)
TARGET_LIB_NAME = libnmsis_$(TARGET_LOWER)_$(RISCV_ARCH)
ifeq ($(wildcard $(TARGET_CMAKEFILE)),)
$(error Target CMake file $(TARGET_CMAKEFILE) doesn\'t exist, please check!)
endif

ifdef PARALLEL
MAKE_OPT = -j
endif

NNREFLIB_TARGETS = $(addsuffix _nnreflib,$(SUPPORTED_CORES))

.DEFAULT_GOAL = all

all: no_dsp
	@echo "Build, generate and install $(TARGET) library into $(TARGET_LIB_ROOT) all finished"

gen: gen_nnref_lib

help:
	@echo "Help about build and install Reference NN Library"
	@echo "Here are a list of make target we supported."
	@echo "CORE=[n205/n307/n307fd] all  It will build and install library for CORE"
	@echo "By default, the installed library will be saved into $(LIBIRAY_ROOT)"

no_dsp: build_no_dsp ar_no_dsp install_no_dsp
	@echo "Build, generate and install $(TARGET) library into $(TARGET_LIB_ROOT) without DSP enabled"

clean:
	rm -rf $(TARGET_CMAKE_BUILD)
	mkdir -p $(TARGET_CMAKE_BUILD)

build_no_dsp: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

install_no_dsp:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME).a $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)p.a $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)v.a $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)pv.a $(TARGET_LIB_ROOT)

ar_no_dsp:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME).a
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME)p.a
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME)v.a
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME)pv.a

$(NNREFLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _nnreflib,,$@)

gen_nnref_lib: $(NNREFLIB_TARGETS)
	@echo "Generated NN Reference library for cores: $(subst _nnreflib,,$^)"
