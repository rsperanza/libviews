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

#include "PieChart.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views::graphics;


PieChart::PieChart(ViewDisplay display) : View(display)
{
	qDebug()  << "PieChart::PieChart ";

	_graphics2D = new Graphics2D(display);

	_defaultStroke = _graphics2D->createStroke(2.0);
	_thinStroke = _graphics2D->createStroke(1.0);

	_slatePro = NULL;
	_slateProSub = NULL;
	_slateProLight = NULL;

	//_testImage = _graphics2D->loadImage("app/native/assets/images/handle_inactive.png");
	//_testImage1 = _graphics2D->loadImage("app/native/assets/images/handle_pressed.png");

	_pieGradientPercentages = new float[3] { 0.0, 0.45, 1.0 };
    _pieGradientRedCrimsonColors = new GLColor[2] { { 1.0, 0.0, 0.0, 1.0 }, { 0.75, 0.0, 0.0, 1.0 } };
    _pieGradientGreenPineColors = new GLColor[2] { { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.75, 0.0, 1.0 } };
    _pieGradientBlueSlateColors = new GLColor[2] { { 0.0, 0.0, 1.0, 1.0 }, { 0.0, 0.0, 0.75, 1.0 } };
    _pieGradientYellowOrangeColors = new GLColor[2] { { 1.0, 1.0, 0.0, 1.0 }, { 1.0, 0.75, 0.0, 1.0 } };
    _pieGradientPurpleVioletColors = new GLColor[2] { { 1.0, 0.0, 1.0, 1.0 }, { 0.75, 0.0, 0.75, 1.0 } };
    _pieGradientCyanAcquaColors = new GLColor[2] { { 0.0, 1.0, 1.0, 1.0 }, { 0.0, 0.75, 0.75, 1.0 } };

	_pieGradientRedCrimson = _graphics2D->createGradient(1, _pieGradientRedCrimsonColors, _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);
	_pieGradientGreenPine = _graphics2D->createGradient(1, _pieGradientGreenPineColors, _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);
	_pieGradientBlueSlate = _graphics2D->createGradient(1, _pieGradientBlueSlateColors, _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);
	_pieGradientYellowOrange = _graphics2D->createGradient(1, _pieGradientYellowOrangeColors, _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);
	_pieGradientPurpleViolet = _graphics2D->createGradient(1, _pieGradientPurpleVioletColors, _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);
	_pieGradientCyanAcqua = _graphics2D->createGradient(1, _pieGradientCyanAcquaColors, _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);

	// register graphics with base class
	registerGraphics(_graphics2D);
}

PieChart::~PieChart() {
	// TODO Auto-generated destructor stub
}

void PieChart::update()
{
	//qDebug()  << "PieChart::update";
}

void PieChart::onVisible()
{
	qDebug()  << "PieChart::onVisible";
}

