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

#include "KeyEvent.hpp"
#include <math.h>
#include <sys/keycodes.h>

#include <QDebug>

namespace views {
	namespace event {

KeyEvent::KeyEvent()
{
}

/*!
 * @brief Constructs a key event.
 *
 * @param keyFlags - key flags
 * @param keyModifiers - key symbols
 * @param keySymbols - key symbols
 * @param target - The key event target of this event.
 * @return A mouse event.
 */
KeyEvent::KeyEvent(int keyFlags, int keyModifiers, int keySymbols, QObject* target)
{
    _keyFlags     = keyFlags;
    _keyModifiers = keyModifiers;
    _keySymbols   = keySymbols;
    _target       = target;
}

/*!
 * @brief Destructs the mouse event.
 */
KeyEvent::~KeyEvent() {
	// TODO Auto-generated destructor stub
}

/*!
 * @brief  @c key flags for the key event target.
 */
int KeyEvent::keyFlags() const
{
	return _keyFlags;
}

/*!
 * @brief  @c key modifiers for the key event target.
 */
int KeyEvent::keyModifiers() const
{
	return _keyModifiers;
}

/*!
 * @brief  @c key symbols for the key event target.
 */
int KeyEvent::keySymbols() const
{
	return _keySymbols;
}

/*!
 * @brief  @c touch target.
 */
QObject* KeyEvent::target() const
{
	return _target;
}

/*!
 * @brief Convenience function for checking if a key is pressed.
 *
 * @return @c true if a key is pressed.
 */
bool KeyEvent::isDown() const
{
	return (_keyFlags & KEY_DOWN);
}

/*!
 * @brief Convenience function for checking if a key is repeated.
 *
 * @return @c true if a key is repeated.
 */
bool KeyEvent::isRepeat() const
{
	return (_keyFlags & KEY_REPEAT);
}

/*!
 * @brief Convenience function for checking if a key cap is valid.
 *
 * @return @c true if a key a key cap is valid.
 */
bool KeyEvent::isValidCap() const
{
	return (_keyFlags & KEY_CAP_VALID);
}

/*!
 * @brief Convenience function for checking if a key symbol is valid.
 *
 * @return @c true if a key a key symbol is valid.
 */
bool KeyEvent::isValidSymbol() const
{
	return (_keyFlags & KEY_SYM_VALID);
}


/*!
 * @brief Convenience function for checking if the shift key is pressed.
 *
 * @return @c true if the shift key is pressed.
 */
bool KeyEvent::isShiftDown() const
{
	return (_keyModifiers & KEYMOD_SHIFT);
}

/*!
 * @brief Convenience function for checking if the caps key is locked.
 *
 * @return @c true if the caps key is locked.
 */
bool KeyEvent::isCapsLocked() const
{
	return (_keyModifiers & KEYMOD_CAPS_LOCK);
}

/*!
 * @brief Convenience function for checking if the ctrl key is pressed.
 *
 * @return @c true if the ctrl key is pressed.
 */
bool KeyEvent::isCtrlDown() const
{
	return (_keyModifiers & KEYMOD_CTRL);
}

/*!
 * @brief Convenience function for checking if the alt key is pressed.
 *
 * @return @c true if the alt key is pressed.
 */
bool KeyEvent::isAltDown() const
{
	return (_keyModifiers & KEYMOD_ALT);
}

/*!
 * @brief Convenience function for checking if the num key is locked.
 *
 * @return @c true if the num key is locked.
 */
bool KeyEvent::isNumLocked() const
{
	return (_keyModifiers & KEYMOD_NUM_LOCK);
}

/*!
 * @brief Convenience function for checking if the scroll key is locked.
 *
 * @return @c true if the scroll key is locked.
 */
bool KeyEvent::isScrollLocked() const
{
	return (_keyModifiers & KEYMOD_SCROLL_LOCK);
}

	}
}
