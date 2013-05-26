include ../ThirdParty/PSCommon/BuildSystem/CommonDefs.mak

BIN_DIR = ../Bin

INC_DIRS = \
	../Include \
    $(OPEN_NI_INCLUDE)

SRC_FILES = \
	*.cpp

ifeq ("$(OSTYPE)","Darwin")
	INC_DIRS += /opt/local/include
	LIB_DIRS += /opt/local/lib
	LDFLAGS += -framework CoreFoundation -framework IOKit
endif

LIB_NAME = NiteControls

USED_LIBS = dl pthread OpenNI
ifneq ("$(OSTYPE)","Darwin")
        USED_LIBS += rt  
endif

DEFINES += XNV_NITE_CONTROLS_EXPORTS

#CFLAGS += -Wall

include ../ThirdParty/PSCommon/BuildSystem/CommonCppMakefile
