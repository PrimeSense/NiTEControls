include ../../ThirdParty/PSCommon/BuildSystem/CommonDefs.mak

BIN_DIR = ../../Bin

INC_DIRS = \
	../../Include \
    $(OPEN_NI_INCLUDE)

SRC_FILES = *.cpp

ifeq ("$(OSTYPE)","Darwin")
	CFLAGS += -DMACOS
else
	CFLAGS += -DUNIX -DGLX_GLXEXT_LEGACY
endif

LIB_DIRS += $(BIN_DIR)

USED_LIBS += OpenNI NiteControls

EXE_NAME = SingleControl

include ../../ThirdParty/PSCommon/BuildSystem/CommonCppMakefile
