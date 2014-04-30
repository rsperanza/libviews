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


#ifndef VIEW_HPP
#define VIEW_HPP

#include <stdlib.h>
#include <sys/platform.h>
#include <sys/keycodes.h>

#include <bps/virtualkeyboard.h>
//#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>

#include <pthread.h>

#include <screen/screen.h>

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QVariant>

#include "../event/MultitouchEvent.hpp"
#include "../event/MouseEvent.hpp"
#include "../event/KeyEvent.hpp"
#include "../event/GamepadEvent.hpp"
#include "../graphics/Graphics.hpp"

namespace views {
	namespace native {
		class NativeWindow;
	}
}

#define MAX_CONTROLLERS 2

using namespace views::event;
using namespace views::graphics;
using namespace views::native;

namespace views {
	namespace base {

typedef enum ViewDisplay {DISPLAY_UNKNOWN = 0, DISPLAY_DEVICE = 1, DISPLAY_HDMI = 4} ViewDisplay;

class Q_DECL_EXPORT View : public QObject {

	Q_OBJECT

	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
	Q_PROPERTY(bool visible READ visible WRITE setVisible)
	Q_PROPERTY(bool altered READ altered WRITE setAltered)
	Q_PROPERTY(bool initialized READ initialized)

	Q_PROPERTY(int  display  READ display WRITE setDisplay)
	Q_PROPERTY(int  angle    READ angle   WRITE setAngle)
	Q_PROPERTY(int  x        READ x       WRITE setX)
	Q_PROPERTY(int  y        READ y       WRITE setY)
	Q_PROPERTY(int  z        READ z       WRITE setZ)
	Q_PROPERTY(int  width    READ width   WRITE setWidth)
	Q_PROPERTY(int  height   READ height  WRITE setHeight)
	Q_PROPERTY(int  transparency READ transparency WRITE setTransparency)
    Q_PROPERTY(int  bufferWidth    READ bufferWidth   WRITE setBufferWidth)
    Q_PROPERTY(int  bufferHeight   READ bufferHeight  WRITE setBufferHeight)
    Q_PROPERTY(int  format   READ format  WRITE setFormat)
    Q_PROPERTY(int  sourceX        READ sourceX       WRITE setSourceX)
    Q_PROPERTY(int  sourceY        READ sourceY       WRITE setSourceY)
    Q_PROPERTY(int  sourceWidth    READ sourceWidth   WRITE setSourceWidth)
    Q_PROPERTY(int  sourceHeight   READ sourceHeight  WRITE setSourceHeight)
	Q_PROPERTY(QString  windowID READ windowID WRITE setWindowID)
	Q_PROPERTY(QString  windowGroup READ windowGroup WRITE setWindowGroup)
	Q_PROPERTY(bool  createWindowGroup READ createWindowGroup WRITE setCreateWindowGroup)
	Q_PROPERTY(bool  createFullWindow READ createFullWindow WRITE setCreateFullWindow)

	friend class ViewsThread;

public:
	View(ViewDisplay display = DISPLAY_DEVICE);

	virtual ~View();

	// must be defined in derived classes
	virtual int initialize();

	// state functions
	// enable is used to enable / disable rendering
	bool enabled();

	virtual bool visible();
	virtual void onVisible();

	// flags initialization of the view
	bool initialized();

	// stale flag indicates when the view needs to be rendered again
	bool stale();

	// this flag signals that the view was altered (rotated or resized) and needs to be regenerated
	bool altered();

	int display();
	int angle();
	int x();
	int y();
	int z();
    int width();
    int height();
    int transparency();
    int bufferWidth();
    int bufferHeight();
    int format();
    int sourceX();
    int sourceY();
    int sourceWidth();
    int sourceHeight();
	QString windowID();
	QString windowGroup();
	bool createWindowGroup();
	bool createFullWindow();

	// custom update handling to be defined in derived class
	virtual void update();

	// main render function - obtains GL context and swaps buffer so that the derived class doesn't need to handle it
	void renderView();


