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


#ifndef PIECHART_HPP
#define PIECHART_HPP

#include <assert.h>
#include <screen/screen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "views/views.hpp"

using namespace views::base;
using namespace views::graphics;

class PieChart : public View {

Q_OBJECT

public:
	PieChart(ViewDisplay display = DISPLAY_DEVICE);

	virtual ~PieChart();

	void update();

	void onAltered();
	void onVisible();

private:
	Stroke*    _defaultStroke;
	Stroke*    _thinStroke;
	ImageData* _testImage;
	ImageData* _testImage1;
	Font*      _slatePro;
	Font*      _slateProSub;
	Font*      _slateProLight;
	float*     _pieGradientPercentages;
	GLColor*     _pieGradientRedCrimsonColors;
	GLColor*     _pieGradientGreenPineColors;
	GLColor*     _pieGradientBlueSlateColors;
	GLColor*     _pieGradientYellowOrangeColors;
	GLColor*     _pieGradientPurpleVioletColors;
	GLColor*     _pieGradientCyanAcquaColors;
	Gradient*  _pieGradientRedCrimson;
	Gradient*  _pieGradientGreenPine;
	Gradient*  _pieGradientBlueSlate;
	Gradient*  _pieGradientYellowOrange;
	Gradient*  _pieGradientPurpleViolet;
	Gradient*  _pieGradientCyanAcqua;

	Graphics2D* _graphics2D;
};

#endif /* PIECHART_HPP */
