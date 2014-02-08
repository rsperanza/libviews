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


#ifndef SKETCHPAD_HPP
#define SKETCHPAD_HPP

#include "views/Views.hpp"

using namespace views::graphics;

class SketchPad : public View {

	Q_OBJECT

	Q_PROPERTY(QVariantList backgroundColor READ backgroundColor WRITE setBackgroundColor) // background color
	Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE setBackgroundImage) // background image
	Q_PROPERTY(QVariantList strokeColor READ strokeColor WRITE setStrokeColor) // stroke color
	Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth) // stroke width
	Q_PROPERTY(QString tool READ tool WRITE setTool) // tool

public:
	SketchPad();

	virtual ~SketchPad();

	int initialize();

	void update();

	void onAltered();

	QVariantList& backgroundColor();
	QString       backgroundImage();
	QVariantList& strokeColor();
	qreal         strokeWidth();
	QString       tool();


	virtual void updatePad();

Q_SIGNALS:
	void captured();

public Q_SLOTS:
	// property slots
	void setBackgroundColor(QVariantList backgroundColor);
	void setBackgroundImage(QString backgroundImage);
	void setStrokeColor(QVariantList strokeColor);
	void setStrokeWidth(qreal strokeWidth);
	void setTool(QString tool);

	void captureImage(int x, int y, int width, int height);
	void saveImage(const QString& filename);

	// touch handler
	void onMultitouch(MultitouchEvent *event);
	void onMouse(MouseEvent *event);


private:
	bool _saving;

	QString* _backgroundImageName;
	ImageData* _backgroundImage;

	Color _backgroundColor;
	Color _currentColor;
	QString* _tool;

	float _originX;
	float _originY;

	Stroke *_defaultStroke;
	Stroke *_currentStroke;

	float _lastX;
	float _lastY;

	int _strokes;
	int _strokeIndices[1000];
	Stroke* _strokeStructs[500];
	Color _strokeColors[500];
	double _strokeVerticesX[5000];
	int _strokeVertices;
	double _strokeVerticesY[5000];

	Graphics2D* _graphics2D;
};

#endif /* SKETCHPAD_HPP */

