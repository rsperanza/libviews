/*
 * Copyright (c) 2013 SurF-X Interactive
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

#ifndef PHOTOVIEW_HPP
#define PHOTOVIEW_HPP

#include <assert.h>
#include <screen/screen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <math.h>

#include "../graphics/Graphics2D.hpp"

#include <QObject>
#include <QString>

#include "../base/View.hpp"
#include "../base/View.hpp"

using namespace bb;
using namespace views::base;

namespace views {

	namespace media {

class Q_DECL_EXPORT PhotoView : public View {

Q_OBJECT

Q_PROPERTY(QString photoFilename READ photoFilename WRITE setPhotoFilename) // photo Filename

public:
	PhotoView(ViewDisplay display = DISPLAY_DEVICE);
	virtual ~PhotoView();

	// property signals
	QString photoFilename();

public Q_SLOTS:
	// property slots
	void setPhotoFilename(QString photoFilename);

	int generateThumbnail(const QString& filename, const QString& thumbFilename);

public:
	void cleanup();
	void update();

	void onRegenerated();
	void onVisible();

private:
	// view transform functions

	// photo file names
	QString* _photoFilename;

	ImageData* _photoImage;

	Graphics2D* _graphics2D;
};

	} /* end namespace media */

} /* end namespace views */

#endif /* PHOTOVIEW_HPP */

