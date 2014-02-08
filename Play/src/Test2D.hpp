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


#ifndef TEST2D_HPP
#define TEST2D_HPP

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

class Test2D : public View {

Q_OBJECT

public:
	Test2D(ViewDisplay display = DISPLAY_DEVICE);

	virtual ~Test2D();

	void update();

	void onAltered();
	void onVisible();

private:
	float*     _evenDashes;
	Stroke*    _defaultStroke;
	Stroke*    _evenDashesStroke;
	ImageData* _testImage;
	ImageData* _testImage1;
	Font*      _slatePro;
	Font*      _slateProLight;
	Color*     _grayScaleColors;
	float*     _grayscalePercentages;
	Gradient*  _grayscaleGradient;

	Graphics2D* _graphics2D;
};

#endif /* TEST2D_HPP */
