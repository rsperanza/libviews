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


#ifndef GRAPHICS2D_HPP
#define GRAPHICS2D_HPP

#include <assert.h>
#include <screen/screen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#include <bps/navigator.h>
#include <bps/screen.h>
#include <bps/bps.h>
#include <bps/event.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <bb/cascades/TouchEvent>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "Graphics.hpp"

namespace views {
	namespace graphics {

#if defined(__cplusplus)
extern "C" {
#endif


typedef struct GLColor
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;

} GLColor;


#define MAX_GLYPHS	65536

typedef struct Font {
	unsigned int fontTexture;
    float pt;
    int numberCharacters;
    int* charMap;
    float *kerning;
    float *advance;
    float *width;
    float *height;
    float *texX1;
    float *texX2;
    float *texY1;
    float *texY2;
    float *offsetX;
    float *offsetY;
	int initialized;
} Font;

typedef struct Gradient
{
	int segments; // the number of segments defining the gradient mapping.
	GLColor* colors; // The colors used to render the gradient texture.
	GLfloat* percentages; // The percentages defining color placement and midpoint interpolation.
	GLfloat radius; // radius of gradient map (0 for linear gradients, > 0 for radial gradients)
	GLfloat angle; // angle of gradient map (0 for linear gradients, > 0 for radial gradients)
	GLfloat originU; // U coordinate of origin (default 0.0)
	GLfloat originV; // U coordinate of origin (default 0.0)

} Gradient;


typedef struct ImageTexture
{
	ImageData*   image;        // image.
	int          scaling;      // type of scaling to use
	int          tiling;       // type of tiling
	GLfloat      uScale;       // scale in U coordinates
	GLfloat      vScale;       // scale in V coordinates
	int          leftMargin;   // left margin for nine-slicing
	int          rightMargin;  // right margin for nine-slicing
	int          topMargin;    // top margin for nine-slicing
	int          bottomMargin; // bottom margin for nine-slicing

} ImageTexture;

typedef enum ImageScaling {
	IMAGE_FILL_ASPECT,
	IMAGE_FILL_STRETCH,
	IMAGE_FILL_TILE
} ImageScaling;

typedef enum ImageTiling {
	IMAGE_TILE_REPEAT_NONE,
	IMAGE_TILE_REPEAT_X,
	IMAGE_TILE_REPEAT_Y,
	IMAGE_TILE_REPEAT_BOTH
} ImageTiling;


typedef struct Stroke
{
	GLfloat width; // The pen width, measured perpendicularly to the pen trajectory.
	int cap; // The decoration applied to the ends of unclosed subpaths and dash segments. Subpaths that start and end on the same point are still considered unclosed if they do not have a CLOSE segment. See SEG_CLOSE for more information on the CLOSE segment. The three different decorations are: CAP_BUTT, CAP_ROUND, and CAP_SQUARE.
	int join; // The decoration applied at the intersection of two path segments and at the intersection of the endpoints of a subpath that is closed using SEG_CLOSE. The three different decorations are: JOIN_BEVEL, JOIN_MITER, and JOIN_ROUND.
	GLfloat miterLimit; // The limit to trim a line join that has a JOIN_MITER decoration. A line join is trimmed when the ratio of miter length to stroke width is greater than the miterlimit value. The miter length is the diagonal length of the miter, which is the distance between the inside corner and the outside corner of the intersection. The smaller the angle formed by two line segments, the longer the miter length and the sharper the angle of intersection. The default miterlimit value of 10.0f causes all angles less than 11 degrees to be trimmed. Trimming miters converts the decoration of the line join to bevel.
	GLfloat* dash; // The definition of how to make a dash pattern by alternating between opaque and transparent sections.
	int dashCount; // number of dash segments
	GLfloat dashPhase; // offset into dash pattern to start rendering

} Stroke;

typedef enum StrokeCap {
	CAP_NONE,
    CAP_BUTT, // Ends unclosed subpaths and dash segments with no added decoration.
    CAP_ROUND, // Ends unclosed subpaths and dash segments with a round decoration that has a radius equal to half of the width of the pen.
    CAP_SQUARE, // Ends unclosed subpaths and dash segments with a square projection that extends beyond the end of the segment to a distance equal to half of the line width.
} StrokeCap;

typedef enum StrokeJoin {
	JOIN_NONE,
    JOIN_BEVEL, // Joins path segments by connecting the outer corners of their wide outlines with a straight segment.
    JOIN_MITER, // Joins path segments by extending their outside edges until they meet.
    JOIN_ROUND, // Joins path segments by rounding off the corner at a radius of half the line width.
} StrokeJoin;


typedef enum RenderCommand {
	RENDER_SKIP = 0,
	RENDER_CLEAR_RECT,
	RENDER_CLIP_RECT,
	RENDER_DRAW_ARC,
	RENDER_FILL_ARC,
	RENDER_DRAW_IMAGE,
	RENDER_DRAW_LINE,
	RENDER_DRAW_POLYLINE,
	RENDER_DRAW_ROUNDRECT,
	RENDER_FILL_ROUNDRECT,
	RENDER_DRAW_STRING,
	RENDER_FILL_POLYGON,
	RENDER_SET_BACKGROUND,
	RENDER_SET_COLOR,
	RENDER_SET_FONT,
	RENDER_SET_GRADIENT,
	RENDER_SET_IMAGE_TEXTURE,
	RENDER_SET_STROKE,
	RENDER_TRANSFORM,
	RENDER_TRANSFORM_ROTATE,
	RENDER_TRANSFORM_TRANSLATE,
	RENDER_TRANSFORM_SCALE,
	RENDER_TRANSFORM_SHEAR,
	RENDER_XXX
} RenderCommand;

#if defined(__cplusplus)
}
#endif

// pre-defined colours
const GLColor COLOR_BLACK { 0.0f, 0.0f, 0.0f, 1.0f };
const GLColor COLOR_WHITE { 1.0f, 1.0f, 1.0f, 1.0f };
const GLColor COLOR_GREY { 0.5f, 0.5f, 0.5f, 1.0f };
const GLColor COLOR_RED { 1.0f, 0.0f, 0.0f, 1.0f };
const GLColor COLOR_GREEN { 0.0f, 1.0f, 0.0f, 1.0f };
const GLColor COLOR_BLUE { 0.0f, 0.0f, 1.0f, 1.0f };
const GLColor COLOR_YELLOW { 1.0f, 1.0f, 0.0f, 1.0f };
const GLColor COLOR_ORANGE { 1.0f, 0.8f, 0.0f, 1.0f };
const GLColor COLOR_TRANSPARENT { 0.0f, 0.0f, 0.0f, 0.0f };
const GLColor COLOR_MULTITOUCH_TRANSPARENT { 0.0f, 0.0f, 0.0f, 0.01f };

// predefined font names

// SlatePro (Cascades)
const QString FONT_NAME_SLATE_PRO("/usr/fonts/font_repository/monotype/SlatePro.ttf");
const QString FONT_NAME_SLATE_PRO_CONDENSED("/usr/fonts/font_repository/monotype/SlatePro-Condensed.ttf");
const QString FONT_NAME_SLATE_PRO_ITALIC("/usr/fonts/font_repository/monotype/SlatePro-Italic.ttf");
const QString FONT_NAME_SLATE_PRO_LIGHT("/usr/fonts/font_repository/monotype/SlatePro-Light.ttf");
const QString FONT_NAME_SLATE_PRO_LIGHT_ITALIC("/usr/fonts/font_repository/monotype/SlatePro-LightItalic.ttf");
const QString FONT_NAME_SLATE_PRO_MEDIUM("/usr/fonts/font_repository/monotype/SlatePro-Medium.ttf");
const QString FONT_NAME_SLATE_PRO_MEDIUM_CONDENSED("/usr/fonts/font_repository/monotype/SlatePro-Medium-Condensed.ttf");

/*
 *
 * AkhbarOTS 	Akhbar MT OTS
AkhbarOTS-Bold 	Akhbar MT OTS Bold
HadassahOTS 	Hadassah OTS
HadassahOTS-Bold 	Hadassah OTS Bold
HeiseiKakuGothW5_213_C 	HeiseiKakuGothW5_JIS0213C
HeiseiKakuGoth_JIS0213-B_C 	HeiseiKakuGoth_JIS0213-Bold
HeiseiMinchoW3_JIS0213_C 	HeiseiMinchoW3_JIS0213_C
HYGothic-Medium 	HYGothic-Medium
HYGothic-Bold 	HYGothic-Bold
MHeiB.GB18030_200910 	Monotype Hei Bold GB18030
MHeiM.GB18030_200910 	Monotype Hei Medium GB18030
BengaliOTS_v121 	Bengali OTS
DevanagariOTS_v120 	Devanagari OTS
GujaratiOTS_v120 	Gujarati OTS
GurmukhiOTS_v120 	Gurmukhi OTS
KannadaOTS_v120 	Kannada OTS
MalayalamOTS_v1.20 	Malayalam OTS
TamilOTS_v120 	Tamil OTS
TeluguOTS_v120 	Telugu OTS
ThaiAltOTS-Bold 	Thai Alt OTS Bold
ThaiAltOTS 	Thai Alt OTS
browa 	Browallia New
browab 	Browallia New Bold
browai 	Browallia New Italic
browaz 	Browallia New Bold Italic
MSungL.cp950.v311.1 	MSung Light CP950
MSungM.cp950.v311.1 	MSung Medium CP950
 *
 * Web Core
 * WINGDNG2 	Wingdings 2
WINGDNG3 	Wingdings 3
andalemo 	Andale Mono
arial 	Arial
arialbd 	Arial Bold
arialbi 	Arial Bold Italic
ariali 	Arial Italic
ariblk 	Arial Black
comic 	Comic Sans MS
comicbd 	Comic Sans MS Bold
cour 	Courier New
courbd 	Courier New Bold
courbi 	Courier New Bold Italic
couri 	Courier New Italic
georgia 	Georgia
georgiab 	Georgia Bold
georgiai 	Georgia Italic
georgiaz 	Georgia Bold Italic
impact 	Impact
symbol 	Symbol
tahoma 	Tahoma
tahomabd 	Tahoma Bold
times 	Times New Roman
timesbd 	Times New Roman Bold
timesbi 	Times New Roman Bold Italic
timesi 	Times New Roman Italic
trebuc 	Trebuchet MS
trebucbd 	Trebuchet MS Bold
trebucbi 	Trebuchet MS Bold Italic
trebucit 	Trebuchet MS Italic
verdana 	Verdana
verdanab 	Verdana Bold
verdanai 	Verdana Italic
verdanaz 	Verdana Bold Italic
webdings 	Webdings
wingding 	Wingdings
 *
 *
 * MTEXTRA 	MT Extra
DejaVuSans-Bold.ttf	DejaVu Sans Bold
DejaVuSans-BoldOblique.ttf	DejaVu Sans Bold Oblique
DejaVuSans-ExtraLight.ttf	DejaVu Sans ExtraLight
DejaVuSans-Oblique.ttf	DejaVu Sans Oblique
DejaVuSans.ttf	DejaVu Sans
DejaVuSansCondensed-Bold.ttf	DejaVu Sans Condensed Bold
DejaVuSansCondensed-BoldOblique.ttf	DejaVu Sans Condensed Bold Oblique
DejaVuSansCondensed-Oblique.ttf	DejaVu Sans Condensed Oblique
DejaVuSansCondensed.ttf	DejaVu Sans Condensed
DejaVuSansMono-Bold.ttf	DejaVu Sans Mono Bold
DejaVuSansMono-BoldOblique.ttf	DejaVu Sans Mono Bold Oblique
DejaVuSansMono-Oblique.ttf	DejaVu Sans Mono Oblique
DejaVuSansMono.ttf	DejaVu Sans Mono
DejaVuSerif-Bold.ttf	DejaVu Serif Bold
DejaVuSerif-BoldOblique.ttf	DejaVu Serif Bold Oblique
DejaVuSerif-Oblique.ttf	DejaVu Serif Oblique
DejaVuSerif.ttf	DejaVu Serif
DejaVuSerifCondensed-Bold.ttf	DejaVu Serif Condensed Bold
DejaVuSerifCondensed-BoldOblique.ttf	DejaVu Serif Condensed Bold Oblique
DejaVuSerifCondensed-Oblique.ttf	DejaVu Serif Condensed Oblique
DejaVuSerifCondensed.ttf	DejaVu Serif Condensed
Garuda-Bold.ttf	Garuda Bold
Garuda-BoldOblique.ttf	Garuda Bold Oblique
Garuda-Oblique.ttf	Garuda Oblique
Garuda.ttf	Garuda
Vera.ttf	Bitstream Vera Sans
VeraBI.ttf	Bitstream Vera Sans Bold Oblique
VeraBd.ttf	Bitstream Vera Sans Bold
VeraIt.ttf	Bitstream Vera Sans Oblique
VeraMoBI.ttf	Bitstream Vera Sans Mono Bold Oblique
VeraMoBd.ttf	Bitstream Vera Sans Mono Bold
VeraMoIt.ttf	Bitstream Vera Sans Mono Oblique
VeraMono.ttf	Bitstream Vera Sans Mono
VeraSe.ttf	Bitstream Vera Serif
VeraSeBd.ttf	Bitstream Vera Serif Bold
 *
 *
 */

// limits
#define MAX_RENDER_COMMANDS	10000
#define MAX_VERTEX_COORDINATES	1000

class Q_DECL_EXPORT Graphics2D : public Graphics {

Q_OBJECT


public:
	Graphics2D(int display, Graphics2D* master = NULL);

