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
#include <QSharedPointer>

#include "View.hpp"
#include "NativeWindow.hpp"
#include "ViewsThread.hpp"

namespace views {
	namespace base {

View::View(ViewDisplay display) : QObject(ViewsThread::getInstance()), _display(display)
{
	qDebug()  << "View: View " << display;

	_enabled = false;
	_initialized = false;
	_stale = false;
	_altered = false;
	_visible = false;

	_angle = 0.0;
	_x = 0;
	_y = 0;
	_z = 0;
	_width = 0;
	_height = 0;
    _interval = 0;
	_transparency = SCREEN_TRANSPARENCY_NONE;
	_renderCount = 0;

    _sourceWidth = 0;
    _sourceHeight = 0;
    _format = SCREEN_FORMAT_RGBA8888;

	_createGroup = false;
	_createFullWindow = false;

	_renderGraphics = NULL;
	_copyBuffer = NULL;

	_nativeWindow = NULL;
	_screenWindow = NULL;

	_capture = false;
}

View::~View() {
	if (_nativeWindow) {
		delete _nativeWindow;
		_nativeWindow = NULL;
		_screenWindow = NULL;
	}
}

int View::initialize()
{
    int returnCode;

    _nativeWindow = new NativeWindow(_display);
    _nativeWindow->setAngle(_angle);
    _nativeWindow->setPosition(_x, _y);

	// for HDMI displays, set this window to display size if no size set
	int size[2];
	if (NativeWindow::screenDisplayAttached(size)) {
		if (_width == 0 && _height == 0) {

			_width = size[0];
			_height = size[1];

			if (_renderGraphics && _width > 0 && _height > 0) {
				_renderGraphics->setSize(_width, _height);
			}

			qDebug()  << "View: initialize: HDMI resize: " << _width << "x" << _height;
		}
	}

	if (_sourceWidth == 0) {
	    _nativeWindow->setSize(_width, _height, _width, _height);
	} else {
        _nativeWindow->setSize(_width, _height, _sourceWidth, _sourceHeight);
	}
    _nativeWindow->setFormat(_format);

    if (_sourceWidth > 0) {
        screen_create_pixmap(&_screenPixmap, _nativeWindow->getScreenContext());

        screen_set_pixmap_property_iv(_screenPixmap, SCREEN_PROPERTY_FORMAT, &_format);

        int usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
        screen_set_pixmap_property_iv(_screenPixmap, SCREEN_PROPERTY_USAGE, &usage);

        int size[2] = { _sourceWidth, _sourceHeight };
        screen_set_pixmap_property_iv(_screenPixmap, SCREEN_PROPERTY_BUFFER_SIZE, size);

        screen_create_pixmap_buffer(_screenPixmap);
        screen_get_pixmap_property_pv(_screenPixmap, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&_screenPixmapBuffer);

        screen_get_buffer_property_pv(_screenPixmapBuffer, SCREEN_PROPERTY_POINTER, (void **)&_screenPixmapBufferPtr);
        int stride;
        screen_get_buffer_property_iv(_screenPixmapBuffer, SCREEN_PROPERTY_STRIDE, &stride);

        screen_buffer_t screenBuffers[2], frontBuffer;
        screen_get_window_property_pv(_screenWindow, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screenBuffers);
        screen_get_window_property_pv(_screenWindow, SCREEN_PROPERTY_FRONT_BUFFER, (void **)&frontBuffer);

        screen_buffer_t nextBuffer = screenBuffers[0];
        if (nextBuffer == frontBuffer) {
            nextBuffer = screenBuffers[1];
        }

        memset(_screenPixmapBufferPtr, 0, stride*_sourceHeight);
        memset(_screenPixmapBufferPtr+stride*_sourceHeight, 0, stride*(_sourceHeight/2));
    }

    _nativeWindow->setZ(_z);
    _nativeWindow->setTransparency(_transparency);

    _nativeWindow->setWindowGroup(_group);
    _nativeWindow->setCreateWindowGroup(_createGroup);
    _nativeWindow->setWindowID(_id);

    returnCode = _nativeWindow->initialize(_createFullWindow);
    if (returnCode == EXIT_SUCCESS) {
    	_screenWindow = _nativeWindow->getScreenWindow();
		qDebug() << "View::initialize: _screenWindow: " << (int)_screenWindow;

        if (_renderGraphics) {
        	returnCode = _renderGraphics->initialize(_screenWindow);
        }

	    if (returnCode == EXIT_SUCCESS) {
	    	setInitialized(true);
	    }
    }

    discoverControllers();

	qDebug()  << "View: initialize: " << returnCode;

	return returnCode;
}

void View::setDisplay(ViewDisplay display)
{
	qDebug()  << "View::setDisplay: "<< display;

	_display = display;
}

void View::cleanup() {
	qDebug()  << "View::cleanup ";

	if (_renderGraphics) {
		delete _renderGraphics;
		_renderGraphics = NULL;
	}

	if (_nativeWindow) {
		delete _nativeWindow;
		_nativeWindow = NULL;
	}
}

void View::setEnabled(bool enabled) {
	qDebug()  << "View::setEnabled " << enabled;

	_enabled = enabled;
}

bool View::enabled() {
	return _enabled;
}

void View::setInitialized(bool initialized)
{
	_viewMutex.lock();

	_initialized = initialized;

	_viewMutex.unlock();
}

bool View::initialized() {
	bool initialized;

	_viewMutex.lock();

	initialized = _initialized;

	_viewMutex.unlock();

	return initialized;
}

void View::setStale(bool stale)
{
	_viewMutex.lock();

	_stale = stale;

	_viewMutex.unlock();
}

bool View::stale() {
	bool stale;

	_viewMutex.lock();

	stale = _stale;

	_viewMutex.unlock();

	return stale;
}

void View::setAltered(bool altered)
{
	_viewMutex.lock();

	_altered = altered;

	_viewMutex.unlock();
}

bool View::altered() {
	bool altered;

	_viewMutex.lock();

	altered = _altered;

	_viewMutex.unlock();

	return altered;
}

void View::setCapture(bool capture)
{
	_viewMutex.lock();

	_capture = capture;

	_viewMutex.unlock();
}

void View::onVisible()
{
	qDebug()  << "View::onVisible";
}

void View::setVisible(bool visible)
{
	_viewMutex.lock();

	_visible = visible;

	_viewMutex.unlock();
}

bool View::visible() {
	bool visible;

	_viewMutex.lock();

	visible = _visible;

	_viewMutex.unlock();

	return visible;
}

int View::display()
{
	return _display;
}

void View::setDisplay(int display)
{
	_display = (ViewDisplay)display;
}

int View::angle()
{
	return _angle;
}

void View::setAngle(int angle)
{
	if ((angle != 0) && (angle != 90) && (angle != 180) && (angle != 270)) {
		fprintf(stderr, "Invalid angle\n");
	} else {
		_angle = angle;
	}
}

int View::x()
{
	return _x;
}

void View::setX(int x)
{
	_x = x;
}

int View::y()
{
	return _y;
}

void View::setY(int y)
{
	_y = y;
}

void View::setPosition(int x, int y)
{
	_x = x;
	_y = y;
}

int View::width() {
	return _width;
}

void View::setWidth(int width)
{
	_width = width;

	if (_renderGraphics && width > 0) {
		_renderGraphics->setSize(_width, _height);
	}
}

int View::height() {
	return _height;
}

void View::setHeight(int height)
{
	_height = height;

	if (_renderGraphics && height > 0) {
		_renderGraphics->setSize(_width, _height);
	}
}

void View::setSize(int width, int height)
{
	_width = width;
	_height = height;

	if (_renderGraphics && width > 0 && height > 0) {
		_renderGraphics->setSize(_width, _height);
	}
}

int View::z()
{
	return _z;
}

void View::setZ(int z)
{
	_z = z;
}

int View::transparency()
{
	return _transparency;
}

void View::setTransparency(int transparency)
{
	_transparency = transparency;
}


int View::sourceWidth() {
    return _sourceWidth;
}

void View::setSourceWidth(int sourceWidth)
{
    _sourceWidth = sourceWidth;
}

int View::sourceHeight() {
    return _sourceHeight;
}

void View::setSourceHeight(int sourceHeight)
{
    _sourceHeight = sourceHeight;
}

int View::format()
{
    return _format;
}

void View::setFormat(int format)
{
    _format = format;
}

QString View::windowID()
{
	return _id;
}

void View::setWindowID(const QString id)
{
	_id = id;
}

QString View::windowGroup()
{
	return _group;
}

void View::setWindowGroup(const QString &group)
{
	_group = group;
}

bool View::createWindowGroup()
{
	return _createGroup;
}

void View::setCreateWindowGroup(bool create)
{
	_createGroup = create;
}

bool View::createFullWindow()
{
	return _createFullWindow;
}

void View::setCreateFullWindow(bool create)
{
	_createFullWindow = create;
}

int View::regenerate() {
	qDebug()  << "View::regenerate: ";

	if (_renderGraphics) {
		Graphics::lockRendering();

		_renderGraphics->regenerateCleanup();

		_nativeWindow->setPosition(_x, _y);

		_nativeWindow->setSize(_width, _height, _sourceWidth, _sourceHeight);

		_nativeWindow->setZ(_z);

		_nativeWindow->setTransparency(_transparency);

		_renderGraphics->regenerate(_screenWindow);

		Graphics::unlockRendering();
	}

	onRegenerated();

	emit regenerated();

	setAltered(false);

	setStale(true);

	return EXIT_SUCCESS;
}

void View::update()
{
	emit updated();
}

void View::renderView()
{
	//qDebug()  << "View::renderView: " << _renderCount << initialized() << " : " << enabled() << " : " << visible() << " : " << stale();
	if (initialized() && enabled() && visible() && stale()) {
		// if a Graphics class is registered, call it to render
		if (_renderGraphics) {
			_renderGraphics->renderSafe(_capture);
			qDebug()  << "View::renderView: rendered ";

			// after rendering this view is no longer stale
			setStale(false);
		} else {
		    int rect[4] = { 0, 0 };
		    screen_get_window_property_iv(_screenWindow, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);

		   int hg[] = {
		        SCREEN_BLIT_SOURCE_WIDTH, _sourceWidth,
		        SCREEN_BLIT_SOURCE_HEIGHT, _sourceHeight,
		        SCREEN_BLIT_DESTINATION_X, 0,
		        SCREEN_BLIT_DESTINATION_Y, 0,
		        SCREEN_BLIT_DESTINATION_WIDTH, _sourceWidth,
		        SCREEN_BLIT_DESTINATION_HEIGHT, _sourceHeight,
		        //SCREEN_BLIT_TRANSPARENCY, SCREEN_TRANSPARENCY_NONE,
		        //SCREEN_BLIT_TRANSPARENCY, SCREEN_TRANSPARENCY_SOURCE_OVER,
		        SCREEN_BLIT_END
		    };

		    screen_buffer_t screenBuffers[2], frontBuffer;
		    screen_get_window_property_pv(_screenWindow, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screenBuffers);
		    screen_get_window_property_pv(_screenWindow, SCREEN_PROPERTY_FRONT_BUFFER, (void **)&frontBuffer);

	        int stride;
	        screen_get_buffer_property_iv(_screenPixmapBuffer, SCREEN_PROPERTY_STRIDE, &stride);

		    screen_buffer_t nextBuffer = screenBuffers[0];
		    if (nextBuffer == frontBuffer) {
		        nextBuffer = screenBuffers[1];
		    }

            _viewMutex.lock();

           if (_copyBuffer) {
                if (_format == SCREEN_FORMAT_NV12) {
                    memcpy(_screenPixmapBufferPtr, _copyBuffer, _bufferRows*stride);
                    for(int index = 0; index < (_bufferRows-1); index++) {
                        memcpy(_screenPixmapBufferPtr + _bufferRows*stride + index*stride/2, _copyBuffer + _bufferRows*_bufferStride + index*_bufferUVStride/2, stride/2);
                    }
                }
            }

            _viewMutex.unlock();

		    screen_blit(_nativeWindow->getScreenContext(), nextBuffer, _screenPixmapBuffer, hg);

		    screen_post_window(_screenWindow, nextBuffer, 1, rect, 0);
		}

		bool isVisible = false;

		_viewMutex.lock();

		_renderCount++;

		if (_visible && _renderCount == 1) {
			isVisible = true;
		}
		if (_capture) {
			_capture = false;
		}

		_viewMutex.unlock();

		if (isVisible) {
			onVisible();
		}

		//qDebug()  << "View::renderView: " << _renderCount << initialized() << " : " << enabled() << " : " << visible() << " : " << stale();
	} else {

		if (!enabled() && _nativeWindow) {
			//_renderGraphics->clear();
			qDebug()  << "View::renderView: disabled " << _renderCount << initialized() << " : " << visible() << " : " << _nativeWindow;

			Graphics::lockRendering();

			if (_renderGraphics) {
				_renderGraphics->regenerateCleanup();
			}

			Graphics::unlockRendering();

			if (_renderGraphics) {
				_renderGraphics->cleanup();
			}

			if (_nativeWindow) {
				delete _nativeWindow;
				_nativeWindow = NULL;
			}

			setInitialized(false);
		}

		// reset render count
		_renderCount = 0;
	}

	//qDebug()  << "View::renderView: " << _renderCount << initialized() << " : " << enabled() << " : " << visible() << " : " << stale();
}

void View::add()
{
	qDebug()  << "View::add: ";

	ViewsThread::getInstance()->addView(this);
}

void View::remove()
{
	qDebug()  << "View::remove: ";

	ViewsThread::getInstance()->removeView(this);
}


void View::loadController(int index)
{
	int analog[3];

    // Query libscreen for information about this device.
	int returnCode = screen_get_device_property_cv(_controller[index], SCREEN_PROPERTY_ID_STRING, sizeof(_controllerID[index]), _controllerID[index]);
	if (returnCode != 0) {
		qCritical()  << "MultitouchPane loadController: failed to retrieve device ID";
	}

	returnCode = screen_get_device_property_iv(_controller[index], SCREEN_PROPERTY_BUTTON_COUNT, &_controllerButtonCount[index]);
	if (returnCode != 0) {
		qCritical()  << "MultitouchPane loadController: failed to retrieve device button count";
	}

    // Check for the existence of analog sticks.
    if (!screen_get_device_property_iv(_controller[index], SCREEN_PROPERTY_ANALOG0, analog)) {
		_controllerAnalog0X[index] = analog[0];
		_controllerAnalog0Y[index] = analog[1];
		_controllerAnalog0Z[index] = analog[2];
    	++_controllerAnalogCount[index];
    }

    if (!screen_get_device_property_iv(_controller[index], SCREEN_PROPERTY_ANALOG1, analog)) {
		_controllerAnalog1X[index] = analog[0];
		_controllerAnalog1Y[index] = analog[1];
		_controllerAnalog1Z[index] = analog[2];
    	++_controllerAnalogCount[index];
    }
}

void View::discoverControllers()
{
	int returnCode;

    // Get an array of all available devices.
    int deviceCount;
    returnCode = screen_get_context_property_iv(_nativeWindow->getScreenContext(), SCREEN_PROPERTY_DEVICE_COUNT, &_controllerCount);;
	if (returnCode != 0) {
		qCritical()  << "MultitouchPane discoverControllers: failed to retrieve device count";
	}

    screen_device_t* devices = (screen_device_t*)calloc(_controllerCount, sizeof(screen_device_t));
    returnCode = screen_get_context_property_pv(_nativeWindow->getScreenContext(), SCREEN_PROPERTY_DEVICES, (void**)devices);
	if (returnCode != 0) {
		qCritical()  << "MultitouchPane discoverControllers: failed to retrieve device list";
	}

    // Scan the list for gamepad and joystick devices.  Use the first compatible device.
    for (int index = 0; index < _controllerCount; index++) {
        int type;
        returnCode = screen_get_device_property_iv(devices[index], SCREEN_PROPERTY_TYPE, &type);
    	if (returnCode != 0) {
    		qCritical()  << "MultitouchPane discoverControllers: failed to retrieve device type";
    	}

        if ((type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)) {
            // Assign this device to control Player 1 or Player 2.
            _controller[index]     = devices[index];
            _controllerType[index] = type;

            loadController(index);

			break;
        }
    }

    free(devices);
}

void View::handleScreenEvent(bps_event_t *event) {
    int screenEventType;
    int buttons;
    int position[2];
    int realPosition[2];
    int touchID;
    int touchPressure;
    int keyFlags;
    int keyModifiers;
    int keySymbols;
    int returnCode;
    int index;
    int analog[3];
	screen_window_t screenWindow;
	screen_device_t device;

    screen_event_t screen_event = screen_event_get_event(event);

	//Query type of screen event and its location on the screen
	returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screenEventType);
	if (returnCode != 0) {
		qCritical()  << "View::handleScreenEvent: failed to retrieve event type";
	} else {
		//qDebug() << "View::handleScreenEvent: event type " << screenEventType << " " << screenWindow << " " << _screenWindow;
	}

