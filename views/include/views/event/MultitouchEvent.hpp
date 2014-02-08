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


#ifndef MULTITOUCHEVENT_HPP
#define MULTITOUCHEVENT_HPP

#include <bps/screen.h>

#include <QtCore/QObject>

namespace views {
	namespace event {

class Q_DECL_EXPORT MultitouchEvent : public QObject {

private:
    Q_OBJECT

    /*!
     * @brief  @c Id of touch target.
     */
    Q_PROPERTY(int Id READ Id )
    /*!
     * @brief The type of the event.
     */
    Q_PROPERTY(int type READ type )
    /*!
     * @brief  @c x coordinate of the touch relative to the window.
     */
    Q_PROPERTY(float windowX READ windowX )
    /*!
     * @brief  @c y coordinate of the touch relative to the window.
     */
    Q_PROPERTY(float windowY READ windowY )
    /*!
     * @brief  @c x coordinate of the touch relative to the screen where the touch signal is emitted.
     */
    Q_PROPERTY(float screenY READ screenX )
    /*!
     * @brief  @c y coordinate of the touch relative to the screen where the touch signal is emitted.
     */
    Q_PROPERTY(float screenY READ screenY )
    /*!
     * @brief  @c pressure of the touch target.
     */
    Q_PROPERTY(int pressure READ pressure )
    /*!
     * @brief The target of the event.
     */
    Q_PROPERTY(QObject* target READ target )



public:
    MultitouchEvent();

    /*!
     * @brief Constructs a touch event.
     *
     * @param type - The type of touch.
     * @param screenX - Touch @c x coordinate relative to the screen.
     * @param screenY - Touch @c x coordinate relative to the screen.
     * @param windowX - Touch @c x coordinate relative to the window.
     * @param windowY - Touch @c x coordinate relative to the window.
     * @param id - Id of touch target.
     * @param pressure - pressure of touch target
     * @param target - The touch target of this event.
     * @return A multitouch event.
     */
    MultitouchEvent(int type, float screenX, float screenY, float windowX, float windowY, int id, int pressure, QObject* target);
    /*!
     * @brief Destructs the touch event.
     */
    virtual ~MultitouchEvent();

    /*!
     * @brief Retrieves the @c x coordinate of the touch relative to the screen.
     *
     * @return The @c x coordinate of the touch.
     */
    float screenX() const;

    /*!
     * @brief Retrieves the @c x coordinate of the touch relative to the screen.
     *
     * @return The @c y coordinate of the touch.
     */
    float screenY() const;

    /*!
     * @brief Retrieves the @c x coordinate of the touch relative to the window.
     *
     * @return The @c x coordinate of the touch.
     */
    float windowX() const;

    /*!
     * @brief Retrieves the @c x coordinate of the touch relative to the window.
     *
     * @return The @c y coordinate of the touch.
     */
    float windowY() const;

    /*!
     * @brief  @c pressure of the touch target.
     */
    int pressure() const;

    /*!
     * @brief  @c Id of touch target.
     */
    int Id() const;

    /*!
     * @brief The type of the event (SCREEN_EVENT_MTOUCH_TOUCH, SCREEN_EVENT_MTOUCH_MOVE, or SCREEN_EVENT_MTOUCH_RELEASE).
     */
    int type() const;

    /*!
     * @brief  @c touch target.
     */
    QObject* target() const;

    /*!
     * @brief Convenience function for checking if the touch type returned from type() is SCREEN_EVENT_MTOUCH_MOVE.
     *
     * @return @c true if the touch type returned from type() is SCREEN_EVENT_MTOUCH_MOVE.
     */
    Q_INVOKABLE bool isMove() const;

    /*!
     * @brief Convenience function for checking if the touch type returned from type() is SCREEN_EVENT_MTOUCH_RELEASE.
     *
     * @return @c true if the touch type returned from type() is SCREEN_EVENT_MTOUCH_RELEASE.
     */
    Q_INVOKABLE bool isUp() const;

    /*!
     * @brief Convenience function for checking if the touch type returned from type() is SCREEN_EVENT_MTOUCH_TOUCH.
     *
     * @return @c true if the touch type returned from type() is SCREEN_EVENT_MTOUCH_TOUCH.
     */
    Q_INVOKABLE bool isDown() const;

private:
    int   _touchId;
    int _type;
    float _screenX;
    float _screenY;
    float _windowX;
    float _windowY;
    float _pressure;
    QObject* _target;

};

	}
}

#endif /* MULTITOUCHEVENT_HPP */

