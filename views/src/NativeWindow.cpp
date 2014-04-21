/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QDebug>

#include "NativeWindow.hpp"

#include "ViewsThread.hpp"

namespace views {

	namespace native {


QMutex             NativeWindow::_displaysMutex;
int                NativeWindow::_numberDisplays = 0;
screen_display_t*  NativeWindow::_screenDisplays = NULL;
screen_context_t   NativeWindow::_screenContext  = NULL;

NativeWindow::NativeWindow(ViewDisplay display) : _display(display)
{
	_screenWindow = NULL;

	_angle = 0;
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;
    _bufferWidth = 0;
    _sourceWidth = 0;
	_interval = 0;
	_transparency = SCREEN_TRANSPARENCY_NONE;
    _format = SCREEN_FORMAT_RGBA8888;
	_nbuffers = 2;
    _screenBuffers[0] = NULL;
    _screenBuffers[1] = NULL;

	_createGroup = false;
}

NativeWindow::~NativeWindow() {
	if (_screenWindow) {
    	qDebug()  << "NativeWindow::~NativeWindow: _screenWindow: " << ":" << _screenWindow;

		screen_destroy_window(_screenWindow);
		_screenWindow = NULL;
	}
}

int NativeWindow::initialize(bool _createFullWindow)
{
    int returnCode;
    int numberModes;
	screen_display_mode_t* screenModes;
    int type;

    if (_createFullWindow) {
    	returnCode = screen_create_window(&_screenWindow, _screenContext);
    	qDebug()  << "NativeWindow::initialize: _screenWindow (full window): " << ":" << _screenWindow << ":" << _screenContext << ":" << returnCode;

    	if (returnCode) {
            perror("screen_create_window");
            return EXIT_FAILURE;
        }

    } else {
    	returnCode = screen_create_window_type(&_screenWindow, _screenContext, SCREEN_CHILD_WINDOW);
    	qDebug()  << "NativeWindow::initialize: _screenWindow (child window): " << _screenWindow << ":" << returnCode;

    	if (returnCode) {
            perror("screen_create_window (child window)");
            return EXIT_FAILURE;
        }

    }

    _displaysMutex.lock();

    if (_screenDisplays) {
    	free (_screenDisplays);
    }

	// try this first as it will fail if an HDMI display is not attached
	screen_get_context_property_iv(_screenContext, SCREEN_PROPERTY_DISPLAY_COUNT, &_numberDisplays);

	_screenDisplays = (screen_display_t *)calloc(_numberDisplays, sizeof(screen_display_t));
	screen_get_context_property_pv(_screenContext, SCREEN_PROPERTY_DISPLAYS, (void **)_screenDisplays);

	for (int index = 0; index < _numberDisplays; index++) {
		int displayID;

        returnCode = screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_ID,  (int *)&displayID);
    	qDebug()  << "NativeWindow::initialize: display: " << ":" << index << ":" << displayID << ":" << _display << ":" << _screenDisplays[index];

    	if (returnCode) {
    		perror("display ID");
    		return EXIT_FAILURE;
    	} else {
			if (displayID == _display) {
			    screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_TYPE,  &type);
			    if (type == SCREEN_DISPLAY_TYPE_HDMI) {
					returnCode = screen_set_window_property_pv(_screenWindow, SCREEN_PROPERTY_DISPLAY, (void **)&(_screenDisplays[index]));

					if (returnCode) {
						perror("window display");
						return EXIT_FAILURE;
					}
		        }
			}
        }
	}

    _displaysMutex.unlock();


	qDebug()  << "NativeWindow::initialize: "<< _screenContext << ":" << _screenWindow;


    returnCode = setWindowFormat(_format);
	if (returnCode) {
		perror("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)");
		return EXIT_FAILURE;
	}

	returnCode = setWindowTransparency(_transparency);
	if (returnCode) {
		perror("transparency");
		return EXIT_FAILURE;
	}

	returnCode = screen_get_window_property_pv(_screenWindow, SCREEN_PROPERTY_DISPLAY, (void **)&_screenDisplay);
	if (returnCode) {
		perror("screenDisplay");
		return EXIT_FAILURE;
	}