	returnCode = screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void**)&screenWindow);
	if (returnCode != 0) {
		qCritical()  << "View::handleScreenEvent: failed to retrieve event window";
	}

	if (screenWindow == _screenWindow) {

		if (screenEventType != SCREEN_EVENT_KEYBOARD && screenEventType != SCREEN_EVENT_DEVICE
			&& screenEventType != SCREEN_EVENT_GAMEPAD && screenEventType != SCREEN_EVENT_JOYSTICK) {

			returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION, position);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event source position";
			}

			returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_POSITION, realPosition);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event position";
			}
		}

		if (screenEventType == SCREEN_EVENT_MTOUCH_TOUCH || screenEventType == SCREEN_EVENT_MTOUCH_MOVE || screenEventType == SCREEN_EVENT_MTOUCH_RELEASE) {

			returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TOUCH_ID, &touchID);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event touch ID";
			}

			returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TOUCH_PRESSURE, &touchPressure);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event touch Pressure";
			}
		}

		switch (screenEventType) {
			case SCREEN_EVENT_MTOUCH_TOUCH:
				//fprintf(stderr, "multitouch: %d %d,%d  %d,%d \n", touchID, (float)realPosition[0], (float)realPosition[1], (float)position[0], (float)position[1] );

				emit multitouch(new MultitouchEvent(screenEventType, (float)realPosition[0], (float)realPosition[1], (float)position[0], (float)position[1], touchID, touchPressure, this));
				break;

			case SCREEN_EVENT_MTOUCH_MOVE:
				//fprintf(stderr, "multitouch move: %d %d,%d  %d,%d \n", touchID, (float)realPosition[0], (float)realPosition[1], position[0], position[1] );

				emit multitouch(new MultitouchEvent(screenEventType, (float)realPosition[0], (float)realPosition[1], (float)position[0], (float)position[1], touchID, touchPressure, this));
				break;

			case SCREEN_EVENT_MTOUCH_RELEASE:
				//fprintf(stderr, "multitouch release: %d %d,%d  %d,%d \n", touchID, (float)realPosition[0], (float)realPosition[1], position[0], position[1] );

				emit multitouch(new MultitouchEvent(screenEventType, (float)realPosition[0], (float)realPosition[1], (float)position[0], (float)position[1], touchID, touchPressure, this));
				break;

			case SCREEN_EVENT_POINTER:
				returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &buttons);
				if (returnCode != 0) {
					qCritical()  << "View::handleScreenEvent: failed to retrieve event buttons";
				}

				emit mouse(new MouseEvent((float)realPosition[0], (float)realPosition[1], (float)position[0], (float)position[1], buttons, this));
				break;

			case SCREEN_EVENT_KEYBOARD:
				returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_FLAGS, &keyFlags);
				if (returnCode != 0) {
					qCritical()  << "View::handleScreenEvent: failed to retrieve event key flags";
				}

				returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_MODIFIERS, &keyModifiers);
				if (returnCode != 0) {
					qCritical()  << "View::handleScreenEvent: failed to retrieve event key modifiers";
				}

				if (keyFlags & KEY_DOWN) {
					returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_SYM, &keySymbols);
					if (returnCode != 0) {
						qCritical()  << "View::handleScreenEvent: failed to retrieve event key symbols";
					}
				}
				if (!(keyFlags & KEY_DOWN)) {
					keySymbols = 0;
				}

				emit key(new KeyEvent(keyFlags, keyModifiers, keySymbols, this));
				break;
		}
	}

	switch (screenEventType) {
		case SCREEN_EVENT_DEVICE:
			// A device was attached or removed.
			int attached;
			int type;

			returnCode = screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void**)&device);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event device";
			}

			returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ATTACHED, &attached);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event device attached state";
			}

			if (attached) {
				returnCode = screen_get_device_property_iv(device, SCREEN_PROPERTY_TYPE, &type);
				if (returnCode != 0) {
					qCritical()  << "View::handleScreenEvent: failed to retrieve event device type";
				}
			}

			if (attached && (type == SCREEN_EVENT_GAMEPAD || type == SCREEN_EVENT_JOYSTICK)) {
				for (int index = 0; index < MAX_CONTROLLERS; ++index) {
					if (!_controller[index]) {
						_controller[index] = device;
						loadController(index);
						_controllerCount++;
						break;
					}
				}
			} else {
				for (int index = 0; index < MAX_CONTROLLERS; ++index) {
					if (device == _controller[index]) {
						_controller[index] = NULL;
						_controllerCount--;
						break;
					}
				}
			}
			break;

		case SCREEN_EVENT_GAMEPAD:
		case SCREEN_EVENT_JOYSTICK:
			returnCode = screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_DEVICE, (void**)&device);
			if (returnCode != 0) {
				qCritical()  << "View::handleScreenEvent: failed to retrieve event device";
			}

			for (index = 0; index < MAX_CONTROLLERS; ++index) {
				if (device == _controller[index]) {
					break;
				}
			}

			if (index < MAX_CONTROLLERS) {
				// Store the controller's new state.
				returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &_controllerButtons[index]);
				if (returnCode != 0) {
					qCritical()  << "View::handleScreenEvent: failed to retrieve event device buttons";
				}

				if (_controllerAnalogCount[index] > 0) {
					returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG0, analog);
					if (returnCode != 0) {
						qCritical()  << "View::handleScreenEvent: failed to retrieve event device analog0";
					} else {
						_controllerAnalog0X[index] = analog[0];
						_controllerAnalog0Y[index] = analog[1];
						_controllerAnalog0Z[index] = analog[2];
					}
				}

				if (_controllerAnalogCount[index] == 2) {
					returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_ANALOG1, analog);
					if (returnCode != 0) {
						qCritical()  << "View::handleScreenEvent: failed to retrieve event device analog1";
					} else {
						_controllerAnalog1X[index] = analog[0];
						_controllerAnalog1Y[index] = analog[1];
						_controllerAnalog1Z[index] = analog[2];
					}
				}

				emit gamepad(new GamepadEvent(index, _controllerType[index], _controllerButtonCount[index], _controllerButtons[index],
													_controllerAnalogCount[index],
													_controllerAnalog0X[index],
													_controllerAnalog0Y[index],
													_controllerAnalog0Z[index],
													_controllerAnalog1X[index],
													_controllerAnalog1Y[index],
													_controllerAnalog1Z[index],
													this));
			}

			break;

	}
}

