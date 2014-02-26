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

#include <screen/screen.h>

#include "NativeWindow.hpp"
#include "VideoView.hpp"

#include <QDebug>

using namespace bb::cascades;

namespace views {
	namespace media {


VideoView::VideoView(ViewDisplay display) : View(display), _mediaURL(), _graphics2D(NULL), _backupGroup(QString(" "))
{
	_mmrConnection = 0;
	_mmrContext = 0;
	_screenVideoWindow = 0;
	_playing = false;

    _leftPadding = 0;
    _rightPadding = 0;
    _topPadding = 0;
    _bottomPadding = 0;

	// I/O variables
	_videoDeviceOutputID = -1;
	_audioDeviceOutputID = -1;

	_transparency = SCREEN_TRANSPARENCY_SOURCE_OVER;

	_graphics2D = new Graphics2D(display);
	registerGraphics(_graphics2D);
}


VideoView::~VideoView() {
	// TODO Auto-generated destructor stub
}

int VideoView::initialize() {
	qDebug()  << "VideoView::initialize";

	if (_display == DISPLAY_DEVICE) {
		_backupGroup = _group;
		_group = QString("tmpvideo000").append(QString::number((int)this));
	}

	int returnCode = View::initialize();
	if (returnCode == EXIT_SUCCESS) {

		// reset stale flag for initialization only
		setStale(true);
	}

	qDebug()  << "VideoView::initialize " << ":" << returnCode;

    return returnCode;
}

void VideoView::onVisible()
{
	qDebug()  << "VideoView::onVisible";
}

void VideoView::update()
{
}

void VideoView::cleanup()
{
	qDebug()  << "VideoView::cleanup";

    mmrCleanup();

	View::cleanup();
}

void VideoView::mmrInitialize()
{
	qDebug()  << "VideoView::mmrInitialize";

   /*
     * Configure mm-renderer.
     */
    _mmrConnection = mmr_connect(NULL);
    if (_mmrConnection) {

        // Name of video context
        QString videoContextName = _id + "Context";

        _mmrContext = mmr_context_create(_mmrConnection, videoContextName.toAscii().constData(), 0, S_IRWXU|S_IRWXG|S_IRWXO);
        if (_mmrContext) {
        	// I/O devices
        	QString videoDeviceUrl = "screen:?nodstviewport=1&winid=";
        	//QString videoDeviceUrl = "screen:?winid=";
        	videoDeviceUrl += _id;
        	videoDeviceUrl += "&wingrp=";
        	videoDeviceUrl += _group;
        	QString audioDeviceUrl = "audio:default";

        	qDebug()  << "VideoView::mmrInitialize: _mediaURL: " << _mediaURL.toString().toAscii().constData();
        	qDebug()  << "VideoView::mmrInitialize: videoDeviceUrl: " << videoDeviceUrl.toAscii().constData();
        	qDebug()  << "VideoView::mmrInitialize: audioDeviceUrl: " << audioDeviceUrl.toAscii().constData();

        	/*
        	 * Configure video and audio output.
        	 */
        	_videoDeviceOutputID = mmr_output_attach(_mmrContext, videoDeviceUrl.toAscii().constData(), "video");
        	if (_videoDeviceOutputID == -1) {
        		qCritical()  << "VideoView::mmrInitialize: mmr_output_attach video failed.";
        	}

        	_audioDeviceOutputID = mmr_output_attach(_mmrContext, audioDeviceUrl.toAscii().constData(), "audio");
        	if (_audioDeviceOutputID == -1) {
        		qCritical()  << "VideoView::mmrInitialize: mmr_output_attach audio failed.";
        	}

        	/*
        	 * Attach the media as input.
        	 */
        	if (mmr_input_attach(_mmrContext, _mediaURL.toString().toAscii().constData(), "track") != 0) {
        		qCritical()  << "VideoView::mmrInitialize: mmr_input_attach track failed.";
        	}
        }
    }
}

void VideoView::mmrCleanup()
{
	qDebug()  << "VideoView::mmrCleanup";

	if (_playing) {
		if (mmr_stop(_mmrContext) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_stop failed.";
		}
	}

	if (_mmrContext) {
		if (mmr_input_detach(_mmrContext) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_input_detach failed.";
		}
    }

	if (_audioDeviceOutputID > 0) {
		if (mmr_output_detach(_mmrContext, _audioDeviceOutputID) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_output_detach audio failed.";
		}
    }

	if (_videoDeviceOutputID > 0) {
		if (mmr_output_detach(_mmrContext, _videoDeviceOutputID) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_output_detach video failed.";
		}
	}

	if (_mmrContext) {
		if (mmr_context_destroy(_mmrContext) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_context_destroy failed.";
		}
    }

	if (_mmrConnection) {
	    mmr_disconnect(_mmrConnection);
    }

    _mmrContext = 0;
    _videoDeviceOutputID = -1;
    _audioDeviceOutputID = -1;
    _mmrConnection = 0;
}

void VideoView::onRegenerated()
{
	qDebug()  << "VideoView::onRegenerated";

	if (!_mmrContext && _mediaURL.toString(QUrl::None).size() > 0) {
		mmrInitialize();
	}

	if (_graphics2D->reset()) {
		_graphics2D->setBackground(COLOR_MULTITOUCH_TRANSPARENT);

		_graphics2D->clearRect(0.0, 0.0, (double)_width, (double)_height);

		_graphics2D->done();
	}
}

/*
 * Display the video full screen
 */
strm_dict_t* VideoView::calculateRect(int x, int y, int width, int height) {
	char buffer[16];
	strm_dict_t *dict = strm_dict_new();

    float mediaAspect = (float)_mediaWidth / (float)_mediaHeight;
	float screenAspect = (float)width/(float)height;
    //const float aspectTolerance = 0.1;

	if (NULL == dict) {
		return NULL;
	}

	//fullscreen is the default.
	dict = strm_dict_set(dict, "video_dest_x", itoa(0, buffer, 10));
	if (NULL == dict)
		goto fail;
	dict = strm_dict_set(dict, "video_dest_y", itoa(0, buffer, 10));
	if (NULL == dict)
		goto fail;
	dict = strm_dict_set(dict, "video_dest_w", itoa(width, buffer, 10));
	if (NULL == dict)
		goto fail;
	dict = strm_dict_set(dict, "video_dest_h", itoa(height/2, buffer, 10));
	if (NULL == dict)
		goto fail;
/*
	if (fabs(screenAspect - mediaAspect) < aspectTolerance) {

	} else if (screenAspect < mediaAspect) {
		if (height < _mediaHeight) {
			// If the screen height is taller than the video's height, we need to center the video vertically and set the video width to be the same as the screen width while maintaining the aspect ratio. We have to figure out how to map the video to the narrow screen. To do that, we want to change the video_dest_y value of the origin of the video window rectangle. We need to take the difference between the screen height and the video height and divide by two so we get an equal amount of unused space above and below. We adjust the height of the video image by dividing the screen width by the video aspect ratio to scale it down. This operation adjusts the video height so that it appears in the same aspect ratio.

			dict = strm_dict_set(dict, "video_dest_y", itoa((_mediaHeight - height) / 2, buffer, 10));
			if (NULL == dict)
				goto fail;
			height = width / mediaAspect;
			dict = strm_dict_set(dict, "video_dest_h", itoa(height, buffer, 10));
			if (NULL == dict)
				goto fail;
		}
		if (height > _mediaHeight) {
			// If the screen height is taller than the video's height, we need to center the video vertically and set the video width to be the same as the screen width while maintaining the aspect ratio. We have to figure out how to map the video to the narrow screen. To do that, we want to change the video_dest_y value of the origin of the video window rectangle. We need to take the difference between the screen height and the video height and divide by two so we get an equal amount of unused space above and below. We adjust the height of the video image by dividing the screen width by the video aspect ratio to scale it down. This operation adjusts the video height so that it appears in the same aspect ratio.

			dict = strm_dict_set(dict, "video_dest_y", itoa((height - _mediaHeight) / 2, buffer, 10));
			if (NULL == dict)
				goto fail;
			height = width / mediaAspect;
			dict = strm_dict_set(dict, "video_dest_h", itoa(height, buffer, 10));
			if (NULL == dict)
				goto fail;
		}
		if (width > _mediaWidth) {
			// If the screen width is wider than the video's width, we need to center the video horizontally and set the video height to be the same as the screen height while maintaining the aspect ratio. We have to figure out how to map the video to the wider screen. To do that, we want to change the video_dest_x value of the origin of the video window rectangle. We need to take the difference between the screen width and the video width and divide by two so that we get an equal amount of unused space on the right and left. We adjust the width of the video image by multiplying the screen height by the video aspect ratio to scale it up. This operation stretches the video width so that it appears in the same aspect ratio.

			dict = strm_dict_set(dict, "video_dest_x", itoa((width - _mediaWidth) / 2, buffer, 10));
			if (NULL == dict)
				goto fail;
			width = height * mediaAspect;
			dict = strm_dict_set(dict, "video_dest_w", itoa(width, buffer, 10));
			if (NULL == dict)
			goto fail;
		}
	}
*/

	return dict;

fail:
	strm_dict_destroy(dict);
	return NULL;
}

void VideoView::handleScreenEvent(bps_event_t *event)
{

    int screenEvent;
	char id[256];
	char group[256];
	int size[2];
	int returnCode;
	screen_window_t screenVideoWindow;

	View::handleScreenEvent(event);

    screen_event_t screen_event = screen_event_get_event(event);

    //Query type of screen event and its location on the screen
    screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE,
            &screenEvent);

