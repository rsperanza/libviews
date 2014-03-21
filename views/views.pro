TEMPLATE = lib

CONFIG += qt warn_on cascades_library
CONFIG += hardening

LIBS   += -lmmrndclient -lbbutility -lbb -lpps -lscreen -lEGL -lfreetype -lpng -lstrm

include(config.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
		DESTDIR = $$QNX_VARIANT/$${GLAPI}
		OBJECTS_DIR = $$DESTDIR/.obj
		MOC_DIR = $$DESTDIR/.moc
		RCC_DIR = $$DESTDIR/.qrc
		UI_DIR = $$DESTDIR/.ui
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
		DESTDIR = $$QNX_VARIANT-g/$${GLAPI}
		OBJECTS_DIR = $$DESTDIR/.obj
		MOC_DIR = $$DESTDIR/.moc
		RCC_DIR = $$DESTDIR/.qrc
		UI_DIR = $$DESTDIR/.ui
    }    
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
		DESTDIR = $$QNX_VARIANT-g/$${GLAPI}
		OBJECTS_DIR = $$DESTDIR/.obj
		MOC_DIR = $$DESTDIR/.moc
		RCC_DIR = $$DESTDIR/.qrc
		UI_DIR = $$DESTDIR/.ui
    }
}

defineTest(setGLApi) {
	# override target
	oldTarget = $${TARGET}
	unset(TARGET)
	TARGET = $${oldTarget}-$$1
	export(TARGET)
		
	message("defineTest Checking target "$${TARGET})
	message("defineTest Checking GLAPI "$${GLAPI})		
		
	# clean GLAPI
	unset(GLAPI)
	GLAPI = $$1
	export(GLAPI)
	
	message("defineTest Building with OpenGL api "$${GLAPI})	
	
	#define moc and obj dirs
	#MOC_DIR = ./.moc_$${GLAPI}
	#export(MOC_DIR)
	
	#OBJECTS_DIR = ./.obj_$${GLAPI}
	#export(OBJECTS_DIR)
	
	#message("defineTest Building with MOC_DIR "$${MOC_DIR}" and OBJECTS_DIR "$${OBJECTS_DIR})	
	
	# define GLAPI for C/C++
	DEFINES += $$upper($$1)
	contains(GLAPI, gles3) {
		DEFINES += GLES2
	}
	export(DEFINES)
	
	message("defineTest Building with DEFINES "$${DEFINES})
		
	# adjust LIBS for GLAPI
	oldLIBS = $${LIBS}
	unset(LIBS)
	contains(GLAPI, gles1) {
		LIBS = $${oldLIBS} -lGLESv1_CM
	} else:contains(GLAPI, gles2) {
		LIBS = $${oldLIBS} -lGLESv2
	} else:contains(GLAPI, gles3) {
		LIBS = $${oldLIBS} -lGLESv2
	}
	export(LIBS)
	
	message("defineTest Building with LIBS "$${LIBS})
}

isEmpty(GLAPI) {
	GLAPI = gles1
	message("isEmpty Checking GLAPI "$${GLAPI})		
	message("isEmpty Checking CONFIG "$${CONFIG})		
	contains(CONFIG, gles2) {
		GLAPI=gles2
	} else:contains(CONFIG, Releasegles2) {
		GLAPI=gles2
	} else:contains(CONFIG, Debuggles2) {
		GLAPI=gles2
	}
	
	contains(CONFIG, gles3) {
		GLAPI=gles3
	} else:contains(CONFIG, Releasegles3) {
		GLAPI=gles3
	} else:contains(CONFIG, Debuggles3) {
		GLAPI=gles3
	}
	message("isEmpty Chose GLAPI "$${GLAPI})		
}

contains(GLAPI, gles1) {
	setGLApi(gles1)
} else:contains(GLAPI, gles2) {
	setGLApi(gles2)
} else:contains(GLAPI, gles3) {
	setGLApi(gles3)
}

!buildpass {
	BUILDS = gles1 gles2 gles3
	
#	message("buildpass isEmpty Checking GLAPI "$${GLAPI})		

#	contains(GLAPI, gles1) {
#		BUILDS = gles1
#	} else:contains(GLAPI, gles2) {
#		BUILDS = gles2
#	} else:contains(GLAPI, gles3) {
#		BUILDS = gles3
#	}

	message("buildpass Chose BUILDS "$${BUILDS})		

	export(BUILDS)
}