void PieChart::onAltered()
{
	qDebug()  << "PieChart::onAltered";

	if (_graphics2D->reset()) {
		//_graphics2D->setBackground(COLOR_YELLOW);
		//_graphics2D->clearRect(0, 0, 100, 100);

		//_graphics2D->shear(0.25, 0.0);

		if (!_slatePro) {
			_slatePro = _graphics2D->createFont(FONT_NAME_SLATE_PRO_CONDENSED, NULL, 20, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
			_slateProSub = _graphics2D->createFont(FONT_NAME_SLATE_PRO_CONDENSED, NULL, 10, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
			_slateProLight = _graphics2D->createFont(FONT_NAME_SLATE_PRO_LIGHT, NULL, 10, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
		}

		// pie slices plus labels
		double textWidth = 0.0, textHeight = 0.0;

		_graphics2D->setFont(_slatePro);
		QString titleText("Sample Pie Chart");
		_graphics2D->measureString(titleText, &textWidth, &textHeight);
		_graphics2D->drawString(titleText, (_width - textWidth) / 2.0, (_height - 100.0));

		_graphics2D->setGradient(_pieGradientYellowOrange);
		_graphics2D->fillArc(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375, 0.0, 60.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setFont(_slateProLight);
		QString qtyText1("16.6 %");
		_graphics2D->measureString(qtyText1, &textWidth, &textHeight);
		_graphics2D->drawString(qtyText1, _width / 2.0 + cos(30.0 * M_PI / 180.0) * _width *.25 - textWidth / 2.0,
										 _height / 2.0 + 100.0 + sin(30.0 * M_PI / 180.0) * _width *.25 - textHeight / 2.0);


		_graphics2D->setGradient(_pieGradientRedCrimson);
		_graphics2D->fillArc(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375, 60.0, 50.0);

		_graphics2D->setColor(COLOR_WHITE);
		_graphics2D->setFont(_slateProLight);
		QString qtyText2("15 %");
		_graphics2D->measureString(qtyText2, &textWidth, &textHeight);
		_graphics2D->drawString(qtyText2, _width / 2.0 + cos(85.0 * M_PI / 180.0) * _width *.25 - textWidth / 2.0,
										 _height / 2.0 + 100.0 + sin(85.0 * M_PI / 180.0) * _width *.25 - textHeight / 2.0);

		_graphics2D->setGradient(_pieGradientGreenPine);
		_graphics2D->fillArc(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375, 110.0, 30.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setFont(_slateProLight);
		QString qtyText3("8.3 %");
		_graphics2D->measureString(qtyText3, &textWidth, &textHeight);
		_graphics2D->drawString(qtyText3, _width / 2.0 + cos(125.0 * M_PI / 180.0) * _width *.25 - textWidth / 2.0,
										 _height / 2.0 + 100.0 + sin(125.0 * M_PI / 180.0) * _width *.25 - textHeight / 2.0);

		_graphics2D->setGradient(_pieGradientBlueSlate);
		_graphics2D->fillArc(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375, 140.0, 70.0);

		_graphics2D->setColor(COLOR_WHITE);
		_graphics2D->setFont(_slateProLight);
		QString qtyText4("19 %");
		_graphics2D->measureString(qtyText4, &textWidth, &textHeight);
		_graphics2D->drawString(qtyText4, _width / 2.0 + cos(175.0 * M_PI / 180.0) * _width *.25 - textWidth / 2.0,
										 _height / 2.0 + 100.0 + sin(175.0 * M_PI / 180.0) * _width *.25 - textHeight / 2.0);

		_graphics2D->setGradient(_pieGradientPurpleViolet);
		_graphics2D->fillArc(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375, 210.0, 20.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setFont(_slateProLight);
		QString qtyText5("4.1 %");
		_graphics2D->measureString(qtyText5, &textWidth, &textHeight);
		_graphics2D->drawString(qtyText5, 50.0, 340.0);

		_graphics2D->setStroke(_thinStroke);
		double* lineX = new double[3] { 100.0, 100.0, _width / 2.0 + cos(220.0 * M_PI / 180.0) * _width * 0.25 };
		double* lineY = new double[3] { 400.0, 450.0, _height / 2.0 + 100.0 + sin(220.0 * M_PI / 180.0) * _width * 0.25 };
		_graphics2D->drawPolyline(lineX, lineY, 3);

		_graphics2D->setGradient(_pieGradientCyanAcqua);
		_graphics2D->fillArc(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375, 230.0, 130.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setFont(_slateProLight);
		QString qtyText6("30 %");
		_graphics2D->measureString(qtyText6, &textWidth, &textHeight);
		_graphics2D->drawString(qtyText6, _width / 2.0 + cos(292.0 * M_PI / 180.0) * _width *.25 - textWidth / 2.0,
										 _height / 2.0 + 100.0 + sin(292.0 * M_PI / 180.0) * _width *.25 - textHeight / 2.0);

		// outlines
		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(0.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(0.0 * M_PI / 180.0) * _width *.375);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(60.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(60.0 * M_PI / 180.0) * _width *.375);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(110.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(110.0 * M_PI / 180.0) * _width *.375);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(140.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(140.0 * M_PI / 180.0) * _width *.375);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(210.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(210.0 * M_PI / 180.0) * _width *.375);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(230.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(230.0 * M_PI / 180.0) * _width *.375);
		_graphics2D->drawLine(_width / 2.0, _height / 2.0 + 100.0,
							  _width / 2.0 + cos(360.0 * M_PI / 180.0) * _width *.375,
							  _height / 2.0 + 100.0 + sin(360.0 * M_PI / 180.0) * _width *.375);

		_graphics2D->setStroke(_defaultStroke);
		_graphics2D->drawOval(_width / 2.0, _height / 2.0 + 100.0, _width *.375, _width *.375);


		// Legend

		_graphics2D->setFont(_slateProSub);
		QString legendTitleText("Legend");
		_graphics2D->drawString(legendTitleText, 50.0, 240.0);

		_graphics2D->setFont(_slateProLight);

		_graphics2D->setGradient(_pieGradientYellowOrange);
		_graphics2D->fillRect(50.0, 20.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(50.0, 20.0, 50.0, 50.0);

		QString legendText1("item 1");
		_graphics2D->drawString(legendText1, 120.0, 30.0);


		_graphics2D->setGradient(_pieGradientRedCrimson);
		_graphics2D->fillRect(50.0, 90.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(50.0, 90.0, 50.0, 50.0);

		QString legendText2("item 2");
		_graphics2D->drawString(legendText2, 120.0, 100.0);


		_graphics2D->setGradient(_pieGradientGreenPine);
		_graphics2D->fillRect(50.0, 160.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(50.0, 160.0, 50.0, 50.0);

		QString legendText3("item 3");
		_graphics2D->drawString(legendText3, 120.0, 170.0);


		_graphics2D->setGradient(_pieGradientBlueSlate);
		_graphics2D->fillRect(_width / 2.0 + 50.0, 20.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(_width / 2.0 + 50.0, 20.0, 50.0, 50.0);

		QString legendText4("item 4");
		_graphics2D->drawString(legendText4, _width / 2.0 + 120.0, 30.0);


		_graphics2D->setGradient(_pieGradientPurpleViolet);
		_graphics2D->fillRect(_width / 2.0 + 50.0, 90.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(_width / 2.0 + 50.0, 90.0, 50.0, 50.0);

		QString legendText5("item 5");
		_graphics2D->drawString(legendText5, _width / 2.0 + 120.0, 100.0);


		_graphics2D->setGradient(_pieGradientCyanAcqua);
		_graphics2D->fillRect(_width / 2.0 + 50.0, 160.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(_width / 2.0 + 50.0, 160.0, 50.0, 50.0);

		QString legendText6("item 6");
		_graphics2D->drawString(legendText6, _width / 2.0 + 120.0, 170.0);


		_graphics2D->done();

		setStale(true);
	}
}

