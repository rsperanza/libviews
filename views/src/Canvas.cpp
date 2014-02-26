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

#include "Canvas.hpp"
#include "../graphics/Graphics2D.hpp"

namespace views {
	namespace cascades {

Canvas::Canvas(Container * parent) : ViewControl(parent)
{
	// create default view with Graphics2D graphics

	_canvasView = new CanvasView(DISPLAY_DEVICE);
	_canvasView->setPosition(0, 0);
	_canvasView->setSize(0, 0);
	_canvasView->setZ(-1);
	_canvasView->setEnabled(false);

	bool connectResult;

	// connect view signals to slots
    Q_UNUSED(connectResult);
	connectResult = connect(_canvasView, SIGNAL(regenerated()), this, SLOT(onRegenerated()));

    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

    Q_UNUSED(connectResult);
	connectResult = connect(_canvasView, SIGNAL(updated()), this, SLOT(onUpdated()));

    // This is only available in Debug builds.
    Q_ASSERT(connectResult);

	addViewToResize(_canvasView);
	addTouchEventView(_canvasView);

	// initialize map ID counters
	int _fontID = 0;
	int _gradientID = 0;
	int _imageTextureID = 0;
	int _strokeID = 0;
}

Canvas::~Canvas() {
	// TODO Auto-generated destructor stub
}

// resets the context to a default state
bool Canvas::reset()
{
	_canvasView->reset();
}

// rendering is complete
void Canvas::done()
{
	_canvasView->done();

	_canvasView->setStale(true);
}

// Creates a new font context and loads a font for use with the API.
int Canvas::createFont(const QString& fontFileName, const QString& fontMetricsFileName, int pointSize, int dpi, const QString& fontCharacters)
{
	return _canvasView->createFont(fontFileName, fontMetricsFileName, pointSize, dpi, fontCharacters);
}

// create a new stroke type
int Canvas::createStroke(float width, int cap, int join, float miterLimit, QVariantList dash, int dashCount, float dashPhase)
{
	return _canvasView->createStroke(width, cap, join, miterLimit, dash, dashCount, dashPhase);
}

// create a new gradient
int Canvas::createGradient(int segments, QVariantList colors, QVariantList percentages, float radius, float angle, float originU, float originV)
{
	return _canvasView->createGradient(segments, colors, percentages, radius, angle, originU, originV);
}

int Canvas::loadImage(const QString& filename)
{
	return loadImage(filename);
}

int Canvas::loadFullImage(const QString& filename)
{
	return loadFullImage(filename);
}

// create a new image texture
int Canvas::createImageTexture(int imageID, int scaling, int tiling, float uScale, float vScale, int leftMargin, int rightMargin, int topMargin, int bottomMargin)
{
	return _canvasView->createImageTexture(imageID, scaling, tiling, uScale, vScale, leftMargin, rightMargin, topMargin, bottomMargin);
}

// Concatenates the current Graphics2D Transform with a rotation transform.
void Canvas::rotate(double theta)
{
	_canvasView->rotate(theta);
}

// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
void Canvas::scale(double sx, double sy)
{
	_canvasView->scale(sx, sy);
}

// Concatenates the current Graphics2D Transform with a translation transform.
void Canvas::translate(double tx, double ty)
{
	_canvasView->translate(tx, ty);
}

// Concatenates the current Graphics2D Transform with a shearing transform.
void Canvas::shear(double shx, double shy)
{
	_canvasView->shear(shx, shy);
}

// Overwrites the Transform in the Graphics2D context.
void Canvas::transform(QVariantList matrix)
{
	return _canvasView->transform(matrix);
}

// Clears the specified rectangle by filling it with the background color of the current drawing surface.
void Canvas::clearRect(int x, int y, int width, int height)
{
	_canvasView->clearRect(x, y, width, height);
}

// Intersects the current clip with the specified rectangle.
void Canvas::clipRect(int x, int y, int width, int height)
{
	_canvasView->clipRect(x, y, width, height);
}

// Returns the background color used for clearing a region.
const bb::cascades::Color Canvas::getBackground()
{
	return _canvasView->getBackground();
}

// Gets the current color.
const bb::cascades::Color Canvas::getColor()
{
	return _canvasView->getColor();
}

// Gets the current font.
const int Canvas::getFont()
{
	return _canvasView->getFont();
}

// Gets the current gradient.
const int Canvas::getGradient()
{
	return _canvasView->getGradient();
}

// Gets the current image texture.
const int Canvas::getImageTexture()
{
	return _canvasView->getImageTexture();
}

// Returns the current Stroke in the Graphics context.
const int Canvas::getStroke()
{
	return _canvasView->getStroke();
}

// Sets the background color for the Graphics2D context.
void Canvas::setBackground(const bb::cascades::Color& color)
{
	_canvasView->setBackground(color);
}

// Sets this graphics context's current color to the specified color.
void Canvas::setColor(const bb::cascades::Color& color)
{
	_canvasView->setColor(color);
}

// Sets this graphics context's font to the specified font.
void Canvas::setFont(int fontID)
{
	_canvasView->setFont(fontID);
}

// Sets the gradient for the Graphics2D context.
void Canvas::setGradient(int gradientID)
{
	_canvasView->setGradient(gradientID);
}

// Sets the image texture for the Graphics2D context.
void Canvas::setImageTexture(int imageTextureID)
{
	_canvasView->setImageTexture(imageTextureID);
}

// Sets the Stroke for the Graphics2D context.
void Canvas::setStroke(int strokeID)
{
	_canvasView->setStroke(strokeID);
}

// Draws the outline of a circular or elliptical arc covering the specified rectangle.
void Canvas::drawArc(double x, double y, double width, double height, double startAngle, double arcAngle)
{
	_canvasView->drawArc(x, y, width, height, startAngle, arcAngle);
}

// Draws as much of the specified image as is currently available.
void Canvas::drawImage(int imageID, double x, double y)
{
	return _canvasView->drawImage(imageID, x, y);
}

// Draws as much of the specified image as is currently available.
void Canvas::drawImage(int imageID, double x, double y, bb::cascades::Color backgroundColor)
{
	return _canvasView->drawImage(imageID, x, y, backgroundColor);
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void Canvas::drawImage(int imageID, double x, double y, double width, double height)
{
	return _canvasView->drawImage(imageID, x, y, width, height);
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void Canvas::drawImage(int imageID, double x, double y, double width, double height, bb::cascades::Color backgroundColor)
{
	return _canvasView->drawImage(imageID, x, y, width, height, backgroundColor);
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Canvas::drawImage(int imageID, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2)
{
	return _canvasView->drawImage(imageID, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2);
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Canvas::drawImage(int imageID, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2, bb::cascades::Color backgroundColor)
{
	return _canvasView->drawImage(imageID, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, backgroundColor);
}

// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
void Canvas::drawLine(double x1, double y1, double x2, double y2)
{
	_canvasView->drawLine(x1, y1, x2, y2);
}

// Draws the outline of an oval.
void Canvas::drawOval(double x, double y, double width, double height)
{
	_canvasView->drawOval(x, y, width, height);
}

// Draws a closed polygon defined by arrays of x and y coordinates.
void Canvas::drawPolygon(QVariantList xPoints, QVariantList yPoints, int nPoints)
{
	_canvasView->drawPolygon(xPoints, yPoints, nPoints);
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void Canvas::drawPolyline(QVariantList xPoints, QVariantList yPoints, int nPoints)
{
	_canvasView->drawPolyline(xPoints, yPoints, nPoints);
}

// Draws the outline of the specified rectangle.
void Canvas::drawRect(double x, double y, double width, double height)
{
	_canvasView->drawRect(x, y, width, height);
}

// Draws an outlined round-cornered rectangle using this graphics context's current color.
void Canvas::drawRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight)
{
	_canvasView->drawRoundRect(x, y, width, height, arcWidth, arcHeight);
}

// Measures the bounding box of the text of the specified String, using the current font in the Graphics2D context.
QVariantList* Canvas::measureString(QString text)
{
	return _canvasView->measureString(text);
}

// Renders the text specified by the specified String, using the current text attribute state in the Graphics2D context.
void Canvas::drawString(QString text, double x, double y)
{
	_canvasView->drawString(text, x, y);
}

// Fills a circular or elliptical arc covering the specified rectangle.
void Canvas::fillArc(double x, double y, double width, double height, double startAngle, double arcAngle)
{
	_canvasView->fillArc(x, y, width, height, startAngle, arcAngle);
}

// Fills an oval bounded by the specified rectangle with the current color.
void Canvas::fillOval(double x, double y, double width, double height)
{
	_canvasView->fillOval(x, y, width, height);
}

// Fills the specified rectangle.
void Canvas::fillRect(double x, double y, double width, double height)
{
	_canvasView->fillRect(x, y, width, height);
}

// Fills a closed polygon defined by arrays of x and y coordinates.
void Canvas::fillPolygon(QVariantList xPoints, QVariantList yPoints, int nPoints)
{
	_canvasView->fillPolygon(xPoints, yPoints, nPoints);
}

// Fills the specified rounded corner rectangle with the current color.
void Canvas::fillRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight)
{
	_canvasView->fillRoundRect(x, y, width, height, arcWidth, arcHeight);
}

void Canvas::onRegenerated()
{
	emit regenerated();
}

void Canvas::onUpdated()
{
	emit updated();
}


	} /* end namespace cascades */
} /* end namespace views */