	int angle = atoi(getenv("ORIENTATION"));

	screen_get_display_property_iv(_screenDisplay, SCREEN_PROPERTY_MODE_COUNT, &numberModes);

	screenModes = (screen_display_mode_t *)calloc(numberModes, sizeof(screen_display_mode_t));
	returnCode = screen_get_display_property_pv(_screenDisplay, SCREEN_PROPERTY_MODE, (void**)screenModes);
	if (returnCode) {
		perror("screen modes");
		return EXIT_FAILURE;
	}

    int dpi = calculateDPI();
    if (dpi == EXIT_FAILURE) {
        fprintf(stderr, "Unable to calculate dpi\n");
        return EXIT_FAILURE;
    }

	returnCode = setWindowPosition(_x, _y);
	if (returnCode) {
		perror("window position");
		return EXIT_FAILURE;
	}

	returnCode = setWindowSize(_width, _height);
	if (returnCode) {
		perror("window size");
		return EXIT_FAILURE;
	}

	returnCode = setWindowZ(_z);
	if (returnCode) {
		perror("z order");
		return EXIT_FAILURE;
	}

	if (_sourceWidth > 0) {
        returnCode = setWindowSourcePosition(_sourceX, _sourceY);
        if (returnCode) {
            perror("source size");
            return EXIT_FAILURE;
        }

        returnCode = setWindowSourceSize(_sourceWidth, _sourceHeight);
        if (returnCode) {
            perror("source size");
            return EXIT_FAILURE;
        }
	}

    if (_bufferWidth > 0) {
        returnCode = setWindowBufferSize(_bufferWidth, _bufferHeight);
        if (returnCode) {
            perror("buffer size");
            return EXIT_FAILURE;
        }
    }

	returnCode = setWindowAngle(_angle);
	if (returnCode) {
		perror("angle");
		return EXIT_FAILURE;
	}

	returnCode = screen_create_window_buffers(_screenWindow, _nbuffers);
	if (returnCode) {
		perror("screen_create_window_buffers");
		return EXIT_FAILURE;
	}

	_usage = Graphics::getNativeWindowUsage();
    // enable this in case we need any native API access to this window
    _usage |= SCREEN_USAGE_NATIVE;

	returnCode = setWindowUsage(_usage);
	if (returnCode) {
		perror("screen_set_window_property_iv(window usage)");
		return EXIT_FAILURE;
	}

	returnCode = joinWindowGroup(_group);
	if (returnCode) {
		perror("window group");
		return EXIT_FAILURE;
	}

	returnCode = setScreenWindowID(_id);
	if (returnCode) {
		perror("window ID");
		return EXIT_FAILURE;
	}

	qDebug()  << "NativeWindow::initialize: "<< angle << ":" << numberModes << ":" << screenModes[0].width << ":" << screenModes[0].height << ":" << dpi;

	if (screenModes != NULL) {
		free(screenModes);
	}

	return EXIT_SUCCESS;
}

void NativeWindow::setAngle(int angle)
{
	if ((angle != 0) && (angle != 90) && (angle != 180) && (angle != 270)) {
		fprintf(stderr, "Invalid angle\n");
	} else {
		_angle = angle;
	}

	int returnCode = setWindowAngle(_angle);
	if (returnCode) {
		perror("window angle");
	}
}

void NativeWindow::setPosition(int x, int y)
{
	_x = x;
	_y = y;

	int returnCode = setWindowPosition(_x, _y);
	if (returnCode) {
		perror("window position");
	}
}

void NativeWindow::setSize(int width, int height)
{
    _width = width;
    _height = height;

    int returnCode = setWindowSize(_width, _height);
    if (returnCode) {
        perror("window size");
    }
}

void NativeWindow::setBufferSize(int bufferWidth, int bufferHeight)
{
    _bufferWidth = bufferWidth;
    _bufferHeight = bufferHeight;

    int returnCode = setWindowBufferSize(_bufferWidth, _bufferHeight);
    if (returnCode) {
        perror("buffer size");
    }
}

