NMSIS_LIB_SORTED := $(sort $(NMSIS_LIB))
NMSIS_LIB_ARCH ?= $(RISCV_ARCH)

INCDIRS += $(NUCLEI_SDK_NMSIS)/Core/Include

# When set to 1, will use compatiable library
# provided in toolchain such as terapines zcc toolchain
# present NMSIS DSP/NN compatiable library
NMSIS_LIB_COMPAT ?=

ifeq ($(filter nmsis_nn,$(NMSIS_LIB_SORTED)),nmsis_nn)
INCDIRS += $(NUCLEI_SDK_NMSIS)/NN/Include
LIBDIRS += $(NUCLEI_SDK_NMSIS)/Library/NN/GCC
ifeq ($(TOOLCHAIN),terapines)
ifeq ($(NMSIS_LIB_COMPAT),1)
LDLIBS += -lnn
else
LDLIBS += -lnmsis_nn_$(NMSIS_LIB_ARCH)
endif
else
LDLIBS += -lnmsis_nn_$(NMSIS_LIB_ARCH)
endif
endif

ifeq ($(filter nmsis_dsp,$(NMSIS_LIB_SORTED)),nmsis_dsp)
INCDIRS += $(NUCLEI_SDK_NMSIS)/DSP/Include \
	$(NUCLEI_SDK_NMSIS)/DSP/PrivateInclude
LIBDIRS += $(NUCLEI_SDK_NMSIS)/Library/DSP/GCC
ifeq ($(TOOLCHAIN),terapines)
ifeq ($(NMSIS_LIB_COMPAT),1)
LDLIBS += -ldsp
else
LDLIBS += -lnmsis_dsp_$(NMSIS_LIB_ARCH)
endif
else
LDLIBS += -lnmsis_dsp_$(NMSIS_LIB_ARCH)
endif
endif