	virtual ~Graphics2D();

	int initialize(screen_window_t screenWindow);

public Q_SLOTS:
	// resets the context to a default state
	bool reset();

	// rendering is complete
	void done();

	// Creates a new font context and loads a font for use with the API.
	Font* createFont(const QString& fontFileName, const QString* fontMetricsFileName, int pointSize, int dpi, const QString* fontCharacters = NULL);

	// Frees the specified font.
	void freeFont(Font* font);

	// create a new stroke type
    Stroke* createStroke(float width = 1.0, int cap = CAP_NONE, int join = JOIN_NONE, float miterLimit = 0.0, float* dash = NULL, int dashCount = 0, float dashPhase = 0.0);

	// create a new gradient
    Gradient* createGradient(int segments, GLColor* colors, float* percentages, float radius = 0.0, float angle = 0.0, float originU = 0.0, float originV = 0.0);

	// create a new image texture
    ImageTexture* createImageTexture(ImageData* image, int scaling, int tiling, float uScale, float vScale, int leftMargin, int rightMargin, int topMargin, int bottomMargin);

	// Concatenates the current Graphics2D Transform with a rotation transform.
	void 	rotate(double theta);

	// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
	void 	scale(double sx, double sy);

	// Translates the origin of the Graphics2D context to the point (x, y) in the current coordinate system.
	void 	translate(int tx, int ty);

