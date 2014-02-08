
	BASEDIR = $${PWD}$$dirname($${_FILE_})
	
	LIBTARGET = views
	contains(GLAPI, gles1) {
		LIBTARGET = $${LIBTARGET}-gles1
	} else:contains(GLAPI, gles2) {
		LIBTARGET = $${LIBTARGET}-gles2
	} else:contains(GLAPI, gles3) {
		LIBTARGET = $${LIBTARGET}-gles3
	}	

	INCLUDEPATH *=  $$quote($${BASEDIR}/include)
	DEPENDPATH *=  $$quote($${BASEDIR}/include)
	
	LIBS += 
	device {
		profile {
		    LIBS += -L$${BASEDIR}/arm/so.le-v7-g -l$${LIBTARGET}
		    PRE_TARGETDEPS +=  $$quote($${BASEDIR}/arm/so.le-v7-g/lib$${LIBTARGET}.so)		
		}
		else {
		    CONFIG(debug, debug|release) {
		        # Device-Debug custom configuration
		        LIBS += -L$${BASEDIR}/arm/so.le-v7-g -l$${LIBTARGET}
		        PRE_TARGETDEPS +=  $$quote($${BASEDIR}/arm/so.le-v7-g/lib$${LIBTARGET}.so)
		    }
		
		    CONFIG(release, debug|release) {
		        # Device-Release custom configuration
		        LIBS += -L$${BASEDIR}/arm/so.le-v7 -l$${LIBTARGET}
		        PRE_TARGETDEPS +=  $$quote($${BASEDIR}/arm/so.le-v7/lib$${LIBTARGET}.so)
		    }
	    }
	}
	
	simulator {
	    CONFIG(debug, debug|release) {
	        # Simulator-Debug custom configuration
	        LIBS += -L$${BASEDIR}/x86/so-g -l$${LIBTARGET}
	        PRE_TARGETDEPS +=  $$quote($${BASEDIR}/x86/so-g/lib$${LIBTARGET}.so)
	    }
	}