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

#ifndef VIEWSTHREAD_HPP
#define VIEWSTHREAD_HPP

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <QtCore/QThread>
#include <QtCore/QList>
#include <QtCore/QString>

#include "View.hpp"
#include "NativeWindow.hpp"

namespace views {

	namespace base {

class ViewsThread :public QThread {

Q_OBJECT

public:
	void run();

	static ViewsThread *getInstance();

	// signal the thread to shut down
	void shutdown();

	// add / remove views
	void addView(View *view);
	void removeView(View *view);

	// control the initialized state
	bool initialized();
	void setInitialized(bool initialized);

	// control the stopped state
	bool stopped();
	void setStopped(bool initialized);

protected:
	ViewsThread();
	virtual ~ViewsThread();

	// control the EGL initialized state
	bool eglInitialized();
	//void setEGLInitialized(bool initialized);


private:
	// initialize / cleanup functions
	int initBPS();
	void cleanup();


	void update();
	void render();

	// screens / windows
	screen_context_t       _screenContext;

	float _screenWidth, _screenHeight;

	QVector<View*> _views;

	// state fields
	bool _initialized;
	bool _stopped;

	// mutexes to control thread access
	QMutex _viewsMutex;
	QMutex _threadMutex;

	static ViewsThread singleton;
};

	} /* end namespace base */

} /* end namespace views */

#endif /* VIEWSTHREAD_HPP */

