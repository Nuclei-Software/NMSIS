TARGET ?= DSP
CORE ?= n307fd
DSP64 ?= ON
TOOLCHAIN ?= GCC
LIBIRAY_ROOT ?= Library
PARELLEL ?=
BUILD := build

CORE_UPPER = $(shell echo $(CORE) | tr a-z A-Z)
TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)

MAKE_PARAMS = DSP64=$(DSP64) TOOLCHAIN=$(TOOLCHAIN) PARELLEL=$(PARELLEL) LIBIRAY_ROOT=$(LIBIRAY_ROOT) BUILD=$(BUILD) 

include Makefile.core

CORE_ARCH_ABI = $($(CORE_UPPER)_CORE_ARCH_ABI)
ifneq ($(words $(CORE_ARCH_ABI)), 2)
$(error There is no coresponding ARCH_ABI setting for CORE $(CORE), please check Makefile.core)
endif

RISCV_ARCH = $(word 1, $(CORE_ARCH_ABI))
RISCV_ABI = $(word 2, $(CORE_ARCH_ABI))
RISCV_ARCH_DEFINE = -DRISCV_ARCH=$(RISCV_ARCH) -DRISCV_ABI=$(RISCV_ABI)

TARGET_CMAKE_ROOT=$(TARGET)/Source
TARGET_CMAKE_BUILD=$(TARGET)/Source/$(BUILD)
TARGET_CMAKEFILE=$(TARGET_CMAKE_ROOT)/CMakeLists.txt
TARGET_LIB_ROOT = $(LIBIRAY_ROOT)/$(TARGET)/$(TOOLCHAIN)
TARGET_LIB_NAME = libnmsis_$(TARGET_LOWER)_$(RISCV_ARCH)
ifeq ($(wildcard $(TARGET_CMAKEFILE)),)
$(error Target CMake file $(TARGET_CMAKEFILE) doesn\'t exist, please check!)
endif

NNLIB_TARGETS = $(addsuffix _nnlib,$(SUPPORTED_CORES))
DSPLIB_TARGETS = $(addsuffix _dsplib,$(SUPPORTED_CORES))

ifdef PARELLEL
MAKE_OPT = -j
endif

.DEFAULT_GOAL = all

.PHONY: all dsp no_dsp help gen clean build_dsp build_no_dsp install_dsp install_no_dsp ar_dsp ar_no_dsp gen_dsp_lib gen_nn_lib doc doc_all ctags

all: dsp no_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) all finished"

help:
	@echo "Help about build and install NMSIS"
	@echo "Command make TARGET=<target> CORE=<core> all  It will build and install library for TARGET and CORE"
	@echo "Here are a list of make target we supported."
	@echo "gen           It will build and install NN and DSP libraries for all the cores list in Makefile.core"
	@echo "gen_nn_lib    It will build and install NN libraries for all the cores list in Makefile.core"
	@echo "gen_dsp_lib   It will build and install DSP libraries for all the cores list in Makefile.core"
	@echo "Here are a list of make options we supported."
	@echo "TARGET       Currently only NN and DSP library suppported, eg. TARGET=DSP"
	@echo "CORE         Core to build, supported cores: $(SUPPORTED_CORES), eg. CORE=n307"
	@echo "DSP64        Enable DSP64 or not using ON, OFF, eg. DSP64=ON"
	@echo "PARELLEL     Compile library in parellel, enable parellel by PARELLEL=1"
	@echo "By default, the installed library will be saved into $(LIBIRAY_ROOT)"

dsp: build_dsp ar_dsp install_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with DSP enabled, DSP64=$(DSP64) finished"

no_dsp: build_no_dsp ar_no_dsp install_no_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) without DSP enabled finished"

gen: gen_nn_lib gen_dsp_lib
	@echo "Build, generate and install all the libraries for $(SUPPORTED_CORES) into $(TARGET_LIB_ROOT)"
	
clean:
	rm -rf $(TARGET_CMAKE_BUILD) 
	mkdir -p $(TARGET_CMAKE_BUILD)

build_dsp: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) -DDSP_ENABLE=ON -DDSP64=$(DSP64) ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

build_no_dsp: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) -DDSP_ENABLE=OFF ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

install_dsp:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)p.a $(TARGET_LIB_ROOT)

install_no_dsp:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME).a $(TARGET_LIB_ROOT)

ar_dsp:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs ar rcs $(TARGET_LIB_NAME)p.a

ar_no_dsp:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs ar rcs $(TARGET_LIB_NAME).a

$(NNLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _nnlib,,$@)    TARGET=NN

$(DSPLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _dsplib,,$@)   TARGET=DSP

gen_dsp_lib: $(DSPLIB_TARGETS)
	@echo "Generated DSP library for cores: $(subst _dsplib,,$^)"

gen_nn_lib: $(NNLIB_TARGETS)
	@echo "Generated NN library for cores: $(subst _nnlib,,$^)"

#Include optional Makefile.release
-include Makefile.release
#Include optional Makefile.utils
-include Makefile.utils
