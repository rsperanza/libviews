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

#include <math.h>

#include <QDebug>

#include "ViewsThread.hpp"

using namespace bb::cascades;

using namespace views::base;

namespace views {
	namespace base {


ViewsThread ViewsThread::singleton;

ViewsThread::ViewsThread()
{
//	qDebug()  << "ViewsThread::ViewsThread: ";

    _initialized = false;
    _stopped = false;
}

ViewsThread::~ViewsThread() {
}

bool ViewsThread::initialized()
{
	bool initialized = false;

	_threadMutex.lock();

	initialized = _initialized;

	_threadMutex.unlock();

	return initialized;
}

void ViewsThread::setInitialized(bool initialized)
{
	_threadMutex.lock();

	_initialized = initialized;

	_threadMutex.unlock();
}

bool ViewsThread::stopped()
{
	bool stopped = false;

	_threadMutex.lock();

	stopped = _stopped;

	_threadMutex.unlock();

	return stopped;
}

void ViewsThread::setStopped(bool stopped)
{
	_threadMutex.lock();

	_stopped = stopped;

	_threadMutex.unlock();
}


void ViewsThread::addView(View *view) {
	qDebug()  << "ViewsThread::addView: ";

	if (view != NULL) {
		_viewsMutex.lock();

		_views.append(view);

		_viewsMutex.unlock();

		while (!view->initialized() && view->visible()) {
			usleep(1);
		}
	}
}

void ViewsThread::removeView(View *view) {
	qDebug()  << "ViewsThread::removeView: ";

	if (view != NULL) {
		_viewsMutex.lock();

		_views.remove(_views.indexOf(view));

		_viewsMutex.unlock();

		view->cleanup();
	}
}

int ViewsThread::initBPS() {
	qDebug()  << "ViewsThread::initBPS";

	//Initialize BPS library
	bps_initialize();

	_screenContext = NativeWindow::getScreenContext();

	// initialize EGL
	if (EXIT_SUCCESS != Graphics::initializeEGL()) {
		fprintf(stderr, "initialize EGL failed\n");
		return EXIT_FAILURE;
	}

	//Signal BPS library that screen events will be requested
	if (BPS_SUCCESS != screen_request_events(_screenContext)) {
		fprintf(stderr, "screen_request_events failed\n");
		return EXIT_FAILURE;
	}

	if (BPS_SUCCESS != virtualkeyboard_request_events(0)) {
		fprintf(stderr, "virtualkeyboard_request_events failed\n");
		return EXIT_FAILURE;
	}

	setInitialized(true);

	return EXIT_SUCCESS;
}

void ViewsThread::update() {
	//qDebug()  << "ViewsThread::update";
	int index;
	int viewCount = 0;
	int returnCode = 0;

	_viewsMutex.lock();

	viewCount = _views.size();

	_viewsMutex.unlock();

	if (viewCount > 0) {
		int size[2];

		_viewsMutex.lock();

		int attached = NativeWindow::screenDisplayAttached(size);

		for(index = 0; index < _views.size(); index++) {
			//qDebug()  << "ViewsThread::update: check attached: " << attached << ":" << index << ":" << _views.at(index)->display();
			if (_views.at(index)->display() == DISPLAY_HDMI) {
				if (attached) {
					if (_views.at(index)->enabled()) {
						if (!_views.at(index)->visible()) {
							_views.at(index)->setVisible(true);
							_views.at(index)->setStale(true);
						}
					} else {
						if (_views.at(index)->visible()) {
							_views.at(index)->setVisible(false);
						}
					}
				} else {
					if (_views.at(index)->visible()) {
						_views.at(index)->setVisible(false);
					}
				}
			} else {
				if (_views.at(index)->enabled()) {
					if (!_views.at(index)->visible()) {
						_views.at(index)->setVisible(true);
						_views.at(index)->setStale(true);
					}
				} else {
					if (_views.at(index)->visible()) {
						_views.at(index)->setVisible(false);
					}
				}
			}
			//qDebug()  << "ViewsThread::update: check attached: " << attached << ":" << index << ":" << _views.at(index)->display() << ":" << _views.at(index)->visible();
		}

		int updateCount = 0;
		for (index = 0; index < _views.size(); index++) {
			if (!_views.at(index)->initialized() && _views.at(index)->visible()) {
				qDebug()  << "ViewsThread::update: initialize: " << index << ":" << _views.at(index)->display() << ":" << _views.at(index)->visible() << ":" << returnCode;
				returnCode = _views.at(index)->initialize();
				if (returnCode == EXIT_FAILURE) {
					qDebug()  << "ViewsThread::update: view initialization failed: " << index;
					_views.at(index)->setVisible(false);
					_views.at(index)->setStale(false);
					setStopped(true);
				}
			}

			if (_views.at(index)->altered() && _views.at(index)->visible()) {
				returnCode = _views.at(index)->regenerate();
				if (returnCode == EXIT_FAILURE) {
					qDebug()  << "ViewsThread::update: view regenerate failed: " << index;
					_views.at(index)->setVisible(false);
					_views.at(index)->setStale(false);
				}
				//qDebug()  << "ViewsThread::update: regenerate: " << index << ":" << _views.at(index)->display() << ":" << _views.at(index)->altered() << ":" << _views.at(index)->visible();
			}

			if (_views.at(index)->initialized() && _views.at(index)->enabled() && _views.at(index)->visible()) {
				_views.at(index)->update();
				updateCount++;
			}
		}
		if (updateCount) {
			for (index = 0; index < _views.size(); index++) {
				if (_views.at(index)->initialized() && !_views.at(index)->enabled()) {
					_views.at(index)->update();
				}
			}
		}

		_viewsMutex.unlock();
	}
}

void ViewsThread::cleanup() {
	qDebug()  << "ViewsThread::cleanup";

	int viewCount = 0;

	_viewsMutex.lock();

	viewCount = _views.size();

	_viewsMutex.unlock();

	if (viewCount > 0) {
		do {
			removeView(_views.at(_views.size()-1));

			_viewsMutex.lock();

			viewCount = _views.size();

			_viewsMutex.unlock();

		} while (viewCount > 0);
	}
}

void ViewsThread::render() {
	//qDebug()  << "ViewsThread::render: ";

	int index;
	int viewCount = 0;

	_viewsMutex.lock();

	viewCount = _views.size();

	_viewsMutex.unlock();

	if (viewCount > 0) {
		int size[2];

		_viewsMutex.lock();

		int attached = NativeWindow::screenDisplayAttached(size);

		for(index = 0; index < _views.size(); index++) {
			if (_views.at(index)->display() == DISPLAY_HDMI) {
				if (attached) {
					if (_views.at(index)->enabled()) {
						if (!_views.at(index)->visible()) {
							_views.at(index)->setVisible(true);
							_views.at(index)->setStale(true);
						}
					} else {
						if (_views.at(index)->visible()) {
							_views.at(index)->setVisible(false);
						}
					}
				} else {
					if (_views.at(index)->visible()) {
						_views.at(index)->setVisible(false);
					}
				}
			} else {
				if (_views.at(index)->enabled()) {
					if (!_views.at(index)->visible()) {
						_views.at(index)->setVisible(true);
						_views.at(index)->setStale(true);
					}
				} else {
					if (_views.at(index)->visible()) {
						_views.at(index)->setVisible(false);
					}
				}
			}
		}

		for (int index = 0; index < _views.size(); index++) {
			_views.at(index)->renderView();
		}

		_viewsMutex.unlock();
	}
}


void ViewsThread::run()
{
	qDebug()  << "ViewsThread::run: ";

	int index = 0;

	setStopped(false);

	qDebug()  << "ViewsThread started: " << !stopped();

	int returnCode = initBPS();

	if (returnCode == EXIT_SUCCESS && initialized() && Graphics::eglInitialized()) {
		bps_event_t *event = NULL;

		while (!stopped()) {
			do {
				//Request and process BPS next available event
				event = NULL;
				returnCode = bps_get_event(&event, 0);
				//assert(rc == BPS_SUCCESS);

				if (event) {
					int domain = bps_event_get_domain(event);

					if (domain == screen_get_domain()) {
						int size[2];

						int attached = NativeWindow::screenDisplayAttached(event, size);
						if (attached >= 0) {
							int viewCount = 0;

							_viewsMutex.lock();

							viewCount = _views.size();

							_viewsMutex.unlock();

							if (viewCount > 0) {
								_viewsMutex.lock();

								for(index = 0; index < _views.size(); index++) {
									if (_views.at(index)->display() == DISPLAY_HDMI) {
										if (attached) {
											_views.at(index)->setVisible(true);
										} else {
											_views.at(index)->setVisible(false);
										}
									}
								}

								_viewsMutex.unlock();
							}
						}

						int viewCount = 0;

						_viewsMutex.lock();

						viewCount = _views.size();

						_viewsMutex.unlock();

						if (viewCount > 0) {
							_viewsMutex.lock();

							for(index = 0; index < _views.size(); index++) {
								_views.at(index)->handleScreenEvent(event);
							}

							_viewsMutex.unlock();
						}
					}
				}
			} while (event);

			if (stopped()) {
				break;
			}

			//qDebug()  << "ViewsThread stopped: " << stopped();

			update();

			render();

			usleep(5);
		}

		// remove and cleanup each view
		cleanup();
	}

	//Stop requesting events from libscreen
	screen_stop_events(_screenContext);

	//Shut down BPS library for this process
	bps_shutdown();

	setInitialized(false);

	//Use utility code to terminate EGL setup
	Graphics::cleanupEGL();

	//Destroy screen context
	screen_destroy_context(_screenContext);
}

void ViewsThread::shutdown()
{
	qDebug()  << "ViewsThread::shutdown: ";

	setStopped(true);

	while (singleton.isRunning()) {
		usleep(50);
	};
}

ViewsThread* ViewsThread::getInstance()
{
	qDebug()  << "ViewsThread::getInstance: ";

	if (!singleton.isRunning() && !singleton.stopped()) {
		singleton.start();
	}

	while (!Graphics::eglInitialized()) {
		usleep(1);
	};

	return &singleton;
}


	} /* end namespace base */
} /* end namespace views */

