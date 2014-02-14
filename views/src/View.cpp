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
	qDebug()  << "View: View ";

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

	_createGroup = false;
	_createFullWindow = false;

	_renderGraphics = NULL;

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

    _nativeWindow->setSize(_width, _height);
    _nativeWindow->setZ(_z);
    _nativeWindow->setTransparency(_transparency);
    _nativeWindow->setWindowGroup(_group);
    _nativeWindow->setCreateWindowGroup(_createGroup);
    _nativeWindow->setWindowID(_id);

    returnCode = _nativeWindow->initialize(_createFullWindow);
    if (returnCode == EXIT_SUCCESS) {
    	_screenWindow = _nativeWindow->getScreenWindow();

        if (_renderGraphics) {
        	returnCode = _renderGraphics->initialize(_nativeWindow->getScreenWindow());
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

screen_window_t View::getScreenWindow()
{
	return _screenWindow;
}

int View::regenerate() {
	qDebug()  << "View::regenerate: ";

	if (_renderGraphics) {
		Graphics::lockRendering();

		_renderGraphics->regenerateCleanup();

		_nativeWindow->setPosition(_x, _y);

		_nativeWindow->setSize(_width, _height);

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

			// after rendering this view is no longer stale
			setStale(false);
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
		if (!visible()) {
			_renderGraphics->clear();
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


	} /* end namespace base */
} /* end namespace views */
