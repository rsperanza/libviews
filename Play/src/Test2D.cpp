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

#include "Test2D.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views::graphics;


Test2D::Test2D(ViewDisplay display) : View(display)
{
	qDebug()  << "Test2D::Test2D ";

	_graphics2D = new Graphics2D(display);

	_evenDashes = new float[2] { 20.0, 10.0 };
	_evenDashesStroke = _graphics2D->createStroke(10.0, CAP_NONE, JOIN_NONE, 0.0, _evenDashes, 2, 0.0);
	_defaultStroke = _graphics2D->createStroke(1.0);

	_slatePro = NULL;
	_slateProLight = NULL;

	_testImage = _graphics2D->loadImage("app/native/assets/images/handle_inactive.png");
	_testImage1 = _graphics2D->loadImage("app/native/assets/images/handle_pressed.png");

	_grayScaleColors = new Color[2] { { 1.0, 1.0, 0.0, 1.0 }, { 1.0, 0.75, 0.0, 1.0 } };
	_grayscalePercentages = new float[3] { 0.0, 0.6, 1.0 };
	_grayscaleGradient = _graphics2D->createGradient(1, _grayScaleColors, _grayscalePercentages, 0.0, -60.0, 0.0, 0.5);

	// register graphics with base class
	registerGraphics(_graphics2D);
}

Test2D::~Test2D() {
	// TODO Auto-generated destructor stub
}

void Test2D::update()
{
	//qDebug()  << "Test2D::update";
}

void Test2D::onVisible()
{
	qDebug()  << "Test2D::onVisible";
}

void Test2D::onAltered()
{
	qDebug()  << "Test2D::onAltered";

	if (_graphics2D->reset()) {
		//_graphics2D->setBackground(COLOR_YELLOW);
		//_graphics2D->clearRect(0, 0, 100, 100);

		_graphics2D->shear(0.25, 0.0);

		if (!_slatePro) {
			_slatePro = _graphics2D->createFont(FONT_NAME_SLATE_PRO_CONDENSED, NULL, 10, calculateDPI(), new QString("Thisates. "));
			_slateProLight = _graphics2D->createFont(FONT_NAME_SLATE_PRO_LIGHT, NULL, 20, calculateDPI(), new QString("Thisates. "));
		}

		_graphics2D->setFont(_slateProLight);
		QString text("This is a test ...");
		_graphics2D->drawString(text, 50.0, 50.0);

		_graphics2D->shear(-0.25, 0.0);

		_graphics2D->setColor(COLOR_BLUE);
		_graphics2D->setFont(_slatePro);
		_graphics2D->drawString(text, 75.0, 75.0);

		_graphics2D->drawImage(_testImage, 350.0, 700.0);
		_graphics2D->drawImage(_testImage1, 350.0, 900.0);

		_graphics2D->scale(2.0, 2.0);

		_graphics2D->drawLine(0.0, 100.0, 100.0, 200.0);

		_graphics2D->setColor(COLOR_RED);
		_graphics2D->setStroke(_evenDashesStroke);
		_graphics2D->drawLine(100.0, 200.0, 300.0, 150.0);

		_graphics2D->scale(0.5, 0.5);

		_graphics2D->setColor(COLOR_YELLOW);
		_graphics2D->fillOval(500.0, 500.0, 100.0, 100.0);
		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->drawOval(500.0, 500.0, 100.0, 100.0);

		_graphics2D->setColor(COLOR_RED);
		_graphics2D->drawArc(400.0, 400.0, 100.0, 100.0, 0.0, 180.0);

		_graphics2D->setColor(COLOR_RED);
		_graphics2D->fillArc(300.0, 400.0, 100.0, 200.0, -150.0, 90.0);

		_graphics2D->setColor(COLOR_GREEN);
		_graphics2D->drawLine(300.0, 50.0, 300.0, 0.0);

		_graphics2D->drawArc(300.0, 400.0, 100.0, 200.0, -150.0, 90.0);


		_graphics2D->translate(-400.0, 100.0);
		_graphics2D->rotate(-30.0);

		_graphics2D->setColor(COLOR_BLUE);
		_graphics2D->setStroke(_defaultStroke);
		_graphics2D->drawRect(350.0, 350.0, 200.0, 200.0);

//		_graphics2D->setColor(COLOR_RED);
//		_graphics2D->fillRect(400.0, 375.0, 200.0, 300.0);

		_graphics2D->setGradient(_grayscaleGradient);
		_graphics2D->fillRect(250.0, 425.0, 200.0, 300.0);

		_graphics2D->setColor(COLOR_YELLOW);
		_graphics2D->setStroke(_evenDashesStroke);
		_graphics2D->drawRect(400.0, 375.0, 200.0, 300.0);

		_graphics2D->setColor(COLOR_RED);
		_graphics2D->setStroke(_defaultStroke);
		double *triX = new double[3] { 425.0, 500.0, 600.0 };
		double *triY = new double[3] { 400.0, 475.0, 425.0 };
		_graphics2D->drawPolygon(triX, triY, 3);

		triX = new double[3] { 450.0, 450.0, 500.0 };
		triY = new double[3] { 425.0, 525.0, 450.0 };
		_graphics2D->setColor(COLOR_BLUE);
		_graphics2D->fillPolygon(triX, triY, 3);

		_graphics2D->setColor(COLOR_ORANGE);
		_graphics2D->setStroke(_evenDashesStroke);
		_graphics2D->drawPolygon(triX, triY, 3);

		_graphics2D->setColor(COLOR_RED);
		_graphics2D->setStroke(_defaultStroke);
		triX = new double[3] { 125.0, 200.0, 300.0 };
		triY = new double[3] { 400.0, 475.0, 425.0 };
		_graphics2D->drawPolyline(triX, triY, 3);

		_graphics2D->setColor(COLOR_ORANGE);
		_graphics2D->setStroke(_evenDashesStroke);
		triX = new double[3] { 150.0, 250.0, 300.0 };
		triY = new double[3] { 425.0, 525.0, 450.0 };
		_graphics2D->drawPolyline(triX, triY, 3);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->fillRoundRect(100.0, 350.0, 200.0, 200.0, 20.0, 20.0);

		_graphics2D->setColor(COLOR_YELLOW);
		_graphics2D->setStroke(_defaultStroke);
		_graphics2D->drawRoundRect(100.0, 350.0, 200.0, 200.0, 20.0, 20.0);

		_graphics2D->setColor(COLOR_ORANGE);
		_graphics2D->setStroke(_evenDashesStroke);
		_graphics2D->drawRoundRect(200.0, 475.0, 200.0, 300.0, 20.0, 20.0);

		_graphics2D->done();

		setStale(true);
	}
}

