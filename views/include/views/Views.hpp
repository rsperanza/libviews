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

#ifndef VIEWS_HPP
#define VIEWS_HPP

#include <QtCore/QObject>
#include "base/View.hpp"
#include "graphics/Graphics2D.hpp"
#include "media/PhotoView.hpp"
#include "media/VideoView.hpp"
#include "cascades/ViewControl.hpp"
#include "cascades/Canvas.hpp"

using namespace views;
using namespace views::base;
using namespace views::event;
using namespace views::graphics;
using namespace views::media;
using namespace views::cascades;

#define VIEWS_MAJOR_VERSION		1
#define VIEWS_MINOR_VERSION		0


namespace views {

class Q_DECL_EXPORT Views : public QObject {

Q_OBJECT

public:
	// shutdown - call thread function, hiding it from caller
	static void shutdown();

protected:
	Views();
	virtual ~Views();

	static Views singleton;
};

}

#endif /* VIEWS_HPP */
