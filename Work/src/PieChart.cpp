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

	// disable text by default
	_showCaption = false;
	_showLegend = false;
	_showSliceLabel = false;

    _leftPadding = 0.0;
    _rightPadding = 0.0;
    _topPadding = 0.0;
    _bottomPadding = 0.0;

	_captionFont = NULL;

	_itemColors = NULL;
	_itemPercentages = NULL;
	_itemGradients = NULL;

	_defaultStroke = _graphics2D->createStroke(3.0);
	_thinStroke = _graphics2D->createStroke(2.0);

	_pieGradientPercentages = new float[3] { 0.0, 0.45, 1.0 };

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

void PieChart::onRegenerated()
{
	qDebug()  << "PieChart::onRegenerated";

	if (_graphics2D->reset()) {
		double textWidth = 0.0, textHeight = 0.0;
		double availWidth = _width - _leftPadding - _rightPadding, availHeight = _height - _topPadding - _bottomPadding,
			   availX = _leftPadding, availY = _bottomPadding;

		// caption
		if (_showCaption) {
			if (!_captionFont) {
				_captionFont = _graphics2D->createFont(_captionFontFile, NULL, _captionFontSize, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
			}
			_graphics2D->setFont(_captionFont);
			_graphics2D->measureString(_caption, &textWidth, &textHeight);

			_captionX = _leftPadding;
			if (_captionAlignment == "left") {
				_captionX = _leftPadding;
			} else
			if (_captionAlignment == "right") {
				_captionX = _width - _rightPadding - textWidth;
			} else
			if (_captionAlignment == "center") {
				_captionX = _leftPadding + (_width - textWidth - _leftPadding - _rightPadding) / 2.0;
			}

			if (_captionPlacement == "top") {
				_captionY = _height - textHeight - _topPadding;

				availHeight -= (textHeight) / 2.0;
			} else
			if (_captionPlacement == "bottom") {
				_captionY = _bottomPadding;

				availHeight -= (textHeight) / 2.0;
				availY += textHeight;
			}

			_graphics2D->drawString(_caption, _captionX, _captionY);

			// subcaption
			if (_subCaption.size() > 0) {
				if (!_subCaptionFont) {
					_subCaptionFont = _graphics2D->createFont(_subCaptionFontFile, NULL, _subCaptionFontSize, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
				}
				_graphics2D->setFont(_subCaptionFont);
				_graphics2D->measureString(_subCaption, &textWidth, &textHeight);

				_subCaptionX = _leftPadding;
				if (_captionAlignment == "left") {
					_subCaptionX = _leftPadding;
				} else
				if (_captionAlignment == "right") {
					_subCaptionX = _width - _rightPadding - textWidth;
				} else
				if (_captionAlignment == "center") {
					_subCaptionX = _leftPadding + (_width - textWidth - _leftPadding - _rightPadding) / 2.0;
				}

				if (_captionPlacement == "top") {
					_subCaptionY = _captionY - textHeight * 1.25;

					availHeight -= (textHeight * 1.25) / 2.0;
				} else
				if (_captionPlacement == "bottom") {
					_subCaptionY = _bottomPadding;
					_captionY = _subCaptionY + textHeight * 1.25;

					availHeight -= (textHeight * 1.25) / 2.0;
					availY += textHeight * 1.25;
				}

				_graphics2D->drawString(_subCaption, _subCaptionX, _subCaptionY);

			}
		}

		// Legend

		if (_showLegend) {
			if (!_legendTitleFont) {
				_legendTitleFont = _graphics2D->createFont(_legendTitleFontFile, NULL, _legendTitleFontSize, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
			}
			if (!_legendFont) {
				_legendFont = _graphics2D->createFont(_legendFontFile, NULL, _legendFontSize, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
			}

			double maxItemWidth = 0.0,  maxItemHeight = 0.0, maxTitleHeight = 0.0;
			int columns = 0, rows = 0;

			QString legendTitle("Legend");

			_graphics2D->setFont(_legendTitleFont);
			_graphics2D->measureString(legendTitle, &textWidth, &textHeight);
			maxTitleHeight = textHeight;

			if (_legendPlacement == "left" || _legendPlacement == "right") {
				_graphics2D->setFont(_legendFont);
				for(int index = 0; index < _itemLabels.size(); index++) {
					_graphics2D->measureString(_itemLabels[index].toString(), &textWidth, &textHeight);

					textWidth += 60.0;
					textHeight += 20.0;

					if (textWidth > maxItemWidth) {
						maxItemWidth = textWidth;
					}
					if (textHeight > maxItemHeight) {
						maxItemHeight = textHeight;
					}
				}

				columns = 1;
				rows = _itemLabels.size();
			} else
			if (_legendPlacement == "top" || _legendPlacement == "bottom") {
				_graphics2D->setFont(_legendFont);
				for(int index = 0; index < _itemLabels.size(); index++) {
					_graphics2D->measureString(_itemLabels[index].toString(), &textWidth, &textHeight);

					textWidth += 60.0;
					textHeight += 20.0;

					if (textWidth > maxItemWidth) {
						maxItemWidth = textWidth;
					}
					if (textHeight > maxItemHeight) {
						maxItemHeight = textHeight;
					}
				}

				qDebug()  << "PieChart::onRegenerated: item info: " << rows << " : " << columns << " : " << availWidth << " : " << maxItemWidth;

				columns = 1;
				double rowWidth = 0.0;
				for(int index = 0; index < _itemLabels.size(); index++) {
					rowWidth += maxItemWidth;
					if ((rowWidth + maxItemWidth) > _width) {
						break;
					}
					columns++;
				}

				qDebug()  << "PieChart::onRegenerated: item info: " << rows << " : " << columns << " : " << availWidth << " : " << maxItemWidth;

				rows = 1;
				for(int index = 0; index < _itemLabels.size(); index++) {
					if (index > 0 && index % columns == 0) {
						rows++;
					}
				}

				qDebug()  << "PieChart::onRegenerated: item info: " << rows << " : " << columns << " : " << availWidth << " : " << maxItemWidth;

				maxItemWidth = availWidth / columns;
			}


			if (_legendPlacement == "left") {
				availWidth -= maxItemWidth;
				availX += maxItemWidth;
			} else
			if (_legendPlacement == "right") {
				availWidth -= maxItemWidth;
			} else
			if (_legendPlacement == "top") {
				availHeight -= maxTitleHeight + rows * maxItemHeight;
			} else
			if (_legendPlacement == "bottom") {
				availHeight -= maxTitleHeight + rows * maxItemHeight;
				availY += maxTitleHeight + rows * maxItemHeight + 10.0;
			}

			if (_legendPlacement == "left") {
				_legendTitleX = availX;
				_legendTitleY = availY + availHeight - maxTitleHeight;
				_legendTopItemX = availX;
				_legendTopItemY = availY + availHeight - maxTitleHeight - maxItemHeight;
				_legendItemBoxSize = 50.0;
				_legendItemHeight = maxItemHeight - 10.0;
			} else
			if (_legendPlacement == "right") {
				_legendTitleX = availX + availWidth - maxItemWidth;
				_legendTitleY = availY + availHeight - maxTitleHeight;
				_legendTopItemX = availX + availWidth - maxItemWidth;
				_legendTopItemY = availY + availHeight - maxTitleHeight - maxItemHeight;
				_legendItemBoxSize = 50.0;
				_legendItemHeight = maxItemHeight - 10.0;
			} else
			if (_legendPlacement == "top") {
				_legendTitleX = availX;
				_legendTitleY = _bottomPadding + availHeight - maxTitleHeight;
				_legendTopItemX = availX;
				_legendTopItemY = _bottomPadding + availHeight - maxTitleHeight - maxItemHeight;
				_legendItemBoxSize = 50.0;
				_legendItemHeight = maxItemHeight - 10.0;
			} else
			if (_legendPlacement == "bottom") {
				_legendTitleX = availX;
				_legendTitleY = _bottomPadding + maxTitleHeight + rows * maxItemHeight;
				_legendTopItemX = availX;
				_legendTopItemY = _bottomPadding + (rows-1) * maxItemHeight;
				_legendItemBoxSize = 50.0;
				_legendItemHeight = maxItemHeight - 10.0;
			}



			_graphics2D->setFont(_legendTitleFont);
			_graphics2D->drawString(legendTitle, _legendTitleX, _legendTitleY);

			_graphics2D->setFont(_legendFont);

			int row = 0, column = 0;
			for(int index = 0; index < _itemLabels.size(); index++) {
				if (_itemGradients) {
					_graphics2D->setGradient(_itemGradients[index]);
				} else {
					_graphics2D->setColor(_itemColors[index]);
				}

				_graphics2D->fillRect(_legendTopItemX + column * maxItemWidth, _legendTopItemY - row * maxItemHeight, _legendItemBoxSize, _legendItemHeight);

				_graphics2D->setColor(COLOR_BLACK);
				_graphics2D->setStroke(_thinStroke);
				_graphics2D->drawRect(_legendTopItemX + column * maxItemWidth, _legendTopItemY - row * maxItemHeight, _legendItemBoxSize, _legendItemHeight);

				_graphics2D->drawString(_itemLabels[index].toString(), _legendTopItemX + column * maxItemWidth + _legendItemBoxSize + 10.0, _legendTopItemY - row * maxItemHeight + 10.0);

				if (_legendPlacement == "left" || _legendPlacement == "right") {
					row++;
				} else
				if (_legendPlacement == "top" || _legendPlacement == "bottom") {
					column++;
					if (column % columns == 0) {
						row++;
						column = 0;
					}
				}
			}
		}

		if (_showSliceLabel) {
			if (!_sliceLabelFont) {
				_sliceLabelFont = _graphics2D->createFont(_sliceLabelFontFile, NULL, _sliceLabelFontSize, calculateDPI(), new QString("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-,%0123456789 eE"));
			}

			_graphics2D->setFont(_sliceLabelFont);
			_graphics2D->measureString("100.0 %", &textWidth, &textHeight);

			if (_sliceLabelPlacement == "vertical") {
				availHeight -= (textHeight * 2.5);
				availY += textHeight * 1.25;
			} else if (_sliceLabelPlacement == "horizontal") {
				availWidth -= (textWidth * 2.5);
				availX += textWidth * 1.25;
			}
		}

		_pieCenterX = availX + (availWidth / 2.0);
		_pieCenterY = availY + (availHeight / 2.0);
		if (_width < _height) {
			_pieRadius = (availWidth / 2.0) - 40.0;
		} else {
			_pieRadius = (availHeight / 2.0) - 40.0;
		}

		double startAngle = 0.0, sliceAngle, labelAngle;

		for(int index = 0; index < _itemValuesList.size(); index++) {
			// pie slice plus label

			sliceAngle = _itemPercentages[index] * 360.0;

			qDebug()  << "PieChart::onRegenerated: sizes: " << _itemColorsList.size() << " : " << _itemValuesList.size();

			if (_itemGradients) {
				_graphics2D->setGradient(_itemGradients[index]);
			} else {
				_graphics2D->setColor(_itemColors[index]);
			}

			_graphics2D->fillArc(_pieCenterX, _pieCenterY, _pieRadius, _pieRadius, startAngle, sliceAngle);

			startAngle += sliceAngle;
		}

		// outlines
		startAngle = 0.0;

		for(int index = 0; index < _itemValuesList.size(); index++) {
			// pie slice plus label

			sliceAngle = _itemPercentages[index] * 360.0;

			_graphics2D->setColor(COLOR_BLACK);
			_graphics2D->drawLine(_pieCenterX, _pieCenterY,
								  _pieCenterX + cos(startAngle * M_PI / 180.0) * _pieRadius,
								  _pieCenterY + sin(startAngle * M_PI / 180.0) * _pieRadius);

			startAngle += sliceAngle;
		}

		// labels
		startAngle = 0.0;

		for(int index = 0; index < _itemValuesList.size(); index++) {
			// pie slice plus label

			sliceAngle = _itemPercentages[index] * 360.0;
			labelAngle = startAngle + sliceAngle / 2.0;

			if (_showSliceLabel) {
				QString qtyText("");
				qtyText.append(QString::number(_itemPercentages[index] * 100.0, 'g', 3));
				qtyText.append(" %");

				if (sliceAngle > 30.0) {
					if (_itemColors[index*2].green > 0.2) {
						_graphics2D->setColor(COLOR_BLACK);
					} else {
						_graphics2D->setColor(COLOR_WHITE);
					}

					_graphics2D->setFont(_sliceLabelFont);
					_graphics2D->measureString(qtyText, &textWidth, &textHeight);
					_graphics2D->drawString(qtyText, _pieCenterX + cos(labelAngle * M_PI / 180.0) * _pieRadius *.6 - textWidth / 2.0,
													 _pieCenterY + sin(labelAngle * M_PI / 180.0) * _pieRadius *.6 - textHeight / 2.0);
				}
			}

			startAngle += sliceAngle;
		}

		if (_showSliceLabel) {
			double externLabelX     [_itemValuesList.size()];
			double externLabelY     [_itemValuesList.size()];
			double externLabelWidth [_itemValuesList.size()];
			double externLabelHeight[_itemValuesList.size()];
			double externLabelAngle [_itemValuesList.size()];

			startAngle = 0.0;
			int otherSideIndex = -1;

			for(int index = 0; index < _itemValuesList.size(); index++) {
				// pie slice plus label

				sliceAngle = _itemPercentages[index] * 360.0;
				labelAngle = startAngle + sliceAngle / 2.0;

				externLabelAngle[index] = -1.0;

				if (sliceAngle < 30.0) {
					QString qtyText("");
					qtyText.append(QString::number(_itemPercentages[index] * 100.0, 'g', 3));
					qtyText.append(" %");

					_graphics2D->measureString(qtyText, &textWidth, &textHeight);

					externLabelWidth[index]  = textWidth;
					externLabelHeight[index] = textHeight;
					externLabelAngle[index]  = labelAngle;
					externLabelX[index]      = _pieCenterX + cos(labelAngle * M_PI / 180.0) * _pieRadius * 1.05 - (textWidth / 2.0);
					externLabelY[index]      = _pieCenterY + sin(labelAngle * M_PI / 180.0) * _pieRadius * 1.05 - (textHeight / 2.0);

					if (_sliceLabelPlacement == "vertical") {
						if (index == 0) {
							if (externLabelX[index] + textWidth > (_pieCenterX + _pieRadius)) {
								externLabelX[index] = _pieCenterX + _pieRadius - textWidth;
							}
						}

						if (index > 0 && otherSideIndex < 0) {
							if (externLabelAngle[index-1] >= 0 && externLabelX[index] + textWidth > externLabelX[index-1]) {
								externLabelX[index] = externLabelX[index-1] - textWidth - 10.0;
							}
							if (externLabelX[index] - textWidth < 0) {
								externLabelX[index] =_leftPadding;
							}
						}

						if (index > otherSideIndex) {
							if (externLabelX[index] - textWidth < 0) {
								externLabelX[index] = _leftPadding;
							}
							if (externLabelAngle[index-1] >= 0 && externLabelX[index] + textWidth > externLabelX[index-1]) {
								externLabelX[index] = externLabelX[index-1] - textWidth - 10.0;
							}
						}

						if (sin(labelAngle * M_PI / 180.0) >= 0.0) {
							externLabelY[index]      = _pieCenterY + _pieRadius + 15.0;
						} else {
							if (otherSideIndex < 0) {
								otherSideIndex = index;
							}

							externLabelY[index]      = _pieCenterY - _pieRadius - 15.0 - textHeight;
						}

					} else if (_sliceLabelPlacement == "horizontal") {
						if (index == 0) {
							if (externLabelY[index] + textHeight > (_pieCenterY + _pieRadius)) {
								externLabelY[index] = _pieCenterY + _pieRadius - textHeight;
							}
						}

						if (index > 0 && otherSideIndex < 0) {
							if (externLabelAngle[index-1] >= 0 && externLabelY[index] + textHeight > externLabelX[index-1]) {
								externLabelY[index] = externLabelY[index-1] - textHeight - 10.0;
							}
							if (externLabelY[index] - textHeight < 0) {
								externLabelY[index] = 0.0;
							}
						}

						if (index > otherSideIndex) {
							if (externLabelAngle[index-1] >= 0 && externLabelY[index] - textHeight < 0) {
								externLabelY[index] = 0.0;
							}
							if (externLabelY[index] + textHeight > externLabelY[index-1]) {
								externLabelY[index] = externLabelY[index-1] - textHeight - 10.0;
							}
						}

						if (cos(labelAngle * M_PI / 180.0) >= 0.0) {
							externLabelX[index]      = _rightPadding - textWidth;

							if (otherSideIndex > 0) {
								otherSideIndex = -1;
							}
						} else {
							if (otherSideIndex < 0) {
								otherSideIndex = index;
							}

							externLabelX[index]      = _leftPadding;
						}
					}

					qDebug()  << "PieChart::onRegenerated: ext labels: " << index << " : " << externLabelAngle[index] << " : " << externLabelX[index] << " : " << externLabelY[index] << " : " << externLabelWidth[index] << " : " << externLabelHeight[index];

				}

				startAngle += sliceAngle;
			}

			startAngle = 0.0;

			for(int index = 0; index < _itemValuesList.size(); index++) {
				// pie slice plus label

				sliceAngle = _itemPercentages[index] * 360.0;
				labelAngle = startAngle + sliceAngle / 2.0;

				if (sliceAngle < 30.0 && externLabelAngle[index] > -1) {
					QString qtyText("");
					qtyText.append(QString::number(_itemPercentages[index] * 100.0, 'g', 3));
					qtyText.append(" %");

					_graphics2D->measureString(qtyText, &textWidth, &textHeight);

					if (_sliceLabelPlacement == "vertical") {
						_graphics2D->setStroke(_thinStroke);
						double* lineX = new double[3] {
							(_pieCenterX + cos(labelAngle * M_PI / 180.0) * _pieRadius * 0.6),
							(_pieCenterX + cos(labelAngle * M_PI / 180.0) * _pieRadius * 1.05),
							(externLabelX[index] + 10.0 + externLabelWidth[index] / 2.0) };
						double* lineY = new double[3] {
							(_pieCenterY + sin(labelAngle * M_PI / 180.0) * _pieRadius * 0.6),
							(_pieCenterY + sin(labelAngle * M_PI / 180.0) * _pieRadius * 1.05),
							(externLabelY[index] - 5.0) };

						if (sin(labelAngle * M_PI / 180.0) < 0.0) {
							lineY[2] += externLabelHeight[index] + 10.0;
						}

						_graphics2D->drawPolyline(lineX, lineY, 3);

					} else if (_sliceLabelPlacement == "horizontal") {
						_graphics2D->setStroke(_thinStroke);
						double* lineX = new double[3] {
							(_pieCenterX + cos(labelAngle * M_PI / 180.0) * _pieRadius * 0.6),
							(_pieCenterX + cos(labelAngle * M_PI / 180.0) * _pieRadius * 1.05),
							(externLabelX[index] + 10.0 + externLabelWidth[index] / 2.0) };
						double* lineY = new double[3] {
							(_pieCenterY + sin(labelAngle * M_PI / 180.0) * _pieRadius * 0.6),
							(_pieCenterY + sin(labelAngle * M_PI / 180.0) * _pieRadius * 1.05),
							(externLabelY[index] + externLabelHeight[index] / 2.0) };

						if (cos(labelAngle * M_PI / 180.0) >= 0.0) {
							lineX[2] = (externLabelX[index] - 10.0);
						}

						_graphics2D->drawPolyline(lineX, lineY, 3);

					}

					_graphics2D->setColor(COLOR_BLACK);
					_graphics2D->setFont(_sliceLabelFont);
					_graphics2D->measureString(qtyText, &textWidth, &textHeight);
					_graphics2D->drawString(qtyText, externLabelX[index], externLabelY[index]);

				}

				startAngle += sliceAngle;
			}
		}

		_graphics2D->setStroke(_defaultStroke);
		_graphics2D->drawOval(_pieCenterX, _pieCenterY, _pieRadius, _pieRadius);


		_graphics2D->done();

		setStale(true);
	}
}

qreal PieChart::leftPadding() {
	return _leftPadding;
}

qreal PieChart::rightPadding() {
	return _rightPadding;
}

qreal PieChart::topPadding() {
	return _topPadding;
}

qreal PieChart::bottomPadding() {
	return _bottomPadding;
}

QString PieChart::caption()
{
	return _caption;
}

QString PieChart::captionFont()
{
	return _captionFontFile;
}

qreal  PieChart::captionFontSize()
{
	return _captionFontSize;
}

QString PieChart::captionPlacement()
{
	return _captionPlacement;
}

QString PieChart::captionAlignment()
{
	return _captionAlignment;
}


QString PieChart::subCaption()
{
	return _subCaption;
}

QString PieChart::subCaptionFont()
{
	return _subCaptionFontFile;
}

qreal  PieChart::subCaptionFontSize()
{
	return _subCaptionFontSize;
}


bool PieChart::showCaption()
{
	return _showCaption;
}


void PieChart::setLeftPadding(qreal leftPadding)
{
	_viewMutex.lock();

	_leftPadding = leftPadding;

	_viewMutex.unlock();

	setStale(true);
}

void PieChart::setRightPadding(qreal rightPadding)
{
	_viewMutex.lock();

	_rightPadding = rightPadding;

	_viewMutex.unlock();

	setStale(true);
}

void PieChart::setTopPadding(qreal topPadding)
{
	_viewMutex.lock();

	_topPadding = topPadding;

	_viewMutex.unlock();

	setStale(true);
}

void PieChart::setBottomPadding(qreal bottomPadding)
{
	_viewMutex.lock();

	_bottomPadding = bottomPadding;

	_viewMutex.unlock();

	setStale(true);
}

void PieChart::setCaption(QString caption)
{
	_caption = caption;
}

void PieChart::setCaptionFont(QString captionFont)
{
	_captionFontFile = captionFont;
}

void PieChart::setCaptionFontSize(qreal captionFontSize)
{
	_captionFontSize = captionFontSize;
}

void PieChart::setCaptionPlacement(QString captionPlacement)
{
	_captionPlacement = captionPlacement;
}

void PieChart::setCaptionAlignment(QString captionAlignment)
{
	_captionAlignment = captionAlignment;
}


void PieChart::setSubCaption(QString subCaption)
{
	_subCaption = subCaption;
}

void PieChart::setSubCaptionFont(QString subCaptionFont)
{
	_subCaptionFontFile = subCaptionFont;
}

void PieChart::setSubCaptionFontSize(qreal subCaptionFontSize)
{
	_subCaptionFontSize = subCaptionFontSize;
}


void PieChart::setShowCaption(bool showCaption)
{
	_showCaption = showCaption;
}


QString PieChart::legendTitleFont()
{
	return _legendTitleFontFile;
}

qreal  PieChart::legendTitleFontSize()
{
	return _legendTitleFontSize;
}

QString PieChart::legendFont()
{
	return _legendFontFile;
}

qreal  PieChart::legendFontSize()
{
	return _legendFontSize;
}

QString PieChart::legendPlacement()
{
	return _legendPlacement;
}

bool PieChart::showLegend()
{
	return _showLegend;
}


void PieChart::setLegendTitleFont(QString legendTitleFont)
{
	_legendTitleFontFile = legendTitleFont;
}

void PieChart::setLegendTitleFontSize(qreal legendTitleFontSize)
{
	_legendTitleFontSize = legendTitleFontSize;
}

void PieChart::setLegendFont(QString legendFont)
{
	_legendFontFile = legendFont;
}

void PieChart::setLegendFontSize(qreal  legendFontSize)
{
	_legendFontSize = legendFontSize;
}

void PieChart::setLegendPlacement(QString legendPlacement)
{
	_legendPlacement = legendPlacement;
}

void PieChart::setShowLegend(bool showLegend)
{
	_showLegend = showLegend;
}


bool PieChart::showSliceLabel()
{
	return _showSliceLabel;
}

QString PieChart::sliceLabelPlacement()
{
	return _sliceLabelPlacement;
}

QString PieChart::sliceLabelFont()
{
	return _sliceLabelFontFile;
}

qreal  PieChart::sliceLabelFontSize()
{
	return _sliceLabelFontSize;
}


void PieChart::setShowSliceLabel(bool showSliceLabel)
{
	_showSliceLabel = showSliceLabel;
}

void PieChart::setSliceLabelPlacement(QString sliceLabelPlacement)
{
	_sliceLabelPlacement = sliceLabelPlacement;
}

void PieChart::setSliceLabelFont(QString sliceLabelFont)
{
	_sliceLabelFontFile = sliceLabelFont;
}

void PieChart::setSliceLabelFontSize(qreal  sliceLabelFontSize)
{
	_sliceLabelFontSize = sliceLabelFontSize;
}

QVariantList PieChart::itemColors()
{
	return _itemColorsList;
}

QVariantList PieChart::itemLabels()
{
	return _itemLabels;
}

QVariantList PieChart::itemValues()
{
	return _itemValuesList;
}

void PieChart::setItemColors(QVariantList itemColors)
{
	_itemColorsList = itemColors;

	if (_itemColors) {
		delete _itemColors;
	}

	if (itemColors.size() > 0) {
		_itemColors = new GLColor[itemColors.size()];
		for(int index = 0; index < itemColors.size(); index++) {
			_itemColors[index].red   = itemColors[index].value<Color>().red();
			_itemColors[index].green = itemColors[index].value<Color>().green();
			_itemColors[index].blue  = itemColors[index].value<Color>().blue();
			_itemColors[index].alpha = itemColors[index].value<Color>().alpha();
		}


		if (itemColors.size() == (_itemValuesList.size() * 2)) {
			_itemGradients = new Gradient*[_itemValuesList.size()];

			for(int index = 0; index < _itemValuesList.size(); index++) {
				_itemGradients[index] = _graphics2D->createGradient(1, &_itemColors[index*2], _pieGradientPercentages, 1.0, 0.0, 0.5, 0.5);
			}
		}

	}
}

void PieChart::setItemLabels(QVariantList itemLabels)
{
	_itemLabels = itemLabels;
}

void PieChart::setItemValues(QVariantList itemValues)
{
	_itemValuesList = itemValues;

	if (_itemPercentages) {
		delete _itemPercentages;
	}

	if (itemValues.size() > 0) {
		_itemPercentages = new double[itemValues.size()];
		for(int index = 0; index < itemValues.size(); index++) {
			_itemPercentages[index] = itemValues[index].value<qreal>();
		}
	}
}
