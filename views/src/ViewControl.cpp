/*
 * Copyright (c) 2012-2014 Research In Motion Limited.
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

#include "ViewControl.hpp"

namespace views {
	namespace cascades {

ViewControl::ViewControl(Container * parent) : ForeignWindowControl(parent)
{
	bool connectResult;

	// connect ForeignWindowControl base signals to slots
    Q_UNUSED(connectResult);
	connectResult = connect(this, SIGNAL(controlFrameChanged(const QRectF &)), this, SLOT(onControlFrameChanged(const QRectF &)));

    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

	Q_UNUSED(connectResult);
	connectResult = connect(this, SIGNAL(touch(bb::cascades::TouchEvent *)),
					 		this,   SLOT(onTouch(bb::cascades::TouchEvent *)) );

	// This is only available in Debug builds.
	Q_ASSERT(connectResult);

	// no children added to internal lists yet
	_childrenAdded = false;
	_childID = 0;
}

ViewControl::~ViewControl() {
	// TODO Auto-generated destructor stub
}

void ViewControl::onControlFrameChanged(const QRectF &controlFrame) {
	qDebug()  << "ViewControl::onControlFrameChanged";

	qDebug() << "ViewControl size: " << controlFrame.x() << "," << controlFrame.y() << " " << controlFrame.width() << "x" << controlFrame.height() << "\n";

	if (!_childrenAdded) {
		for(int index = 0; index < children().size(); index++) {
			View* view = (View*)children()[index];

			if (viewsToResize.indexOf(view) < 0) {
				addViewToResize(view);
			}
			if (touchEventViews.indexOf(view) < 0) {
				addTouchEventView(view);
			}

			if (view->z() > 0 && view->width() > 0 && view->height() > 0) {
				if (view->enabled() == false) {
					view->setWindowGroup(windowGroup());
				}
				if (view->windowID().size() == 0) {
					_childID++;
					view->setWindowID(windowId().append(QString("-child")).append(QString::number(_childID)));
				}

				view->setEnabled(true);
				view->setAltered(true);
				view->setStale(true);
				view->add();

				qDebug() << "ViewControl other view add: " << view << "\n";
			}
		}

		_childrenAdded = true;

		qDebug() << "ViewControl children added\n";
	}

	if (controlFrame.width() > 0 && controlFrame.height() > 0) {
		for(int index = 0; index < viewsToResize.size(); index++) {
			View* view = viewsToResize[index];
			if (view->enabled() == false) {
				view->setWindowGroup(windowGroup());
			}
			if (view->windowID().size() == 0) {
				_childID++;
				view->setWindowID(windowId().append(QString("-child")).append(QString::number(_childID)));
			}

			bool addView = false;
			if (view->width() == 0 && view->height() == 0) {
				addView = true;
			}

			view->setPosition(controlFrame.x(), controlFrame.y());
			view->setSize(controlFrame.width(), controlFrame.height());
			view->setEnabled(true);
			view->setAltered(true);
			view->setStale(true);

			if (addView) {
				view->add();

				qDebug() << "ViewControl resize view add: " << view << "\n";
			}
		}
	}
}

void ViewControl::addViewToResize(View* view)
{
	if (view) {
		if (view->width() == 0 && view->height() == 0) {
			viewsToResize << view;

			qDebug() << "ViewControl add view to resize: " << view << "\n";
		}
	}
}

void ViewControl::addTouchEventView(View* view)
{
	if (view) {
		if (view->z() < 0) {
			touchEventViews << view;

			qDebug() << "ViewControl add touch event view: " << view << "\n";

	    	bool connectResult;

			// register view slot with internal touch handler generated signal
			Q_UNUSED(connectResult);
			connectResult = connect(this, SIGNAL(viewMultitouch(MultitouchEvent *)),
							 		view,   SLOT(onMultitouch(MultitouchEvent *)) );

			// This is only available in Debug builds.
			//Q_ASSERT(connectResult);

			// register view signal with internal relay handler for external signal
			Q_UNUSED(connectResult);
			connectResult = connect(view, SIGNAL(multitouch(MultitouchEvent *)),
							 		this,   SLOT(onMultitouch(MultitouchEvent *)) );

			// This is only available in Debug builds.
			Q_ASSERT(connectResult);

			// register view signal with internal relay handler for external signal
			Q_UNUSED(connectResult);
			connectResult = connect(view, SIGNAL(mouse(MouseEvent *)),
							 		this,   SLOT(onMouse(MouseEvent *)) );

			// This is only available in Debug builds.
			Q_ASSERT(connectResult);

			// register view signal with internal relay handler for external signal
			Q_UNUSED(connectResult);
			connectResult = connect(view, SIGNAL(key(KeyEvent *)),
							 		this,   SLOT(onKey(KeyEvent *)) );

			// This is only available in Debug builds.
			Q_ASSERT(connectResult);

			// register view signal with internal relay handler for external signal
			Q_UNUSED(connectResult);
			connectResult = connect(view, SIGNAL(gamepad(GamepadEvent *)),
							 		this,   SLOT(onGamepad(GamepadEvent *)) );

			// This is only available in Debug builds.
			Q_ASSERT(connectResult);

			if (view->width() > 0 && view->height() > 0) {
				if (view->enabled() == false) {
					view->setWindowGroup(windowGroup());
				}
				if (view->windowID().size() == 0) {
					_childID++;
					view->setWindowID(windowId().append(QString("-child")).append(QString::number(_childID)));
				}

				view->setEnabled(true);
				view->setAltered(true);
				view->add();

				qDebug() << "ViewControl touch event view add: " << view << "\n";
			}
		}
	}
}

void ViewControl::onTouch(bb::cascades::TouchEvent *event) {

	int touchID = 100;  // dummy value for Cascades touch events routed through the multitouch handling in Views
	int touchPressure = 100; // Cascades would only send out the signal if the touch pressure was hight

	for(int index = 0; index < touchEventViews.size(); index++) {
		if (event->isDown()) {
			emit viewMultitouch(new MultitouchEvent(SCREEN_EVENT_MTOUCH_TOUCH, event->screenX(), event->screenY(), event->localX(), event->localY(), touchID, touchPressure, touchEventViews[index]));
		}
		if (event->isUp()) {
			emit viewMultitouch(new MultitouchEvent(SCREEN_EVENT_MTOUCH_RELEASE, event->screenX(), event->screenY(), event->localX(), event->localY(), touchID, touchPressure, touchEventViews[index]));
		}
		if (event->isMove()) {
			emit viewMultitouch(new MultitouchEvent(SCREEN_EVENT_MTOUCH_MOVE, event->screenX(), event->screenY(), event->localX(), event->localY(), touchID, touchPressure, touchEventViews[index]));
		}
	}
}

void ViewControl::onMultitouch(MultitouchEvent *event)
{
	emit multitouch(event);
}

void ViewControl::onMouse(MouseEvent *event)
{
	emit mouse(event);
}

void ViewControl::onKey(KeyEvent *event)
{
	emit key(event);
}

void ViewControl::onGamepad(GamepadEvent *event)
{
	emit gamepad(event);
}

	} /* end namespace cascades */
} /* end namespace views */