	// Concatenates the current Graphics2D Transform with a translation transform.
	void 	translate(double tx, double ty);

	// Concatenates the current Graphics2D Transform with a shearing transform.
	void 	shear(int shx, int shy);

	// Concatenates the current Graphics2D Transform with a shearing transform.
	void 	shear(double shx, double shy);

	// Overwrites the Transform in the Graphics2D context.
	void 	transform(double** matrix);

	// Clears the specified rectangle by filling it with the background color of the current drawing surface.
	void clearRect(int x, int y, int width, int height);

	// Intersects the current clip with the specified rectangle.
	void clipRect(int x, int y, int width, int height);

	// Returns the background color used for clearing a region.
	const GLColor& getBackground();

	// Gets the current color.
	const GLColor& getColor();

	// Gets the current font.
	const Font* getFont();

	// Gets the current gradient.
	const Gradient* getGradient();

	// Gets the current image texture.
	const ImageTexture* getImageTexture();

	// Returns the current Stroke in the Graphics context.
    const Stroke* getStroke();

	// Sets the background color for the Graphics2D context.
	void setBackground(const GLColor& color);

	// Sets this graphics context's current color to the specified color.
	void setColor(const GLColor& color);

	// Sets this graphics context's font to the specified font.
	void setFont(Font* font);

