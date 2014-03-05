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

#ifndef VIEWCONTROL_HPP_
#define VIEWCONTROL_HPP_

#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/TouchEvent>
#include <QObject>

#include "../base/View.hpp"

using namespace bb::cascades;
using namespace views::base;

namespace views {
	namespace cascades {


class ViewControl: public ForeignWindowControl {

    Q_OBJECT

public:
    ViewControl(Container * parent = 0);
    virtual ~ViewControl();


Q_SIGNALS:
	void multitouch(MultitouchEvent *event);
	void mouse     (MouseEvent *event);
	void key       (KeyEvent *event);
	void gamepad   (GamepadEvent *event);

	// for internal use only
	void viewMultitouch(MultitouchEvent *event);

private slots:
	void onEnabledChanged(bool enabled);
	void onVisibleChanged(bool visible);

	void onTouch(bb::cascades::TouchEvent *event);
	void onControlFrameChanged(const QRectF &controlFrame);
	void onMultitouch(MultitouchEvent *event);
	void onMouse(MouseEvent *event);
	void onKey(KeyEvent *event);
	void onGamepad(GamepadEvent *event);

protected:
	void addView(View* view);
	void addViewToResize(View* view);
	void addTouchEventView(View* view);

	bool _childrenAdded;
	int _childID;


	// main mutex for controlling view access
	QMutex _viewsMutex;
    QList<View*> _views;
    QList<View*> _viewsToResize;
    QList<View*> _touchEventViews;
};

	} /* end namespace cascades */

} /* end namespace views */

#endif /* VIEWCONTROL_HPP_ */