	switch (screenEvent) {
		case SCREEN_EVENT_CREATE:
			returnCode = screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void**)&screenVideoWindow);
			if (returnCode != 0) {
				qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window";
			} else {
				qDebug() << "VideoView::handleScreenEvent: video_window: " << (int)screenVideoWindow;


				returnCode = screen_get_window_property_cv(screenVideoWindow, SCREEN_PROPERTY_ID_STRING, 256, id);
				if (returnCode != 0) {
					qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window ID";
				} else {
					qDebug() << "VideoView::handleScreenEvent: window ID is " << id << "\n";
					if (strncmp(id, _id.toAscii().constData(), strlen(_id.toAscii().constData())) != 0) {
						qCritical()  << "VideoView::handleScreenEvent: ID doesn't match overlay window ID " << _id << " - skipping rest of setup";
					} else {
						returnCode = screen_get_window_property_cv(screenVideoWindow, SCREEN_PROPERTY_GROUP, 256, group);
						if (returnCode != 0) {
							qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window group";
						} else {
							qDebug() << "VideoView::handleScreenEvent: window group is " << group << "\n";
							qDebug() << "VideoView::handleScreenEvent: window backup group is " << _backupGroup << "\n";

							if (strncmp(group, _backupGroup.toAscii().constData(), strlen(_backupGroup.toAscii().constData())) == 0) {
								qCritical()  << "VideoView::handleScreenEvent: group match overlay window group - already setup";
							} else {
								qDebug() << "VideoView::handleScreenEvent: correct video window detected\n";


								_screenVideoWindow = screenVideoWindow;

								if (_screenWindow != NULL) {
									int position[2];

									//position[0] = _x + _leftPadding;
									//position[1] = _y + _topPadding;
									position[0] = _leftPadding;
									position[1] = _topPadding;

									returnCode = screen_set_window_property_iv(_screenVideoWindow, SCREEN_PROPERTY_POSITION, position);
									if (returnCode != 0) {
										qCritical()  << "VideoView::handleScreenEvent: failed to override position";
									} else {
										qDebug() << "VideoView::handleScreenEvent: position is " << position[0] << "," << position[1] << "\n";

										size[0] = _width - _leftPadding - _rightPadding;
										size[1] = _height - _topPadding - _bottomPadding;

										returnCode = screen_set_window_property_iv(_screenVideoWindow, SCREEN_PROPERTY_SIZE, size);
										if (returnCode != 0) {
											qCritical()  << "VideoView::handleScreenEvent: failed to override size";
										} else {
											qDebug() << "VideoView::handleScreenEvent: size is " << size[0] << "," << size[1] << "\n";

											int videoZ = _z - 1;

											returnCode = screen_set_window_property_iv(_screenVideoWindow, SCREEN_PROPERTY_ZORDER, &videoZ);
											if (returnCode != 0) {
												qCritical()  << "VideoView::handleScreenEvent: failed to override Z";
											} else {
												qDebug() << "VideoView::handleScreenEvent: Z is " << videoZ << "\n";

												if (_display == DISPLAY_HDMI) {
													//returnCode = screen_get_window_property_pv(_nativeWindow->getScreenWindow(), SCREEN_PROPERTY_DISPLAY, &display);
													screen_display_t* attachedDisplay = NativeWindow::getAttachedDisplay();
													if (attachedDisplay) {
														returnCode = screen_set_window_property_pv(_screenVideoWindow, SCREEN_PROPERTY_DISPLAY, (void**)attachedDisplay);
														if (returnCode != 0) {
															qCritical()  << "VideoView::handleScreenEvent: failed to override display";
														} else {
															qDebug() << "VideoView::handleScreenEvent: display is " << attachedDisplay << "\n";
														}
													}
												}

												if (_display == DISPLAY_DEVICE) {
													_group = _backupGroup;

													returnCode = screen_join_window_group( _screenWindow,       _group.toAscii().data());
												}
												returnCode = screen_join_window_group(_screenVideoWindow, _group.toAscii().data());

												int visible = 1;
												returnCode = screen_set_window_property_iv(_screenVideoWindow, SCREEN_PROPERTY_VISIBLE, &visible);
												if (returnCode != 0) {
													qCritical()  << "VideoView::handleScreenEvent: failed to override visible";
												} else {
													qDebug() << "VideoView::handleScreenEvent: visible is " << visible << "\n";
												}

												int returnCode = NativeWindow::updateScreen();

												qDebug()  << "VideoView::handleScreenEvent: just about to play ...";


/*
												strm_dict_t* dict = calculateRect(_leftPadding, _topPadding, size[0], size[1]);
												//strm_dict_t* dict = calculateRect(position[0], position[1], size[0], size[1]);
												if (NULL == dict) {
													qCritical()  << "calculate_rect failed\n";
												}

												if (mmr_output_parameters(_mmrContext, _videoDeviceOutputID, dict) != 0) {
													qCritical()  << "mmr_output_parameters failed\n";
												}
												dict = NULL;
*/

												setAltered(true);

												play();
											}
										}
									}
								}
							}
						}
					}
				}
			}