void View::registerGraphics(Graphics *renderGraphics)
{
	qDebug()  << "View::registerGraphics: ";

	_renderGraphics = renderGraphics;

	if (_renderGraphics && _width > 0 && _height > 0) {
		_renderGraphics->setSize(_width, _height);
	}
}

int View::calculateDPI()
{
	return _nativeWindow->calculateDPI();
}

ImageData* View::getRenderedImage()
{
	return _renderGraphics->getRenderedImage();
}



screen_window_t View::getScreenWindow()
{
    return _screenWindow;
}

screen_buffer_t View::screenBuffer(int index)
{
    return _nativeWindow->screenBuffer(index);
}

void View::copyBufferFrom(screen_buffer_t incomingBuffer, uint64_t frameSize, int rows, int stride, int uvStride)
{
    qDebug()  << "View::copyBufferFrom: " << incomingBuffer << " : " << frameSize << " : " << rows << " : " << stride << " : " << uvStride;

    _viewMutex.lock();

    if (_copyBuffer) {
        free(_copyBuffer);
    }

    _bufferRows = rows;
    _bufferStride = stride;
    _bufferUVStride = uvStride;
    _bufferSize = frameSize;
    _copyBuffer = (unsigned char*)incomingBuffer;

    _viewMutex.unlock();

    qDebug()  << "View::copyBufferFrom: done";
}

/*


The stride is the number of bytes between pixels on different rows, represented by the stride variable. If a pixel at position (x,y) is at ptr, the pixel at location (x, y+1) will be at (ptr + stride). Because you set write and native usage, there's no guarantee that each line is 400 bytes in this case. Drivers often have constraints that require the stride to be larger than width * bytes per pixel.

int stride = 0;
screen_get_buffer_property_iv(screen_pbuf, SCREEN_PROPERTY_STRIDE, &stride);


Rather than load an image, as a real application might at this stage, the hourglass is simple enough to calculate. The calculation adjusts the alpha channel to be transparent or opaque based on a test that determines whether a pixel is inside or outside the hourglass.

for (i = 0; i < size[1]; i++, ptr += stride) {
    for (j = 0; j < size[0]; j++) {
        ptr[j*4] = 0xa0;
        ptr[j*4+1] = 0xa0;
        ptr[j*4+2] = 0xa0;
        ptr[j*4+3] = ((j >= i && j <= size[1]-i) || (j <= i && j >= size[1]-i)) ? 0xff : 0;
    }
}

 */



	} /* end namespace base */
} /* end namespace views */
