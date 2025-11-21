SUITE ?= BasicTestsF32

ifeq ($(findstring F16,$(SUITE)),)
PICKLE := Output.pickle 
else
ifeq ($(findstring zfh,$(ARCH_EXT)),)
$(error Unsupported architecture)
else
PICKLE := Output_f16.pickle
endif
endif

TARGET := $(shell python3 processTests.py -f $(PICKLE) -e $(SUITE) >/dev/null 2>&1 && echo test)

CXX_SRCS = Source/Tests/$(SUITE).cpp

ifneq ($(findstring MFCC,$(SUITE)),)
C_SRCS += Source/Tests/mfccdata.c Source/Tests/mfccdata_f16.c
endif

SRCDIRS += . FrameworkSource GeneratedSource

EXCLUDE_SRCS = Source/Tests/*Neon*.cpp \
			   FrameworkSource/Semihosting.cpp \
			   Source/Tests/FullyConnected.cpp \
			   Source/Tests/NNSupport.cpp \
			   Source/Tests/Pooling.cpp \
			   Source/Tests/Softmax.cpp

INCDIRS += FrameworkInclude GeneratedInclude Include/Tests

LDLIBS = -lm

NMSIS_LIB := nmsis_dsp

COMMON_FLAGS += -DEMBEDDED -DNOTIMING

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base

