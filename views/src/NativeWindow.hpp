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


#ifndef NATIVEWINDOW_HPP
#define NATIVEWINDOW_HPP

#include <stdlib.h>
#include <sys/platform.h>

#include <pthread.h>

#include <screen/screen.h>

#include "View.hpp"
#include "NativeWindow.hpp"

using namespace bb;
using namespace views::base;

namespace views {
	namespace native {

class Q_DECL_EXPORT NativeWindow {

	//friend class ViewsThread;

public:
	NativeWindow(ViewDisplay display);

	virtual ~NativeWindow();

	int initialize(bool _createFullWindow);

	// API / screen property calls
	screen_window_t getScreenWindow();
	//void setScreenContext(screen_context_t screen_ctx);

	// view settings
	void setAngle(int angle);
	void setPosition(int x, int y);
    void setSize(int width, int height);
    void setSourcePosition(int x, int y);
    void setSourceSize(int sourceWidth, int sourceHeight);
    void setBufferSize(int bufferWidth, int bufferHeight);
    void setFormat(int format);
	void setZ(int z);
	void setTransparency(int transparency);

	void setWindowGroup(const QString &group);
	void setCreateWindowGroup(bool create);
	void setWindowID(const QString id);

	screen_buffer_t screenBuffer(int index);

	/**
	 * @return dpi for current screen
	 */
	int calculateDPI();

	static int screenDisplayAttached(int *size);
	static int screenDisplayAttached(bps_event_t *event, int *size);
	static screen_display_t* getAttachedDisplay();
	static int updateScreen();
	static screen_context_t getScreenContext();

protected:
	// window property calls
	int setWindowAngle(int angle);
	int setWindowPosition(int x, int y);
	int setWindowSize(int width, int height);
	int setWindowZ(int z);
	int setWindowBufferSize(int width, int height);
    int setWindowSourcePosition(int x, int y);
    int setWindowSourceSize(int width, int height);
    int setWindowTransparency(int transparency);
    int setWindowFormat(int format);
	int setWindowUsage(int usage);

	// window group / ID calls
	int joinWindowGroup(const QString &group);
	int setScreenWindowID(const QString id);

	ViewDisplay _display;
	int _angle;
	int _x;
	int _y;
	int _z;
	int _width;
	int _height;
	int _interval;
	int _transparency;
	int _nbuffers;
    int _bufferWidth;
    int _bufferHeight;
    int _sourceX;
    int _sourceY;
    int _sourceWidth;
    int _sourceHeight;
    int _format;
    int _usage;

	// window group / ID
	bool _createGroup;
	QString _group;
	QString _id;

	// screens / windows
    screen_buffer_t _screenBuffers[2];

	screen_window_t        _screenWindow;
	screen_display_t       _screenDisplay;

    static int                    _numberDisplays;
	static screen_display_t*      _screenDisplays;
	static screen_context_t       _screenContext;

	// mutex for controlling render access across all views
	static QMutex _displaysMutex;

};


	} /* end namespace base */

} /* end namespace views */

#endif /* NATIVEWINDOW_HPP */

