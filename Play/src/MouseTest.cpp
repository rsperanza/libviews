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

#include <math.h>

#include "MouseTest.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views::graphics;


MouseTest::MouseTest(ViewDisplay display) : View(display)
{
	qDebug()  << "MouseTest::MouseTest ";

	_mouseX = -1;
	_mouseY = -1;
	_mouseButtons = 0;
	_keyCode = 0;

	_lastMouseX = -1;
	_lastMouseY = -1;
	_lastMouseButtons = 0;
	_lastKeyCode = 0;

	_graphics2D = new Graphics2D(display);

	_evenDashes = new float[2] { 20.0, 10.0 };
	_evenDashesStroke = _graphics2D->createStroke(10.0, CAP_NONE, JOIN_NONE, 0.0, _evenDashes, 2, 0.0);
	_defaultStroke = _graphics2D->createStroke(1.0);
	_slatePro = NULL;
	_slateProLight = NULL;

	_testImage = _graphics2D->loadImage("app/native/assets/images/handle_inactive.png");
	_testImage1 = _graphics2D->loadImage("app/native/assets/images/handle_pressed.png");

	// register graphics with base class
	registerGraphics(_graphics2D);
}

MouseTest::~MouseTest() {
	// TODO Auto-generated destructor stub
}

void MouseTest::update()
{
	//qDebug()  << "MouseTest::update";

	if (_lastMouseX != _mouseX || _lastMouseY != _mouseY || _lastMouseButtons != _mouseButtons || _lastKeyCode != _keyCode) {
		updateGraphics();

		_lastMouseX = _mouseX;
		_lastMouseY = _mouseY;
		_lastMouseButtons = _mouseButtons;
		_lastKeyCode = _keyCode;
	}
}

void MouseTest::onVisible()
{
	qDebug()  << "MouseTest::onVisible";
}

void MouseTest::onAltered()
{
	qDebug()  << "MouseTest::onAltered";

	updateGraphics();
}

void MouseTest::updateGraphics()
{
	qDebug()  << "MouseTest::updateGraphics";

	if (_graphics2D->reset()) {
		//_graphics2D->setBackground(COLOR_YELLOW);
		//_graphics2D->clearRect(0, 0, 100, 100);

		//_graphics2D->scale(2.0, 2.0);

		if (!_slatePro) {
			_slatePro = _graphics2D->createFont(FONT_NAME_SLATE_PRO_CONDENSED, (QString*)NULL, 15, calculateDPI(), new QString("mouse: ()LMRabcd"));
			_slateProLight = _graphics2D->createFont(FONT_NAME_SLATE_PRO_LIGHT, (QString*)NULL, 15, calculateDPI(), new QString(" .-,0123456789 "));
		}

		_graphics2D->setFont(_slatePro);

		double textWidth, textHeight, tempWidth, tempHeight, startInfoX, startNumberX, startButtonsX, startKeyX;
		textWidth = 0.0;
		textHeight = 0.0;
		_graphics2D->measureString("mouse: ", &textWidth, &textHeight);
		startNumberX = textWidth;

		QString mouseCoords(QString::number(_mouseX) + QString(", ") + QString::number(_mouseY));

		_graphics2D->setFont(_slateProLight);
		_graphics2D->measureString(mouseCoords, &tempWidth, &tempHeight);
		textWidth += tempWidth;
		startButtonsX = textWidth;

		QString mouseButtons(" (");
		_graphics2D->setFont(_slatePro);
		if (_mouseButtons > 0) {
			_graphics2D->measureString(" (", &tempWidth, &tempHeight);
			textWidth += tempWidth;
			if ((_mouseButtons & SCREEN_LEFT_MOUSE_BUTTON) == SCREEN_LEFT_MOUSE_BUTTON) {
				_graphics2D->measureString("L", &tempWidth, &tempHeight);
				textWidth += tempWidth;
				mouseButtons += QString("L");
			}
			if ((_mouseButtons & SCREEN_MIDDLE_MOUSE_BUTTON) == SCREEN_MIDDLE_MOUSE_BUTTON) {
				_graphics2D->measureString("M", &tempWidth, &tempHeight);
				textWidth += tempWidth;
				mouseButtons += QString("M");
			}
			if ((_mouseButtons & SCREEN_RIGHT_MOUSE_BUTTON) == SCREEN_RIGHT_MOUSE_BUTTON) {
				_graphics2D->measureString("R", &tempWidth, &tempHeight);
				textWidth += tempWidth;
				mouseButtons += QString("R");
			}
			_graphics2D->measureString(")", &tempWidth, &tempHeight);
			textWidth += tempWidth;
			mouseButtons += QString(")");
		}
		startKeyX = textWidth;

		QString key(" ");

		if (_keyCode > 0) {
			char keyChars[2];
			keyChars[0] = (char)_keyCode;
			keyChars[1] = 0;
			key += QChar((char)_keyCode);

			_graphics2D->measureString(key, &tempWidth, &tempHeight);
			textWidth += tempWidth;

			qDebug()  << "MouseTest::updateGraphics: other: " << _keyCode << " " << keyChars[0] << " " << key;
		}


		startInfoX = (double)(_width - textWidth) / 2.0;
		startNumberX += startInfoX;
		startButtonsX += startInfoX;
		startKeyX += startInfoX;
		qDebug()  << "MouseTest::updateGraphics: textStarts: " << startInfoX << " " << startNumberX << " " << startButtonsX;

		_graphics2D->setColor(COLOR_BLUE);

 		_graphics2D->setFont(_slatePro);
		QString text("mouse: ");
		_graphics2D->drawString(text, startInfoX, 150.0);

 		_graphics2D->setFont(_slateProLight);
		_graphics2D->drawString(mouseCoords, startNumberX, 150.0);

		if (_mouseButtons > 0) {
			_graphics2D->setFont(_slatePro);
			_graphics2D->drawString(mouseButtons, startButtonsX, 150.0);
		}

		if (_keyCode > 0) {
			_graphics2D->setFont(_slatePro);
			_graphics2D->drawString(key, startKeyX, 150.0);
		}

		if (_mouseButtons > 0) {
			if ((_mouseButtons & SCREEN_LEFT_MOUSE_BUTTON) == SCREEN_LEFT_MOUSE_BUTTON) {
				_graphics2D->drawImage(_testImage, _mouseX - 50.0, _height - _mouseY - 50.0);
			}
			if ((_mouseButtons & SCREEN_MIDDLE_MOUSE_BUTTON) == SCREEN_MIDDLE_MOUSE_BUTTON) {
			}
			if ((_mouseButtons & SCREEN_RIGHT_MOUSE_BUTTON) == SCREEN_RIGHT_MOUSE_BUTTON) {
				_graphics2D->drawImage(_testImage1, _mouseX - 50.0, _height - _mouseY - 50.0);
			}
		} else {
			_graphics2D->setColor(COLOR_BLACK);
			_graphics2D->setStroke(_defaultStroke);
			_graphics2D->drawOval((double)_mouseX, (double)(_height - _mouseY), 10.0, 10.0);
		}

		_graphics2D->done();

		setStale(true);
	}
}