void NativeWindow::setSourcePosition(int x, int y)
{
    _x = x;
    _y = y;

    int returnCode = setWindowSourcePosition(_x, _y);
    if (returnCode) {
        perror("source position");
    }
}

void NativeWindow::setSourceSize(int sourceWidth, int sourceHeight)
{
    _sourceWidth = sourceWidth;
    _sourceHeight = sourceHeight;

    int returnCode = setWindowSourceSize(_sourceWidth, sourceHeight);
    if (returnCode) {
        perror("source size");
    }
}

void NativeWindow::setZ(int z)
{
	_z = z;

	int returnCode = setWindowZ(_z);
	if (returnCode) {
		perror("window Z");
	}

}

void NativeWindow::setTransparency(int transparency)
{
    _transparency = transparency;

    int returnCode = setWindowTransparency(_transparency);
    if (returnCode) {
        perror("window transparency");
    }
}

void NativeWindow::setFormat(int format)
{
    _format = format;

    int returnCode = setWindowFormat(_format);
    if (returnCode) {
        perror("window format");
    }
}

void NativeWindow::setWindowGroup(const QString &group)
{
	_group = group;
}

void NativeWindow::setCreateWindowGroup(bool create)
{
	_createGroup = create;
}

void NativeWindow::setWindowID(const QString id)
{
	_id = id;
}

screen_window_t NativeWindow::getScreenWindow()
{
	return _screenWindow;
}

int NativeWindow::setWindowAngle(int angle)
{
	int returnCode = EXIT_SUCCESS;

	if (_screenWindow != NULL) {
		returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_ROTATION, &angle);
	} else {
		returnCode = EXIT_SUCCESS;
	}

	return returnCode;
}

int NativeWindow::setWindowPosition(int x, int y)
{
	int returnCode = EXIT_SUCCESS;
	int position[2];

	if (_screenWindow != NULL) {
		position[0] = x;
		position[1] = y;

		returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_POSITION, position);
	} else {
		returnCode = EXIT_SUCCESS;
	}

	return returnCode;
}

int NativeWindow::setWindowSize(int width, int height)
{
	int returnCode = EXIT_SUCCESS;
	int size[2];

	if (_screenWindow != NULL) {
		size[0] = width;
		size[1] = height;

		returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_SIZE, size);
	} else {
		returnCode = EXIT_SUCCESS;
	}

	return returnCode;
}

int NativeWindow::setWindowZ(int z)
{
	int returnCode = EXIT_SUCCESS;

	if (_screenWindow != NULL) {
		returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_ZORDER, &z);
	} else {
		returnCode = EXIT_SUCCESS;
	}

	return returnCode;
}

int NativeWindow::setWindowTransparency(int transparency)
{
    int returnCode = EXIT_SUCCESS;

    if (_screenWindow != NULL) {
        returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_TRANSPARENCY, &transparency);
    } else {
        returnCode = EXIT_SUCCESS;
    }

    return returnCode;
}

int NativeWindow::setWindowFormat(int format)
{
    int returnCode = EXIT_SUCCESS;

    if (_screenWindow != NULL) {
        returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_FORMAT, &format);
        qDebug()  << "NativeWindow::setWindowFormat: " << format << ":" << returnCode;
    } else {
        returnCode = EXIT_SUCCESS;
    }

    return returnCode;
}

int NativeWindow::setWindowUsage(int usage)
{
	int returnCode = EXIT_SUCCESS;

	if (_screenWindow != NULL) {
		returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_USAGE, &usage);
	} else {
		returnCode = EXIT_SUCCESS;
	}

	return returnCode;
}

int NativeWindow::setWindowBufferSize(int width, int height)
{
    int returnCode = EXIT_SUCCESS;
    int size[2];

    if (_screenWindow != NULL) {
        size[0] = width;
        size[1] = height;

        returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, size);
    } else {
        returnCode = EXIT_SUCCESS;
    }

    return returnCode;
}

int NativeWindow::setWindowSourcePosition(int x, int y)
{
    int returnCode = EXIT_SUCCESS;
    int position[2];

    if (_screenWindow != NULL) {
        position[0] = x;
        position[1] = y;

        returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_SOURCE_POSITION, position);
    } else {
        returnCode = EXIT_SUCCESS;
    }

    return returnCode;
}

