/*
 * Copyright (c) 2012-2013 BlackBerry
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

#include "MouseEvent.hpp"
#include <math.h>

#include <QDebug>

namespace views {
	namespace event {

MouseEvent::MouseEvent()
{
}

/*!
 * @brief Constructs a mouse event.
 *
 * @param screenX - Touch @c x coordinate relative to the screen.
 * @param screenY - Touch @c x coordinate relative to the screen.
 * @param windowX - Touch @c x coordinate relative to the window.
 * @param windowY - Touch @c x coordinate relative to the window.
 * @param target - The mouse event target of this event.
 * @param buttons - The button presses of mouse event.
 * @return A mouse event.
 */
MouseEvent::MouseEvent(float screenX, float screenY, float windowX, float windowY, int buttons, QObject* target)
{
    _screenX = screenX;
    _screenY = screenY;
    _windowX = windowX;
    _windowY = windowY;
    _buttons = buttons;
    _target = target;
}

/*!
 * @brief Destructs the mouse event.
 */
MouseEvent::~MouseEvent() {
	// TODO Auto-generated destructor stub
}

/*!
 * @brief Retrieves the @c x coordinate of the mouse event relative to the screen.
 *
 * @return The @c x coordinate of the mouse event.
 */
float MouseEvent::screenX() const
{
	return _screenX;
}

/*!
 * @brief Retrieves the @c x coordinate of the mouse event relative to the screen.
 *
 * @return The @c y coordinate of the mouse event.
 */
float MouseEvent::screenY() const
{
	return _screenY;
}

/*!
 * @brief Retrieves the @c x coordinate of the mouse event relative to the window.
 *
 * @return The @c x coordinate of the mouse event.
 */
float MouseEvent::windowX() const
{
	return _windowX;
}


/*!
 * @brief Retrieves the @c x coordinate of the mouse event relative to the window.
 *
 * @return The @c y coordinate of the mouse event.
 */
float MouseEvent::windowY() const
{
	return _windowY;
}

/*!
 * @brief  @c buttons pressed for the mouse event target.
 */
int MouseEvent::buttons() const
{
	return _buttons;
}

/*!
 * @brief  @c mouse event target.
 */
QObject* MouseEvent::target() const
{
	return _target;
}

/*!
 * @brief Convenience function for checking if the left mouse button is pressed.
 *
 * @return @c true if the left mouse button is pressed.
 */
bool MouseEvent::isLeftButtonDown() const
{
	return (_buttons & SCREEN_LEFT_MOUSE_BUTTON);
}

/*!
 * @brief Convenience function for checking if the right mouse button is pressed.
 *
 * @return @c true if the middle mouse button is pressed.
 */
bool MouseEvent::isMiddleButtonDown() const
{
	return (_buttons & SCREEN_MIDDLE_MOUSE_BUTTON);
}

/*!
 * @brief Convenience function for checking if the right mouse button is pressed.
 *
 * @return @c true if the right mouse button is pressed.
 */
bool MouseEvent::isRightButtonDown() const
{
	return (_buttons & SCREEN_RIGHT_MOUSE_BUTTON);
}

	}
}
