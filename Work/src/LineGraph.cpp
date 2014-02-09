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

#include "LineGraph.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views::graphics;


LineGraph::LineGraph(ViewDisplay display) : View(display)
{
	qDebug()  << "LineGraph::LineGraph ";

	_graphics2D = new Graphics2D(display);

	_defaultStroke = _graphics2D->createStroke(2.0);
	_thinStroke = _graphics2D->createStroke(1.0);
	_evenDashes = new float[2] { 20.0, 20.0 };
	_evenDashesStroke = _graphics2D->createStroke(5.0, CAP_NONE, JOIN_NONE, 0.0, _evenDashes, 2, 0.0);
	_thickStroke = _graphics2D->createStroke(10.0, CAP_NONE, JOIN_NONE, 0.0, _evenDashes, 2, 0.0);

	_slatePro = NULL;
	_slateProSub = NULL;
	_slateProLight = NULL;

	// register graphics with base class
	registerGraphics(_graphics2D);
}

LineGraph::~LineGraph() {
	// TODO Auto-generated destructor stub
}

void LineGraph::update()
{
	//qDebug()  << "LineGraph::update";
}

void LineGraph::onVisible()
{
	qDebug()  << "LineGraph::onVisible";
}

void LineGraph::onRegenerated()
{
	qDebug()  << "LineGraph::onRegenerated";

	if (_graphics2D->reset()) {
		if (!_slatePro) {
			_slatePro = _graphics2D->createFont(FONT_NAME_SLATE_PRO_CONDENSED, NULL, 20, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,()%0123456789 eE"));
			_slateProSub = _graphics2D->createFont(FONT_NAME_SLATE_PRO_CONDENSED, NULL, 10, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,()%0123456789 eE"));
			_slateProLight = _graphics2D->createFont(FONT_NAME_SLATE_PRO_LIGHT, NULL, 10, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,()%0123456789 eE"));
			_slateProMedium = _graphics2D->createFont(FONT_NAME_SLATE_PRO_LIGHT, NULL, 10, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,()%0123456789 eE"));
		}

		// pie slices plus labels
		double textWidth = 0.0, textHeight = 0.0;

		_graphics2D->setFont(_slatePro);
		QString titleText("Sample Line Graph");
		_graphics2D->measureString(titleText, &textWidth, &textHeight);
		_graphics2D->drawString(titleText, (_width - textWidth) / 2.0, (_height - 100.0));

		double xMarkerValues[9] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0 };
		double yMarkerValues[4] = { 0.0, 50.0, 100.0, 150.0 };

		double xValues[9] = { 0.1, 0.4, 0.75, 1.2, 1.7, 2.05, 2.3, 3.4, 3.9 };

		double y1Values[9] = {  85.0, 105.0,  92.0, 110.0, 115.0, 103.0, 127.0, 150.0, 129.0 };
		double y2Values[9] = {  72.0, 110.0, 102.0,  99.0,  89.0,  94.0,  95.0,  89.0,  88.0 };
		double y3Values[9] = {  93.0,  97.0,  95.0, 103.0, 101.0, 112.0, 105.0,  98.0,  99.0 };
		double y4Values[9] = { 112.0,  89.0,  97.0, 100.0, 125.0, 120.0, 105.0, 101.0, 107.0 };

		double originX = 150.0, originY = 350.0, xScale = 500.0 / 4.0, yScale = 550.0 / 160.0;


		// markers and labels for axes
		for (int index = 0; index < 9; index++) {
			double markerX = originX + xMarkerValues[index] * xScale;

			if (index > 0 && !(index & 1)) {
				for (int index1 = 1; index1 < 4; index1++) {
					double markerX1 = originX + (xMarkerValues[index-2] + index1 / 4.0) * xScale;
					_graphics2D->setColor(COLOR_GREY);
					_graphics2D->setStroke(_thinStroke);
					_graphics2D->drawLine(markerX1, originY, markerX1, originY + 15.0);
				}
			}

			if (!(index & 1)) {
				_graphics2D->setColor(COLOR_BLACK);
				_graphics2D->setStroke(_defaultStroke);
				_graphics2D->drawLine(markerX, originY - 20.0, markerX, originY + 20.0);
			}

			if (!(index & 1)) {
				_graphics2D->setFont(_slateProLight);
				QString markerText = QString::number(xMarkerValues[index]);
				_graphics2D->measureString(markerText, &textWidth, &textHeight);
				_graphics2D->drawString(markerText, markerX - textWidth/2.0, originY - 25.0 - textHeight);
			}
		}
		_graphics2D->setFont(_slateProSub);
		QString xAxisText("time (hours)");
		_graphics2D->measureString(xAxisText, &textWidth, &textHeight);
		_graphics2D->drawString(xAxisText, originX + xScale * 2.0 - textWidth / 2.0, originY - 110.0);

		for (int index = 0; index < 4; index++) {
			double markerY = originY + yMarkerValues[index] * yScale;

			if (index > 0) {
				for (int index1 = 1; index1 < 5; index1++) {
					double markerY = originY + (yMarkerValues[index-1] + 50.0 * index1 / 5.0) * yScale;
					_graphics2D->setColor(COLOR_GREY);
					_graphics2D->setStroke(_thinStroke);
					_graphics2D->drawLine(originX, markerY, originX + 15.0, markerY);
				}
			}

			_graphics2D->setColor(COLOR_BLACK);
			_graphics2D->setStroke(_defaultStroke);
			_graphics2D->drawLine(originX - 20.0, markerY, originX + 20.0, markerY);

			if (index > 0) {
				_graphics2D->setFont(_slateProLight);
				QString markerText = QString::number(yMarkerValues[index]);
				_graphics2D->measureString(markerText, &textWidth, &textHeight);
				_graphics2D->drawString(markerText, originX - textWidth - 30.0, markerY - textHeight / 2.0);
			}
		}

		_graphics2D->rotate(90.0);

		_graphics2D->setFont(_slateProSub);
		QString yAxisText("voltage (V)");
		_graphics2D->measureString(yAxisText, &textWidth, &textHeight);
		_graphics2D->drawString(yAxisText, originY + 80.0 * yScale - textWidth / 2.0, -_width + 20.0);

		_graphics2D->rotate(-90.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_defaultStroke);
		_graphics2D->drawLine(originX, originY, originX + xScale * 4.3, originY);
		_graphics2D->drawLine(originX, originY, originX, originY + yScale * 160.0);
		_graphics2D->drawLine(originX + xScale * 4.3, originY, originX + xScale * 4.3, originY + yScale * 160.0);


		// lines for graph

		double lastX, lastY1, lastY2, lastY3, lastY4;
		for (int index = 0; index < 9; index++) {
			double lineX  = originX + xValues[index] * xScale;
			double lineY1 = originY + y1Values[index] * yScale;
			double lineY2 = originY + y2Values[index] * yScale;
			double lineY3 = originY + y3Values[index] * yScale;
			double lineY4 = originY + y4Values[index] * yScale;

			if (index > 0) {
				_graphics2D->setColor(COLOR_BLUE);
				_graphics2D->setStroke(_defaultStroke);
				_graphics2D->drawLine(lastX, lastY1, lineX, lineY1);

				_graphics2D->setColor(COLOR_RED);
				_graphics2D->setStroke(_thickStroke);
				_graphics2D->drawLine(lastX, lastY2, lineX, lineY2);

				_graphics2D->setColor(COLOR_BLACK);
				_graphics2D->setStroke(_thinStroke);
				_graphics2D->drawLine(lastX, lastY3, lineX, lineY3);

				_graphics2D->setColor(COLOR_GREEN);
				_graphics2D->setStroke(_evenDashesStroke);
				_graphics2D->drawLine(lastX, lastY4, lineX, lineY4);
			}

			lastX  = lineX;
			lastY1 = lineY1;
			lastY2 = lineY2;
			lastY3 = lineY3;
			lastY4 = lineY4;
		}


		// Legend

		_graphics2D->setColor(COLOR_BLACK);

		_graphics2D->setFont(_slateProSub);
		QString legendTitleText("Legend");
		_graphics2D->drawString(legendTitleText, 50.0, 170.0);

		_graphics2D->setFont(_slateProLight);
/*
		_graphics2D->setGradient(_pieGradientYellowOrange);
		_graphics2D->fillRect(50.0, 20.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(50.0, 20.0, 50.0, 50.0);
*/
		_graphics2D->setColor(COLOR_BLUE);
		_graphics2D->setStroke(_defaultStroke);
		_graphics2D->drawLine(50.0, 40.0, 100.0, 40.0);

		_graphics2D->setColor(COLOR_BLACK);
		QString legendText1("item 1");
		_graphics2D->drawString(legendText1, 120.0, 30.0);

/*
		_graphics2D->setGradient(_pieGradientRedCrimson);
		_graphics2D->fillRect(50.0, 90.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(50.0, 90.0, 50.0, 50.0);
*/
		_graphics2D->setColor(COLOR_RED);
		_graphics2D->setStroke(_thickStroke);
		_graphics2D->drawLine(50.0, 110.0, 100.0, 110.0);

		_graphics2D->setColor(COLOR_BLACK);
		QString legendText2("item 2");
		_graphics2D->drawString(legendText2, 120.0, 100.0);


/*
		_graphics2D->setGradient(_pieGradientBlueSlate);
		_graphics2D->fillRect(_width / 2.0 + 50.0, 20.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(_width / 2.0 + 50.0, 20.0, 50.0, 50.0);
*/

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawLine(_width / 2.0 + 50.0, 60.0, _width / 2.0 + 100.0, 60.0);

		_graphics2D->setColor(COLOR_BLACK);
		QString legendText3("item 3");
		_graphics2D->drawString(legendText3, _width / 2.0 + 120.0, 30.0);

/*
		_graphics2D->setGradient(_pieGradientPurpleViolet);
		_graphics2D->fillRect(_width / 2.0 + 50.0, 90.0, 50.0, 50.0);

		_graphics2D->setColor(COLOR_BLACK);
		_graphics2D->setStroke(_thinStroke);
		_graphics2D->drawRect(_width / 2.0 + 50.0, 90.0, 50.0, 50.0);
*/

		_graphics2D->setColor(COLOR_GREEN);
		_graphics2D->setStroke(_evenDashesStroke);
		_graphics2D->drawLine(_width / 2.0 + 50.0, 110.0, _width / 2.0 + 100.0, 110.0);

		_graphics2D->setColor(COLOR_BLACK);
		QString legendText4("item 4");
		_graphics2D->drawString(legendText4, _width / 2.0 + 120.0, 100.0);



		_graphics2D->done();

		setStale(true);
	}
}

