TEMPLATE = lib

CONFIG += qt warn_on cascades_library
CONFIG += hardening

LIBS   += -lmmrndclient -lbbutility -lbb -lpps -lscreen -lEGL -lfreetype -lpng -lstrm

include(config.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
    }    
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}

defineTest(setGLApi) {
	# override target
	oldTarget = $${TARGET}
	unset(TARGET)
	TARGET = $${oldTarget}-$$1
	export(TARGET)
		
	message("Checking target "$${TARGET})
	message("Checking GLAPI "$${GLAPI})		
		
	# clean GLAPI
	unset(GLAPI)
	GLAPI = $$1
	export(GLAPI)
	
	message("Building with OpenGL api "$${GLAPI})	
	
	# define GLAPI for C/C++
	DEFINES += $$upper($$1)
	contains(GLAPI, gles3) {
		DEFINES += GLES2
	}
	export(DEFINES)
	
	message("Building with DEFINES "$${DEFINES})
		
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
}

isEmpty(GLAPI) {
	GLAPI = gles1
	message("Checking GLAPI "$${GLAPI})		
	message("Checking CONFIG "$${CONFIG})		
	contains(CONFIG, Releasegles2) {
		GLAPI=gles2
	} else:contains(CONFIG, Debuggles2) {
		GLAPI=gles2
	}
	contains(CONFIG, Releasegles3) {
		GLAPI=gles3
	} else:contains(CONFIG, Debuggles3) {
		GLAPI=gles3
	}
	message("Checking GLAPI "$${GLAPI})		
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
	export(BUILDS)
}

