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


#ifndef MOUSEEVENT_HPP
#define MOUSEEVENT_HPP

#include <bps/screen.h>

#include <QtCore/QObject>

namespace views {
	namespace event {

class Q_DECL_EXPORT MouseEvent : public QObject {

private:
    Q_OBJECT

    /*!
     * @brief  @c x coordinate of the mouse event relative to the window.
     */
    Q_PROPERTY(float windowX READ windowX )
    /*!
     * @brief  @c y coordinate of the mouse event relative to the window.
     */
    Q_PROPERTY(float windowY READ windowY )
    /*!
     * @brief  @c x coordinate of the mouse event relative to the screen where the mouse event signal is emitted.
     */
    Q_PROPERTY(float screenY READ screenX )
    /*!
     * @brief  @c y coordinate of the mouse event relative to the screen where the mouse event signal is emitted.
     */
    Q_PROPERTY(float screenY READ screenY )
    /*!
     * @brief  @c buttons pressed for the mouse event target.
     */
    Q_PROPERTY(int buttons READ buttons )
    /*!
     * @brief The target of the event.
     */
    Q_PROPERTY(QObject* target READ target )



public:
    MouseEvent();

    /*!
     * @brief Constructs a mouse event.
     *
     * @param screenX - Touch @c x coordinate relative to the screen.
     * @param screenY - Touch @c x coordinate relative to the screen.
     * @param windowX - Touch @c x coordinate relative to the window.
     * @param windowY - Touch @c x coordinate relative to the window.
     * @param buttons - buttons pressed
     * @param target - The mouse event target of this event.
     * @return A mouse event.
     */
    MouseEvent(float screenX, float screenY, float windowX, float windowY, int buttons, QObject* target);
    /*!
     * @brief Destructs the mouse event.
     */
    virtual ~MouseEvent();

    /*!
     * @brief Retrieves the @c x coordinate of the mouse event relative to the screen.
     *
     * @return The @c x coordinate of the mouse event.
     */
    float screenX() const;

    /*!
     * @brief Retrieves the @c x coordinate of the mouse event relative to the screen.
     *
     * @return The @c y coordinate of the mouse event.
     */
    float screenY() const;

    /*!
     * @brief Retrieves the @c x coordinate of the mouse event relative to the window.
     *
     * @return The @c x coordinate of the mouse event.
     */
    float windowX() const;

    /*!
     * @brief Retrieves the @c x coordinate of the mouse event relative to the window.
     *
     * @return The @c y coordinate of the mouse event.
     */
    float windowY() const;

    /*!
     * @brief  @c buttons pressed for the mouse event target.
     */
    int buttons() const;

    /*!
     * @brief  @c touch target.
     */
    QObject* target() const;

    /*!
     * @brief Convenience function for checking if the left mouse button is pressed.
     *
     * @return @c true if the left mouse button is pressed.
     */
    Q_INVOKABLE bool isLeftButtonDown() const;

    /*!
     * @brief Convenience function for checking if the right mouse button is pressed.
     *
     * @return @c true if the middle mouse button is pressed.
     */
    Q_INVOKABLE bool isMiddleButtonDown() const;

    /*!
     * @brief Convenience function for checking if the right mouse button is pressed.
     *
     * @return @c true if the right mouse button is pressed.
     */
    Q_INVOKABLE bool isRightButtonDown() const;

private:
    float _screenX;
    float _screenY;
    float _windowX;
    float _windowY;
    int   _buttons;
    QObject* _target;

};

	}
}

#endif /* MOUSEEVENT_HPP */

