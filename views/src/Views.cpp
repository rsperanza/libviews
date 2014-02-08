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

#include <pthread.h>

#include "Views.hpp"
#include "ViewsThread.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views;
using namespace views::base;
using namespace views::cascades;

namespace views {

Views Views::singleton;

Views::Views() : QObject(NULL)
{
	qDebug()  << "Views: Views ";

	// register QML types for the library

	// base types
    qmlRegisterType < View > ("views", 1, 0, "View");

    // media types
    qmlRegisterType < PhotoView > ("views", VIEWS_MAJOR_VERSION, VIEWS_MINOR_VERSION, "PhotoView");
    qmlRegisterType < VideoView > ("views", VIEWS_MAJOR_VERSION, VIEWS_MINOR_VERSION, "VideoView");

	// event types
    qmlRegisterType < GamepadEvent >    ("views", VIEWS_MAJOR_VERSION, VIEWS_MINOR_VERSION, "GamepadEvent");
    qmlRegisterType < KeyEvent >        ("views", VIEWS_MAJOR_VERSION, VIEWS_MINOR_VERSION, "KeyEvent");
    qmlRegisterType < MouseEvent >      ("views", VIEWS_MAJOR_VERSION, VIEWS_MINOR_VERSION, "MouseEvent");
    qmlRegisterType < MultitouchEvent > ("views", VIEWS_MAJOR_VERSION, VIEWS_MINOR_VERSION, "MultitouchEvent");

    // cascades controls
    qmlRegisterType < ViewControl > ("views", 1, 0, "ViewControl");
    qmlRegisterType < Canvas >      ("views", 1, 0, "Canvas");
    //qmlRegisterType < StrokeCap >      ("views", 1, 0, "StrokeCap");
    //qmlRegisterType < StrokeJoin >      ("views", 1, 0, "StrokeJoin");

}

Views::~Views() {
}

void Views::shutdown() {
	ViewsThread* viewsThread = ViewsThread::getInstance();

	viewsThread->shutdown();

	while (viewsThread->isRunning()) {
		usleep(100);
	}
}


}