			break;

		case SCREEN_EVENT_MTOUCH_TOUCH:
			qDebug()  << "VideoView::handleScreenEvent: just about to play after touch ...";
			if (_playing) {
				stop();
			} else {
				play();
			}
			break;

		case SCREEN_EVENT_MTOUCH_MOVE:
			break;

		case SCREEN_EVENT_MTOUCH_RELEASE:
			break;

		case SCREEN_EVENT_POINTER:
			int buttons;

	        screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS,
				&buttons);

	    	switch (buttons) {
	    		case SCREEN_LEFT_MOUSE_BUTTON:
	    		case SCREEN_RIGHT_MOUSE_BUTTON:
	    			if (_playing) {
	    				stop();
	    			} else {
	    				play();
	    			}
	    			break;
	    	}

			break;

		default:
			break;
	}
}

QString VideoView::mediaURL()
{
	QString mediaURL;

	_viewMutex.lock();

	mediaURL = _mediaURL.toString(QUrl::None);

	_viewMutex.unlock();

	return mediaURL;
}

bool VideoView::playing() {
	return _playing;
}

int VideoView::leftPadding() {
	return _leftPadding;
}

int VideoView::rightPadding() {
	return _rightPadding;
}

int VideoView::topPadding() {
	return _topPadding;
}

