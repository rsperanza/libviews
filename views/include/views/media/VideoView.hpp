/*
 * Copyright (c) 2011-2012 Research In Motion Limited.
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


#ifndef VIDEOVIEW_HPP
#define VIDEOVIEW_HPP

#include <assert.h>
#include <screen/screen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <mm/renderer.h>
#include <stdlib.h>

#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <bb/cascades/TouchEvent>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "../base/View.hpp"
#include "../graphics/Graphics2D.hpp"

using namespace views::base;

namespace views {

	namespace media {

class Q_DECL_EXPORT VideoView : public View {

Q_OBJECT

Q_PROPERTY(QString mediaURL     READ mediaURL     WRITE setMediaURL)
Q_PROPERTY(bool playing      READ playing)
Q_PROPERTY(int leftPadding   READ leftPadding   WRITE setLeftPadding)
Q_PROPERTY(int rightPadding  READ rightPadding  WRITE setRightPadding)
Q_PROPERTY(int topPadding    READ topPadding    WRITE setTopPadding)
Q_PROPERTY(int bottomPadding READ bottomPadding WRITE setBottomPadding)

public:
	VideoView(ViewDisplay display = DISPLAY_DEVICE);

	virtual ~VideoView();

	QString mediaURL();

	bool playing();

	int leftPadding();
	int rightPadding();
	int topPadding();
	int bottomPadding();

	// overriden methods from OpenGLView
	int initialize();
	void update();
	void cleanup();

	virtual void onAltered();
	virtual void onVisible();

public Q_SLOTS:
	void setMediaURL(QString mediaURL);
	void setMediaSize(int width, int height);

	void setLeftPadding(int leftPadding);
	void setRightPadding(int rightPadding);
	void setTopPadding(int topPadding);
	void setBottomPadding(int bottomPadding);

	// control the media
	int seek(int position);
	int play();
	int stop();

protected:
	void mmrInitialize();
	void mmrCleanup();

	strm_dict_t* calculateRect(int x, int y, int width, int height);
	virtual void handleScreenEvent(bps_event_t *event);

    QString _backupGroup;

    // Renderer variables
    mmr_connection_t* _mmrConnection;
    mmr_context_t*    _mmrContext;

    // I/O variables
    int  _videoDeviceOutputID;
    int  _audioDeviceOutputID;

    QString videoDeviceUrl;
    QString audioDeviceUrl;

    // Name of video context
    QString videoContextName;

    int _mediaWidth;
    int _mediaHeight;
    QUrl _mediaURL;

    int _leftPadding;
    int _rightPadding;
    int _topPadding;
    int _bottomPadding;

    bool _playing;

	screen_window_t _screenVideoWindow;

	Graphics2D* _graphics2D;
};

	} /* end namespace media */

} /* end namespace views */

#endif /* VIDEOVIEW_HPP */

