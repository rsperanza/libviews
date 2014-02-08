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

#include <math.h>

#include <QDebug>
#include <QFile>

#include "PhotoView.hpp"

using namespace bb;
using namespace bb::cascades;
using namespace bb::utility;

namespace views {
	namespace media {

PhotoView::PhotoView(ViewDisplay display) :
		View(display),
		_photoFilename(NULL),
		_photoImage(NULL),
		_graphics2D(NULL)
{
	_graphics2D = new Graphics2D(display);
	registerGraphics(_graphics2D);
}

PhotoView::~PhotoView() {
	// TODO Auto-generated destructor stub
}

void PhotoView::cleanup()
{
	if (_photoImage) {
		delete _photoImage;
	}
}

void PhotoView::onVisible()
{
	qDebug()  << "PhotoView::onVisible";
}

void PhotoView::update()
{
	if (!_photoImage && _photoFilename) {
		_photoImage = _graphics2D->loadImage(*_photoFilename);

		setEnabled(true);
		setAltered(true);
		setStale(true);

		qDebug()  << "PhotoView::update: " << _photoImage << ":" << _photoFilename;
	}
}

void PhotoView::onAltered()
{
	qDebug()  << "PhotoView::onAltered";

	if (_photoImage) {
		if (_graphics2D->reset()) {
			_graphics2D->setColor(COLOR_WHITE);

			_graphics2D->drawRect(0.0, 0.0, (double)_width, (double)_height);

			_graphics2D->drawImage(_photoImage, 0.0, (double)0.0, (double)_width, (double)_height);

			_graphics2D->done();

			setStale(true);
		}
	}
}

// properties and slots
QString PhotoView::photoFilename() {
	QString *photoFilename;

	_viewMutex.lock();

	photoFilename = new QString(*_photoFilename);

	_viewMutex.unlock();

	return *photoFilename;
}

void PhotoView::setPhotoFilename(QString photoFilename) {

	_viewMutex.lock();

	if (_photoFilename != NULL) {
		delete _photoFilename;
	}

	_photoFilename = new QString(photoFilename);

	_viewMutex.unlock();

	onAltered();

	setStale(true);
}

int PhotoView::generateThumbnail(const QString& filename, const QString& thumbFilename)
{
    int returnCode = 0;

    ImageData* image = _graphics2D->loadImage(filename);

    if (image->isValid()) {
		qDebug() << "PhotoView::generateThumbnail: save to: " << QUrl::fromLocalFile(thumbFilename);

		if (ImageConverter::encode (QUrl::fromLocalFile(thumbFilename), *image, 100)) {
			returnCode = EXIT_SUCCESS;
		} else {
			returnCode = EXIT_FAILURE;
		}

		//clean up memory and close stuff
		if (image) {
			delete image;
		}
    }

    return returnCode;
}

	} /* end namespace media */
} /* end namespace views */

