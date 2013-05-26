include ../../../ThirdParty/PSCommon/BuildSystem/CommonDefs.mak

BIN_DIR = ../../../Bin

INC_DIRS = \
    .   \
    $(OPEN_NI_INCLUDE) \
    ../../NiteControls.c/API \
    ../../../Include

SRC_FILES = *.cpp

ifeq ("$(OSTYPE)","Darwin")
	CFLAGS += -DMACOS
else
	CFLAGS += -DUNIX
endif

LIB_DIRS += $(BIN_DIR)

USED_LIBS += OpenNI NiteControls NiteControls.c

LIB_NAME = NiteControls.jni

DEFINES += XNV_NITE_CONTROLS_C_EXPORTS

#CFLAGS += -Wall

include ../../../ThirdParty/PSCommon/BuildSystem/CommonCppMakefile
