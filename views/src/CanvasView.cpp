/*
 * Copyright (c) 2012-2014 Research In Motion Limited.
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

#include "CanvasView.hpp"
#include "../graphics/Graphics2D.hpp"

namespace views {
	namespace graphics {

CanvasView::CanvasView(ViewDisplay display) : View(display)
{
	// initialize map ID counters
	int _fontID = 0;
	int _gradientID = 0;
	int _imageTextureID = 0;
	int _strokeID = 0;

	_graphics2D = new Graphics2D(DISPLAY_DEVICE);
	registerGraphics(_graphics2D);

}

CanvasView::~CanvasView()
{
}

// resets the context to a default state
bool CanvasView::reset()
{
	_graphics2D->reset();
}

// rendering is complete
void CanvasView::done()
{
	_graphics2D->done();

	setStale(true);
}

// Creates a new font context and loads a font for use with the API.
int CanvasView::createFont(const QString& fontFileName, const QString& fontMetricsFileName, int pointSize, int dpi, const QString& fontCharacters)
{
	QString* fontMetricsFilename = NULL;
	QString* fontChars = NULL;
	if (fontMetricsFileName.size() > 0) {
		fontMetricsFilename = (QString*)&fontMetricsFileName;
	}
	if (fontCharacters.size() > 0) {
		fontChars = (QString*)&fontCharacters;
	}

	Font* font = _graphics2D->createFont(fontFileName, fontMetricsFilename, pointSize, dpi, fontChars);
	_fontID++;
	_fontIDMap.insert(_fontID, font);

	return _fontID;

}

// create a new stroke type
int CanvasView::createStroke(float width, int cap, int join, float miterLimit, QVariantList dash, int dashCount, float dashPhase)
{
	float *dashValues = NULL;

	if (dash.size() > 0) {
		dashValues = new float[dash.size()];
		for(int index = 0; index < dash.size(); index++) {
			dashValues[index] = dash[index].value<float>();
		}
	}

	Stroke *stroke = _graphics2D->createStroke(width, cap, join, miterLimit, dashValues, dashCount, dashPhase);
	_strokeID++;
	_strokeIDMap.insert(_strokeID, stroke);

	return _strokeID;
}

// create a new gradient
int CanvasView::createGradient(int segments, QVariantList colors, QVariantList percentages, float radius, float angle, float originU, float originV)
{
	float *percentageValues = NULL;
	GLColor* glColors = NULL;

	if (percentages.size() > 0) {
		percentageValues = new float[percentages.size()];
		for(int index = 0; index < percentages.size(); index++) {
			percentageValues[index] = percentages[index].value<float>();
		}
	}

	if (colors.size() > 0) {
		glColors = new GLColor[colors.size()];
		for(int index = 0; index < colors.size(); index++) {
			percentageValues[index] = percentages[index].value<float>();
		}

	    Gradient* gradient = _graphics2D->createGradient(segments, glColors, percentageValues, radius, angle, originU, originV);
		_gradientID++;
		_gradientIDMap.insert(_gradientID, gradient);
	} else {
		return -1;
	}

	return _gradientID;
}

int CanvasView::loadImage(const QString& filename)
{
	ImageData *image = _graphics2D->loadImage(filename);

	if (image) {
		_imageID++;
		_imageIDMap.insert(_imageID, image);
	} else {
		return -1;
	}

	return _imageID;
}

int CanvasView::loadFullImage(const QString& filename)
{
	ImageData *image = _graphics2D->loadFullImage(filename);

	if (image) {
		_imageID++;
		_imageIDMap.insert(_imageID, image);
	} else {
		return -1;
	}

	return _imageID;
}

// create a new image texture
int CanvasView::createImageTexture(int imageID, int scaling, int tiling, float uScale, float vScale, int leftMargin, int rightMargin, int topMargin, int bottomMargin)
{
/*
	float *dashValues = NULL;

	if (dash.size() > 0) {
		dashValues = new float[dash.size()];
		for(int index = 0; index < dash.size(); index++) {
			dashValues[index] = dash[index].value<float>();
		}
	}

	Stroke *stroke = _graphics2D->createStroke(width, cap, join, miterLimit, dashValues, dashCount, dashPhase);
    ImageTexture* createImageTexture(ImageData* image, int scaling, int tiling, float uScale, float vScale, int leftMargin, int rightMargin, int topMargin, int bottomMargin);
	_strokeID++;
	_strokeIDMap.insert(_strokeID, stroke);
*/
	return _strokeID;
}