int NativeWindow::setWindowSourceSize(int width, int height)
{
	int returnCode = EXIT_SUCCESS;
	int size[2];

	if (_screenWindow != NULL) {
		size[0] = width;
		size[1] = height;

		returnCode = screen_set_window_property_iv(_screenWindow, SCREEN_PROPERTY_SOURCE_SIZE, size);
	} else {
		returnCode = EXIT_SUCCESS;
	}

	return returnCode;
}

int NativeWindow::joinWindowGroup(const QString &group)
{
	int returnCode = EXIT_SUCCESS;

	if (_screenWindow != NULL) {
		if (_createGroup) {
			returnCode = screen_create_window_group(_screenWindow, group.toAscii().data());
			qDebug()  << "NativeWindow::joinWindowGroup: create group: " << group << ":" << returnCode;
		}

		if (!returnCode) {
			returnCode = screen_join_window_group(_screenWindow, group.toAscii().data());
			qDebug()  << "NativeWindow::joinWindowGroup: join group: " << group << ":" << returnCode;
		}
	}

	return returnCode;
}

int NativeWindow::setScreenWindowID(const QString id)
{
	int returnCode = EXIT_SUCCESS;

	if (_screenWindow != NULL) {
		returnCode = screen_set_window_property_cv(_screenWindow, SCREEN_PROPERTY_ID_STRING, id.toAscii().length(), id.toAscii());
	}

	return returnCode;
}

int NativeWindow::calculateDPI() {
    int returnCode;
    int screenPhysicalSize[2];
    int screenResolution[2];
    int diagonalPixels;
    int diagonalInches;

    returnCode = screen_get_display_property_iv(_screenDisplay, SCREEN_PROPERTY_PHYSICAL_SIZE, screenPhysicalSize);
    if (returnCode) {
        perror("screen_get_display_property_iv");
        return EXIT_FAILURE;
    }

	qDebug()  << "NativeWindow::calculateDPI: physical: "<< screenPhysicalSize[0] << " x " << screenPhysicalSize[1];

    //Simulator will return 0,0 for physical size of the screen, so use 170 as default dpi
    if ((screenPhysicalSize[0] == 0) && (screenPhysicalSize[1] == 0)) {
        qDebug()  << "NativeWindow::calculateDPI: DPI: 170 (default)";
        return 170;
    } else {
        returnCode = screen_get_display_property_iv(_screenDisplay, SCREEN_PROPERTY_SIZE, screenResolution);
        if (returnCode) {
            perror("screen_get_display_property_iv");
            return EXIT_FAILURE;
        }

        qDebug()  << "NativeWindow::calculateDPI: screen: "<< screenResolution[0] << " x " << screenResolution[1];

        diagonalPixels = sqrt(screenResolution[0] * screenResolution[0] + screenResolution[1] * screenResolution[1]);
        diagonalInches = 0.0393700787 * sqrt(screenPhysicalSize[0] * screenPhysicalSize[0] + screenPhysicalSize[1] * screenPhysicalSize[1]);
    }

    qDebug()  << "NativeWindow::calculateDPI: DPI: "<< (int)(diagonalPixels / diagonalInches);

    return (int)(diagonalPixels / diagonalInches);
}

screen_context_t NativeWindow::getScreenContext()
{
	if (!_screenContext) {
		//Create a screen context that will be used to create an EGL surface to to receive libscreen events
		if (EXIT_SUCCESS != screen_create_context(&_screenContext, 0)) {
			fprintf(stderr, "screen_create_context failed\n");
		}
	}

	return _screenContext;
}

