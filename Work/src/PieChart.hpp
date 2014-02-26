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

Q_PROPERTY(qreal leftPadding   READ leftPadding   WRITE setLeftPadding)
Q_PROPERTY(qreal rightPadding  READ rightPadding  WRITE setRightPadding)
Q_PROPERTY(qreal topPadding    READ topPadding    WRITE setTopPadding)
Q_PROPERTY(qreal bottomPadding READ bottomPadding WRITE setBottomPadding)

Q_PROPERTY(QString caption READ caption WRITE setCaption)
Q_PROPERTY(QString captionFont READ captionFont WRITE setCaptionFont)
Q_PROPERTY(qreal  captionFontSize  READ captionFontSize WRITE setCaptionFontSize)
Q_PROPERTY(QString captionPlacement READ captionPlacement WRITE setCaptionPlacement)
Q_PROPERTY(QString captionAlignment READ captionAlignment WRITE setCaptionAlignment)

Q_PROPERTY(QString subCaption READ subCaption WRITE setSubCaption)
Q_PROPERTY(QString subCaptionFont READ subCaptionFont WRITE setSubCaptionFont)
Q_PROPERTY(qreal  subCaptionFontSize  READ subCaptionFontSize WRITE setSubCaptionFontSize)

Q_PROPERTY(bool showCaption READ showCaption WRITE setShowCaption)

Q_PROPERTY(QString legendTitleFont READ legendTitleFont WRITE setLegendTitleFont)
Q_PROPERTY(qreal  legendTitleFontSize  READ legendTitleFontSize WRITE setLegendTitleFontSize)
Q_PROPERTY(QString legendFont READ legendFont WRITE setLegendFont)
Q_PROPERTY(qreal  legendFontSize  READ legendFontSize WRITE setLegendFontSize)
Q_PROPERTY(QString legendPlacement READ legendPlacement WRITE setLegendPlacement)
Q_PROPERTY(bool showLegend READ showLegend WRITE setShowLegend)

Q_PROPERTY(bool showSliceLabel READ showSliceLabel WRITE setShowSliceLabel)
Q_PROPERTY(QString sliceLabelPlacement READ sliceLabelPlacement WRITE setSliceLabelPlacement)
Q_PROPERTY(QString sliceLabelFont READ sliceLabelFont WRITE setSliceLabelFont)
Q_PROPERTY(qreal  sliceLabelFontSize  READ sliceLabelFontSize WRITE setSliceLabelFontSize)

Q_PROPERTY(QVariantList itemColors READ itemColors WRITE setItemColors)
Q_PROPERTY(QVariantList itemLabels READ itemLabels WRITE setItemLabels)
Q_PROPERTY(QVariantList itemValues READ itemValues WRITE setItemValues)


public:
	PieChart(ViewDisplay display = DISPLAY_DEVICE);

	virtual ~PieChart();

	void update();


	qreal leftPadding();
	qreal rightPadding();
	qreal topPadding();
	qreal bottomPadding();

	QString caption();
	QString captionFont();
	qreal  captionFontSize();
	QString captionPlacement();
	QString captionAlignment();

	QString subCaption();
	QString subCaptionFont();
	qreal  subCaptionFontSize();

	bool showCaption();

	QString legendTitleFont();
	qreal  legendTitleFontSize();
	QString legendFont();
	qreal  legendFontSize();
	QString legendPlacement();
	bool showLegend();

	bool showSliceLabel();
	QString sliceLabelPlacement();
	QString sliceLabelFont();
	qreal  sliceLabelFontSize();

	QVariantList itemColors();
	QVariantList itemLabels();
	QVariantList itemValues();



public Q_SLOTS:
	void onRegenerated();
	void onVisible();

	void setLeftPadding(qreal leftPadding);
	void setRightPadding(qreal rightPadding);
	void setTopPadding(qreal topPadding);
	void setBottomPadding(qreal bottomPadding);

	void setCaption(QString caption);
	void setCaptionFont(QString captionFont);
	void setCaptionFontSize(qreal captionFontSize);
	void setCaptionPlacement(QString captionPlacement);
	void setCaptionAlignment(QString captionAlignment);

	void setSubCaption(QString subCaption);
	void setSubCaptionFont(QString subCaptionFont);
	void setSubCaptionFontSize(qreal subCaptionFontSize);

	void setShowCaption(bool showCaption);

	void setLegendTitleFont(QString legendTitleFont);
	void setLegendTitleFontSize(qreal legendTitleFontSize);
	void setLegendFont(QString legendFont);
	void setLegendFontSize(qreal  legendFontSize);
	void setLegendPlacement(QString legendPlacement);
	void setShowLegend(bool showLegend);

	void setShowSliceLabel(bool showSliceLabel);
	void setSliceLabelPlacement(QString sliceLabelPlacement);
	void setSliceLabelFont(QString sliceLabelFont);
	void setSliceLabelFontSize(qreal  sliceLabelFontSize);

	void setItemColors(QVariantList itemColors);
	void setItemLabels(QVariantList itemValues);
	void setItemValues(QVariantList itemValues);


private:


	Stroke*    _defaultStroke;
	Stroke*    _thinStroke;

	float*     _pieGradientPercentages;

	double       _leftPadding;
	double       _rightPadding;
	double       _topPadding;
	double       _bottomPadding;

	double       _captionX;
	double       _captionY;
	Font*        _captionFont;
	double       _subCaptionX;
	double       _subCaptionY;
	Font*        _subCaptionFont;

	double       _legendTitleX;
	double       _legendTitleY;
	double       _legendTopItemX;
	double       _legendTopItemY;
	double       _legendItemBoxSize;
	double       _legendItemHeight;

	double       _pieCenterX;
	double       _pieCenterY;
	double       _pieRadius;

	GLColor*      _itemColors;
	double*		  _itemPercentages;
	Gradient**    _itemGradients;

	QString      _caption;
	QString      _captionFontFile;
	double        _captionFontSize;
	QString      _captionPlacement;
	QString      _captionAlignment;

	QString      _subCaption;
	QString      _subCaptionFontFile;
	qreal        _subCaptionFontSize;

	bool         _showCaption;

	QString      _legendTitleFontFile;
	double       _legendTitleFontSize;
	Font*        _legendTitleFont;
	QString      _legendFontFile;
	double       _legendFontSize;
	Font*        _legendFont;
	QString      _legendPlacement;
	bool         _showLegend;

	bool         _showSliceLabel;
	QString      _sliceLabelPlacement;
	QString      _sliceLabelFontFile;
	double       _sliceLabelFontSize;
	Font*        _sliceLabelFont;

	QVariantList _itemColorsList;
	QVariantList _itemLabels;
	QVariantList _itemValuesList;

	Graphics2D* _graphics2D;
};

#endif /* PIECHART_HPP */
