#############################################################################
# OpenNI makefile.
# 
# default configuration is Release. for a debug version use:
# 	make CFG=Debug
#
# default compiler is g++. for another one use:
#   make CXX=<comp>
#
# By default, CLR projects will only be build if mono is installed.
# To force CLR projects use:
#   make FORCE_BUILD_CLR=1
#
#############################################################################

include ThirdParty/PSCommon/BuildSystem/CommonDefs.mak

CONTROLS = Source

# list all wrappers
ALL_WRAPPERS = \
    Wrappers/NiteControls.c \
	Wrappers/java/NiteControls.jni \
	Wrappers/java/NiteControls.java 

# list all core projects
ALL_CORE_PROJS = \
	$(CONTROLS) \
	$(ALL_WRAPPERS) \

# list all samples
CORE_SAMPLES = \
    Samples/SingleControl

# list all java samples
JAVA_SAMPLES = \
	Samples/Boxes.java	

ifeq "$(GLUT_SUPPORTED)" "1"
	CORE_SAMPLES += \
		Samples/Boxes
else
	ifeq "$(GLES_SUPPORTED)" "1"
		CORE_SAMPLES += 
	endif
endif

ALL_SAMPLES = \
	$(CORE_SAMPLES) \
	$(JAVA_SAMPLES)

# list all projects that are build
ALL_BUILD_PROJS = \
	$(ALL_CORE_PROJS) \
	$(ALL_SAMPLES)

ALL_PROJS = \
	$(ALL_BUILD_PROJS)

ALL_PROJS_CLEAN = $(foreach proj,$(ALL_PROJS),$(proj)-clean)

# define a function which creates a target for each proj
define CREATE_PROJ_TARGET
$1: 
	$$(MAKE) -C $1

$1-clean: 
	$$(MAKE) -C $1 clean
endef

################ TARGETS ##################

.PHONY: all $(ALL_PROJS) $(ALL_PROJS_CLEAN) install uninstall clean release

# make all makefiles
all: $(ALL_PROJS)

core: $(ALL_CORE_PROJS)

samples: $(ALL_SAMPLES)

# create projects targets
$(foreach proj,$(ALL_PROJS),$(eval $(call CREATE_PROJ_TARGET,$(proj))))

# additional dependencies
$(CONTROLS):
Wrappers/java/Controls.jni:   $(CONTROLS) $(XNLIB)

Samples/Boxes:              $(CONTROLS)
Samples/SingleControl:      $(CONTROLS)
Samples/SimpleViewer.java:            Wrappers/java/NiteControls.java

$(FINAL_DIR):
	mkdir -p $(FINAL_DIR)

# clean is cleaning all projects
clean: $(ALL_PROJS_CLEAN)