	// Sets the gradient for the Graphics2D context.
	void setGradient(Gradient* gradient);

	// Sets the image texture for the Graphics2D context.
	void setImageTexture(ImageTexture* imageTexture);

	// Sets the Stroke for the Graphics2D context.
    void setStroke(Stroke *stroke);

	// Draws the outline of a circular or elliptical arc covering the specified rectangle.
	void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle);

	// Draws the outline of a circular or elliptical arc covering the specified rectangle.
	void drawArc(double x, double y, double width, double height, double startAngle, double arcAngle);

	// Draws as much of the specified image as is currently available.
	void drawImage(ImageData* image, int x, int y);

	// Draws as much of the specified image as is currently available.
	void drawImage(ImageData* image, int x, int y, GLColor backgroundColor);

	// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
	void drawImage(ImageData* image, int x, int y, int width, int height);

	// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
	void drawImage(ImageData* image, int x, int y, int width, int height, GLColor backgroundColor);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void drawImage(ImageData* image, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void drawImage(ImageData* image, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2, GLColor backgroundColor);

	// Draws as much of the specified image as is currently available.
	void drawImage(ImageData* image, double x, double y);

	// Draws as much of the specified image as is currently available.
	void drawImage(ImageData* image, double x, double y, GLColor backgroundColor);

	// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
	void drawImage(ImageData* image, double x, double y, double width, double height);

	// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
	void drawImage(ImageData* image, double x, double y, double width, double height, GLColor backgroundColor);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void drawImage(ImageData* image, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void drawImage(ImageData* image, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2, GLColor backgroundColor);

	// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
	void drawLine(int x1, int y1, int x2, int y2);

	// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
	void drawLine(double x1, double y1, double x2, double y2);

	// Draws the outline of an oval.
	void drawOval(int x, int y, int width, int height);

	// Draws the outline of an oval.
	void drawOval(double x, double y, double width, double height);

	// Draws a closed polygon defined by arrays of x and y coordinates.
	void drawPolygon(int* xPoints, int* yPoints, int nPoints);

	// Draws a closed polygon defined by arrays of x and y coordinates.
	void drawPolygon(double* xPoints, double* yPoints, int nPoints);

	// Draws a sequence of connected lines defined by arrays of x and y coordinates.
	void drawPolyline(int* xPoints, int* yPoints, int nPoints);

	// Draws a sequence of connected lines defined by arrays of x and y coordinates.
	void drawPolyline(double* xPoints, double* yPoints, int nPoints);

	// Draws the outline of the specified rectangle.
	void drawRect(int x, int y, int width, int height);

	// Draws the outline of the specified rectangle.
	void drawRect(double x, double y, double width, double height);

	// Draws an outlined round-cornered rectangle using this graphics context's current color.
	void drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight);

	// Draws an outlined round-cornered rectangle using this graphics context's current color.
	void drawRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight);