int NativeWindow::screenDisplayAttached(int *size)
{
    //qDebug()  << "NativeWindow::screenDisplayAttached: ";
    int type;
    int attached = 0;

    _displaysMutex.lock();

    if (_screenDisplays) {
    	free (_screenDisplays);
    }

	// try this first as it will fail if an HDMI display is not attached
	screen_get_context_property_iv(_screenContext, SCREEN_PROPERTY_DISPLAY_COUNT, &_numberDisplays);

	_screenDisplays = (screen_display_t *)calloc(_numberDisplays, sizeof(screen_display_t));
	screen_get_context_property_pv(_screenContext, SCREEN_PROPERTY_DISPLAYS, (void **)_screenDisplays);

	for (int index = 0; index < _numberDisplays; index++) {
        screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_TYPE,  &type);
		attached = 0;

        if (type == SCREEN_DISPLAY_TYPE_HDMI) {
		    //qDebug()  << "NativeWindow::screenDisplayAttached: HDMI: " << index;

			screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_ATTACHED, &attached);

			if (attached) {
				screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_SIZE, size);

			    //qDebug()  << "NativeWindow::screenDisplayAttached: size: " << size[0] << "x" << size[1];

				if (size[0] == 0 || size[1] == 0) {
					attached = 0;
				}
			}
		}
	}

    _displaysMutex.unlock();

    //qDebug()  << "NativeWindow::screenDisplayAttached:  "<< attached;

	return attached;
}

int NativeWindow::screenDisplayAttached(bps_event_t *event, int *size)
{
    screen_display_t eventDisplay;
    int type;
    int attached = -1;

    screen_event_t screen_event = screen_event_get_event(event);

    //Query type of screen event and its location on the screen
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
            &type);

    _displaysMutex.lock();

    if (_screenDisplays) {
    	free (_screenDisplays);
    }

	// try this first as it will fail if an HDMI display is not attached
	screen_get_context_property_iv(_screenContext, SCREEN_PROPERTY_DISPLAY_COUNT, &_numberDisplays);

	_screenDisplays = (screen_display_t *)calloc(_numberDisplays, sizeof(screen_display_t));
	screen_get_context_property_pv(_screenContext, SCREEN_PROPERTY_DISPLAYS, (void **)_screenDisplays);

	switch (type) {
		case SCREEN_EVENT_DISPLAY:
			screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DISPLAY, (void **)&eventDisplay);
			for (int index = 0; index < _numberDisplays; index++) {
				if (eventDisplay == _screenDisplays[index]) {
					screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ATTACHED, &attached);

					if (attached) {
						screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_SIZE, size);
						if (size[0] == 0 || size[1] == 0) {
							attached = 0;
						}
					}
				}
			}
			break;
	}

    _displaysMutex.unlock();

	return attached;
}

screen_display_t* NativeWindow::getAttachedDisplay()
{
    int type;
    int attached = 0;
    screen_display_t* attachedDisplay = NULL;

    _displaysMutex.lock();

    if (_screenDisplays) {
    	free (_screenDisplays);
    }

	// try this first as it will fail if an HDMI display is not attached
	screen_get_context_property_iv(_screenContext, SCREEN_PROPERTY_DISPLAY_COUNT, &_numberDisplays);

	_screenDisplays = (screen_display_t *)calloc(_numberDisplays, sizeof(screen_display_t));
	screen_get_context_property_pv(_screenContext, SCREEN_PROPERTY_DISPLAYS, (void **)_screenDisplays);

	for (int index = 0; index < _numberDisplays; index++) {
        screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_TYPE,  &type);
		attached = 0;

        if (type == SCREEN_DISPLAY_TYPE_HDMI) {
			screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_ATTACHED, &attached);

			if (attached) {
				int size[2];
				screen_get_display_property_iv(_screenDisplays[index], SCREEN_PROPERTY_SIZE, size);
				if (size[0] == 0 || size[1] == 0) {
					attached = 0;
				} else {
					attachedDisplay = &_screenDisplays[index];
				}
			}
		}
	}

    _displaysMutex.unlock();

	return attachedDisplay;
}

int NativeWindow::updateScreen()
{
	int returnCode = screen_flush_context(_screenContext, SCREEN_WAIT_IDLE);

	return returnCode;
}


screen_buffer_t NativeWindow::screenBuffer(int index)
{
    if (_screenWindow != NULL) {
        screen_get_window_property_pv(_screenWindow, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)_screenBuffers);
    }

    return _screenBuffers[index];
}


	} /* end namespace base */

} /* end namespace views */
