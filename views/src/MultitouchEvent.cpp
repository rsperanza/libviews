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

#include "MultitouchEvent.hpp"
#include <math.h>

#include <QDebug>

namespace views {
	namespace event {

MultitouchEvent::MultitouchEvent()
{
}

/*!
 * @brief Constructs a touch event.
 *
 * @param touchType - The type of touch.
 * @param screenX - Touch @c x coordinate relative to the screen.
 * @param screenY - Touch @c x coordinate relative to the screen.
 * @param windowX - Touch @c x coordinate relative to the window.
 * @param windowY - Touch @c x coordinate relative to the window.
 * @param target - The touch target of this event.
 * @return A touch event.
 */
MultitouchEvent::MultitouchEvent(int type, float screenX, float screenY, float windowX, float windowY, int id, int pressure, QObject* target)
{
    _type = type;
    _screenX = screenX;
    _screenY = screenY;
    _windowX = windowX;
    _windowY = windowY;
    _touchId = id;
    _pressure = pressure;
    _target = target;
}

/*!
 * @brief Destructs the touch event.
 */
MultitouchEvent::~MultitouchEvent() {
	// TODO Auto-generated destructor stub
}

/*!
 * @brief Retrieves the @c x coordinate of the touch relative to the screen.
 *
 * @return The @c x coordinate of the touch.
 */
float MultitouchEvent::screenX() const
{
	return _screenX;
}

/*!
 * @brief Retrieves the @c x coordinate of the touch relative to the screen.
 *
 * @return The @c y coordinate of the touch.
 */
float MultitouchEvent::screenY() const
{
	return _screenY;
}

/*!
 * @brief Retrieves the @c x coordinate of the touch relative to the window.
 *
 * @return The @c x coordinate of the touch.
 */
float MultitouchEvent::windowX() const
{
	return _windowX;
}


/*!
 * @brief Retrieves the @c x coordinate of the touch relative to the window.
 *
 * @return The @c y coordinate of the touch.
 */
float MultitouchEvent::windowY() const
{
	return _windowY;
}

/*!
 * @brief  @c pressure of the touch target.
 */
int MultitouchEvent::pressure() const
{
	return _pressure;
}

/*!
 * @brief  @c Id of touch target.
 */
int MultitouchEvent::Id() const
{
	return _touchId;
}

/*!
 * @brief The type of the event (SCREEN_EVENT_MTOUCH_TOUCH, SCREEN_EVENT_MTOUCH_MOVE, or SCREEN_EVENT_MTOUCH_RELEASE).
 *
 * @return The type.
 */
int MultitouchEvent::type() const
{
	return _type;
}

/*!
 * @brief  @c touch target.
 */
QObject* MultitouchEvent::target() const
{
	return _target;
}

/*!
     * @brief Convenience function for checking if the touch type returned from type() is SCREEN_EVENT_MTOUCH_MOVE.
 *
 * @return @c true if the touch type is TouchType::Move.
 */
Q_INVOKABLE bool MultitouchEvent::isMove() const
{
	return (_type == SCREEN_EVENT_MTOUCH_MOVE);
}

/*!
     * @brief Convenience function for checking if the touch type returned from type() is SCREEN_EVENT_MTOUCH_RELEASE.
 *
 * @return @c true if the touch type is TouchType::Up.
 */
Q_INVOKABLE bool MultitouchEvent::isUp() const
{
	return (_type == SCREEN_EVENT_MTOUCH_RELEASE);
}

/*!
     * @brief Convenience function for checking if the touch type returned from type() is SCREEN_EVENT_MTOUCH_TOUCH.
 *
 * @return @c true if the touch type is TouchType::Down.
 */
Q_INVOKABLE bool MultitouchEvent::isDown() const
{
	return (_type == SCREEN_EVENT_MTOUCH_TOUCH);
}

	}
}
