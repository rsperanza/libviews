APP_NAME = Work

GLAPI = gles2
include(../views/views.pri)

CONFIG += qt warn_on cascades10 views
LIBS += -lbbsystem -lscreen -lbb
QMAKE_CXXFLAGS += -DGLES2

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
