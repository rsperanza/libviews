QMAKE_TARGET  = views
PROJECT_DIR	  := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
QMAKE = $(QNX_HOST)/usr/bin/qmake
 
ifdef NUMBER_OF_PROCESSORS
  MAKEFLAGS += -j${NUMBER_OF_PROCESSORS}
endif

QMAKEFEATURES = $(dir $(PWD)/$(lastword $(MAKEFILE_LIST)))
export QMAKEFEATURES
 
default: Device-Release	
 
.PHONY: FORCE
 
Makefile-all: arm/Makefile arm-p/Makefile x86/Makefile translations/Makefile	
 
arm/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p arm
	cd arm && $(QMAKE) -spec blackberry-armv7le-qcc ../$(QMAKE_TARGET).pro CONFIG+=debug_and_release CONFIG+=device
 
arm-p/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p arm-p
	cd arm-p && $(QMAKE) -spec blackberry-armv7le-qcc ../$(QMAKE_TARGET).pro CONFIG+=debug_and_release CONFIG+=device CONFIG+=profile
 
x86/Makefile: $(QMAKE_TARGET).pro
	@mkdir -p x86
	cd x86 && $(QMAKE) -spec blackberry-x86-qcc ../$(QMAKE_TARGET).pro  CONFIG+=debug_and_release CONFIG+=simulator
   
release-gles1: arm/Makefile FORCE 	
	$(MAKE) -C ./arm -f Makefile release-gles1
  
release-gles1-mocclean: arm/Makefile FORCE release-gles1 	
	$(MAKE) -C ./arm -f Makefile release-gles1-mocclean
  
release-gles1-clean: release-gles1 release-gles1-mocclean
	$(MAKE) -C ./arm -f Makefile release-gles1-clean

release-gles2: arm/Makefile FORCE 	
	$(MAKE) -C ./arm -f Makefile release-gles2
  
release-gles2-mocclean: arm/Makefile FORCE release-gles2	
	$(MAKE) -C ./arm -f Makefile release-gles2-mocclean
  
release-gles2-clean: release-gles2 release-gles2-mocclean
	$(MAKE) -C ./arm -f Makefile release-gles2-clean

Device-Release: arm/Makefile FORCE release-gles1 release-gles1-mocclean release-gles1-clean release-gles2 release-gles2-mocclean release-gles2-clean
	$(MAKE) -C ./arm -f Makefile release-gles3
  
debug-gles1: arm/Makefile FORCE 	
	$(MAKE) -C ./arm -f Makefile debug-gles1
  
debug-gles1-mocclean: arm/Makefile FORCE debug-gles1 	
	$(MAKE) -C ./arm -f Makefile debug-gles1-mocclean
  
debug-gles1-clean:debug-gles1 debug-gles1-mocclean
	$(MAKE) -C ./arm -f Makefile debug-gles1-clean
  
debug-gles2: arm/Makefile FORCE debug-gles1-clean
	$(MAKE) -C ./arm -f Makefile debug-gles2
  
debug-gles2-mocclean: arm/Makefile FORCE debug-gles2 	
	$(MAKE) -C ./arm -f Makefile debug-gles2-mocclean
  
debug-gles2-clean:debug-gles2 debug-gles2-mocclean
	$(MAKE) -C ./arm -f Makefile debug-gles2-clean
  
Device-Debug: arm/Makefile FORCE debug-gles1 debug-gles1-mocclean debug-gles1-clean	debug-gles2 debug-gles2-mocclean debug-gles2-clean	
	$(MAKE) -C ./arm -f Makefile debug-gles3
  
debugp-gles1: arm-p/Makefile FORCE 	
	$(MAKE) -C ./arm-p -f Makefile debug-gles1
  
debugp-gles1-mocclean: arm-p/Makefile FORCE debugp-gles1 	
	$(MAKE) -C ./arm-p -f Makefile debug-gles1-mocclean
  
debugp-gles1-clean: debugp-gles1 debugp-gles1-mocclean
	$(MAKE) -C ./arm-p -f Makefile debug-gles1-clean
  
Device-Profile: arm-p/Makefile FORCE debugp-gles1 debugp-gles1-mocclean debugp-gles1-clean
	$(MAKE) -C ./arm-p -f Makefile debug-gles2
  
debugx-gles1: x86/Makefile FORCE 	
	$(MAKE) -C ./x86 -f Makefile debug-gles1
  
debugx-gles1-mocclean: x86/Makefile FORCE debugx-gles1 	
	$(MAKE) -C ./x86 -f Makefile debug-gles1-mocclean
  
debugx-gles1-clean: debugx-gles1 debugx-gles1-mocclean
	$(MAKE) -C ./x86 -f Makefile debug-gles1-clean
  
Simulator-Debug: x86/Makefile FORCE debugx-gles1 debugx-gles1-mocclean debugx-gles1-clean
	$(MAKE) -C ./x86 -f Makefile debug-gles2

clean: FORCE
	rm -rf arm arm-p x86
	rm -f $(I18N_DIR)/*.qm

