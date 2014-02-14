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

#ifndef CANVASVIEW_HPP_
#define CANVASVIEW_HPP_

#include "../base/View.hpp"
#include "../graphics/Graphics2D.hpp"
#include <bb/cascades/Color>

using namespace bb::cascades;
using namespace views::base;
using namespace views::graphics;

namespace views {
	namespace graphics {

class CanvasView : public View {

    Q_OBJECT

public:
    CanvasView(ViewDisplay display = DISPLAY_DEVICE);
    virtual ~CanvasView();

public:
	void onRegenerated();
	void onUpdated();

public Q_SLOTS:
	// resets the context to a default state
	bool reset();

	// rendering is complete
	void done();

	// Creates a new font context and loads a font for use with the API.
	int createFont(const QString& fontFileName, const QString& fontMetricsFileName, int pointSize, int dpi, const QString& fontCharacters);

	// create a new stroke type
	int createStroke(float width = 1.0, int cap = CAP_NONE, int join = JOIN_NONE, float miterLimit = 0.0, QVariantList dash = *(new QVariantList()), int dashCount = 0, float dashPhase = 0.0);

	// create a new gradient
	int createGradient(int segments, QVariantList colors, QVariantList percentages, float radius = 0.0, float angle = 0.0, float originU = 0.0, float originV = 0.0);

	// create a new image texture
    int createImageTexture(int imageID, int scaling, int tiling, float uScale, float vScale, int leftMargin, int rightMargin, int topMargin, int bottomMargin);

	// Concatenates the current Graphics2D Transform with a rotation transform.
	void 	rotate(double theta);

	// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
	void 	scale(double sx, double sy);

	// Concatenates the current Graphics2D Transform with a translation transform.
	void 	translate(double tx, double ty);

	// Concatenates the current Graphics2D Transform with a shearing transform.
	void 	shear(double shx, double shy);

	// Overwrites the Transform in the Graphics2D context.
	void 	transform(QVariantList matrix);

	// Clears the specified rectangle by filling it with the background color of the current drawing surface.
	void clearRect(int x, int y, int width, int height);

	// Intersects the current clip with the specified rectangle.
	void clipRect(int x, int y, int width, int height);

	// Returns the background color used for clearing a region.
	const bb::cascades::Color getBackground();

	// Gets the current color.
	const bb::cascades::Color getColor();

	// Gets the current font.
	const int getFont();

	// Gets the current gradient.
	const int getGradient();

	// Gets the current image texture.
	const int getImageTexture();

	// Returns the current Stroke in the Graphics context.
	const int getStroke();

	// Sets the background color for the Graphics2D context.
	void setBackground(const bb::cascades::Color& color);

	// Sets this graphics context's current color to the specified color.
	void setColor(const bb::cascades::Color& color);

	// Sets this graphics context's font to the specified font.
	void setFont(int fontID);

	// Sets the gradient for the Graphics2D context.
	void setGradient(int gradientID);

	// Sets the image texture for the Graphics2D context.
	void setImageTexture(int imageTextureID);

	// Sets the Stroke for the Graphics2D context.
    void setStroke(int strokeID);

	// Draws the outline of a circular or elliptical arc covering the specified rectangle.
	void drawArc(double x, double y, double width, double height, double startAngle, double arcAngle);

	// Draws as much of the specified image as is currently available.
	void drawImage(int imageID, double x, double y);

	// Draws as much of the specified image as is currently available.
	void drawImage(int imageID, double x, double y, bb::cascades::Color backgroundColor);

	// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
	void drawImage(int imageID, double x, double y, double width, double height);

	// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
	void drawImage(int imageID, double x, double y, double width, double height, bb::cascades::Color backgroundColor);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void drawImage(int imageID, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void drawImage(int imageID, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2, bb::cascades::Color backgroundColor);

	// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
	void drawLine(double x1, double y1, double x2, double y2);

	// Draws the outline of an oval.
	void drawOval(double x, double y, double width, double height);

	// Draws a closed polygon defined by arrays of x and y coordinates.
	void drawPolygon(QVariantList xPoints, QVariantList yPoints, int nPoints);

	// Draws a sequence of connected lines defined by arrays of x and y coordinates.
	void drawPolyline(QVariantList xPoints, QVariantList yPoints, int nPoints);

	// Draws the outline of the specified rectangle.
	void drawRect(double x, double y, double width, double height);

	// Draws an outlined round-cornered rectangle using this graphics context's current color.
	void drawRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight);

	// Measures the bounding box of the text of the specified String, using the current font in the Graphics2D context.
	QVariantList* measureString(QString text);

	// Renders the text specified by the specified String, using the current text attribute state in the Graphics2D context.
	void drawString(QString text, double x, double y);

	// Fills a circular or elliptical arc covering the specified rectangle.
	void fillArc(double x, double y, double width, double height, double startAngle, double arcAngle);

	// Fills an oval bounded by the specified rectangle with the current color.
	void fillOval(double x, double y, double width, double height);

	// Fills the specified rectangle.
	void fillRect(double x, double y, double width, double height);

	// Fills a closed polygon defined by arrays of x and y coordinates.
	void fillPolygon(QVariantList xPoints, QVariantList yPoints, int nPoints);

	// Fills the specified rounded corner rectangle with the current color.
	void fillRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight);

Q_SIGNALS:
	void regenerated();
	void updated();

protected:
	int _fontID;
	QMap<int,Font*> _fontIDMap;
	int _gradientID;
	QMap<int,Gradient*> _gradientIDMap;
	int _imageTextureID;
	QMap<int,ImageTexture*> _imageTextureIDMap;
	int _strokeID;
	QMap<int,Stroke*> _strokeIDMap;

	GLColor _tempColor;

	Graphics2D* _graphics2D;
};

	} /* end namespace graphics */
} /* end namespace views */

#endif /* CANVASVIEW_HPP_ */
