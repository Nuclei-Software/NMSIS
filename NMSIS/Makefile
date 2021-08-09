TARGET ?= DSP
CORE ?= nx600fd
DSP64 ?= ON
RISCV_UNALIGN ?= OFF
TOOLCHAIN ?= GCC
LIBIRAY_ROOT ?= Library
BUILD_SELECT ?= ALL
PARALLEL ?=
BUILD := build
AR := riscv-nuclei-elf-ar

CORE_UPPER = $(shell echo $(CORE) | tr a-z A-Z)
TARGET_LOWER = $(shell echo $(TARGET) | tr A-Z a-z)

MAKE_PARAMS = DSP64=$(DSP64) RISCV_UNALIGN=$(RISCV_UNALIGN) TOOLCHAIN=$(TOOLCHAIN) PARALLEL=$(PARALLEL) LIBIRAY_ROOT=$(LIBIRAY_ROOT) BUILD=$(BUILD)

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

DSP_NNLIB_TARGETS = $(addsuffix _nnlib_dsp,$(SUPPORTED_CORES))
VECTOR_NNLIB_TARGETS = $(addsuffix _nnlib_vector,$(VECTOR_SUPPORTED_CORES))
DSP_DSPLIB_TARGETS = $(addsuffix _dsplib_dsp,$(SUPPORTED_CORES))
VECTOR_DSPLIB_TARGETS = $(addsuffix _dsplib_vector,$(VECTOR_SUPPORTED_CORES))

ifdef PARALLEL
MAKE_OPT = -j
endif

.DEFAULT_GOAL = all

.PHONY: all all_vector dsp vector vector_dsp refer help gen clean build_dsp build_vector build_vector_dsp build_refer install_dsp install_vector install_vector_dsp install_refer ar_dsp ar_vector ar_vector_dsp ar_refer gen_dsp_lib gen_nn_lib strip strip_nn_lib strip_dsp_lib strip_symbols doc doc_all ctags

all: dsp refer
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) all finished"

all_vector: vector vector_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) all finished"

help:
	@echo "Help about build and install NMSIS"
	@echo "Command make TARGET=<target> CORE=<core> all  It will build and install dsp and refer library for TARGET and CORE"
	@echo "Command make TARGET=<target> CORE=<core> all_vector  It will build and install vector and vector with library for TARGET and CORE"
	@echo "Here are a list of make target we supported."
	@echo "gen           It will build and install NN and DSP libraries for all the cores list in Makefile.core"
	@echo "gen_nn_lib    It will build and install NN libraries for all the cores list in Makefile.core"
	@echo "gen_dsp_lib   It will build and install DSP libraries for all the cores list in Makefile.core"
	@echo "Here are a list of make options we supported."
	@echo "BUILD_SELECT  Select supported extension, support: ALL REFER DSP VECTOR DSP_VECTOR. eg. BUILD_SELECT=ALL"
	@echo "TARGET       Currently only NN and DSP library suppported, eg. TARGET=DSP"
	@echo "CORE         Core to build, dsp supported cores: $(SUPPORTED_CORES), vector supported cores: $(VECTOR_SUPPORTED_CORES), eg. CORE=n307"
	@echo "DSP64        Enable DSP64 or not using ON, OFF, eg. DSP64=ON"
	@echo "RISCV_UNALIGN  Enable RISC-V memory access without unalign R/W feature using ON, OFF, eg. RISCV_UNALIGN=ON"
	@echo "PARALLEL     Compile library in PARALLEL, enable PARALLEL by PARALLEL=1"
	@echo "By default, the installed library will be saved into $(LIBIRAY_ROOT)"

ifeq ($(BUILD_SELECT),ALL)

dsp: build_dsp ar_dsp install_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with DSP enabled, DSP64=$(DSP64) finished"

refer: build_refer ar_refer install_refer
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) without DSP enabled finished"

vector: build_vector ar_vector install_vector
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with VECTOR enabled, DSP64=$(DSP64) finished"

vector_dsp: build_vector_dsp ar_vector_dsp install_vector_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with VECTOR enabled, DSP64=$(DSP64) finished"

else
ifeq ($(BUILD_SELECT),DSP)

dsp: build_dsp ar_dsp install_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with DSP enabled, DSP64=$(DSP64) finished"

refer:

vector:

vector_dsp:
	@echo "Only build DSP"

else
ifeq ($(BUILD_SELECT),REFER)

refer: build_refer ar_refer install_refer
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) without DSP enabled finished"

dsp:

