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

#include "SignaturePad.hpp"
#include "SketchPad.hpp"
#include "views/graphics/Graphics2D.hpp"

namespace views {
	namespace cascades {

SignaturePad::SignaturePad(Container * parent) : ViewControl(parent)
{
	// create default view with Graphics2D graphics

	// replace this with logic to extract colors from control properties
	QVariantList backgroundColor;
	backgroundColor << COLOR_WHITE.red << COLOR_WHITE.green << COLOR_WHITE.blue << COLOR_WHITE.alpha;
	QVariantList strokeColor;
	strokeColor << COLOR_BLACK.red << COLOR_BLACK.green << COLOR_BLACK.blue << COLOR_BLACK.alpha;

	_signPad = new SketchPad();
	_signPad->setPosition(0, 0);
	_signPad->setSize(0, 0);
	_signPad->setZ(-1);

	_signPad->setBackgroundColor(backgroundColor);
	_signPad->setStrokeColor(strokeColor);
	_signPad->setStrokeWidth(10.0);
	_signPad->setTool("freedraw");
	_signPad->setEnabled(false);

	addView(_signPad);
	addViewToResize(_signPad);
	addTouchEventView(_signPad);

	QObject::connect(
			_signPad,
			SIGNAL(captured()),
			this,
			SLOT(onCaptured()));

	_saveFilename.clear();
}

SignaturePad::~SignaturePad() {
	// TODO Auto-generated destructor stub
}

void SignaturePad::saveImage(const QString& filename)
{
	qDebug()  << "SignaturePad::saveImage";

	_saveFilename = filename;
	_signPad->captureImage(0, 0, _signPad->width(), _signPad->height());
}

void SignaturePad::onCaptured()
{
	if (_saveFilename.size() > 0) {
		qDebug()  << "SignaturePad::onCaptured" << _saveFilename;

		_signPad->saveImage(_saveFilename);
		_saveFilename.clear();

		emit saved();
	}
}

	} /* end namespace cascades */
} /* end namespace views */
