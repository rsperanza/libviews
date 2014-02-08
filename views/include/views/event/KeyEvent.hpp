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


#ifndef KEYEVENT_HPP
#define KEYEVENT_HPP

#include <bps/screen.h>

#include <QtCore/QObject>

namespace views {
	namespace event {

class Q_DECL_EXPORT KeyEvent : public QObject {

private:
    Q_OBJECT

    /*!
     * @brief  @c key flags for the key event target.
     */
    Q_PROPERTY(int keyFlags READ keyFlags )
    /*!
     * @brief  @c key modifiers for the key event target.
     */
    Q_PROPERTY(int keyModifiers READ keyModifiers )
    /*!
     * @brief  @c key flags for the key event target.
     */
    Q_PROPERTY(int keySymbols READ keySymbols )
    /*!
     * @brief The target of the event.
     */
    Q_PROPERTY(QObject* target READ target )



public:
    KeyEvent();

    /*!
     * @brief Constructs a key event.
     *
     * @param keyFlags - key flags
     * @param keyModifiers - key symbols
     * @param keySymbols - key symbols
     * @param target - The key event target of this event.
     * @return A key event.
     */
    KeyEvent(int keyFlags, int keyModifiers, int keySymbols, QObject* target);

    /*!
     * @brief Destructs the mouse event.
     */
    virtual ~KeyEvent();

    /*!
     * @brief  @c key flags for the key event target.
     */
    int keyFlags() const;

    /*!
     * @brief  @c key modifiers for the key event target.
     */
    int keyModifiers() const;

    /*!
     * @brief  @c key symbols for the key event target.
     */
    int keySymbols() const;

    /*!
     * @brief  @c touch target.
     */
    QObject* target() const;

    /*!
     * @brief Convenience function for checking if a key is pressed.
     *
     * @return @c true if a key is pressed.
     */
    Q_INVOKABLE bool isDown() const;

    /*!
     * @brief Convenience function for checking if a key is repeated.
     *
     * @return @c true if a key is repeated.
     */
    Q_INVOKABLE bool isRepeat() const;

    /*!
     * @brief Convenience function for checking if a key cap is valid.
     *
     * @return @c true if a key a key cap is valid.
     */
    Q_INVOKABLE bool isValidCap() const;

    /*!
     * @brief Convenience function for checking if a key symbol is valid.
     *
     * @return @c true if a key a key symbol is valid.
     */
    Q_INVOKABLE bool isValidSymbol() const;

    /*!
     * @brief Convenience function for checking if the shift key is pressed.
     *
     * @return @c true if the shift key is pressed.
     */
    Q_INVOKABLE bool isShiftDown() const;

    /*!
     * @brief Convenience function for checking if the caps key is locked.
     *
     * @return @c true if the caps key is locked.
     */
    Q_INVOKABLE bool isCapsLocked() const;

    /*!
     * @brief Convenience function for checking if the ctrl key is pressed.
     *
     * @return @c true if the ctrl key is pressed.
     */
    Q_INVOKABLE bool isCtrlDown() const;

    /*!
     * @brief Convenience function for checking if the alt key is pressed.
     *
     * @return @c true if the alt key is pressed.
     */
    Q_INVOKABLE bool isAltDown() const;

    /*!
     * @brief Convenience function for checking if the num key is locked.
     *
     * @return @c true if the num key is locked.
     */
    Q_INVOKABLE bool isNumLocked() const;

    /*!
     * @brief Convenience function for checking if the scroll key is locked.
     *
     * @return @c true if the scroll key is locked.
     */
    Q_INVOKABLE bool isScrollLocked() const;

private:
    int   _keyFlags;
    int   _keyModifiers;
    int   _keySymbols;
    QObject* _target;

};

	}
}

#endif /* KEYEVENT_HPP */

