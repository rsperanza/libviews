/*
 * Copyright (c) 2012-2013 BlackBerry
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

#include "SketchPad.hpp"
#include <math.h>

#include <QDebug>

using namespace views::graphics;

SketchPad::SketchPad() : View(DISPLAY_DEVICE)
{
	qDebug()  << "SketchPad::SketchPad";

	QObject::connect(
			(View*)this,
			SIGNAL(multitouch(MultitouchEvent*)),
			this,
			SLOT(onMultitouch(MultitouchEvent*)));

	QObject::connect(
			(View*)this,
			SIGNAL(mouse(MouseEvent*)),
			this,
			SLOT(onMouse(MouseEvent*)));

	_strokes = 0;
	_strokeVertices = 0;
	_lastX = -1;
	_lastY = -1;
	_originX = _originY = 0.0;

	_graphics2D = new Graphics2D(DISPLAY_DEVICE);

	// initialize graphic attributes
	_defaultStroke = _graphics2D->createStroke(10.0, CAP_NONE, JOIN_NONE, 0.0, NULL, 0, 0.0);
	_currentStroke = _defaultStroke;

	// register graphics with base class
	registerGraphics(_graphics2D);

	_backgroundImageName = NULL;

	_saving = false;
}

SketchPad::~SketchPad() {
	// TODO Auto-generated destructor stub
}

int SketchPad::initialize() {
	qDebug()  << "SketchPad::initialize";

	int returnCode = View::initialize();
	if (returnCode == EXIT_SUCCESS) {

		// reset stale flag for initialization only
		setStale(true);
	}

	qDebug()  << "SketchPad::initialize " << ":" << returnCode;

    return returnCode;
}


// properties and slots
QVariantList& SketchPad::backgroundColor() {

	_viewMutex.lock();

	QVariantList* backgroundColor = new QVariantList();
	*backgroundColor << _backgroundColor.red << _backgroundColor.green << _backgroundColor.blue << _backgroundColor.alpha;

	_viewMutex.unlock();

	return *backgroundColor;
}

void SketchPad::setBackgroundColor(QVariantList backgroundColor) {

	_viewMutex.lock();

	_backgroundColor.red   = backgroundColor[0].value<float>();
	_backgroundColor.green = backgroundColor[1].value<float>();
	_backgroundColor.blue  = backgroundColor[2].value<float>();
	_backgroundColor.alpha = backgroundColor[3].value<float>();

	_viewMutex.unlock();

	setStale(true);
}

QString SketchPad::backgroundImage() {
	QString *backgroundImage;

	_viewMutex.lock();

	backgroundImage = new QString(*_backgroundImageName);

	_viewMutex.unlock();

	return *backgroundImage;
}

void SketchPad::setBackgroundImage(QString backgroundImage) {

	_viewMutex.lock();

	if (_backgroundImageName) {
		delete _backgroundImageName;
	}

	if (backgroundImage.size() > 0) {
		_backgroundImageName = new QString(backgroundImage);
		if (_graphics2D) {
			_backgroundImage = _graphics2D->loadImage(*_backgroundImageName);
		}
	}

	_viewMutex.unlock();

	setStale(true);
}

QVariantList& SketchPad::strokeColor() {

	_viewMutex.lock();

	QVariantList* strokeColor = new QVariantList();
	*strokeColor << _currentColor.red << _currentColor.green << _currentColor.blue << _currentColor.alpha;

	_viewMutex.unlock();

	return *strokeColor;
}

void SketchPad::setStrokeColor(QVariantList strokeColor) {

	_viewMutex.lock();

	_currentColor.red = strokeColor[0].value<float>();
	_currentColor.green = strokeColor[1].value<float>();
	_currentColor.blue = strokeColor[2].value<float>();
	_currentColor.alpha = strokeColor[3].value<float>();

	_viewMutex.unlock();

	setStale(true);
}

qreal SketchPad::strokeWidth() {

	_viewMutex.lock();

	qreal strokeWidth = _currentStroke->width;

	_viewMutex.unlock();

	return strokeWidth;
}

void SketchPad::setStrokeWidth(qreal strokeWidth) {

	_viewMutex.lock();

	if (_defaultStroke->width == strokeWidth) {
		_currentStroke = _defaultStroke;
	} else
	if (_currentStroke->width != strokeWidth) {
		Stroke* existingStroke = NULL;
		for (int index = 0; index < _strokes; index++) {
			if (_strokeStructs[index]->width == strokeWidth) {
				existingStroke = _strokeStructs[index];
				break;
			}
		}
		if (!existingStroke) {
			_currentStroke = _graphics2D->createStroke(strokeWidth, CAP_NONE, JOIN_NONE, 0.0, NULL, 0, 0.0);
		} else {
			_currentStroke = existingStroke;
		}
	}

	_viewMutex.unlock();

	setStale(true);
}

QString SketchPad::tool() {
	QString *tool;

	_viewMutex.lock();

	tool = new QString(*_tool);

	_viewMutex.unlock();

	return *tool;
}

void SketchPad::setTool(QString tool) {

	_viewMutex.lock();

	delete _tool;

	_tool = new QString(tool);

	_viewMutex.unlock();

	setStale(true);
}

void SketchPad::update()
{
	//qDebug()  << "SketchPad::update";

	if (stale()) {
		updatePad();
	}
	if (_saving && _graphics2D->getRenderedImage()) {
		qDebug()  << "SketchPad::update" << " " << _saving;

		emit captured();
	}
}

void SketchPad::onRegenerated()
{
	qDebug()  << "SketchPad::onRegenerated";

	updatePad();
}

void SketchPad::updatePad()
{
	//qDebug()  << "SketchPad::updatePad";

	if (_graphics2D->reset()) {
		_graphics2D->translate(-_originX, -_originY);

		if (_backgroundImageName) {
			_graphics2D->drawImage(_backgroundImage, 0.0, 0.0);
		} else {
			_graphics2D->setBackground(_backgroundColor);
			_graphics2D->setColor(_currentColor);
			_graphics2D->clearRect(0, 0, _width, _height);
		}

		if (_strokes) {
			//qDebug()  << "SketchPad::updatePad: strokes " << _strokes;

			for(int index = 0; index < _strokes; index++) {
				_graphics2D->setColor(_strokeColors[index]);
				_graphics2D->setStroke(_strokeStructs[index]);

				_graphics2D->drawPolyline(&(_strokeVerticesX[_strokeIndices[index*2]]), &(_strokeVerticesY[_strokeIndices[index*2]]),
										  _strokeIndices[index*2+1] - _strokeIndices[index*2] + 1);

				//qDebug()  << "SketchPad::updatePad: stroke: " << index << " " << _strokeIndices[index*2] << " " << _strokeIndices[index*2+1] << " " << (_strokeIndices[index*2+1] - _strokeIndices[index*2] + 1);
				//qDebug()  << "SketchPad::updatePad: stroke: color: " << index << " " << _strokeColors[index].red << " " << _strokeColors[index].green  << " " << _strokeColors[index].blue << " " << _strokeColors[index].alpha;
				//qDebug()  << "SketchPad::updatePad: stroke: stroke: " << index << " " << _strokeStructs[index] << " " << _defaultStroke;
			}
		}

		_graphics2D->done();
	}
}

void SketchPad::onMultitouch(MultitouchEvent *event) {

	//qDebug()  << "SketchPad::onMultitouch: " << event->windowX() << "," << event->windowY() << " " << event->isDown() << " " << event->isMove() << " " << event->isUp();

	float length;

	if (event->isDown()) {
		if (tool() == "pan") {
			_lastX = event->windowX();
			_lastY = event->windowY();
		}
		if (tool() == "freedraw") {
			_strokes++;

			_strokeVertices++;
			_strokeIndices[(_strokes-1)*2  ] = _strokeVertices-1;
			_strokeIndices[(_strokes-1)*2+1] = _strokeVertices-1;
			_strokeStructs[_strokes-1      ] = _currentStroke;
			memcpy((void*)&_strokeColors[_strokes-1], (void*)&_currentColor, sizeof(GLColor));

			_strokeVerticesX[_strokeVertices-1] = (double)event->windowX();
			_strokeVerticesY[_strokeVertices-1] = _height - (double)event->windowY();

			_lastX = event->windowX();
			_lastY = event->windowY();

			setStale(true);
		}
	}

	if (event->isMove()) {
		if (tool() == "pan") {
			_originX += event->windowX() - _lastX;
			_originY += event->windowY() - _lastY;

			if (_backgroundImage && _originX < 0.0) {
				_originX = 0.0;
			}
			if (_backgroundImage && _originX > _backgroundImage->width()) {
				_originX = _backgroundImage->width();
			}
			if (_backgroundImage && _originY < 0.0) {
				_originY = 0.0;
			}
			if (_backgroundImage && _originY > _backgroundImage->height()) {
				_originY = _backgroundImage->height();
			}

			_lastX = event->windowX();
			_lastY = event->windowY();
		}
		if (tool() == "freedraw") {
			length = sqrt((event->windowX() - _lastX)*(event->windowX() - _lastX) + (event->windowY() - _lastY)*(event->windowY() - _lastY));
			if (length > 10.0) {
				_strokeVertices++;
				_strokeIndices[(_strokes-1)*2+1] = _strokeVertices-1;

				_strokeVerticesX[_strokeVertices-1] = (double)event->windowX();
				_strokeVerticesY[_strokeVertices-1] = _height - (double)event->windowY();

				_lastX = event->windowX();
				_lastY = event->windowY();
			}

			setStale(true);
		}
	}

	if (event->isUp()) {
		if (tool() == "pan") {
			_lastX = -1;
			_lastY = -1;
		}
		if (tool() == "freedraw") {
			_strokeVertices++;
			_strokeIndices[(_strokes-1)*2+1] = _strokeVertices-1;

			_strokeVerticesX[_strokeVertices-1] = (double)event->windowX();
			_strokeVerticesY[_strokeVertices-1] = _height - (double)event->windowY();

			_lastX = -1;
			_lastY = -1;

			setStale(true);
		}
	}

	event->deleteLater();
}

void SketchPad::onMouse(MouseEvent *event)
{
	qDebug()  << "SketchPad::onMouse";


	setStale(true);

	event->deleteLater();
}

void SketchPad::captureImage(int x, int y, int width, int height)
{
	qDebug()  << "SketchPad::captureImage" << _saving << " " << x << " " << y << " " << width << " " << height;

	_graphics2D->setCaptureRect(0, 0, width, height);
	setCapture(true);
	setStale(true);

	_saving = true;

	qDebug()  << "SketchPad::captureImage" << _saving;
}

void SketchPad::saveImage(const QString& filename)
{
	if (_saving) {
		ImageData *imageCaptured = _graphics2D->getRenderedImage();

		if (imageCaptured) {
			ImageData *imageToSave = new ImageData(bb::PixelFormat::RGBA_Premultiplied, imageCaptured->width(), imageCaptured->height());

			unsigned char *srcLine = imageCaptured->pixels() + (imageCaptured->height()-1) * imageCaptured->bytesPerLine();
			unsigned char *dstLine = imageToSave->pixels();

			for (int yi = imageCaptured->height()-1; yi >= 0; yi--) {
				unsigned char * dst = dstLine;
				unsigned char * src = srcLine;

				for (int xi = 0; xi < imageCaptured->width()*4; xi++) {
					*dst++ = *(src + xi);
				}

				dstLine += imageToSave->bytesPerLine();
				srcLine -= imageCaptured->bytesPerLine();
			}

			_graphics2D->saveImage(imageToSave, filename);
			_graphics2D->setCaptureRect(0, 0, 0, 0);

			if (imageToSave) {
				delete imageToSave;
			}
		}

		_saving = false;

		qDebug()  << "SketchPad::saveImage" << " " << _saving;
	}
}
