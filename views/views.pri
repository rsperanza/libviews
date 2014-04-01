
views {
	BASEDIR = $${PWD}$$dirname($${_FILE_})
	
	LIBTARGET = views-$${GLAPI}

	INCLUDEPATH *=  $$quote($${BASEDIR}/include)
	DEPENDPATH *=  $$quote($${BASEDIR}/include)
	
	device {
		profile {
		    LIBS += -L$${BASEDIR}/arm/so.le-v7-g/$${GLAPI} -l$${LIBTARGET}
		    PRE_TARGETDEPS +=  $$quote($${BASEDIR}/arm-p/so.le-v7-g/$${GLAPI}/lib$${LIBTARGET}.so)		
		}
		else {
		    CONFIG(debug, debug|release) {
		        # Device-Debug custom configuration
		        LIBS += -L$${BASEDIR}/arm/so.le-v7-g/$${GLAPI} -l$${LIBTARGET}
		        PRE_TARGETDEPS +=  $$quote($${BASEDIR}/arm/so.le-v7-g/$${GLAPI}/lib$${LIBTARGET}.so)
		    }
		
		    CONFIG(release, debug|release) {
		        # Device-Release custom configuration
		        LIBS += -L$${BASEDIR}/arm/so.le-v7/$${GLAPI} -l$${LIBTARGET}
		        PRE_TARGETDEPS +=  $$quote($${BASEDIR}/arm/so.le-v7/$${GLAPI}/lib$${LIBTARGET}.so)		        
		    }
	    }
	}
	
	simulator {
	    CONFIG(debug, debug|release) {
	        # Simulator-Debug custom configuration
	        LIBS += -L$${BASEDIR}/x86/so-g/$${GLAPI} -l$${LIBTARGET}
	        PRE_TARGETDEPS +=  $$quote($${BASEDIR}/x86/so-g/$${GLAPI}/lib$${LIBTARGET}.so)
	    }
	}
}