vector:

vector_dsp:
	@echo "Only build REFER"

else
ifeq ($(BUILD_SELECT),VECTOR)

vector: build_vector ar_vector install_vector
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with VECTOR enabled, DSP64=$(DSP64) finished"

dsp:

refer:

vector_dsp:
	@echo "Only build VECTOR"

else
ifeq ($(BUILD_SELECT),DSP_VECTOR)

vector_dsp: build_vector_dsp ar_vector_dsp install_vector_dsp
	@echo "Build, generate and install $(TARGET) library for $(CORE) into $(TARGET_LIB_ROOT) with VECTOR enabled, DSP64=$(DSP64) finished"

dsp:

refer:

vector:
	@echo "Only build DSP_VECTOR"
endif
endif
endif
endif
endif

gen: gen_nn_lib gen_dsp_lib
	@echo "Build, generate and install all the libraries for $(SUPPORTED_CORES) into $(TARGET_LIB_ROOT)"

strip: strip_nn_lib strip_dsp_lib
	@echo "Strip symbols in libraries for $(SUPPORTED_CORES) in $(TARGET_LIB_ROOT)"

clean:
	rm -rf $(TARGET_CMAKE_BUILD)
	mkdir -p $(TARGET_CMAKE_BUILD)

build_dsp: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) -DDSP_ENABLE=ON -DVECTOR_ENABLE=OFF -DDSP64=$(DSP64) ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

build_vector: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) -DDSP_ENABLE=OFF -DVECTOR_ENABLE=ON -DDSP64=$(DSP64) ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

build_vector_dsp: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) -DDSP_ENABLE=ON -DVECTOR_ENABLE=ON -DDSP64=$(DSP64) ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

build_refer: clean
	cd $(TARGET_CMAKE_BUILD); cmake $(RISCV_ARCH_DEFINE) -DDSP_ENABLE=OFF -DVECTOR_ENABLE=OFF -DRISCV_UNALIGN=$(RISCV_UNALIGN) ../
	make $(MAKE_OPT) -C $(TARGET_CMAKE_BUILD)

install_dsp:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)p.a $(TARGET_LIB_ROOT)

install_vector:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)v.a $(TARGET_LIB_ROOT)

install_vector_dsp:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME)pv.a $(TARGET_LIB_ROOT)

install_refer:
	-mkdir -p $(TARGET_LIB_ROOT)
	cp -f $(TARGET_CMAKE_BUILD)/$(TARGET_LIB_NAME).a $(TARGET_LIB_ROOT)

ar_dsp:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME)p.a

ar_vector:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME)v.a

ar_vector_dsp:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME)pv.a

ar_refer:
	cd $(TARGET_CMAKE_BUILD) ; find ./ -name "*.o" | xargs $(AR) rcs $(TARGET_LIB_NAME).a

$(DSP_NNLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _nnlib_dsp,,$@)    TARGET=NN all

$(VECTOR_NNLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _nnlib_vector,,$@)    TARGET=NN all_vector

$(DSP_DSPLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _dsplib_dsp,,$@)   TARGET=DSP all

$(VECTOR_DSPLIB_TARGETS):
	make $(MAKE_PARAMS) CORE=$(subst _dsplib_vector,,$@)   TARGET=DSP all_vector

strip_symbols:
	@echo "Strip out symbols for TARGET=$(TARGET)"
	riscv-nuclei-elf-strip -g $(TARGET_LIB_ROOT)/*.a

strip_nn_lib:
	make $(MAKE_PARAMS) TARGET=NN strip_symbols

strip_dsp_lib:
	make $(MAKE_PARAMS) TARGET=DSP strip_symbols

gen_dsp_lib: $(DSP_DSPLIB_TARGETS) $(VECTOR_DSPLIB_TARGETS)
	@echo "Generated DSP library for cores: $(subst _dsplib_dsp,,$(SUPPORTED_CORES))"
	@echo "Generated DSP library for cores: $(subst _dsplib_vector,,$(VECTOR_SUPPORTED_CORES))"

gen_nn_lib: $(DSP_NNLIB_TARGETS) $(VECTOR_NNLIB_TARGETS)
	@echo "Generated NN library for cores: $(subst _nnlib_dsp,,$(SUPPORTED_CORES))"
	@echo "Generated NN library for cores: $(subst _nnlib_vector,,$(VECTOR_SUPPORTED_CORES))"

#Include optional Makefile.release
-include Makefile.release
#Include optional Makefile.utils
-include Makefile.utils
