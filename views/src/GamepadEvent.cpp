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

#include "GamepadEvent.hpp"
#include <math.h>

#include <QDebug>

namespace views {
	namespace event {

GamepadEvent::GamepadEvent()
{
}

/*!
 * @brief Constructs a touch event.
 *
 * @param controllerId - controller Id.
 * @param type - controller type.
 * @param buttonCount - controller button count.
 * @param buttons - buttons pressed
 * @param analogCount - controller analog count.
 * @param controllerAnalog0X - analog 0 x data
 * @param controllerAnalog0Y - analog 0 y data
 * @param controllerAnalog0Z - analog 0 z data
 * @param controllerAnalog1X - analog 1 x data
 * @param controllerAnalog1Y - analog 1 y data
 * @param controllerAnalog1Z - analog 1 z data
 * @param target - The touch target of this event.
 * @return A gamepad event.
 */
GamepadEvent::GamepadEvent(int controllerId, int type, int buttonCount, int buttons, int analogCount,
				int controllerAnalog0X,
				int controllerAnalog0Y,
				int controllerAnalog0Z,
				int controllerAnalog1X,
				int controllerAnalog1Y,
				int controllerAnalog1Z,
    			QObject* target)
{
    _controllerId = controllerId;
    _type = type;
    _buttonCount = buttonCount;
    _buttons = buttons;
    _analogCount = analogCount;

	_controllerAnalog0X = controllerAnalog0X;
	_controllerAnalog0Y = controllerAnalog0Y;
	_controllerAnalog0Z = controllerAnalog0Z;
	_controllerAnalog1X = controllerAnalog1X;
	_controllerAnalog1Y = controllerAnalog1Y;
	_controllerAnalog1Z = controllerAnalog1Z;

    _target = target;
}

/*!
 * @brief Destructs the mouse event.
 */
GamepadEvent::~GamepadEvent() {
	// TODO Auto-generated destructor stub
}


/*!
 * @brief  @c Id of touch target.
 */
int GamepadEvent::Id() const
{
	return _controllerId;
}

/*!
 * @brief The type of the event.
 *
 * @return The type.
 */
int GamepadEvent::type() const
{
	return _type;
}

/*!
 * @brief  @c number of controller buttons for the gamepad event target.
 */
int GamepadEvent::buttonCount() const
{
	return _buttonCount;
}

/*!
 * @brief  @c buttons pressed for the controller.
 */
int GamepadEvent::buttons() const
{
	return _buttons;
}

/*!
 * @brief  @c number of analog joysticks for the gamepad event target.
 */
int GamepadEvent::analogCount() const
{
	return _analogCount;
}

/*!
 * @brief  @c analog 0 x value for the gamepad event target.
 */
int GamepadEvent::controllerAnalog0X()
{
	return _controllerAnalog0X;
}

/*!
 * @brief  @c analog 0 y value for the gamepad event target.
 */
int GamepadEvent::controllerAnalog0Y()
{
	return _controllerAnalog0Y;
}

/*!
 * @brief  @c analog 0 z value for the gamepad event target.
 */
int GamepadEvent::controllerAnalog0Z()
{
	return _controllerAnalog0Z;
}

/*!
 * @brief  @c analog 1 x value for the gamepad event target.
 */
int GamepadEvent::controllerAnalog1X()
{
	return _controllerAnalog1X;
}

/*!
 * @brief  @c analog 1 y value for the gamepad event target.
 */
int GamepadEvent::controllerAnalog1Y()
{
	return _controllerAnalog1Y;
}

/*!
 * @brief  @c analog 1 z value for the gamepad event target.
 */
int GamepadEvent::controllerAnalog1Z()
{
	return _controllerAnalog1Z;
}

/*!
 * @brief  @c mouse event target.
 */
QObject* GamepadEvent::target() const
{
	return _target;
}

/*!
 * @brief Convenience function for checking if the A button is pressed.
 *
 * @return @c true if the A button is pressed.
 */
bool GamepadEvent::isAButtonDown() const
{
	return ((_buttons & SCREEN_A_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the B button is pressed.
 *
 * @return @c true if the B button is pressed.
 */
bool GamepadEvent::isBButtonDown() const
{
	return ((_buttons & SCREEN_B_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the C button is pressed.
 *
 * @return @c true if the C button is pressed.
 */
bool GamepadEvent::isCButtonDown() const
{
	return ((_buttons & SCREEN_C_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the X button is pressed.
 *
 * @return @c true if the X button is pressed.
 */
bool GamepadEvent::isXButtonDown() const
{
	return ((_buttons & SCREEN_X_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Y button is pressed.
 *
 * @return @c true if the Y button is pressed.
 */
bool GamepadEvent::isYButtonDown() const
{
	return ((_buttons & SCREEN_Y_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Z button is pressed.
 *
 * @return @c true if the Z button is pressed.
 */
bool GamepadEvent::isZButtonDown() const
{
	return ((_buttons & SCREEN_Z_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Menu 1 button is pressed.
 *
 * @return @c true if the Menu 1 button is pressed.
 */
bool GamepadEvent::isMenu1ButtonDown() const
{
	return ((_buttons & SCREEN_MENU1_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Menu 2 button is pressed.
 *
 * @return @c true if the Menu 2 button is pressed.
 */
bool GamepadEvent::isMenu2ButtonDown() const
{
	return ((_buttons & SCREEN_MENU2_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Menu 3 button is pressed.
 *
 * @return @c true if the Menu 3 button is pressed.
 */
bool GamepadEvent::isMenu3ButtonDown() const
{
	return ((_buttons & SCREEN_MENU3_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Menu 4 button is pressed.
 *
 * @return @c true if the Menu 4 button is pressed.
 */
bool GamepadEvent::isMenu4ButtonDown() const
{
	return ((_buttons & SCREEN_MENU4_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Left 1 button is pressed.
 *
 * @return @c true if the Left 1 button is pressed.
 */
bool GamepadEvent::isLeft1ButtonDown() const
{
	return ((_buttons & SCREEN_L1_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Left 2 button is pressed.
 *
 * @return @c true if the Left 2 button is pressed.
 */
bool GamepadEvent::isLeft2ButtonDown() const
{
	return ((_buttons & SCREEN_L2_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Left 3 button is pressed.
 *
 * @return @c true if the Left 3 button is pressed.
 */
bool GamepadEvent::isLeft3ButtonDown() const
{
	return ((_buttons & SCREEN_L3_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Right 1 button is pressed.
 *
 * @return @c true if the Right 1 button is pressed.
 */
bool GamepadEvent::isRight1ButtonDown() const
{
	return ((_buttons & SCREEN_R1_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Right 2 button is pressed.
 *
 * @return @c true if the Right 2 button is pressed.
 */
bool GamepadEvent::isRight2ButtonDown() const
{
	return ((_buttons & SCREEN_R2_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the Right 3 button is pressed.
 *
 * @return @c true if the Right 3 button is pressed.
 */
bool GamepadEvent::isRight3ButtonDown() const
{
	return ((_buttons & SCREEN_R3_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the DPad Up button is pressed.
 *
 * @return @c true if the DPad Up button is pressed.
 */
bool GamepadEvent::isDPadUpButtonDown() const
{
	return ((_buttons & SCREEN_DPAD_UP_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the DPad Down button is pressed.
 *
 * @return @c true if the DPad Down button is pressed.
 */
bool GamepadEvent::isDPadDownButtonDown() const
{
	return ((_buttons & SCREEN_DPAD_DOWN_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the DPad Left button is pressed.
 *
 * @return @c true if the DPad Left button is pressed.
 */
bool GamepadEvent::isRDPadLeftButtonDown() const
{
	return ((_buttons & SCREEN_DPAD_LEFT_GAME_BUTTON) != 0);
}

/*!
 * @brief Convenience function for checking if the DPad Right button is pressed.
 *
 * @return @c true if the DPad Right button is pressed.
 */
bool GamepadEvent::isRDPadRightButtonDown() const
{
	return ((_buttons & SCREEN_DPAD_RIGHT_GAME_BUTTON) != 0);
}

	}
}
