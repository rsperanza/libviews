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


#ifndef GAMEPADEVENT_HPP
#define GAMEPADEVENT_HPP

#include <bps/screen.h>

#include <QtCore/QObject>

namespace views {
	namespace event {

class Q_DECL_EXPORT GamepadEvent : public QObject {

private:
    Q_OBJECT

    /*!
     * @brief  @c Id of gamepad.
     */
    Q_PROPERTY(int Id READ Id )
    /*!
     * @brief The type of the event.
     */
    Q_PROPERTY(int type READ type )
     /*!
     * @brief  @c number of controller buttons for the gamepad event target.
     */
    Q_PROPERTY(int buttonCount READ buttonCount )
    /*!
     * @brief  @c buttons pressed for the gamepad event target.
     */
    Q_PROPERTY(int buttons READ buttons )
    /*!
     * @brief  @c number of analog joysticks for the gamepad event target.
     */
    Q_PROPERTY(int analogCount READ analogCount )
    /*!
     * @brief  @c analog 0 x value for the gamepad event target.
     */
    Q_PROPERTY(int controllerAnalog0X READ controllerAnalog0X )
    /*!
     * @brief  @c analog 0 y value for the gamepad event target.
     */
    Q_PROPERTY(int controllerAnalog0Y READ controllerAnalog0Y )
    /*!
     * @brief  @c analog 0 z value for the gamepad event target.
     */
    Q_PROPERTY(int controllerAnalog0Z READ controllerAnalog0Z )
    /*!
     * @brief  @c analog 1 x value for the gamepad event target.
     */
    Q_PROPERTY(int controllerAnalog1X READ controllerAnalog1X )
    /*!
     * @brief  @c analog 1 y value for the gamepad event target.
     */
    Q_PROPERTY(int controllerAnalog1Y READ controllerAnalog1Y )
    /*!
     * @brief  @c analog 1 z value for the gamepad event target.
     */
    Q_PROPERTY(int controllerAnalog1Z READ controllerAnalog1Z )
    /*!
     * @brief The target of the event.
     */
    Q_PROPERTY(QObject* target READ target )

public:
    GamepadEvent();

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
    GamepadEvent(int controllerId, int type, int buttonCount, int buttons, int analogCount,
				int controllerAnalog0X,
				int controllerAnalog0Y,
				int controllerAnalog0Z,
				int controllerAnalog1X,
				int controllerAnalog1Y,
				int controllerAnalog1Z,
    			QObject* target);
    /*!
     * @brief Destructs the touch event.
     */
    virtual ~GamepadEvent();

    /*!
     * @brief  @c Id of gamepad.
     */
    int Id() const;

    /*!
     * @brief The type of the event.
     */
    int type() const;

    /*!
     * @brief  @c number of controller buttons for the gamepad event target.
     */
    int buttonCount() const;

    /*!
     * @brief  @c buttons pressed for the mouse event target.
     */
    int buttons() const;

    /*!
     * @brief  @c number of analog joysticks for the gamepad event target.
     */
    int analogCount() const;

    /*!
     * @brief  @c analog 0 x value for the gamepad event target.
     */
    int controllerAnalog0X();

    /*!
     * @brief  @c analog 0 y value for the gamepad event target.
     */
    int controllerAnalog0Y();

    /*!
     * @brief  @c analog 0 z value for the gamepad event target.
     */
    int controllerAnalog0Z();

    /*!
     * @brief  @c analog 1 x value for the gamepad event target.
     */
    int controllerAnalog1X();

	/*!
	* @brief  @c analog 1 y value for the gamepad event target.
	*/
    int controllerAnalog1Y();

	/*!
	* @brief  @c analog 1 z value for the gamepad event target.
	*/
    int controllerAnalog1Z();

    /*!
     * @brief  @c touch target.
     */
    QObject* target() const;

    /*!
     * @brief Convenience function for checking if the A button is pressed.
     *
     * @return @c true if the A button is pressed.
     */
    Q_INVOKABLE bool isAButtonDown() const;

    /*!
     * @brief Convenience function for checking if the B button is pressed.
     *
     * @return @c true if the B button is pressed.
     */
    Q_INVOKABLE bool isBButtonDown() const;

    /*!
     * @brief Convenience function for checking if the C button is pressed.
     *
     * @return @c true if the C button is pressed.
     */
    Q_INVOKABLE bool isCButtonDown() const;

    /*!
     * @brief Convenience function for checking if the X button is pressed.
     *
     * @return @c true if the X button is pressed.
     */
    Q_INVOKABLE bool isXButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Y button is pressed.
     *
     * @return @c true if the Y button is pressed.
     */
    Q_INVOKABLE bool isYButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Z button is pressed.
     *
     * @return @c true if the Z button is pressed.
     */
    Q_INVOKABLE bool isZButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Menu 1 button is pressed.
     *
     * @return @c true if the Menu 1 button is pressed.
     */
    Q_INVOKABLE bool isMenu1ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Menu 2 button is pressed.
     *
     * @return @c true if the Menu 2 button is pressed.
     */
    Q_INVOKABLE bool isMenu2ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Menu 3 button is pressed.
     *
     * @return @c true if the Menu 3 button is pressed.
     */
    Q_INVOKABLE bool isMenu3ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Menu 4 button is pressed.
     *
     * @return @c true if the Menu 4 button is pressed.
     */
    Q_INVOKABLE bool isMenu4ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Left 1 button is pressed.
     *
     * @return @c true if the Left 1 button is pressed.
     */
    Q_INVOKABLE bool isLeft1ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Left 2 button is pressed.
     *
     * @return @c true if the Left 2 button is pressed.
     */
    Q_INVOKABLE bool isLeft2ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Left 3 button is pressed.
     *
     * @return @c true if the Left 3 button is pressed.
     */
    Q_INVOKABLE bool isLeft3ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Right 1 button is pressed.
     *
     * @return @c true if the Right 1 button is pressed.
     */
    Q_INVOKABLE bool isRight1ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Right 2 button is pressed.
     *
     * @return @c true if the Right 2 button is pressed.
     */
    Q_INVOKABLE bool isRight2ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the Right 3 button is pressed.
     *
     * @return @c true if the Right 3 button is pressed.
     */
    Q_INVOKABLE bool isRight3ButtonDown() const;

    /*!
     * @brief Convenience function for checking if the DPad Up button is pressed.
     *
     * @return @c true if the DPad Up button is pressed.
     */
    Q_INVOKABLE bool isDPadUpButtonDown() const;

    /*!
     * @brief Convenience function for checking if the DPad Down button is pressed.
     *
     * @return @c true if the DPad Down button is pressed.
     */
    Q_INVOKABLE bool isDPadDownButtonDown() const;

    /*!
     * @brief Convenience function for checking if the DPad Left button is pressed.
     *
     * @return @c true if the DPad Left button is pressed.
     */
    Q_INVOKABLE bool isRDPadLeftButtonDown() const;

    /*!
     * @brief Convenience function for checking if the DPad Right button is pressed.
     *
     * @return @c true if the DPad Right button is pressed.
     */
    Q_INVOKABLE bool isRDPadRightButtonDown() const;

private:
    int  _controllerId;
    int  _type;
    //char _controllerIdString[64];
	int _buttonCount;
    int _analogCount;
    int _buttons;
    int _controllerAnalog0X;
    int _controllerAnalog0Y;
    int _controllerAnalog0Z;
    int _controllerAnalog1X;
    int _controllerAnalog1Y;
    int _controllerAnalog1Z;
    QObject* _target;

};

	}
}

#endif /* GAMEPADEVENT_HPP */