	// Measures the bounding box of the text of the specified String, using the current font in the Graphics2D context.
	void measureString(QString text, double* width, double* height);

	// Renders the text of the specified String, using the current text attribute state in the Graphics2D context.
	void drawString(QString text, int x, int y);

	// Renders the text specified by the specified String, using the current text attribute state in the Graphics2D context.
	void drawString(QString text, double x, double y);

	// Fills a circular or elliptical arc covering the specified rectangle.
	void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle);

	// Fills a circular or elliptical arc covering the specified rectangle.
	void fillArc(double x, double y, double width, double height, double startAngle, double arcAngle);

	// Fills an oval bounded by the specified rectangle with the current color.
	void fillOval(int x, int y, int width, int height);

	// Fills an oval bounded by the specified rectangle with the current color.
	void fillOval(double x, double y, double width, double height);

	// Fills the specified rectangle.
	void fillRect(int x, int y, int width, int height);

	// Fills the specified rectangle.
	void fillRect(double x, double y, double width, double height);

	// Fills a closed polygon defined by arrays of x and y coordinates.
	void fillPolygon(int* xPoints, int* yPoints, int nPoints);

	// Fills a closed polygon defined by arrays of x and y coordinates.
	void fillPolygon(double* xPoints, double* yPoints, int nPoints);

	// Fills the specified rounded corner rectangle with the current color.
	void fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight);

	// Fills the specified rounded corner rectangle with the current color.
	void fillRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight);