	// called by the thread to regenerate the surface
	virtual int regenerate();

	// custom cleanup to be implemented by derived class
	virtual void cleanup();

	// methods to add or remove this view - hide thread functions from caller
	void add();
	void remove();


	void setPosition(int x, int y);
	void setSize(int width, int height);

	// register Graphics class for rendering
	void registerGraphics(Graphics *renderGraphics);


Q_SIGNALS:
	void multitouch(MultitouchEvent *event);
	void mouse     (MouseEvent *event);
	void key       (KeyEvent *event);
	void gamepad   (GamepadEvent *event);

	void regenerated();
	void updated();

public Q_SLOTS:
	// state slots
	void setEnabled(bool enabled);
	virtual void setVisible(bool visible);
	void setInitialized(bool initialized);
	void setStale(bool stale);
	void setAltered(bool stale);
	void setCapture(bool capture);


	// display methods
	void setDisplay(ViewDisplay display);

	// view settings
	void setDisplay(int display);
	void setAngle(int angle);
	void setX(int x);
	void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setZ(int z);
    void setTransparency(int transparency);
    void setBufferWidth(int bufferWidth);
    void setBufferHeight(int bufferHeight);
    void setSourceX(int sourceX);
    void setSourceY(int sourceY);
    void setSourceWidth(int sourceWidth);
    void setSourceHeight(int sourceHeight);
    void setFormat(int format);

	void setWindowID(const QString id);
	void setWindowGroup(const QString &group);
	void setCreateFullWindow(bool create);
	void setCreateWindowGroup(bool create);


	virtual void onRegenerated() {};
	ImageData* getRenderedImage();


    // TODO move to a private class later
    screen_window_t getScreenWindow();
    screen_buffer_t screenBuffer(int index);
    void copyBufferFrom(uint8_t** incomingBuffers, int incomingWidth, int incomingHeight, uint32_t* stride);

protected:
	virtual void handleScreenEvent(bps_event_t *event);

	int calculateDPI();

	Graphics* _renderGraphics;

	// state
	int  _renderCount;
	bool _enabled;
	bool _initialized;
	bool _altered;
	bool _visible;
	bool _stale;
	ViewDisplay _display;

	// capture state
	bool _capture;

	// view properties
	int _angle;
	int _x;
	int _y;
	int _z;
    int _width;
    int _height;
    int _interval;
    int _transparency;
    int _format;
    int _bufferWidth;
    int _bufferHeight;

    int _sourceX;
    int _sourceY;
    int _sourceWidth;
    int _sourceHeight;

    // probably move to NativeWindow
    screen_pixmap_t _screenPixmap;
    screen_buffer_t _screenPixmapBuffer;
    unsigned char* _screenPixmapBufferPtr;
    int _pixmapStride;

	// window group / ID
	bool _createFullWindow;
	bool _createGroup;
	QString _group;
	QString _id;

	screen_window_t _screenWindow;

    // main mutex for controlling view access
    QMutex _viewMutex;

    // main mutex for controlling view access
    QMutex _copyMutex;

private:
	void loadController(int index);
	void discoverControllers();

	int _controllerCount;
	screen_device_t _controller[MAX_CONTROLLERS];
	int _controllerType[MAX_CONTROLLERS];
    char _controllerID[MAX_CONTROLLERS][64];
	int _controllerButtonCount[MAX_CONTROLLERS];
    int _controllerAnalogCount[MAX_CONTROLLERS];
    int _controllerButtons[MAX_CONTROLLERS];
    int _controllerAnalog0X[MAX_CONTROLLERS];
    int _controllerAnalog0Y[MAX_CONTROLLERS];
    int _controllerAnalog0Z[MAX_CONTROLLERS];
    int _controllerAnalog1X[MAX_CONTROLLERS];
    int _controllerAnalog1Y[MAX_CONTROLLERS];
    int _controllerAnalog1Z[MAX_CONTROLLERS];

	NativeWindow* _nativeWindow;
};



	} /* end namespace base */

} /* end namespace views */

#endif /* VIEW_HPP */