// Concatenates the current Graphics2D Transform with a rotation transform.
void CanvasView::rotate(double theta)
{
	_graphics2D->rotate(theta);
}

// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
void CanvasView::scale(double sx, double sy)
{
	_graphics2D->scale(sx, sy);
}

// Concatenates the current Graphics2D Transform with a translation transform.
void CanvasView::translate(double tx, double ty)
{
	_graphics2D->translate(tx, ty);
}

// Concatenates the current Graphics2D Transform with a shearing transform.
void CanvasView::shear(double shx, double shy)
{
	_graphics2D->shear(shx, shy);
}

// Overwrites the Transform in the Graphics2D context.
void CanvasView::transform(QVariantList matrix)
{
	double matrixf[4][4];

	if (matrix.size() == 16) {
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				matrixf[i][j] = matrix[i*4+j].value<float>();
			}
		}

		_graphics2D->transform((double**)matrixf);
	}
}

// Clears the specified rectangle by filling it with the background color of the current drawing surface.
void CanvasView::clearRect(int x, int y, int width, int height)
{
	_graphics2D->clearRect(x, y, width, height);
}

// Intersects the current clip with the specified rectangle.
void CanvasView::clipRect(int x, int y, int width, int height)
{
	_graphics2D->clipRect(x, y, width, height);
}

// Returns the background color used for clearing a region.
const bb::cascades::Color CanvasView::getBackground()
{
	const GLColor& backgroundColor = _graphics2D->getBackground();

	return Color::fromRGBA(backgroundColor.red, backgroundColor.green, backgroundColor.blue, backgroundColor.alpha);
}

// Gets the current color.
const bb::cascades::Color CanvasView::getColor()
{
	const GLColor& currentColor = _graphics2D->getColor();

	return Color::fromRGBA(currentColor.red, currentColor.green, currentColor.blue, currentColor.alpha);
}

// Gets the current font.
const int CanvasView::getFont()
{
	return _fontIDMap.key((Font*)_graphics2D->getFont());
}

// Gets the current gradient.
const int CanvasView::getGradient()
{
	return _gradientIDMap.key((Gradient*)_graphics2D->getGradient());
}

// Gets the current image texture.
const int CanvasView::getImageTexture()
{
	return _imageTextureIDMap.key((ImageTexture*)_graphics2D->getImageTexture());
}

// Returns the current Stroke in the Graphics context.
const int CanvasView::getStroke()
{
	return _strokeIDMap.key((Stroke*)_graphics2D->getStroke());
}

// Sets the background color for the Graphics2D context.
void CanvasView::setBackground(const bb::cascades::Color& color)
{
	_tempColor.red = color.red();
	_tempColor.green = color.green();
	_tempColor.blue = color.blue();
	_tempColor.alpha = color.alpha();

	_graphics2D->setBackground(_tempColor);
}

// Sets this graphics context's current color to the specified color.
void CanvasView::setColor(const bb::cascades::Color& color)
{
	_tempColor.red = color.red();
	_tempColor.green = color.green();
	_tempColor.blue = color.blue();
	_tempColor.alpha = color.alpha();

	_graphics2D->setColor(_tempColor);
}

// Sets this graphics context's font to the specified font.
void CanvasView::setFont(int fontID)
{
	_graphics2D->setFont(_fontIDMap.value(fontID));
}