public:
	void render();

protected:

	// view transform functions
	void setupView(int x, int y, int width, int height);

	// low level primitives
	// Renders a quad or triangle.
	void renderDrawTriangles(int renderCount, int renderPoints);


	// Clears the specified rectangle by filling it with the background color of the current drawing surface.
	void renderClearRect(int commandCount);

	// Intersects the current clip with the specified rectangle.
	void renderClipRect(int commandCount);

	// Concatenates the current Graphics2D Transform with a rotation transform.
	void 	renderRotate(int commandCount);

	// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
	void 	renderScale(int commandCount);

	// Concatenates the current Graphics2D Transform with a translation transform.
	void 	renderTranslate(int commandCount);

	// Concatenates the current Graphics2D Transform with a shearing transform.
	void 	renderShear(int commandCount);

	// Overwrites the Transform in the Graphics2D context.
	void 	renderTransform(int commandCount);

	// Sets the background color for the Graphics2D context.
	void    renderSetBackground(int commandCount);

	// Sets this graphics context's current color to the specified color.
	void renderSetColor(int commandCount);

	// Sets this graphics context's font to the specified font.
	void renderSetFont(int commandCount);

	// Sets this graphics context's gradient to the specified gradient.
	void renderSetGradient(int commandCount);

	// Sets this graphics context's image texture to the specified image texture.
	void renderSetImageTexture(int commandCount);

	// Sets the Stroke for the Graphics2D context.
    void renderSetStroke(int commandCount);

	// Draws the outline of a circular or elliptical arc covering the specified rectangle.
	void renderDrawFillArc(int commandCount);

	// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
	void renderDrawImage(int commandCount);

	// Renders a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
	void renderDrawLine(int commandCount);

	// Draws a sequence of connected lines defined by arrays of x and y coordinates.
	void renderDrawPolyline(int commandCount);

	// Draws a sequence of connected lines defined by arrays of x and y coordinates.
	void renderDrawFillRoundRect(int commandCount);

	// Renders the text specified by the specified String, using the current text attribute state in the Graphics2D context.
	void 	renderDrawString(int commandCount);

	// Fills the specified polygon.
	void renderFillPolygon(int commandCount);


	// defaults for drawing
	GLColor _defaultForegroundColor;
	GLColor _defaultBackgroundColor;
	Stroke* _defaultStroke;

	// current values for drawing
	GLColor _foregroundColor;
	GLColor _backgroundColor;
	Font* _currentFont;
	Gradient* _currentGradient;
	ImageTexture* _currentImageTexture;
	Stroke* _currentStroke;

	int _commandCount;
	RenderCommand* _drawCommands;

	int _currentDrawFloatIndex;
	int* _drawFloatIndices;
	GLfloat* _drawFloats;

	int _currentDrawIntIndex;
	int* _drawIntIndices;
	int* _drawInts;
/*
	int _currentDrawBoolIndex;
	int* _drawBoolIndices;
	bool* _drawBools;
*/
	int _currentDrawPointerIndex;
	int* _drawPointerIndices;
	void** _drawPointers;

	// current values for rendering
	GLColor _renderForegroundColor;
	GLColor _renderBackgroundColor;
	Font* _renderFont;
	Gradient* _renderGradient;
	ImageTexture* _renderImageTexture;
	Stroke* _renderStroke;
#ifdef GLES2
	GLfloat* _transformMatrix;
	GLfloat* _renderModelMatrix;
#endif

	GLuint* _renderVertexIndices;
	GLfloat* _renderVertexCoords;
	GLfloat* _renderTextureCoords;

#ifdef GLES2
	GLuint   _polyColorRenderingProgram;
	GLuint   _polyTextureRenderingProgram;
	GLuint   _polyGradientRenderingProgram;
	GLuint   _textRenderingProgram;
	GLfloat* _orthoMatrix;
	GLfloat* _scaleMatrix;
#endif


	// state variables
	QMap<ImageData*,int> _glTextureIDImageMap;
	QMutex _drawMutex;
	QMutex _refreshMutex;
	bool _drawing;

	Graphics2D* _master2D;
};

  }
}

#endif /* GRAPHICS2D_HPP */