void MouseTest::handleScreenEvent(bps_event_t *event)
{
    int screenEvent;
    int position[2];
    int realPosition[2];
    int keyFlags;
    int keySymbols;
    int returnCode;
	screen_window_t screenWindow;

	View::handleScreenEvent(event);

    screen_event_t screen_event = screen_event_get_event(event);

	returnCode = screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void**)&screenWindow);
	if (returnCode != 0) {
		qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event window";
	}

	if (screenWindow == _screenWindow) {
	    //Query type of screen event and its location on the screen
		returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screenEvent);
		if (returnCode != 0) {
			qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event type";
		}

		returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION, position);
		if (returnCode != 0) {
			qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event source position";
		}

		returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_POSITION, realPosition);
		if (returnCode != 0) {
			qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event position";
		}

		switch (screenEvent) {
			case SCREEN_EVENT_POINTER:
				returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &_mouseButtons);
				if (returnCode != 0) {
					qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event buttons";
				}

				_mouseX = position[0];
				_mouseY = position[1];

				qDebug()  << "MouseTest::handleScreenEvent: mouse: " << _mouseX << "," << _mouseY << " buttons: " << _mouseButtons ;

				break;
			case SCREEN_EVENT_KEYBOARD:
				returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_FLAGS, &keyFlags);
				if (returnCode != 0) {
					qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event key flags";
				}

				if (keyFlags & KEY_DOWN) {
					returnCode = screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_KEY_SYM, &keySymbols);
					if (returnCode != 0) {
						qCritical()  << "MouseTest::handleScreenEvent: failed to retrieve event key symbols";
					}

					qDebug()  << "The '" << (char)keySymbols << "' key was pressed.";

					switch (keySymbols) {
					case KEYCODE_A:
						_keyCode = (int)'a';
						break;
					case KEYCODE_B:
						_keyCode = (int)'b';
						break;
					case KEYCODE_C:
						_keyCode = (int)'c';
						break;
					case KEYCODE_D:
						_keyCode = (int)'d';
						break;
					case KEYCODE_E:
						_keyCode = (int)'e';
						break;
					default:
						break;
					}

					qDebug()  << "The keyCode is " << (char)_keyCode << ".";
				}
				if ((keyFlags & KEY_DOWN) == 0) {
					_keyCode = 0;
				}
			   break;

			default:
				break;
		}
	}
}