// Sets the gradient for the Graphics2D context.
void CanvasView::setGradient(int gradientID)
{
	_graphics2D->setGradient(_gradientIDMap.value(gradientID));
}

// Sets the image texture for the Graphics2D context.
void CanvasView::setImageTexture(int imageTextureID)
{
	_graphics2D->setImageTexture(_imageTextureIDMap.value(imageTextureID));
}

// Sets the Stroke for the Graphics2D context.
void CanvasView::setStroke(int strokeID)
{
	_graphics2D->setStroke(_strokeIDMap.value(strokeID));
}

// Draws the outline of a circular or elliptical arc covering the specified rectangle.
void CanvasView::drawArc(double x, double y, double width, double height, double startAngle, double arcAngle)
{
	_graphics2D->drawArc(x, y, width, height, startAngle, arcAngle);
}

// Draws as much of the specified image as is currently available.
void CanvasView::drawImage(int imageID, double x, double y)
{
	ImageData* image = _imageIDMap.value(imageID);

	if (image) {
		_graphics2D->drawImage(image, x, y);
	}
}

// Draws as much of the specified image as is currently available.
void CanvasView::drawImage(int imageID,  double x, double y, bb::cascades::Color backgroundColor)
{
	ImageData* image = _imageIDMap.value(imageID);

	if (image) {
		_tempColor.red = backgroundColor.red();
		_tempColor.green = backgroundColor.green();
		_tempColor.blue = backgroundColor.blue();
		_tempColor.alpha = backgroundColor.alpha();

		_graphics2D->drawImage(image, x, y, _tempColor);
	}
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void CanvasView::drawImage(int imageID,  double x, double y, double width, double height)
{
	ImageData* image = _imageIDMap.value(imageID);

	if (image) {
		_graphics2D->drawImage(image, x, y, width, height);
	}
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void CanvasView::drawImage(int imageID,  double x, double y, double width, double height, bb::cascades::Color backgroundColor)
{
	ImageData* image = _imageIDMap.value(imageID);

	if (image) {
		_tempColor.red = backgroundColor.red();
		_tempColor.green = backgroundColor.green();
		_tempColor.blue = backgroundColor.blue();
		_tempColor.alpha = backgroundColor.alpha();

		_graphics2D->drawImage(image, x, y, width, height, _tempColor);
	}
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void CanvasView::drawImage(int imageID, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2)
{
	ImageData* image = _imageIDMap.value(imageID);

	if (image) {
		_graphics2D->drawImage(image, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2);
	}
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void CanvasView::drawImage(int imageID, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2, bb::cascades::Color backgroundColor)
{
	ImageData* image = _imageIDMap.value(imageID);

	if (image) {
		_tempColor.red = backgroundColor.red();
		_tempColor.green = backgroundColor.green();
		_tempColor.blue = backgroundColor.blue();
		_tempColor.alpha = backgroundColor.alpha();

		_graphics2D->drawImage(image, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, _tempColor);
	}
}

// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
void CanvasView::drawLine(double x1, double y1, double x2, double y2)
{
	_graphics2D->drawLine(x1, y1, x2, y2);
}

// Draws the outline of an oval.
void CanvasView::drawOval(double x, double y, double width, double height)
{
	_graphics2D->drawOval(x, y, width, height);
}

// Draws a closed polygon defined by arrays of x and y coordinates.
void CanvasView::drawPolygon(QVariantList xPoints, QVariantList yPoints, int nPoints)
{
	double *xPointsValues = NULL;
	double *yPointsValues = NULL;

	if (xPoints.size() > 0 && xPoints.size() == nPoints) {
		xPointsValues = new double[nPoints];
		for(int index = 0; index < nPoints; index++) {
			xPointsValues[index] = xPoints[index].value<double>();
		}

		if (yPoints.size() > 0 && yPoints.size() == nPoints) {
			yPointsValues = new double[nPoints];
			for(int index = 0; index < nPoints; index++) {
				yPointsValues[index] = yPoints[index].value<double>();
			}

			_graphics2D->drawPolygon(xPointsValues, yPointsValues, nPoints);
		}
	}
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void CanvasView::drawPolyline(QVariantList xPoints, QVariantList yPoints, int nPoints)
{
	double *xPointsValues = NULL;
	double *yPointsValues = NULL;

	if (xPoints.size() > 0 && xPoints.size() == nPoints) {
		xPointsValues = new double[nPoints];
		for(int index = 0; index < nPoints; index++) {
			xPointsValues[index] = xPoints[index].value<double>();
		}

		if (yPoints.size() > 0 && yPoints.size() == nPoints) {
			yPointsValues = new double[nPoints];
			for(int index = 0; index < nPoints; index++) {
				yPointsValues[index] = yPoints[index].value<double>();
			}

			_graphics2D->drawPolyline(xPointsValues, yPointsValues, nPoints);
		}
	}
}

// Draws the outline of the specified rectangle.
void CanvasView::drawRect(double x, double y, double width, double height)
{
	_graphics2D->drawRect(x, y, width, height);
}

// Draws an outlined round-cornered rectangle using this graphics context's current color.
void CanvasView::drawRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight)
{
	_graphics2D->drawRoundRect(x, y, width, height, arcWidth, arcHeight);
}

// Measures the bounding box of the text of the specified String, using the current font in the Graphics2D context.
QVariantList* CanvasView::measureString(QString text)
{
	QVariantList* measures = new QVariantList();

	double textWidth = 0.0, textHeight = 0.0;

	const Font* font = _graphics2D->getFont();
	if (font) {
		_graphics2D->measureString(text, &textWidth, &textHeight);

		*measures << textWidth << textHeight;
	}

	return measures;
}

// Renders the text specified by the specified String, using the current text attribute state in the Graphics2D context.
void CanvasView::drawString(QString text, double x, double y)
{
	_graphics2D->drawString(text, x, y);
}

// Fills a circular or elliptical arc covering the specified rectangle.
void CanvasView::fillArc(double x, double y, double width, double height, double startAngle, double arcAngle)
{
	_graphics2D->fillArc(x, y, width, height, startAngle, arcAngle);
}

// Fills an oval bounded by the specified rectangle with the current color.
void CanvasView::fillOval(double x, double y, double width, double height)
{
	_graphics2D->fillOval(x, y, width, height);
}

// Fills the specified rectangle.
void CanvasView::fillRect(double x, double y, double width, double height)
{
	_graphics2D->fillRect(x, y, width, height);
}

// Fills a closed polygon defined by arrays of x and y coordinates.
void CanvasView::fillPolygon(QVariantList xPoints, QVariantList yPoints, int nPoints)
{
	double *xPointsValues = NULL;
	double *yPointsValues = NULL;

	if (xPoints.size() > 0 && xPoints.size() == nPoints) {
		xPointsValues = new double[nPoints];
		for(int index = 0; index < nPoints; index++) {
			xPointsValues[index] = xPoints[index].value<double>();
		}

		if (yPoints.size() > 0 && yPoints.size() == nPoints) {
			yPointsValues = new double[nPoints];
			for(int index = 0; index < nPoints; index++) {
				yPointsValues[index] = yPoints[index].value<double>();
			}

			_graphics2D->fillPolygon(xPointsValues, yPointsValues, nPoints);
		}
	}
}

// Fills the specified rounded corner rectangle with the current color.
void CanvasView::fillRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight)
{
	_graphics2D->fillRoundRect(x, y, width, height, arcWidth, arcHeight);
}

void CanvasView::onRegenerated()
{
	emit regenerated();
}

void CanvasView::onUpdated()
{
	emit updated();
}


	} /* end namespace cascades */
} /* end namespace views */
