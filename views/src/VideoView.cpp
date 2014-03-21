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
	_screenVideoCCWindow = 0;

	_playing = false;

    _leftPadding = 0;
    _rightPadding = 0;
    _topPadding = 0;
    _bottomPadding = 0;

	// I/O variables
	_videoDeviceOutputID = -1;
	_audioDeviceOutputID = -1;
	_captionDeviceOutputID = -1;
	_captionExternalDeviceInputID = -1;

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


/*
 * Display the video full screen
 */
strm_dict_t* VideoView::getCaptionParams() {
	char buffer[16];
	strm_dict_t *dict = strm_dict_new();

	dict = strm_dict_set(dict, "subpicture_url", _captionFileUrl.toString().toAscii().data());
	if (NULL == dict)
		goto fail;

	return dict;

fail:
	strm_dict_destroy(dict);
	return NULL;
}

/*
 * Display the video full screen
 */
strm_dict_t* VideoView::calculateRect(int x, int y, int width, int height) {
    const int image_width = _mediaWidth;
    const int image_height = _mediaHeight;
    const float image_aspect = (float)image_width / (float)image_height;
    const float aspect_tolerance = 0.1;
    float screen_aspect = (float)width / (float)height;

    char buffer[16];
    strm_dict_t *dict = strm_dict_new();

    if (NULL == dict) {
        return NULL;
    }

    //fullscreen is the default.
    dict = strm_dict_set(dict, "video_dest_x", "0");
    if (NULL == dict)
        goto fail;
    dict = strm_dict_set(dict, "video_dest_y", "0");
    if (NULL == dict)
        goto fail;
    dict = strm_dict_set(dict, "video_dest_w", itoa(width, buffer, 10));
    if (NULL == dict)
        goto fail;
    dict = strm_dict_set(dict, "video_dest_h", itoa(height, buffer, 10));
    if (NULL == dict)
        goto fail;

    if (fabs(screen_aspect - image_aspect) < aspect_tolerance) {
        //if the screen is at almost the same aspect as the video, just
        //do full screen.  Nothing to do here.  Fall through and return
        //full screen.
    } else if (screen_aspect < image_aspect) {
        /* The screen is too tall.  We need to centre top to bottom, set the
         * width the same as the screen's while maintaining the same aspect
         * ratio.
         */
        dict = strm_dict_set(dict, "video_dest_y", itoa((height - image_height) / 2, buffer, 10));
        if (NULL == dict)
            goto fail;

        height = width / image_aspect;

        dict = strm_dict_set(dict, "video_dest_h", itoa(height, buffer, 10));
        if (NULL == dict)
            goto fail;
    } else {
        /* The screen is too wide.  We need to centre left to right, set the
         * height the same as the screen's while maintaining the same aspect
         * ratio.
         */
        dict = strm_dict_set(dict, "video_dest_x", itoa((width - image_width) / 2, buffer, 10));
        if (NULL == dict)
            goto fail;

        width = height * image_aspect;

        dict = strm_dict_set(dict, "video_dest_w", itoa(width, buffer, 10));
        if (NULL == dict)
            goto fail;
    }

    return dict;

fail:
    strm_dict_destroy(dict);
    return NULL;
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
        	QString _videoDeviceUrl = "screen:?nodstviewport=1&winid=";
        	_videoDeviceUrl += _id;
        	_videoDeviceUrl += "&wingrp=";
        	_videoDeviceUrl += _group;

        	QString _audioDeviceUrl = "audio:default";

        	qDebug()  << "VideoView::mmrInitialize: _mediaURL: " << _mediaURL.toString().toAscii().constData();
        	qDebug()  << "VideoView::mmrInitialize: videoDeviceUrl: " << _videoDeviceUrl.toAscii().constData();
        	qDebug()  << "VideoView::mmrInitialize: _audioDeviceUrl: " << _audioDeviceUrl.toAscii().constData();

        	if (_showCaptions) {
            	_captionDeviceUrl = "screen:?nodstviewport=1&winid=";
            	_captionDeviceUrl += QString("CC_").append(_id);
            	_captionDeviceUrl += "&wingrp=";
            	_captionDeviceUrl += _group;
               // _captionDeviceUrl += "&initflags=invisible";  // should be included but this is causing the CC's to not appear right now
                _captionDeviceUrl += "&ymmf.VideoWriterDisplayIndex=0";  //CAPTIONS;
            	qDebug()  << "VideoView::mmrInitialize: captionDeviceUrl: " << _captionDeviceUrl.toAscii().constData();

            	if (_captionFileUrl.toString(QUrl::None).size() > 0) {
    	        	qDebug()  << "VideoView::mmrInitialize: _captionFileUrl: " <<  _captionFileUrl.toString(QUrl::None).toAscii().constData();
    			}
        	}

        	/*
        	 * Configure video and audio output.
        	 */
        	_videoDeviceOutputID = mmr_output_attach(_mmrContext, _videoDeviceUrl.toAscii().constData(), "video");
        	if (_videoDeviceOutputID == -1) {
        		qCritical()  << "VideoView::mmrInitialize: mmr_output_attach video failed.";
        	} else {
            	qDebug()  << "VideoView::mmrInitialize: _videoDeviceOutputID: " << _videoDeviceOutputID;
        	}

        	_audioDeviceOutputID = mmr_output_attach(_mmrContext, _audioDeviceUrl.toAscii().constData(), "audio");
        	if (_audioDeviceOutputID == -1) {
        		qCritical()  << "VideoView::mmrInitialize: mmr_output_attach audio failed.";
        	} else {
            	qDebug()  << "VideoView::mmrInitialize: _audioDeviceOutputID: " << _audioDeviceOutputID;
        	}

        	if (_captionDeviceUrl.length() > 0) {
				_captionDeviceOutputID = mmr_output_attach(_mmrContext, _captionDeviceUrl.toAscii().constData(), "subpicture");  //CAPTIONS
				if (_captionDeviceOutputID == -1) {																										//
					qCritical()  << "VideoView::mmrInitialize: mmr_output_attach caption failed.";
	        	} else {
	            	qDebug()  << "VideoView::mmrInitialize: _captionDeviceOutputID: " << _captionDeviceOutputID;
	        	}

	        	if (_captionFileUrl.toString(QUrl::None).size() > 0) {
	        		_captionExternalDeviceInputID = mmr_input_parameters(_mmrContext, getCaptionParams()); 			// SMPTE-TT FILES
	        		if (_captionExternalDeviceInputID == -1)															//
	        		{																									//
						qCritical()  << "VideoView::mmrInitialize: mmr_input_parameters caption file failed.";
	            	} else {
	                	qDebug()  << "VideoView::mmrInitialize: _captionExternalDeviceInputID: " << _captionExternalDeviceInputID;
	        		}
	        	}
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

	if (_captionDeviceOutputID > 0) {
		if (mmr_output_detach(_mmrContext, _captionDeviceOutputID) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_output_detach caption failed.";
		}																										//
    }																										//

	if (_mmrContext) {
		if (mmr_context_destroy(_mmrContext) != 0) {
    		qCritical()  << "VideoView::mmrCleanup: mmr_context_destroy failed.";
		}
    }

	if (_mmrConnection) {
	    mmr_disconnect(_mmrConnection);
    }

    _mmrContext = 0;
    _mmrConnection = 0;

    _videoDeviceOutputID = -1;
    _audioDeviceOutputID = -1;
    _captionDeviceOutputID = -1;
	_captionExternalDeviceInputID = -1;
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

void VideoView::handleScreenEvent(bps_event_t *event)
{
    int screenEvent;
	char id[256];
	char group[256];
	int size[2];
	int returnCode;
	screen_window_t screenWindow;

	View::handleScreenEvent(event);

	if (initialized()) {
		screen_event_t screen_event = screen_event_get_event(event);

		//Query type of screen event and its location on the screen
		screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screenEvent);

		switch (screenEvent) {
			case SCREEN_EVENT_CREATE:
				returnCode = screen_get_event_property_pv(screen_event, SCREEN_PROPERTY_WINDOW, (void**)&screenWindow);
				if (returnCode != 0) {
					qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window";
				} else {
					qDebug() << "VideoView::handleScreenEvent: video_window: " << (int)screenWindow;


					memset(id, 0, 256);
					returnCode = screen_get_window_property_cv(screenWindow, SCREEN_PROPERTY_ID_STRING, 255, id);
					if (returnCode != 0) {
						qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window ID";
					} else {
						qDebug() << "VideoView::handleScreenEvent: window ID is " << id << "\n";

						memset(group, 0, 256);
						returnCode = screen_get_window_property_cv(screenWindow, SCREEN_PROPERTY_GROUP, 256, group);
						if (returnCode != 0) {
							qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window group";
						} else {
							qDebug() << "VideoView::handleScreenEvent: window group is " << group << "\n";
						}

						int videoZ;

						returnCode = screen_get_window_property_iv(screenWindow, SCREEN_PROPERTY_ZORDER, &videoZ);
						if (returnCode != 0) {
							qCritical()  << "VideoView::handleScreenEvent: failed to retrieve Z";
						} else {
							qDebug() << "VideoView::handleScreenEvent: Z is " << videoZ << "\n";
						}

						returnCode = screen_get_window_property_iv(screenWindow, SCREEN_PROPERTY_SIZE, size);
						if (returnCode != 0) {
							qCritical()  << "VideoView::handleScreenEvent: failed to retrieve size";
						} else {
							qDebug() << "VideoView::handleScreenEvent: size is " << size[0] << "," << size[1] << "\n";
						}

						if (strcmp(id, _id.toAscii().constData()) != 0) {
							if (_showCaptions) {
								QString _id_CC = QString("CC_").append(_id);
								if (strcmp(id, _id_CC.toAscii().constData()) == 0) {
									qCritical()  << "VideoView::handleScreenEvent: detected a second window ... probably the CC overlay";

									returnCode = screen_get_window_property_cv(screenWindow, SCREEN_PROPERTY_GROUP, 256, group);
									if (returnCode != 0) {
										qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window group";
									} else {
										qDebug() << "VideoView::handleScreenEvent: window group is " << group << "\n";
										qDebug() << "VideoView::handleScreenEvent: window backup group is " << _backupGroup << "\n";

										if (strncmp(group, _backupGroup.toAscii().constData(), strlen(_backupGroup.toAscii().constData())) == 0) {
											qCritical()  << "VideoView::handleScreenEvent: group match overlay window group - already setup";
										} else {
											qDebug() << "VideoView::handleScreenEvent: correct video CC window detected\n";

											_screenVideoCCWindow = screenWindow;

											if (_screenWindow) {
												int position[2];

												position[0] = _leftPadding;
												position[1] = _topPadding;

												returnCode = screen_set_window_property_iv(_screenVideoCCWindow, SCREEN_PROPERTY_POSITION, position);
												if (returnCode != 0) {
													qCritical()  << "VideoView::handleScreenEvent: failed to override position";
												} else {
													qDebug() << "VideoView::handleScreenEvent: position is " << position[0] << "," << position[1] << "\n";

													size[0] = _width - _leftPadding - _rightPadding;
													size[1] = _height - _topPadding - _bottomPadding;

													returnCode = screen_set_window_property_iv(_screenVideoCCWindow, SCREEN_PROPERTY_SIZE, size);
													if (returnCode != 0) {
														qCritical()  << "VideoView::handleScreenEvent: failed to override size";
													} else {
														qDebug() << "VideoView::handleScreenEvent: size is " << size[0] << "," << size[1] << "\n";

														videoZ = _z - 1;

														returnCode = screen_set_window_property_iv(_screenVideoCCWindow, SCREEN_PROPERTY_ZORDER, &videoZ);
														if (returnCode != 0) {
															qCritical()  << "VideoView::handleScreenEvent: failed to override Z";
														} else {
															qDebug() << "VideoView::handleScreenEvent: Z is " << videoZ << "\n";

															if (_display == DISPLAY_HDMI) {
																screen_display_t* attachedDisplay = NativeWindow::getAttachedDisplay();
																if (attachedDisplay) {
																	returnCode = screen_set_window_property_pv(_screenVideoCCWindow, SCREEN_PROPERTY_DISPLAY, (void**)attachedDisplay);
																	if (returnCode != 0) {
																		qCritical()  << "VideoView::handleScreenEvent: failed to override display";
																	} else {
																		qDebug() << "VideoView::handleScreenEvent: display is " << attachedDisplay << "\n";
																	}
																}
															}

		                                                    if (_display == DISPLAY_DEVICE) {
		                                                        if (_showCaptions) {
		                                                            returnCode = screen_join_window_group( _screenWindow, _group.toAscii().data());
		                                                        }
		                                                    }
		                                                    if (_showCaptions) {
		                                                        returnCode = screen_join_window_group(_screenVideoWindow, _group.toAscii().data());
	                                                            returnCode = screen_join_window_group(_screenVideoCCWindow, _group.toAscii().data());
		                                                    }


															int visible = 1;
															returnCode = screen_set_window_property_iv(_screenVideoCCWindow, SCREEN_PROPERTY_VISIBLE, &visible);
															if (returnCode != 0) {
																qCritical()  << "VideoView::handleScreenEvent: failed to override visible";
															} else {
																qDebug() << "VideoView::handleScreenEvent: visible is " << visible << "\n";
															}

															int returnCode = NativeWindow::updateScreen();

															qDebug()  << "VideoView::handleScreenEvent: just about to play ...";

															setAltered(true);

															play();
														}
													}
												}
											}
										}
									}
								} else {
									qCritical()  << "VideoView::handleScreenEvent: ID doesn't match overlay window ID " << _id << " - skipping rest of setup";
								}
							}
						} else {
							returnCode = screen_get_window_property_cv(screenWindow, SCREEN_PROPERTY_GROUP, 256, group);
							if (returnCode != 0) {
								qCritical()  << "VideoView::handleScreenEvent: failed to retrieve video window group";
							} else {
								qDebug() << "VideoView::handleScreenEvent: window group is " << group << "\n";
								qDebug() << "VideoView::handleScreenEvent: window backup group is " << _backupGroup << "\n";

								if (strncmp(group, _backupGroup.toAscii().constData(), strlen(_backupGroup.toAscii().constData())) == 0) {
									qCritical()  << "VideoView::handleScreenEvent: group match overlay window group - already setup";
								} else {
									qDebug() << "VideoView::handleScreenEvent: correct video window detected\n";


									_screenVideoWindow = screenWindow;

									if (_screenWindow) {
										int position[2];

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

												videoZ = _z - 1;
												if (_showCaptions) {
													videoZ = _z - 2;
												}

												returnCode = screen_set_window_property_iv(_screenVideoWindow, SCREEN_PROPERTY_ZORDER, &videoZ);
												if (returnCode != 0) {
													qCritical()  << "VideoView::handleScreenEvent: failed to override Z";
												} else {
													qDebug() << "VideoView::handleScreenEvent: Z is " << videoZ << "\n";

													if (_display == DISPLAY_HDMI) {
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

		                                                if (!_showCaptions) {
		                                                    returnCode = screen_join_window_group( _screenWindow,       _group.toAscii().data());
		                                                }
													}
                                                    if (!_showCaptions) {
                                                        returnCode = screen_join_window_group(_screenVideoWindow, _group.toAscii().data());
                                                    }

													int visible = 1;
													returnCode = screen_set_window_property_iv(_screenVideoWindow, SCREEN_PROPERTY_VISIBLE, &visible);
													if (returnCode != 0) {
														qCritical()  << "VideoView::handleScreenEvent: failed to override visible";
													} else {
														qDebug() << "VideoView::handleScreenEvent: visible is " << visible << "\n";
													}

													int returnCode = NativeWindow::updateScreen();

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

													if (!_showCaptions) {
														qDebug()  << "VideoView::handleScreenEvent: just about to play ...";
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
}

QString VideoView::mediaURL()
{
	QString mediaURL;

	_viewMutex.lock();

	mediaURL = _mediaURL.toString(QUrl::None);

	_viewMutex.unlock();

	return mediaURL;
}

bool VideoView::playing()
{
	return _playing;
}

bool VideoView::showCaptions()
{
	return _showCaptions;
}

QString VideoView::captionFileURL()
{
	return _captionFileUrl.toString(QUrl::None);
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

void VideoView::setShowCaptions(bool showCaptions)
{
	_viewMutex.lock();

	_showCaptions = showCaptions;

	_viewMutex.unlock();
}

void VideoView::setCaptionFileURL(QString captionFileURL)
{
	_viewMutex.lock();

	_captionFileUrl = QUrl(captionFileURL);

	_viewMutex.unlock();

	qDebug()  << "VideoView::setCaptionFileURL: " << captionFileURL;

	setStale(true);
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