int VideoView::bottomPadding() {
	return _bottomPadding;
}

void VideoView::setMediaURL(QString mediaURL)
{
	_viewMutex.lock();

	_mediaURL = QUrl(mediaURL);

	_viewMutex.unlock();

	if (_mmrContext) {
		mmrCleanup();
	}

	qDebug()  << "VideoView::setMediaURL: " << mediaURL;

	if (_display == DISPLAY_DEVICE) {
		if (_group == _backupGroup) {
			_group = QString("tmpvideo000").append(QString::number((int)this));
			int returnCode = screen_create_window_group(_screenWindow, _group.toAscii().data());
			returnCode = screen_join_window_group(_screenWindow, _group.toAscii().data());
		}
	}

	setAltered(true);
}

void VideoView::setMediaSize(int width, int height)
{
	_viewMutex.lock();

	_mediaWidth = width;
	_mediaHeight = height;

	_viewMutex.unlock();
}

void VideoView::setLeftPadding(int leftPadding)
{
	_viewMutex.lock();

	_leftPadding = leftPadding;

	_viewMutex.unlock();

	setStale(true);
}

void VideoView::setRightPadding(int rightPadding)
{
	_viewMutex.lock();

	_rightPadding = rightPadding;

	_viewMutex.unlock();

	setStale(true);
}

void VideoView::setTopPadding(int topPadding)
{
	_viewMutex.lock();

	_topPadding = topPadding;

	_viewMutex.unlock();

	setStale(true);
}

void VideoView::setBottomPadding(int bottomPadding)
{
	_viewMutex.lock();

	_bottomPadding = bottomPadding;

	_viewMutex.unlock();

	setStale(true);
}

int VideoView::seek(int position)
{
	int returnCode = -1;

	if (_mmrContext) {
		qDebug()  << "VideoView::seek" << position;

		returnCode = mmr_seek(_mmrContext, QString::number(position).toAscii().constData());

		if (returnCode != 0) {
			qCritical()  << "VideoView::seek failed.";
		}
    }

    return returnCode;
}

int VideoView::play()
{
	int returnCode = -1;

	if (_mmrContext && !_playing) {
		qDebug()  << "VideoView::play";

		returnCode = mmr_play(_mmrContext);

		if (returnCode != 0) {
			qCritical()  << "VideoView::play failed.";
		} else {
			_playing = true;
		}
    }

    return returnCode;
}

int VideoView::stop()
{
	int returnCode = -1;

	if (_mmrContext) {
		qDebug()  << "VideoView::stop";

		returnCode = mmr_stop(_mmrContext);

		if (returnCode != 0) {
			qCritical()  << "VideoView::stop failed.";
		} else {
			_playing = false;
		}
    }

    return returnCode;
}

	} /* end namespace media */
} /* end namespace views */

