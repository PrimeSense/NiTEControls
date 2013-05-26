include ../../ThirdParty/PSCommon/BuildSystem/CommonDefs.mak

BIN_DIR = ../../Bin

INC_DIRS = \
	../../Include \
    $(OPEN_NI_INCLUDE) \
    GL \

SRC_FILES = *.cpp

ifeq ("$(OSTYPE)","Darwin")
	CFLAGS += -DMACOS
	LDFLAGS += -framework OpenGL -framework GLUT
else
	CFLAGS += -DUNIX -DGLX_GLXEXT_LEGACY
	USED_LIBS += glut GL
endif

LIB_DIRS += $(BIN_DIR)

DEFINES += USE_GLUT

USED_LIBS += OpenNI NiteControls

EXE_NAME = Boxes

include ../../ThirdParty/PSCommon/BuildSystem/CommonCppMakefile
