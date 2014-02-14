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

#include "VideoOverlay.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views::graphics;


VideoOverlay::VideoOverlay(ViewDisplay display) : VideoView(display)
{
	qDebug()  << "VideoOverlay::VideoOverlay ";

	_leftPadding   = 20;
	_rightPadding  = 20;
	_topPadding    = 20;
	_bottomPadding = 20;

	_evenDashes = new float[2] { 20.0, 10.0 };
	_evenDashesStroke = _graphics2D->createStroke(5.0, CAP_NONE, JOIN_NONE, 0.0, _evenDashes, 2, 0.0);
	_defaultStroke = _graphics2D->createStroke(5.0);
	_lineColor = new GLColor { 1.0f, 1.0f, 0.5f, 0.75f };
}

VideoOverlay::~VideoOverlay() {
	// TODO Auto-generated destructor stub
}

void VideoOverlay::update()
{
	//qDebug()  << "VideoOverlay::update";
}

void VideoOverlay::onRegenerated()
{
	qDebug()  << "VideoOverlay::onRegenerated";

	VideoView::onRegenerated();

	if (_screenVideoWindow) {
		if (_graphics2D->reset()) {
			_graphics2D->setBackground(COLOR_MULTITOUCH_TRANSPARENT);

			_graphics2D->clearRect(0.0, 0.0, (double)_width, (double)_height);

			// cross-hairs
			_graphics2D->setColor(*_lineColor);
			_graphics2D->setStroke(_defaultStroke);

			float crossHairSize = _height / 4.0;
			_graphics2D->drawLine(_width/2.0, _height/2.0 + crossHairSize/2.0, _width/2.0, _height/2.0 + crossHairSize);
			_graphics2D->drawLine(_width/2.0, _height/2.0 - crossHairSize/2.0, _width/2.0, _height/2.0 - crossHairSize);
			_graphics2D->drawLine(_width/2.0 + crossHairSize/2.0, _height/2.0, _width/2.0 + crossHairSize, _height/2.0);
			_graphics2D->drawLine(_width/2.0 - crossHairSize/2.0, _height/2.0, _width/2.0 - crossHairSize, _height/2.0);

			_graphics2D->drawOval(_width/2.0, _height/2.0, crossHairSize*.75, crossHairSize*.75);

			// corner brackets
			double leftCornersX[3] { _leftPadding + _width/20.0, _leftPadding, _leftPadding };
			double topCornersY[3]  { _height - _topPadding, _height - _topPadding, _height - _topPadding - _height/20.0 };
			double rightCornersX[3] { _width - _rightPadding - _width/20.0, _width - _rightPadding, _width - _rightPadding };
			double bottomCornersY[3]  { _bottomPadding, _bottomPadding, _bottomPadding + _height/20.0 };

			_graphics2D->setStroke(_evenDashesStroke);
			_graphics2D->drawPolyline(leftCornersX, topCornersY, 3);
			_graphics2D->drawPolyline(rightCornersX, topCornersY, 3);
			_graphics2D->drawPolyline(leftCornersX, bottomCornersY, 3);
			_graphics2D->drawPolyline(rightCornersX, bottomCornersY, 3);

			_graphics2D->done();

			setStale(true);
		}
	}
}

