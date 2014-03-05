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

#include "Graphics2D.hpp"
#include <math.h>

#include <QDebug>

using namespace bb::cascades;

namespace views {
	namespace graphics {

#ifdef GLES2
// shaders for 2D graphics

// vertex shader for 2D projection
const char* vSource_2D =
		"attribute vec2 a_position;\r\n"
		"uniform mat4 u_modelViewMatrix;\r\n"
		"uniform mat4 u_projectionMatrix;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(a_position, 0.0, 1.0);\r\n"
		"}";

const char* vSource_2DTexture =
		"attribute vec2 a_position;\r\n"
		"attribute vec2 a_texcoord;\r\n"
		"uniform mat4 u_modelViewMatrix;\r\n"
		"uniform mat4 u_projectionMatrix;\r\n"
		"varying vec2 v_texcoord;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(a_position, 0.0, 1.0);\r\n"
		"    v_texcoord = a_texcoord;\r\n"
		"}";

const char* vSource_2DMaskTexture =
		"attribute vec2 a_position;\r\n"
		"attribute vec2 a_texcoord;\r\n"
		"attribute vec2 a_maskTexcoord;\r\n"
		"uniform mat4 u_modelViewMatrix;\r\n"
		"uniform mat4 u_projectionMatrix;\r\n"
		"varying vec2 v_texcoord;\r\n"
		"varying vec2 v_maskTexcoord;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    gl_Position = u_projectionMatrix * u_modelViewMatrix * vec4(a_position, 0.0, 1.0);\r\n"
		"    v_texcoord = a_texcoord;\r\n"
		"    v_maskTexcoord = a_maskTexcoord;\r\n"
		"}";

// fragment shader for 2D texture maps
const char* fSource_2DTexture =
		"#ifdef GL_ES\r\n"
		"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
		"        precision highp float;\r\n"
		"    #else\r\n"
		"        precision mediump float;\r\n"
		"    #endif\r\n"
		"#endif\r\n"
		"uniform sampler2D u_texture;\r\n"
		"varying vec2 v_texcoord;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    gl_FragColor = texture2D(u_texture, v_texcoord);\r\n"
		"}";

// fragment shader for solid color
const char* fSource_solidColor =
		"#ifdef GL_ES\r\n"
		"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
		"        precision highp float;\r\n"
		"    #else\r\n"
		"        precision mediump float;\r\n"
		"    #endif\r\n"
		"#endif\r\n"
		"uniform vec4 u_color;\r\n"
		"void main()\r\n"
		"{\r\n"
        "    gl_FragColor = u_color;\r\n"
		"}";

const char* fSource_uvtest =
		"#ifdef GL_ES\r\n"
		"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
		"        precision highp float;\r\n"
		"    #else\r\n"
		"        precision mediump float;\r\n"
		"    #endif\r\n"
		"#endif\r\n"
		"varying vec2 v_texcoord;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    gl_FragColor = vec4(v_texcoord, 1.0, 1.0);\r\n"
		"}";

const char* fSource_gradient =
		"#ifdef GL_ES\r\n"
		"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
		"        precision highp float;\r\n"
		"    #else\r\n"
		"        precision mediump float;\r\n"
		"    #endif\r\n"
		"#endif\r\n"
		"varying vec2 v_texcoord;\r\n"
		"varying vec2 v_maskTexcoord;\r\n"
		"uniform int u_segments;\r\n"
		"uniform vec4[20] u_colors;\r\n"
		"uniform float[41] u_percentages;\r\n"
		"uniform float u_radius;\r\n"
		"uniform float u_angle;\r\n"
		"uniform vec2 u_origin;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    float r;\r\n"
		"    float g;\r\n"
		"    float b;\r\n"
		"    float a;\r\n"
		"    float u;\r\n"
		"    float v;\r\n"
		"    float rotu;\r\n"
		"    float rotv;\r\n"
		"    float length;\r\n"
		"    float percent;\r\n"
		"    float realpercent;\r\n"
		"    float factor;\r\n"
		"    float realfactor;\r\n"
		"    int index;\r\n"
		"    \r\n"
		"    r = 0.0;\r\n"
		"    g = 0.0;\r\n"
		"    b = 0.0;\r\n"
		"    a = 1.0;\r\n"
		"    u = v_texcoord.x - u_origin.x;\r\n"
		"    v = v_texcoord.y - u_origin.y;\r\n"
		"    if (u_radius > 0.0) {\r\n"
		"        length = sqrt(u*u + v*v);\r\n"
		"        percent = length / u_radius;\r\n"
		"    } else {\r\n"
		"        rotu = cos(radians(u_angle))*u + sin(radians(u_angle))*v;\r\n"
		"        rotv = cos(radians(u_angle+90.0))*u + sin(radians(u_angle+90.0))*v;\r\n"
		"        percent = rotu;\r\n"
		"    }\r\n"
		"    if (percent < 0.0) {\r\n"
		"        percent += 1.0;\r\n"
		"    }\r\n"
		"    if (percent > 1.0) {\r\n"
		"        percent = 1.0;\r\n"
		"    }\r\n"
		"    if (percent < 0.0) {\r\n"
		"        percent = 0.0;\r\n"
		"    }\r\n"
		"    \r\n"
		"    if (u_segments > 0) {\r\n"
		"        for (index = 0; index < 2*u_segments; index += 2) {\r\n"
		"            if (percent >= u_percentages[ index] && percent <= u_percentages[ index+2]) {\r\n"
		"                factor = (percent - u_percentages[ index]) / (u_percentages[ index+2] - u_percentages[ index]);\r\n"
		"                if (factor <= u_percentages[ index+1]) {\r\n"
		"                    realfactor = 0.5 * (factor - u_percentages[ index]) / (u_percentages[ index+1] - u_percentages[ index]);\r\n"
		"                }\r\n"
		"                if (factor > u_percentages[index+1]) {\r\n"
		"                    realfactor = 0.5 + (factor - u_percentages[ index+1]) / (u_percentages[ index+2] - u_percentages[ index+1]);\r\n"
		"                }\r\n"
		"                realpercent = (1.0 - realfactor) * u_percentages[ index] + realfactor * u_percentages[ index+2];\r\n"
		"                r = (1.0-realpercent) * u_colors[ 0].x + realpercent * u_colors[ 1].x;\r\n"
		"                g = (1.0-realpercent) * u_colors[ 0].y + realpercent * u_colors[ 1].y;\r\n"
		"                b = (1.0-realpercent) * u_colors[ 0].z + realpercent * u_colors[ 1].z;\r\n"
		"                a = (1.0-realpercent) * u_colors[ 0].a + realpercent * u_colors[ 1].a;\r\n"
		"            }\r\n"
		"        }\r\n"
		"    }\r\n"
		"    gl_FragColor = vec4(r, g, b, a);\r\n"
		"}";

const char* fSource_2DMaskGradient =
		"#ifdef GL_ES\r\n"
		"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
		"        precision highp float;\r\n"
		"    #else\r\n"
		"        precision mediump float;\r\n"
		"    #endif\r\n"
		"#endif\r\n"
		"varying vec2 v_texcoord;\r\n"
		"varying vec2 v_maskTexcoord;\r\n"
		"uniform int u_segments;\r\n"
		"uniform vec4[20] u_colors;\r\n"
		"uniform float[41] u_percentages;\r\n"
		"uniform float u_radius;\r\n"
		"uniform float u_angle;\r\n"
		"uniform vec2 u_origin;\r\n"
		"uniform sampler2D u_texture;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    float r;\r\n"
		"    float g;\r\n"
		"    float b;\r\n"
		"    float a;\r\n"
		"    float u;\r\n"
		"    float v;\r\n"
		"    float rotu;\r\n"
		"    float rotv;\r\n"
		"    float length;\r\n"
		"    float percent;\r\n"
		"    float realpercent;\r\n"
		"    float factor;\r\n"
		"    float realfactor;\r\n"
		"    int index;\r\n"
		"    \r\n"
		"    r = 0.0;\r\n"
		"    g = 0.0;\r\n"
		"    b = 0.0;\r\n"
		"    a = 1.0;\r\n"
		"    u = v_texcoord.x - u_origin.x;\r\n"
		"    v = v_texcoord.y - u_origin.y;\r\n"
		"    if (u_radius > 0.0) {\r\n"
		"        length = sqrt(u*u + v*v);\r\n"
		"        percent = length / u_radius;\r\n"
		"    } else {\r\n"
		"        rotu = cos(radians(u_angle))*u + sin(radians(u_angle))*v;\r\n"
		"        rotv = cos(radians(u_angle+90.0))*u + sin(radians(u_angle+90.0))*v;\r\n"
		"        percent = rotu;\r\n"
		"    }\r\n"
		"    if (percent < 0.0) {\r\n"
		"        percent += 1.0;\r\n"
		"    }\r\n"
		"    if (percent > 1.0) {\r\n"
		"        percent = 1.0;\r\n"
		"    }\r\n"
		"    if (percent < 0.0) {\r\n"
		"        percent = 0.0;\r\n"
		"    }\r\n"
		"    \r\n"
		"    if (u_segments > 0) {\r\n"
		"        for (index = 0; index < 2*u_segments; index += 2) {\r\n"
		"            if (percent >= u_percentages[ index] && percent <= u_percentages[ index+2]) {\r\n"
		"                factor = (percent - u_percentages[ index]) / (u_percentages[ index+2] - u_percentages[ index]);\r\n"
		"                if (factor <= u_percentages[ index+1]) {\r\n"
		"                    realfactor = 0.5 * (factor - u_percentages[ index]) / (u_percentages[ index+1] - u_percentages[ index]);\r\n"
		"                }\r\n"
		"                if (factor > u_percentages[index+1]) {\r\n"
		"                    realfactor = 0.5 + (factor - u_percentages[ index+1]) / (u_percentages[ index+2] - u_percentages[ index+1]);\r\n"
		"                }\r\n"
		"                realpercent = (1.0 - realfactor) * u_percentages[ index] + realfactor * u_percentages[ index+2];\r\n"
		"                r = (1.0-realpercent) * u_colors[ 0].x + realpercent * u_colors[ 1].x;\r\n"
		"                g = (1.0-realpercent) * u_colors[ 0].y + realpercent * u_colors[ 1].y;\r\n"
		"                b = (1.0-realpercent) * u_colors[ 0].z + realpercent * u_colors[ 1].z;\r\n"
		"                a = (1.0-realpercent) * u_colors[ 0].a + realpercent * u_colors[ 1].a;\r\n"
		"            }\r\n"
		"        }\r\n"
		"    }\r\n"
		"    vec4 temp = texture2D(u_texture, v_maskTexcoord);\r\n"
		"    gl_FragColor = vec4(r, g, b, a) * temp;\r\n"
		"}";


// fragment shader for text surfaces
const char* fSource_textSurface =
		"#ifdef GL_ES\r\n"
		"    #ifdef GL_FRAGMENT_PRECISION_HIGH\r\n"
		"        precision highp float;\r\n"
		"    #else\r\n"
		"        precision mediump float;\r\n"
		"    #endif\r\n"
		"#endif\r\n"
		"varying vec2 v_texcoord;\r\n"
		"uniform sampler2D u_texture;\r\n"
		"uniform vec4 u_color;\r\n"
		"void main()\r\n"
		"{\r\n"
		"    vec4 temp = texture2D(u_texture, v_texcoord);\r\n"
		"    gl_FragColor = u_color * temp;\r\n"
		"}";

#endif

Graphics2D::Graphics2D(int display, Graphics2D* master) : Graphics(display, master)
{
	qDebug()  << "Graphics2D: Graphics2D : " << master;

	if (master) {
		_master2D = master;
	} else {
		_master2D = this;
	}

	// let set some reasonable limits for the time being
	_commandCount = 0;
	if (_master2D != this) {
		_drawCommands = NULL;
	} else {
		_drawCommands = new RenderCommand[MAX_RENDER_COMMANDS];
	}

	_currentDrawFloatIndex = 0;
	if (_master2D != this) {
		_drawFloatIndices = NULL;
		_drawFloats = NULL;
	} else {
		_drawFloatIndices = new int[2*MAX_RENDER_COMMANDS];
		_drawFloats = new GLfloat[2*MAX_RENDER_COMMANDS];
	}

	_currentDrawIntIndex = 0;
	if (_master2D != this) {
		_drawIntIndices = NULL;
		_drawInts = NULL;
	} else {
		_drawIntIndices = new int[2*MAX_RENDER_COMMANDS];
		_drawInts = new int[2*MAX_RENDER_COMMANDS];
	}

	_currentDrawPointerIndex = 0;
	if (_master2D != this) {
		_drawPointerIndices = NULL;
		_drawPointers = NULL;
	} else {
		_drawPointerIndices = new int[2*MAX_RENDER_COMMANDS];
		_drawPointers = new void*[MAX_RENDER_COMMANDS];
	}


	if (_master2D != this) {
		_renderVertexIndices = NULL;
	} else {
		_renderVertexIndices = new GLuint[MAX_VERTEX_COORDINATES];
	}

	if (_master2D != this) {
		_renderVertexCoords = NULL;
	} else {
		_renderVertexCoords = new GLfloat[MAX_VERTEX_COORDINATES];
	}

	if (_master2D != this) {
		_renderTextureCoords = NULL;
	} else {
		_renderTextureCoords = new GLfloat[MAX_VERTEX_COORDINATES];
	}

	if (_master2D != this) {
		_renderMaskTextureCoords = NULL;
	} else {
		_renderMaskTextureCoords = new GLfloat[MAX_VERTEX_COORDINATES];
	}

#ifdef GLES2
	// allocate transformation matrices

	_orthoMatrix = (GLfloat*)calloc(4 * 4, sizeof(GLfloat));
	memset(_orthoMatrix, 0, sizeof(GLfloat) * 4 * 4);
	_orthoMatrix[0]  = 1.0f;
	_orthoMatrix[5]  = 1.0f;
	_orthoMatrix[10] = 1.0f;
	_orthoMatrix[15] = 1.0f;

	_scaleMatrix = (GLfloat*)calloc(4 * 4, sizeof(GLfloat));
	memset(_scaleMatrix, 0, sizeof(GLfloat) * 4 * 4);
	_scaleMatrix[0]  = 1.0f;
	_scaleMatrix[5]  = 1.0f;
	_scaleMatrix[10] = 1.0f;
	_scaleMatrix[15] = 1.0f;

	_transformMatrix = (GLfloat*)calloc(4 * 4, sizeof(GLfloat));
	memset(_transformMatrix, 0, sizeof(GLfloat) * 4 * 4);
	_transformMatrix[0]  = 1.0f;
	_transformMatrix[5]  = 1.0f;
	_transformMatrix[10] = 1.0f;
	_transformMatrix[15] = 1.0f;

	_renderModelMatrix = (GLfloat*)calloc(4 * 4, sizeof(GLfloat));
	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	_renderModelMatrix[0]  = 1.0f;
	_renderModelMatrix[5]  = 1.0f;
	_renderModelMatrix[10] = 1.0f;
	_renderModelMatrix[15] = 1.0f;

#endif

	// set the default foreground colour to black
	_defaultForegroundColor.red = 0.0f;
	_defaultForegroundColor.green = 0.0f;
	_defaultForegroundColor.blue = 0.0f;
	_defaultForegroundColor.alpha = 1.0f;

	// set the default background colour to white
	_defaultBackgroundColor.red = 1.0f;
	_defaultBackgroundColor.green = 1.0f;
	_defaultBackgroundColor.blue = 1.0f;
	_defaultBackgroundColor.alpha = 1.0f;

	_defaultStroke = createStroke(1.0);

	_drawing = false;
}

Graphics2D::~Graphics2D()
{
	cleanup();

#ifdef GLES2
	if (_transformMatrix) {
		free(_transformMatrix);
	}

	if (_scaleMatrix) {
		free(_scaleMatrix);
	}

	if (_renderModelMatrix) {
		free(_renderModelMatrix);
	}
#endif

	if (_drawCommands) {
		delete _drawCommands;
	}

	if (_drawFloatIndices) {
		delete _drawFloatIndices;
	}

	if (_drawFloats) {
		delete _drawFloats;
	}

	if (_drawIntIndices) {
		delete _drawIntIndices;
	}
	if (_drawInts) {
		delete _drawInts;
	}

	if (_drawPointerIndices) {
		delete _drawPointerIndices;
	}

	if (_drawPointers) {
		delete _drawPointers;
	}

	if (_renderVertexIndices) {
		delete _renderVertexIndices;
	}

	if (_renderVertexCoords) {
		delete _renderVertexCoords;
	}

	if (_renderTextureCoords) {
		delete _renderTextureCoords;
	}

	if (_renderMaskTextureCoords) {
		delete _renderMaskTextureCoords;
	}
}

void Graphics2D::cleanup() {
	QList<ImageData*> textureImages = _glTextureIDImageMap.keys();

	if (_glTextureIDImageMap.size() > 0) {
		do {
			ImageData *image = textureImages[0];
			_glTextureIDImageMap.take(image);
			textureImages.removeAt(0);
		} while (_glTextureIDImageMap.size() > 0);
	}
}

int Graphics2D::initialize(screen_window_t screenWindow)
{
	qDebug()  << "Graphics2D::initialize";

	int returnCode = Graphics::initialize(screenWindow);
	if (returnCode == EXIT_SUCCESS) {
		// initialize 2D-specific code

#ifdef GLES2
		_polyColorRenderingProgram = loadShader(vSource_2D, fSource_solidColor);
		if(_polyColorRenderingProgram == 0) {
			qCritical() << "Initialize _polyColorRenderingProgram failed\n";
		}

		_polyTextureRenderingProgram = loadShader(vSource_2DTexture, fSource_2DTexture);
		if(_polyTextureRenderingProgram == 0) {
			qCritical() << "Initialize _polyTextureRenderingProgram failed\n";
		}

		_polyGradientRenderingProgram = loadShader(vSource_2DTexture, fSource_gradient);
		if(_polyGradientRenderingProgram == 0) {
			qCritical() << "Initialize _polyGradientRenderingProgram failed\n";
		}

		_textRenderingProgram = loadShader(vSource_2DTexture, fSource_textSurface);
		//_textRenderingProgram = loadShader(vSource_2DTexture, fSource_uvtest);
		if(_textRenderingProgram == 0) {
			qCritical() << "Initialize _textRenderingProgram failed\n";
		}

		_textGradientRenderingProgram = loadShader(vSource_2DMaskTexture, fSource_2DMaskGradient);
		if(_textGradientRenderingProgram == 0) {
			qCritical() << "Initialize _textGradientRenderingProgram failed\n";
		}
#endif
	}

	qDebug()  << "Graphics2D::initialize " << ":" << returnCode;

    return returnCode;
}

bool Graphics2D::reset()
{
	//qDebug()  << "Graphics2D::reset";

	bool proceed = false;

	_master2D->_refreshMutex.lock();

	_master2D->_drawMutex.lock();

	if (_master2D->_drawing == false) {
		_master2D->_drawing = true;
		proceed = true;
	}

	_master2D->_drawMutex.unlock();

	if (proceed) {
		_master2D->_commandCount = 0;
		_master2D->_currentDrawFloatIndex = 0;
		_master2D->_currentDrawIntIndex = 0;
		_master2D->_currentDrawPointerIndex = 0;

#ifdef GLES2
		// setup with identity matrix
		memset(_transformMatrix, 0, sizeof(GLfloat) * 4 * 4);
		_transformMatrix[0]  = 1.0f;
		_transformMatrix[5]  = 1.0f;
		_transformMatrix[10] = 1.0f;
		_transformMatrix[15] = 1.0f;

		int i, j, k;
		memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				for(k = 0; k < 4; k++) {
					_renderModelMatrix[i * 4 + j] += _transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
				}
			}
		}
#endif

		// set the default foreground color and background color
		_master2D->setColor(_defaultForegroundColor);
		_master2D->setBackground(_defaultBackgroundColor);
		_master2D->setStroke(_defaultStroke);
		_master2D->clearRect(0, 0, _width, _height);
		_master2D->_currentFont = NULL;
		_master2D->_currentGradient = NULL;
		_master2D->_currentImageTexture = NULL;
	}

	//qDebug()  << "Graphics2D::reset: proceed: " << proceed;

	return proceed;
}

void Graphics2D::done()
{
	//qDebug()  << "Graphics2D::done";

	_master2D->_drawMutex.lock();

	if (_master2D->_drawing == true) {
		_master2D->_drawing = false;
	}

	_master2D->_drawMutex.unlock();

	_master2D->_refreshMutex.unlock();
}

// creates a new font
Font* Graphics2D::createFont(const QString& fontFileName, const QString* fontMetricsFileName, int pointSize, int dpi, const QString* fontCharacters)
{
    FT_Library library;
    FT_Face face;
    int c;
    int i, j;
    Font* font;
    wchar_t* wideCharacters = NULL;
    const int FREETYPE_BITMAP_DPI = 64;

    if (fontFileName.size() == 0){
        qCritical() << "Graphics2D::createFont: Invalid font file\n";
        return NULL;
    }

    if(FT_Init_FreeType(&library)) {
    	qCritical() << "Graphics2D::createFont: Error loading Freetype library\n";
        return NULL;
    }
    if (FT_New_Face(library, fontFileName.toAscii().constData(), 0,&face)) {
    	qCritical() << "Graphics2D::createFont: Error loading font from " << fontFileName << "\n";
        return NULL;
    }

    if (fontMetricsFileName) {
        if (FT_Attach_File( face, fontMetricsFileName->toAscii().constData() )) {
			qCritical() << "Graphics2D::createFont: Error loading font metrics from " << fontMetricsFileName << "\n";
			return NULL;
    	}
	}

    if(FT_Set_Char_Size ( face, pointSize * FREETYPE_BITMAP_DPI, pointSize * FREETYPE_BITMAP_DPI, dpi, dpi)) {
    	qCritical() << "Graphics2D::createFont: Error initializing character parameters\n";
        return NULL;
    }

    font = (Font*) malloc(sizeof(Font));

    if (!font) {
    	qCritical() << "Graphics2D::createFont: Unable to allocate memory for font structure\n";
        return NULL;
    }

    font->initialized = 0;
    font->pt = pointSize;

    // calculate the max number of characters in the font
    font->numberCharacters = 0;
    int glyphIndex;

    // initialize char code for processing
	if (fontCharacters) {
		wideCharacters = new wchar_t[fontCharacters->size()+1];
		int length = fontCharacters->toWCharArray(wideCharacters);
		wideCharacters[fontCharacters->size()] = 0;

		for(i = 0; i < wcslen(wideCharacters); i++) {
			glyphIndex = FT_Get_Char_Index(face, wideCharacters[i]);
			if (glyphIndex <= 0) {
				//qDebug() << "Graphics2D::createFont: font does not contain this character 0x" << c << "\n";
			} else {
				font->numberCharacters++;
			}
		}
    } else {
 		for(c = 0; c < MAX_GLYPHS; c++) {
	        glyphIndex = FT_Get_Char_Index(face, c);
	        if (glyphIndex <= 0) {
	            //qDebug() << "Graphics2D::createFont: font does not contain this character 0x" << c << "\n";
	    	} else {
	    		font->numberCharacters++;
	    	}
	    }
    }

    font->charMap = (int*)calloc(font->numberCharacters, sizeof(int));
	memset(font->charMap, 0, font->numberCharacters * sizeof(int));

	int count = 0;
	if (fontCharacters) {
		for(i = 0; i < wcslen(wideCharacters); i++) {
			glyphIndex = FT_Get_Char_Index(face, wideCharacters[i]);
			if (glyphIndex <= 0) {
				//qDebug() << "Graphics2D::createFont: font does not contain this character 0x" << c << "\n";
			} else {
	    		font->charMap[count] = wideCharacters[i];
	    		count++;
			}
		}
    } else {
		for(c = 0; c < MAX_GLYPHS; c++) {
	        glyphIndex = FT_Get_Char_Index(face, c);
	        if (glyphIndex <= 0) {
	            //qDebug() << "Graphics2D::createFont: font does not contain this character 0x" << c << "\n";
	    	} else {
	    		font->charMap[count] = c;
	    		count++;
	    	}
	    }
    }

	//qDebug() << ""font max characters: " << font->numberCharacters << "\n";

    font->kerning = (float*)calloc(font->numberCharacters*font->numberCharacters, sizeof(float));
	memset(font->kerning, 0, font->numberCharacters*font->numberCharacters*sizeof(float));

    font->advance = (float*)calloc(font->numberCharacters, sizeof(float));
    font->width = (float*)calloc(font->numberCharacters, sizeof(float));
    font->height = (float*)calloc(font->numberCharacters, sizeof(float));
    font->texX1 = (float*)calloc(font->numberCharacters, sizeof(float));
    font->texX2 = (float*)calloc(font->numberCharacters, sizeof(float));
    font->texY1 = (float*)calloc(font->numberCharacters, sizeof(float));
    font->texY2 = (float*)calloc(font->numberCharacters, sizeof(float));
    font->offsetX = (float*)calloc(font->numberCharacters, sizeof(float));
    font->offsetY = (float*)calloc(font->numberCharacters, sizeof(float));

    //Let each glyph reside in 32x32 section of the font texture
    int segmentsSizeX = 0, segmentsSizeY = 0;
    int numSegmentsX;
    int numSegmentsY;
    if (font->numberCharacters <= 128) {
    	   numSegmentsX = 16;
    	   numSegmentsY = 8;
    } else if (font->numberCharacters <= 256) {
		   numSegmentsX = 16;
		   numSegmentsY = 16;
	} else if (font->numberCharacters <= 512) {
		   numSegmentsX = 32;
		   numSegmentsY = 16;
	} else if (font->numberCharacters <= 1024) {
		   numSegmentsX = 32;
		   numSegmentsY = 32;
	} else if (font->numberCharacters <= 2048) {
		   numSegmentsX = 64;
		   numSegmentsY = 32;
	} else if (font->numberCharacters <= 4096) {
		   numSegmentsX = 64;
		   numSegmentsY = 64;
	} else if (font->numberCharacters <= 8192) {
		   numSegmentsX = 128;
		   numSegmentsY = 64;
	} else if (font->numberCharacters <= 16384) {
		   numSegmentsX = 128;
		   numSegmentsY = 128;
	} else if (font->numberCharacters <= 32768) {
		   numSegmentsX = 256;
		   numSegmentsY = 128;
	} else if (font->numberCharacters <= 65536) {
		   numSegmentsX = 256;
		   numSegmentsY = 256;
    }

    //qDebug() << "Graphics2D::createFont: font texture segments: " << numSegmentsX << " " << numSegmentsY << "\n";

    FT_GlyphSlot slot;
    FT_Bitmap bmp;
    int glyphWidth, glyphHeight;

    //First calculate the max width and height of a character in a passed font
    for(c = 0; c < font->numberCharacters; c++) {
        if(FT_Load_Char(face, font->charMap[c], FT_LOAD_RENDER)) {
        	//qDebug() << "Graphics2D::createFont: FT_Load_Char " << c << " failed ... must be it for glyphs\n";
            //free(font);
            //return NULL;
        } else {

			slot = face->glyph;
			bmp = slot->bitmap;

			glyphWidth = bmp.width;
			glyphHeight = bmp.rows;

			if (glyphWidth > segmentsSizeX) {
				segmentsSizeX = glyphWidth;
			}

			if (glyphHeight > segmentsSizeY) {
				segmentsSizeY = glyphHeight;
			}
        }
    }

    int fontTexWidth = nextp2(numSegmentsX * segmentsSizeX);
    int fontTexHeight = nextp2(numSegmentsY * segmentsSizeY);

    int bitmap_offset_x = 0, bitmap_offset_y = 0;

	font->image = new ImageData(bb::PixelFormat::RGBA_Premultiplied, fontTexWidth, fontTexHeight);

    if (!font->image) {
    	qDebug() << "Graphics2D::createFont: Failed to allocate memory for font texture\n";
        free(font);
        return NULL;
    }

    GLubyte* fontTextureData = (GLubyte*)font->image->pixels();
    //GLubyte* fontTextureData = (GLubyte*) calloc(2 * fontTexWidth * fontTexHeight, sizeof(GLubyte));

    // Fill font texture bitmap with individual bmp data and record appropriate size, texture coordinates and offsets for every glyph
    for(c = 0; c < font->numberCharacters; c++) {
        if(FT_Load_Char(face, font->charMap[c], FT_LOAD_RENDER)) {
        	//qDebug() << "Graphics2D::createFont: FT_Load_Char " << c << " failed ... must be it for glyphs\n";
            //free(font);
            //return NULL;
        } else {
        	//qDebug() << "Graphics2D::createFont: FT_Load_Char " << c << " success ... \n";

			slot = face->glyph;
			bmp = slot->bitmap;

			glyphWidth = bmp.width;
			glyphHeight = bmp.rows;

			div_t temp = div(c, numSegmentsX);

			bitmap_offset_x = segmentsSizeX * temp.rem;
			bitmap_offset_y = segmentsSizeY * temp.quot;

			for (j = 0; j < glyphHeight; j++) {
				for (i = 0; i < glyphWidth; i++) {
					fontTextureData[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * fontTexWidth) + 0] =
							fontTextureData[2 * ((bitmap_offset_x + i) + (j + bitmap_offset_y) * fontTexWidth) + 1] =
						(i >= bmp.width || j >= bmp.rows)? 0 : bmp.buffer[i + bmp.width * j];
				}
			}

			//font->advance[c] = (float)(slot->advance.x >> 6);
			font->advance[c] = (float)(slot->advance.x) / (float)FREETYPE_BITMAP_DPI;
			font->texX1[c] = (float)bitmap_offset_x / (float) fontTexWidth;
			font->texX2[c] = (float)(bitmap_offset_x + bmp.width) / (float)fontTexWidth;
			font->texY1[c] = (float)bitmap_offset_y / (float) fontTexHeight;
			font->texY2[c] = (float)(bitmap_offset_y + bmp.rows) / (float)fontTexHeight;
			font->width[c] = bmp.width;
			font->height[c] = bmp.rows;
			font->offsetX[c] = (float)slot->bitmap_left;
			//font->offsetY[c] =  (float)((slot->metrics.horiBearingY-face->glyph->metrics.height) >> 6);
			font->offsetY[c] =  (float)(slot->metrics.horiBearingY-face->glyph->metrics.height) / (float)FREETYPE_BITMAP_DPI;
		}
    }

	int previousGlyphIndex;
	FT_Vector delta;

    for(i = 0; i < font->numberCharacters; i++) {
        for(j = 0; j < font->numberCharacters; j++) {
			previousGlyphIndex = FT_Get_Char_Index( face, font->charMap[i] );
			glyphIndex = FT_Get_Char_Index( face, font->charMap[j] );

			FT_Get_Kerning( face, previousGlyphIndex, glyphIndex, FT_KERNING_DEFAULT, &delta );

			font->kerning[i * font->numberCharacters + j] += delta.x / (float)FREETYPE_BITMAP_DPI;
    	}
	}

    /*
    	glGenTextures(1, &(font->fontTexture));

        glBindTexture(GL_TEXTURE_2D, font->fontTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, fontTexWidth, fontTexHeight, 0, GL_LUMINANCE_ALPHA , GL_UNSIGNED_BYTE, fontTextureData);
    */
    //free(fontTextureData);

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    font->initialized = 1;
    return font;
}

// Frees the specified font.
void Graphics2D::freeFont(Font* font)
{
	if (!font) {
		return;
	}

	glDeleteTextures(1, &(font->fontTexture));

	free(font->offsetY);
	free(font->offsetX);
	free(font->texY2);
	free(font->texY1);
	free(font->texX2);
	free(font->texX1);
	free(font->height);
	free(font->width);
	free(font->advance);
	free(font->kerning);
	free(font->charMap);

	free(font);

	if (_currentFont == font) {
		_currentFont = NULL;
	}
}

// create a new gradient
Gradient* Graphics2D::createGradient(int segments, GLColor* colors, float* percentages, float radius, float angle, float originU, float originV)
{
	Gradient *gradient = new Gradient;

	if (gradient) {
		gradient->segments = segments;
		gradient->colors = colors;
		gradient->percentages = percentages;
		gradient->radius = radius;
		gradient->angle = angle;
		gradient->originU = originU;
		gradient->originV = originV;
	}

	return gradient;
}

// create a new image texture
ImageTexture* Graphics2D::createImageTexture(ImageData* image, int scaling, int tiling, float uScale, float vScale, int leftMargin, int rightMargin, int topMargin, int bottomMargin)
{
	ImageTexture *imageTexture = new ImageTexture;

	if (imageTexture) {
		imageTexture->image = image;
		imageTexture->scaling = scaling;
		imageTexture->tiling = tiling;
		imageTexture->uScale = uScale;
		imageTexture->vScale = vScale;
		imageTexture->leftMargin = leftMargin;
		imageTexture->rightMargin = rightMargin;
		imageTexture->topMargin = topMargin;
		imageTexture->bottomMargin = bottomMargin;
	}

	return imageTexture;
}

// Returns the current Stroke in the Graphics context.
Stroke* Graphics2D::createStroke(float width, int cap, int join, float miterLimit, float* dash, int dashCount, float dashPhase)
{
	Stroke *stroke = new Stroke;

	if (stroke) {
		stroke->width = width;
		stroke->cap = cap;
		stroke->join = join;
		stroke->miterLimit = miterLimit;
		stroke->dash = dash;
		stroke->dashCount = dashCount;
		stroke->dashPhase = dashPhase;
	}

	return stroke;
}


// Concatenates the current Graphics2D Transform with a rotation transform.
void Graphics2D::rotate(double theta)
{
	//qDebug()  << "Graphics2D::rotate: " << theta;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)theta;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_TRANSFORM_ROTATE;
}

// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
void Graphics2D::scale(double sx, double sy)
{
	//qDebug()  << "Graphics2D::rotate: " << sx << ":" << sy;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)sx;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)sy;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_TRANSFORM_SCALE;
}

// Translates the origin of the Graphics2D context to the point (x, y) in the current coordinate system.
void Graphics2D::translate(int tx, int ty)
{
	translate((double)tx, (double)ty);
}

// Concatenates the current Graphics2D Transform with a translation transform.
void Graphics2D::translate(double tx, double ty)
{
	//qDebug()  << "Graphics2D::translate: " << tx << ":" << ty;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)tx;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)ty;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_TRANSFORM_TRANSLATE;
}

// Concatenates the current Graphics2D Transform with a shearing transform.
void Graphics2D::shear(int shx, int shy)
{
	shear((double)shx, (double)shy);
}

// Concatenates the current Graphics2D Transform with a shearing transform.
void Graphics2D::shear(double shx, double shy)
{
	//qDebug()  << "Graphics2D::shear: " << shx << ":" << shy;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)shx;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)shy;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_TRANSFORM_SHEAR;
}

// Overwrites the Transform in the Graphics2D context.
void Graphics2D::transform(double** matrix)
{
	//qDebug()  << "Graphics2D::transform: " << matrix;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)matrix[i][j];
		}
	}

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_TRANSFORM_SHEAR;
}

// Clears the specified rectangle by filling it with the background color of the current drawing surface.
void Graphics2D::clearRect(int x, int y, int width, int height)
{
	//qDebug()  << "Graphics2D::clearRect: " << x << "," << y << " , " << width << " x " << height;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = x;
	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = y;
	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = width;
	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = height;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_CLEAR_RECT;
}

// Intersects the current clip with the specified rectangle.
void Graphics2D::clipRect(int x, int y, int width, int height)
{
	//qDebug()  << "Graphics2D::clipRect: " << x << "," << y << " , " << width << " x " << height;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = x;
	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = y;
	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = width;
	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = height;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_CLIP_RECT;
}

// Returns the background color used for clearing a region.
const GLColor& Graphics2D::getBackground()
{
	return _master2D->_backgroundColor;
}

// Gets the current color.
const GLColor& Graphics2D::getColor()
{
	return _master2D->_foregroundColor;
}

// Gets the current font.
const Font* Graphics2D::getFont()
{
	return _master2D->_currentFont;
}

// Returns the current Gradient in the Graphics context.
const Gradient* Graphics2D::getGradient()
{
	return _master2D->_currentGradient;
}

// Returns the current Image Texture in the Graphics context.
const ImageTexture* Graphics2D::getImageTexture()
{
	return _master2D->_currentImageTexture;
}

// Returns the current Stroke in the Graphics context.
const Stroke* Graphics2D::getStroke()
{
	return _master2D->_currentStroke;
}

// Sets the background color for the Graphics2D context.
void Graphics2D::setBackground(const GLColor& color)
{
	_master2D->_backgroundColor.red = color.red;
	_master2D->_backgroundColor.green = color.green;
	_master2D->_backgroundColor.blue = color.blue;
	_master2D->_backgroundColor.alpha = color.alpha;

	//qDebug()  << "Graphics2D::setBackground: (rgba) " << color.red << "," << color.green << "," << color.blue << "," << color.alpha;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.red;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.green;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.blue;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.alpha;

	//qDebug()  << "Graphics2D::setBackground " << _drawFloats[_master2D->_currentDrawFloatIndex-4] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-3] << " " <<  _drawFloats[_master2D->_currentDrawFloatIndex-2] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-1];

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_BACKGROUND;
}

// Sets this graphics context's current color to the specified color.
void Graphics2D::setColor(const GLColor& color)
{
	_master2D->_foregroundColor.red = color.red;
	_master2D->_foregroundColor.green = color.green;
	_master2D->_foregroundColor.blue = color.blue;
	_master2D->_foregroundColor.alpha = color.alpha;

	//qDebug()  << "Graphics2D::setColor: (rgba) " << color.red << "," << color.green << "," << color.blue << "," << color.alpha;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.red;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.green;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.blue;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = color.alpha;

	//qDebug()  << "Graphics2D::setColor " << _drawFloats[_master2D->_currentDrawFloatIndex-4] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-3] << " " <<  _drawFloats[_master2D->_currentDrawFloatIndex-2] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-1];

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_COLOR;
}

// Sets this graphics context's font to the specified font.
void Graphics2D::setFont(Font* font)
{
	//qDebug()  << "Graphics2D::setFont: " << font;

	int returnCode = EXIT_SUCCESS;

	if (!_glTextureIDImageMap.contains(font->image)) {
		//returnCode = createTexture2D(font->image, NULL, NULL, &tex_x, &tex_y, &font->fontTexture);

		glGenTextures(1, &(font->fontTexture));

		glBindTexture(GL_TEXTURE_2D, font->fontTexture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, font->image->width(), font->image->height(), 0, GL_LUMINANCE_ALPHA , GL_UNSIGNED_BYTE, font->image->pixels());

		_glTextureIDImageMap.insert(font->image, font->fontTexture);
	} else {
		font->fontTexture = _glTextureIDImageMap.value(font->image);
	}

	if (EXIT_SUCCESS != returnCode) {
		qDebug() << "Graphics2D::setFont: Unable to create texture\n";
	} else {
		_master2D->_currentFont = font;

		_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

		_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = font;

		_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

		_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_FONT;
	}
}

void Graphics2D::setGradient(Gradient* gradient)
{
	qDebug()  << "Graphics2D::setGradient: " << gradient;

	_master2D->_currentGradient = gradient;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

	_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = gradient;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_GRADIENT;

	_master2D->_currentImageTexture = NULL;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

	_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = NULL;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_IMAGE_TEXTURE;
}

void Graphics2D::setImageTexture(ImageTexture* imageTexture)
{
	//qDebug()  << "Graphics2D::setImageTexture: " << stroke->width << " : " << stroke->cap << " " << stroke->join << " " << stroke->miterLimit << " " << stroke->dash << " " << stroke->dashCount << " " << stroke->dashPhase;

	_master2D->_currentImageTexture = imageTexture;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

	_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = imageTexture;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_IMAGE_TEXTURE;

	_master2D->_currentGradient = NULL;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

	_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = NULL;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_GRADIENT;
}

void Graphics2D::setStroke(Stroke *stroke)
{
	//qDebug()  << "Graphics2D::setStroke: " << stroke->width << " : " << stroke->cap << " " << stroke->join << " " << stroke->miterLimit << " " << stroke->dash << " " << stroke->dashCount << " " << stroke->dashPhase;

	_master2D->_currentStroke = stroke;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

	_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = stroke;

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_SET_STROKE;
}


// Draws the outline of a circular or elliptical arc covering the specified rectangle.
void Graphics2D::drawArc(int x, int y, int width, int height, int startAngle, int arcAngle)
{
	_master2D->drawArc((double)x, (double)y, (double)width, (double)height, (double)startAngle, (double)arcAngle);
}

void Graphics2D::drawArc(double x, double y, double width, double height, double startAngle, double arcAngle)
{
	//qDebug()  << "Graphics2D::drawArc: " << x << "," << y << " size " << width << "x" << height << " sweep " << startAngle << ", " << arcAngle;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)y;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)height;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)startAngle;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)arcAngle;

	//qDebug()  << "Graphics2D::drawArc segment " <<  _drawFloats[_master2D->_currentDrawFloatIndex-4] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-3] << " " <<  _drawFloats[_master2D->_currentDrawFloatIndex-2] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-1];

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_ARC;
}

// Draws as much of the specified image as is currently available.
void Graphics2D::drawImage(ImageData* image, int x, int y)
{
	_master2D->drawImage(image, (double)x, (double)y);
}

// Draws as much of the specified image as is currently available.
void Graphics2D::drawImage(ImageData* image, int x, int y, GLColor backgroundColor)
{
	_master2D->drawImage(image, (double)x, (double)y, backgroundColor);
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void Graphics2D::drawImage(ImageData* image, int x, int y, int width, int height)
{
	_master2D->drawImage(image, (double)x, (double)y, (double)width, (double)height);
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void Graphics2D::drawImage(ImageData* image, int x, int y, int width, int height, GLColor backgroundColor)
{
	_master2D->drawImage(image, (double)x, (double)y, (double)width, (double)height, backgroundColor);
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Graphics2D::drawImage(ImageData* image, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2)
{
	_master2D->drawImage(image, (double)dx1, (double)dy1, (double)dx2, (double)dy2, sx1, sy1, sx2, sy2);
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Graphics2D::drawImage(ImageData* image, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2, GLColor backgroundColor)
{
	_master2D->drawImage(image, (double)dx1, (double)dy1, (double)dx2, (double)dy2, sx1, sy1, sx2, sy2, backgroundColor);
}



// Draws as much of the specified image as is currently available.
void Graphics2D::drawImage(ImageData* image, double x, double y)
{
	_master2D->drawImage(image, x, y, x + image->width(), y + image->height(), 0, 0, image->width(), image->height(), COLOR_TRANSPARENT);

}

// Draws as much of the specified image as is currently available.
void Graphics2D::drawImage(ImageData* image, double x, double y, GLColor backgroundColor)
{
	_master2D->drawImage(image, x, y, x + image->width(), y + image->height(), 0, 0, image->width(), image->height(), backgroundColor);
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void Graphics2D::drawImage(ImageData* image, double x, double y, double width, double height)
{
	_master2D->drawImage(image, x, y, x + width, y + height, 0, 0, image->width(), image->height(), COLOR_TRANSPARENT);
}

// Draws as much of the specified image as has already been scaled to fit inside the specified rectangle.
void Graphics2D::drawImage(ImageData* image, double x, double y, double width, double height, GLColor backgroundColor)
{
	_master2D->drawImage(image, x, y, x + width, y + height, 0, 0, image->width(), image->height(), backgroundColor);
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Graphics2D::drawImage(ImageData* image, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2)
{
	_master2D->drawImage(image, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, COLOR_TRANSPARENT);
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Graphics2D::drawImage(ImageData* image, double dx1, double dy1, double dx2, double dy2, int sx1, int sy1, int sx2, int sy2, GLColor backgroundColor)
{
	//qDebug()  << "Graphics2D::drawImage: " << image << " dest: " << dx1 << " " << dy1 << " " << dx2 << " " << dy2 << " src: " << sx1 << " " << sy1 << " " << sx2 << " " << sy2  << "color: (rgba) " << backgroundColor.red << "," << backgroundColor.green << "," << backgroundColor.blue << "," << backgroundColor.alpha;

	GLuint  photo = 0;
	float tex_x = 1.0, tex_y = 1.0;
	float   _photoPosX, _photoPosY, _photoSizeX, _photoSizeY;
	GLfloat imageVertices[8];
	GLfloat imageTexCoord[8];

	if (backgroundColor.alpha != 0.0) {
		GLColor saveColor;

		saveColor.red = _master2D->_foregroundColor.red;
		saveColor.green = _master2D->_foregroundColor.green;
		saveColor.blue = _master2D->_foregroundColor.blue;
		saveColor.alpha = _master2D->_foregroundColor.alpha;

		_master2D->setColor(backgroundColor);
		_master2D->drawRect(dx1, dy1, dx2 - dx1, dy2 - dy1);
		_master2D->setColor(saveColor);
	}

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	int returnCode = EXIT_SUCCESS;
	if (!_glTextureIDImageMap.contains(image)) {
		returnCode = createTexture2D(image, NULL, NULL, &tex_x, &tex_y, &photo);

		_glTextureIDImageMap.insert(image, photo);
	} else {
		photo = _glTextureIDImageMap.value(image);
	}

	if (EXIT_SUCCESS != returnCode) {
		qDebug() << "Graphics2D::drawImage: Unable to create texture\n";
	} else {
		_photoPosX = dx1;
		_photoPosY = dy1;

		_photoSizeX = (float)(dx2 - dx1);
		_photoSizeY = (float)(dy2 - dy1);

		imageTexCoord[0] = 0.0f + (sx1 * tex_x / image->width());
		imageTexCoord[1] = 0.0f + (sy2 * tex_y / image->height());
		imageTexCoord[2] = 0.0f + (sx2 * tex_x / image->width());
		imageTexCoord[3] = 0.0f + (sy2 * tex_y / image->height());
		imageTexCoord[4] = 0.0f + (sx1 * tex_x / image->width());
		imageTexCoord[5] = 0.0f + (sy1 * tex_y / image->height());
		imageTexCoord[6] = 0.0f + (sx2 * tex_x / image->width());
		imageTexCoord[7] = 0.0f + (sy1 * tex_y / image->height());

		imageVertices[0] = _photoPosX;
		imageVertices[1] = _photoPosY;
		imageVertices[2] = _photoPosX + _photoSizeX;
		imageVertices[3] = _photoPosY;
		imageVertices[4] = _photoPosX;
		imageVertices[5] = _photoPosY + _photoSizeY;
		imageVertices[6] = _photoPosX + _photoSizeX;
		imageVertices[7] = _photoPosY + _photoSizeY;

		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[0];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[1];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[2];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[3];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[4];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[5];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[6];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageTexCoord[7];

		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[0];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[1];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[2];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[3];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[4];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[5];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[6];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = imageVertices[7];
	}

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;


	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = photo;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_IMAGE;
}

// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
void Graphics2D::drawLine(int x1, int y1, int x2, int y2)
{
	_master2D->drawLine((double)x1, (double)y1, (double)x2, (double)y2);
}

// Draws a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
void Graphics2D::drawLine(double x1, double y1, double x2, double y2)
{
	//qDebug()  << "Graphics2D::drawLine: " << x1 << "," << y1 << " to " << x2 << "," << y2;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)x1;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)y1;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)x2;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)y2;

	//qDebug()  << "Graphics2D::drawLine segment " <<  _drawFloats[_master2D->_currentDrawFloatIndex-4] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-3] << " " <<  _drawFloats[_master2D->_currentDrawFloatIndex-2] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-1];

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_LINE;
}

// Draws the outline of an oval.
void Graphics2D::drawOval(int x, int y, int width, int height)
{
	drawOval((double)x, (double)y, (double)width, (double)height);
}

// Draws the outline of an oval.
void Graphics2D::drawOval(double x, double y, double width, double height)
{
	drawArc(x, y, width, height, 0.0, 360.0);
}

// Draws a closed polygon defined by arrays of x and y coordinates.
void Graphics2D::drawPolygon(int* xPoints, int* yPoints, int nPoints)
{
	if (nPoints > 0) {
		double* xdPoints = new double[nPoints];
		double* ydPoints = new double[nPoints];

		for(int index = 0; index < nPoints; index++) {
			xdPoints[index] = (double)xPoints[index];
			ydPoints[index] = (double)yPoints[index];
		}

		drawPolygon(xdPoints, ydPoints, nPoints);

		if (xdPoints) {
			delete xdPoints;
		}

		if (ydPoints) {
			delete ydPoints;
		}
	}
}

// Draws a closed polygon defined by arrays of x and y coordinates.
void Graphics2D::drawPolygon(double* xPoints, double* yPoints, int nPoints)
{
	//qDebug()  << "Graphics2D::drawPolygon: " << xPoints << " " << yPoints << " " << nPoints;

	int numberPoints = nPoints+1;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = numberPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	for(int index = 0; index < numberPoints; index++) {
		if (index < (numberPoints-1)) {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = xPoints[index];
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = yPoints[index];
		} else {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = xPoints[0];
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = yPoints[0];
		}
	}

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_POLYLINE;
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void Graphics2D::drawPolyline(int* xPoints, int* yPoints, int nPoints)
{
	if (nPoints > 0) {
		double* xdPoints = new double[nPoints];
		double* ydPoints = new double[nPoints];

		for(int index = 0; index < nPoints; index++) {
			xdPoints[index] = (double)xPoints[index];
			ydPoints[index] = (double)yPoints[index];
		}

		drawPolyline(xdPoints, ydPoints, nPoints);

		if (xdPoints) {
			delete xdPoints;
		}

		if (ydPoints) {
			delete ydPoints;
		}
	}
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void Graphics2D::drawPolyline(double* xPoints, double* yPoints, int nPoints)
{
	//qDebug()  << "Graphics2D::drawPolygon: " << xPoints << " " << yPoints << " " << nPoints;

	int numberPoints = nPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = numberPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	for(int index = 0; index < numberPoints; index++) {
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = xPoints[index];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = yPoints[index];
	}

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_POLYLINE;
}

// Draws the outline of the specified rectangle.
void Graphics2D::drawRect(int x, int y, int width, int height)
{
	drawRect((double)x, (double)y, (double)width, (double)height);
}

// Draws the outline of the specified rectangle.
void Graphics2D::drawRect(double x, double y, double width, double height)
{

	int numberPoints = 5;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = numberPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x + width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x + width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y + height;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y + height;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_POLYLINE;
}


// Draws an outlined round-cornered rectangle using this graphics context's current color.
void Graphics2D::drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight)
{
	drawRoundRect((double)x, (double)y, (double)width, (double)height, (double)arcWidth, (double)arcHeight);
}

// Draws an outlined round-cornered rectangle using this graphics context's current color.
void Graphics2D::drawRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight)
{

	int numberPoints = 5;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = numberPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = height;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = arcWidth;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = arcHeight;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_ROUNDRECT;

}


// Measures the bounding box of the text of the specified String, using the current font in the Graphics2D context.
void Graphics2D::measureString(QString text, double* width, double* height)
{
	//qDebug() << "Graphics2D::measureString: text : " << text;

	wchar_t *wtext;

	wtext = new wchar_t[text.size()+1];
	int length = text.toWCharArray(wtext);
	wtext[text.size()] = 0;

    int i, j, c;

    if (!_currentFont) {
        qCritical() << "Graphics2D::measureString: Font must not be null\n";
        return;
    }

    if (!_currentFont->initialized) {
    	qCritical() << "Graphics2D::measureString: Font has not been loaded\n";
        return;
    }

    int textLength = wcslen(wtext);

	if (width) {
		//Width of a text rectangle is a sum advances for every glyph in a string
		*width = 0.0f;
	}

	if (height) {
		//Height of a text rectangle is a high of a tallest glyph in a string
		*height = 0.0f;
	}

	int charMapIndex;
	int previousCharMapIndex;

	previousCharMapIndex = -1;
	for(i = 0; i < textLength; ++i) {
		c = (int)wtext[i];

		charMapIndex = 0;
		for(j = 0; j < _currentFont->numberCharacters; j++) {
			if (_currentFont->charMap[j] == c) {
				charMapIndex = j;
				break;
			}
		}

		if (i > 0) {
		 	*width += _currentFont->kerning[previousCharMapIndex * _currentFont->numberCharacters + charMapIndex];
		}

		//qDebug() << "Graphics2D::measureString: text width: " << i << " " << wtext[i] << " " << c << " "  << _currentFont->advance[charMapIndex] << "\n";

		if (width) {
			*width += _currentFont->advance[charMapIndex];
		}
		if (height) {
			if (*height < _currentFont->height[charMapIndex]) {
				//qDebug() << "Graphics2D::measureString: text height: " << i << " " << wtext[i] << " " << c << " "  << _currentFont->height[charMapIndex] << "\n";
				*height = _currentFont->height[charMapIndex];
			}
		}

		previousCharMapIndex = charMapIndex;
	}

	//qDebug() << "Graphics2D::measureString: text width: " << *width << "\n";
	//qDebug() << "Graphics2D::measureString: text height: " << *height << "\n";
}

// Draws the text given by the specified string, using this graphics context's current font and color.
void Graphics2D::drawString(QString text, int x, int y)
{
	drawString(text, (double)x, (double)y);
}

// Draws the text given by the specified string, using this graphics context's current font and color.
void Graphics2D::drawString(QString text, double x, double y)
{
	_master2D->_drawPointerIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawPointerIndex;

	_master2D->_drawPointers[_master2D->_currentDrawPointerIndex++] = (void*)(new QString(text));

	_master2D->_drawPointerIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawPointerIndex-1;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;


	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_DRAW_STRING;
}

// Fills a circular or elliptical arc covering the specified rectangle.
void Graphics2D::fillArc(int x, int y, int width, int height, int startAngle, int arcAngle)
{
	_master2D->fillArc((double)x, (double)y, (double)width, (double)height, (double)startAngle, (double)arcAngle);
}

void Graphics2D::fillArc(double x, double y, double width, double height, double startAngle, double arcAngle)
{
	//qDebug()  << "Graphics2D::drawArc: " << x << "," << y << " size " << width << "x" << height << " sweep " << startAngle << ", " << arcAngle;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)y;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)height;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)startAngle;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (GLfloat)arcAngle;

	//qDebug()  << "Graphics2D::drawArc segment " <<  _drawFloats[_master2D->_currentDrawFloatIndex-4] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-3] << " " <<  _drawFloats[_master2D->_currentDrawFloatIndex-2] << "," << _drawFloats[_master2D->_currentDrawFloatIndex-1];

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_FILL_ARC;
}

// Fills an oval bounded by the specified rectangle with the current color.
void Graphics2D::fillOval(int x, int y, int width, int height)
{
	fillOval((double)x, (double)y, (double)width, (double)height);
}

// Fills an oval bounded by the specified rectangle with the current color.
void Graphics2D::fillOval(double x, double y, double width, double height)
{
	fillArc(x, y, width, height, 0.0, 360.0);
}


// Draws the outline of the specified rectangle.
void Graphics2D::fillRect(int x, int y, int width, int height)
{
	fillRect((double)x, (double)y, (double)width, (double)height);
}

// Draws the outline of the specified rectangle.
void Graphics2D::fillRect(double x, double y, double width, double height)
{
	qDebug()  << "Graphics2D::fillRect: " << x << "," << y << " size " << width << "x" << height;

	int numberPoints = 4;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = numberPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y + height;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x + width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y + height;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x + width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	// texture coordinates
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 0;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 1.0;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 1.0;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 1.0;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 0.0;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 0.0;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 1.0;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 0.0;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_FILL_POLYGON;
}

// Draws a closed polygon defined by arrays of x and y coordinates.
void Graphics2D::fillPolygon(int* xPoints, int* yPoints, int nPoints)
{
	if (nPoints > 0) {
		double* xdPoints = new double[nPoints];
		double* ydPoints = new double[nPoints];

		for(int index = 0; index < nPoints; index++) {
			xdPoints[index] = (double)xPoints[index];
			ydPoints[index] = (double)yPoints[index];
		}

		fillPolygon(xdPoints, ydPoints, nPoints);

		if (xdPoints) {
			delete xdPoints;
		}

		if (ydPoints) {
			delete ydPoints;
		}
	}
}

// Draws a closed polygon defined by arrays of x and y coordinates.
void Graphics2D::fillPolygon(double* xPoints, double* yPoints, int nPoints)
{
	//qDebug()  << "Graphics2D::drawPolygon: " << xPoints << " " << yPoints << " " << nPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = nPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	float x = 0.0, y = 0.0, minX = 1.0e6, minY = 1.0e6, maxX = -1.0e6, maxY = -1.0e6;

	for(int index = 0; index < nPoints; index++) {
		x += xPoints[index];
		y += yPoints[index];

		if (xPoints[index] < minX) {
			minX = xPoints[index];
		}
		if (xPoints[index] > maxX) {
			maxX = xPoints[index];
		}

		if (yPoints[index] < minY) {
			minY = yPoints[index];
		}
		if (yPoints[index] > maxY) {
			maxY = yPoints[index];
		}
	}

	x /= nPoints;
	y /= nPoints;

	for(int index = 0; index < nPoints; index++) {
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = xPoints[index];
		_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = yPoints[index];
	}

	for(int index = 0; index < nPoints; index++) {
		if (minX == maxX) {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 0.0;
		} else {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (xPoints[index] - minX) / (maxX - minX);
		}
		if (minY == maxY) {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = 0.0;
		} else {
			_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = (yPoints[index] - minY) / (maxY - minY);
		}
	}

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] =_master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_FILL_POLYGON;
}

// Draws an outlined round-cornered rectangle using this graphics context's current color.
void Graphics2D::fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight)
{
	fillRoundRect((double)x, (double)y, (double)width, (double)height, (double)arcWidth, (double)arcHeight);
}

// Draws an outlined round-cornered rectangle using this graphics context's current color.
void Graphics2D::fillRoundRect(double x, double y, double width, double height, double arcWidth, double arcHeight)
{

	int numberPoints = 4;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawIntIndex;

	_master2D->_drawInts[_master2D->_currentDrawIntIndex++] = numberPoints;

	_master2D->_drawIntIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawIntIndex-1;


	_master2D->_drawFloatIndices[_master2D->_commandCount*2+0] = _master2D->_currentDrawFloatIndex;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = x;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = y;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = width;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = height;

	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = arcWidth;
	_master2D->_drawFloats[_master2D->_currentDrawFloatIndex++] = arcHeight;

	_master2D->_drawFloatIndices[_master2D->_commandCount*2+1] = _master2D->_currentDrawFloatIndex-1;

	_master2D->_drawCommands[_master2D->_commandCount++] = RENDER_FILL_ROUNDRECT;

}


void Graphics2D::setupView(int x, int y, int width, int height)
{
	//qDebug()  << "Graphics2D::setupView" << x << "," << y << " " << width << "x" << height<< " " << _width << "x" << _height;

	GLfloat left, right, bottom, top, near, far;

	left = (float)x / (float)_height;
	right = (float)(x + width) / (float)_height;
	bottom = (float)y / (float)_height;
	top = (float)(y + height) /(float) _height;
	near = -1.0;
	far = 1.0;

    glViewport(x, y, width, height);

#ifdef GLES1
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrthof(left, right, bottom, top, near, far);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glScalef(1.0f / (float)_height, 1.0f / (float)_height, 1.0f);

#elif defined(GLES2)
    // TODO - replace this with proper matrix code later
	GLfloat tx = -(right + left) / (right - left);
	GLfloat ty = -(top + bottom) / (top - bottom);
	GLfloat tz = -(far + near) / (far - near);

	memset(_orthoMatrix, 0, sizeof(GLfloat) * 4 * 4);
	_orthoMatrix[0] = 2.0f / (right - left);
	_orthoMatrix[5] = 2.0f / (top - bottom);
	_orthoMatrix[10] = -2.0f / (far - near);
	_orthoMatrix[12] = tx;
	_orthoMatrix[13] = ty;
	_orthoMatrix[14] = tz;
	_orthoMatrix[15] = 1.0f;

	//qDebug()  << "Graphics2D::setupView: ortho matrix: \n" ;;
	for(int index = 0; index < 16; index++) {
		//qDebug()  << " " << _orthoMatrix[index];
	}

	memset(_scaleMatrix, 0, sizeof(GLfloat) * 4 * 4);
	_scaleMatrix[0] = 1.0f / (float)_height;
	_scaleMatrix[5] = 1.0f / (float)_height;
	_scaleMatrix[10] = 1.0f;
	_scaleMatrix[15] = 1.0f;

	//qDebug()  << "Graphics2D::setupView: scale matrix: \n" ;;
	for(int index = 0; index < 16; index++) {
		//qDebug()  << " " << _scaleMatrix[index];
	}

	int i, j, k;
	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_renderModelMatrix[i * 4 + j] += _transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
			}
		}
	}

	//qDebug()  << "Graphics2D::setupView: transform matrix: \n" ;;
	for(int index = 0; index < 16; index++) {
		//qDebug()  << " " << _transformMatrix[index];
	}

	//qDebug()  << "Graphics2D::setupView: model matrix: \n" ;;
	for(int index = 0; index < 16; index++) {
		//qDebug()  << " " << _renderModelMatrix[index];
	}

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}

void Graphics2D::render() {

	int renderCommandCount;

	//qDebug()  << "Graphics2D::render ";

	_master2D->_refreshMutex.lock();

#ifdef GLES1
	//Common GL ES1 setup
	glShadeModel(GL_SMOOTH);
#elif defined(GLES2)
#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif

	glEnable(GL_CULL_FACE);

	// setup view
	_master2D->setupView(0, 0, _width, _height);

	// start rendering primitives
	for(renderCommandCount = 0; renderCommandCount < _commandCount; renderCommandCount++) {

		//qDebug()  << "Graphics2D::render: " << index << ":" << _master2D->_drawCommands[renderCommandCount];

		switch(_master2D->_drawCommands[renderCommandCount]) {
		case RENDER_CLEAR_RECT:
			_master2D->renderClearRect(renderCommandCount);
			break;
		case RENDER_CLIP_RECT:
			_master2D->renderClipRect(renderCommandCount);
			break;
		case RENDER_TRANSFORM:
			_master2D->renderTransform(renderCommandCount);
			break;
		case RENDER_TRANSFORM_ROTATE:
			_master2D->renderRotate(renderCommandCount);
			break;
		case RENDER_TRANSFORM_TRANSLATE:
			_master2D->renderTranslate(renderCommandCount);
			break;
		case RENDER_TRANSFORM_SCALE:
			_master2D->renderScale(renderCommandCount);
			break;
		case RENDER_TRANSFORM_SHEAR:
			_master2D->renderShear(renderCommandCount);
			break;
		case RENDER_SET_BACKGROUND:
			_master2D->renderSetBackground(renderCommandCount);
			break;
		case RENDER_SET_COLOR:
			_master2D->renderSetColor(renderCommandCount);
			break;
		case RENDER_SET_FONT:
			_master2D->renderSetFont(renderCommandCount);
			break;
		case RENDER_SET_GRADIENT:
			_master2D->renderSetGradient(renderCommandCount);
			break;
		case RENDER_SET_IMAGE_TEXTURE:
			_master2D->renderSetImageTexture(renderCommandCount);
			break;
		case RENDER_SET_STROKE:
			_master2D->renderSetStroke(renderCommandCount);
			break;
		case RENDER_DRAW_ARC:
		case RENDER_FILL_ARC:
			_master2D->renderDrawFillArc(renderCommandCount);
			break;
		case RENDER_DRAW_IMAGE:
			_master2D->renderDrawImage(renderCommandCount);
			break;
		case RENDER_DRAW_LINE:
			_master2D->renderDrawLine(renderCommandCount);
			break;
		case RENDER_DRAW_POLYLINE:
			_master2D->renderDrawPolyline(renderCommandCount);
			break;
		case RENDER_DRAW_ROUNDRECT:
		case RENDER_FILL_ROUNDRECT:
			_master2D->renderDrawFillRoundRect(renderCommandCount);
			break;
		case RENDER_DRAW_STRING:
			_master2D->renderDrawString(renderCommandCount);
			break;
		case RENDER_FILL_POLYGON:
			_master2D->renderFillPolygon(renderCommandCount);
			break;
		default:
			break;
		}
	}

	_master2D->_refreshMutex.unlock();
}


// Concatenates the current Graphics2D Transform with a rotation transform.
void Graphics2D::renderRotate(int commandCount)
{
	//qDebug()  << "Graphics2D::rrenderRotate: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	GLfloat theta  = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];

#ifdef GLES1
	glRotatef(theta, 0.0, 0.0, 1.0);
#elif defined(GLES2)
	GLfloat tempMatrix[16], rotateMatrix[16];
	memset(rotateMatrix, 0, sizeof(GLfloat) * 4 * 4);
	memcpy(tempMatrix, _transformMatrix, sizeof(GLfloat) * 4 * 4);

	rotateMatrix[0] = cosf(theta * M_PI / 180.0);
	rotateMatrix[1] = sinf(theta * M_PI / 180.0);
	rotateMatrix[4] = cosf((theta + 90.0) * M_PI / 180.0);
	rotateMatrix[5] = sinf((theta + 90.0) * M_PI / 180.0);
	rotateMatrix[15] = 1.0f;

	int i, j, k;
	memset(_transformMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_transformMatrix[i * 4 + j] += tempMatrix[i * 4 + k] * rotateMatrix[k * 4 + j];
			}
		}
	}

	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_renderModelMatrix[i * 4 + j] += _transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
			}
		}
	}

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}

// Concatenates the current Graphics2D Transform with a scaling transformation Subsequent rendering is resized according to the specified scaling factors relative to the previous scaling.
void Graphics2D::renderScale(int commandCount)
{
	//qDebug()  << "Graphics2D::renderScale: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	GLfloat sx  = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	GLfloat sy  = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];

#ifdef GLES1
	glScalef(sx, sy, 0.0);
#elif defined(GLES2)
	GLfloat tempMatrix[16], scaleMatrix[16];
	memset(scaleMatrix, 0, sizeof(GLfloat) * 4 * 4);
	memcpy(tempMatrix, _transformMatrix, sizeof(GLfloat) * 4 * 4);

	scaleMatrix[0] = sx;
	scaleMatrix[5] = sy;
	scaleMatrix[15] = 1.0f;

	int i, j, k;
	memset(_transformMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_transformMatrix[i * 4 + j] += tempMatrix[i * 4 + k] * scaleMatrix[k * 4 + j];
			}
		}
	}

	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_renderModelMatrix[i * 4 + j] += _transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
			}
		}
	}

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}

// Concatenates the current Graphics2D Transform with a translation transform.
void Graphics2D::renderTranslate(int commandCount)
{
	//qDebug()  << "Graphics2D::renderTranslate: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	GLfloat tx  = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	GLfloat ty  = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];

#ifdef GLES1
	glTranslatef(tx, ty, 0.0);
#elif defined(GLES2)
	GLfloat tempMatrix[16], translateMatrix[16];
	memset(translateMatrix, 0, sizeof(GLfloat) * 4 * 4);
	memcpy(tempMatrix, _transformMatrix, sizeof(GLfloat) * 4 * 4);

	translateMatrix[0]  = 1.0f;
	translateMatrix[5]  = 1.0f;
	translateMatrix[10] = 1.0f;
	translateMatrix[12] = tx;
	translateMatrix[13] = ty;
	translateMatrix[15] = 1.0f;

	int i, j, k;
	memset(_transformMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_transformMatrix[i * 4 + j] += tempMatrix[i * 4 + k] * translateMatrix[k * 4 + j];
			}
		}
	}

	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_renderModelMatrix[i * 4 + j] += _transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
			}
		}
	}

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}

// Concatenates the current Graphics2D Transform with a shearing transform.
void Graphics2D::renderShear(int commandCount)
{
	//qDebug()  << "Graphics2D::renderShear: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	GLfloat shx  = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	GLfloat shy  = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];

	GLfloat shearMatrix[16];
	memset(shearMatrix, 0, sizeof(GLfloat) * 4 * 4);

	shearMatrix[0] = 1.0f;
	shearMatrix[1] = shy;
	shearMatrix[4] = shx;
	shearMatrix[5] = 1.0f;
	shearMatrix[15] = 1.0f;

	// TODO: fix this
#ifdef GLES1
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

	glMultMatrixf(shearMatrix);
#elif defined(GLES2)
	GLfloat tempMatrix[16];
	memcpy(tempMatrix, _transformMatrix, sizeof(GLfloat) * 4 * 4);

	int i, j, k;
	memset(_transformMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_transformMatrix[i * 4 + j] += tempMatrix[i * 4 + k] * shearMatrix[k * 4 + j];
			}
		}
	}

	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_renderModelMatrix[i * 4 + j] += _transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
			}
		}
	}

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}

// Overwrites the Transform in the Graphics2D context.
void Graphics2D::renderTransform(int commandCount)
{
	//qDebug()  << "Graphics2D::renderShear: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];
	GLfloat transformMatrix[16];

	transformMatrix[ 0]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	transformMatrix[ 1]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	transformMatrix[ 2]  = 0.0;
	transformMatrix[ 3]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	transformMatrix[ 4]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];
	transformMatrix[ 5]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+4];
	transformMatrix[ 6]  = 0.0;
	transformMatrix[ 7]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+5];
	transformMatrix[ 8]  = 0.0;
	transformMatrix[ 9]  = 0.0;
	transformMatrix[10]  = 1.0;
	transformMatrix[11]  = 0.0;
	transformMatrix[12]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+6];
	transformMatrix[13]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+7];
	transformMatrix[14]  = 0.0;
	transformMatrix[15]  = _drawFloats[_drawFloatIndices[commandCount*2+0]+8];

#ifdef GLES1
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

	glMultMatrixf(transformMatrix);
#elif defined(GLES2)
	memcpy(transformMatrix, _transformMatrix, sizeof(GLfloat) * 4 * 4);

	int i, j, k;
	memset(_renderModelMatrix, 0, sizeof(GLfloat) * 4 * 4);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				_renderModelMatrix[i * 4 + j] += transformMatrix[i * 4 + k] * _scaleMatrix[k * 4 + j];
			}
		}
	}
#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}

// Sets the background color for the Graphics2D context.
void Graphics2D::renderSetBackground(int commandCount)
{
	//qDebug()  << "Graphics2D::renderSetBackground: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	_renderBackgroundColor.red   = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	_renderBackgroundColor.green = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	_renderBackgroundColor.blue  = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	_renderBackgroundColor.alpha = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];
}

// Sets this graphics context's current color to the specified color.
void Graphics2D::renderSetColor(int commandCount)
{
	//qDebug()  << "Graphics2D::renderSetColor: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	_renderForegroundColor.red   = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	_renderForegroundColor.green = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	_renderForegroundColor.blue  = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	_renderForegroundColor.alpha = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];

	_renderGradient = NULL;
}

// Sets this graphics context's current color to the specified color.
void Graphics2D::renderSetFont(int commandCount)
{
	//qDebug()  << "Graphics2D::renderSetFont: " << commandCount << " : " << _drawPointerIndices[commandCount*2+0] << " " << _drawPointerIndices[commandCount*2+1];

	_renderFont = (Font*)_drawPointers[_drawPointerIndices[commandCount*2+0]+0];
}

// Sets this graphics context's current color to the specified color.
void Graphics2D::renderSetGradient(int commandCount)
{
	//qDebug()  << "Graphics2D::renderSetGradient: " << commandCount << " : " << _drawPointerIndices[commandCount*2+0] << " " << _drawPointerIndices[commandCount*2+1];

	_renderGradient = (Gradient*)_drawPointers[_drawPointerIndices[commandCount*2+0]+0];
}

// Sets this graphics context's image texture to the specified image texture.
void Graphics2D::renderSetImageTexture(int commandCount)
{
	//qDebug()  << "Graphics2D::renderSetImageTexture: " << commandCount << " : " << _drawPointerIndices[commandCount*2+0] << " " << _drawPointerIndices[commandCount*2+1];

	_renderImageTexture = (ImageTexture*)_drawPointers[_drawPointerIndices[commandCount*2+0]+0];
}

// Sets this graphics context's current color to the specified color.
void Graphics2D::renderSetStroke(int commandCount)
{
	//qDebug()  << "Graphics2D::renderSetStroke: " << commandCount << " : " << _drawPointerIndices[commandCount*2+0] << " " << _drawPointerIndices[commandCount*2+1];

	_renderStroke = (Stroke*)_drawPointers[_drawPointerIndices[commandCount*2+0]+0];
}

// Clears the specified rectangle by filling it with the background color of the current drawing surface.
void Graphics2D::renderClearRect(int commandCount)
{
	//qDebug()  << "Graphics2D::renderClearRect: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1] << " " << _drawFloatIndices[commandCount*2+2] << " " << _drawFloatIndices[commandCount*2+3];

	setupView(_drawInts[_drawIntIndices[commandCount*2+0]+0], _drawInts[_drawIntIndices[commandCount*2+0]+1], _drawInts[_drawIntIndices[commandCount*2+0]+2], _drawInts[_drawIntIndices[commandCount*2+0]+3]);

	glClearColor(_renderBackgroundColor.red, _renderBackgroundColor.green, _renderBackgroundColor.blue, _renderBackgroundColor.alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Intersects the current clip with the specified rectangle.
void Graphics2D::renderClipRect(int commandCount)
{
	//qDebug()  << "Graphics2D::renderClipRect: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1] << " " << _drawFloatIndices[commandCount*2+2] << " " << _drawFloatIndices[commandCount*2+3];

	setupView(_drawInts[_drawIntIndices[commandCount*2+0]+0], _drawInts[_drawIntIndices[commandCount*2+0]+1], _drawInts[_drawIntIndices[commandCount*2+0]+2], _drawInts[_drawIntIndices[commandCount*2+0]+3]);
}

// Render a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
void Graphics2D::renderDrawLine(int commandCount)
{
	//qDebug()  << "Graphics2D::renderDrawLine: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1] << " " << _drawFloatIndices[commandCount*2+2] << " " << _drawFloatIndices[commandCount*2+3];

	GLfloat dx, dy, length, x1, y1, x2, y2;

	x1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	y1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	x2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	y2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];

	dx = ((GLfloat)x2 - (GLfloat)x1);
	dy = ((GLfloat)y2 - (GLfloat)y1);
	length = sqrt (fabs(dx * dx) + fabs(dy * dy));

	float* dashCoords = NULL;
	int   dashPoints = 0;

	//qDebug()  << "Graphics2D::renderDrawLine: " << length;

	if (_renderStroke->dashCount > 1) {

		int dashIndex = -1;
		float currentLength = 0.0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashPoints++;
			}

			//qDebug()  << "Graphics2D::renderDrawLine: calculate array size " << currentLength << " " << dashIndex << " " << dashPoints;

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashPoints++;
		}

		//qDebug()  << "Graphics2D::renderDrawLine: " << currentLength << " " << dashPoints;

		dashCoords = new float[dashPoints*2];

		dashIndex = -1;
		currentLength = 0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}
		//if (dashPhase != -100000.0) {
		//	currentLength = -dashPhase;
		//}
		dashPoints = 0;

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashCoords[0] = x1;
						dashCoords[1] = y1;
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashCoords[0] = x1;
				dashCoords[1] = y1;
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashCoords[dashPoints*2+0] = x1 + currentLength * (x2 - x1) / length;
				dashCoords[dashPoints*2+1] = y1 + currentLength * (y2 - y1) / length;
				dashPoints++;
			}

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashCoords[dashPoints*2+0] = x2;
			dashCoords[dashPoints*2+1] = y2;
			dashPoints++;
		}

	} else {
		dashPoints = 2;
		dashCoords = new float[dashPoints*2];
		dashCoords[0] = x1;
		dashCoords[1] = y1;
		dashCoords[2] = x2;
		dashCoords[3] = y2;
	}

	int renderIndex = 0;
	for(int index = 0; index < dashPoints; index += 2) {
		if (fabs(dy) > fabs(dx)) {
			_renderVertexCoords[renderIndex*4+0] = (GLfloat)dashCoords[index*2+2] - (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+1] = (GLfloat)dashCoords[index*2+3] + (dx * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+2] = (GLfloat)dashCoords[index*2+0] - (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+3] = (GLfloat)dashCoords[index*2+1] + (dx * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+4] = (GLfloat)dashCoords[index*2+2] + (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+5] = (GLfloat)dashCoords[index*2+3] - (dx * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+6] = (GLfloat)dashCoords[index*2+0] + (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+7] = (GLfloat)dashCoords[index*2+1] - (dx * _renderStroke->width / (2.0 * length));
		} else {
			_renderVertexCoords[renderIndex*4+0] = (GLfloat)dashCoords[index*2+0] + (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+1] = (GLfloat)dashCoords[index*2+1] - (dx * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+2] = (GLfloat)dashCoords[index*2+2] + (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+3] = (GLfloat)dashCoords[index*2+3] - (dx * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+4] = (GLfloat)dashCoords[index*2+0] - (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+5] = (GLfloat)dashCoords[index*2+1] + (dx * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+6] = (GLfloat)dashCoords[index*2+2] - (dy * _renderStroke->width / (2.0 * length));
			_renderVertexCoords[renderIndex*4+7] = (GLfloat)dashCoords[index*2+3] + (dx * _renderStroke->width / (2.0 * length));
		}

		_renderTextureCoords[renderIndex*4+0] = (GLfloat)0.0;
		_renderTextureCoords[renderIndex*4+1] = (GLfloat)1.0;
		_renderTextureCoords[renderIndex*4+2] = (GLfloat)1.0;
		_renderTextureCoords[renderIndex*4+3] = (GLfloat)1.0;
		_renderTextureCoords[renderIndex*4+4] = (GLfloat)0.0;
		_renderTextureCoords[renderIndex*4+5] = (GLfloat)0.0;
		_renderTextureCoords[renderIndex*4+6] = (GLfloat)1.0;
		_renderTextureCoords[renderIndex*4+7] = (GLfloat)0.0;

		renderIndex++;

		if ((renderIndex*8 + 8) > MAX_VERTEX_COORDINATES) {
			renderDrawTriangles(renderIndex, 4);

			renderIndex = 0;
		}
	}

	renderDrawTriangles(renderIndex, 4);

	if (dashCoords) {
		delete dashCoords;
	}

	//qDebug()  << "Graphics2D::renderDrawLine: " << _drawFloatIndices[commandCount*2+0]/2;
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void Graphics2D::renderDrawPolyline(int commandCount)
{
	//qDebug()  << "Graphics2D::renderDrawPolyline: " << commandCount;

	GLfloat dx, dy, dx1, dy1, length, lineLength, x1, y1, x2, y2, xl1, yl1, xl2, yl2;

	int numberPoints = _drawInts[_drawIntIndices[commandCount*2+0]+0];

	float* dashCoords = NULL;
	int   dashPoints = 0;


	if (_renderStroke->dashCount > 1) {

		length = 0;
		for(int index = 0; index < (numberPoints-1); index++) {
			x1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+0];
			y1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+1];
			x2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+2];
			y2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+3];

			dx = ((GLfloat)x2 - (GLfloat)x1);
			dy = ((GLfloat)y2 - (GLfloat)y1);
			lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
			dx /= lineLength;
			dy /= lineLength;

			length += lineLength;
		}


		int dashIndex = -1;
		float currentLength = 0.0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}
		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashPoints++;
			}

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashPoints++;
		}

		dashCoords = new float[dashPoints*2];

		dashIndex = -1;
		currentLength = 0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}
		dashPoints = 0;

		x1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
		y1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
		x2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+(numberPoints-1)*2+0];
		y2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+(numberPoints-1)*2+1];

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashCoords[0] = x1;
						dashCoords[1] = y1;
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashCoords[0] = x1;
				dashCoords[1] = y1;
				dashPoints++;
			}

			if (currentLength > 0.0) {
				float linesLength = 0;
				for(int index = 0; index < (numberPoints-1); index++) {
					xl1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+0];
					yl1 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+1];
					xl2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+2];
					yl2 = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+3];

					dx = ((GLfloat)xl2 - (GLfloat)xl1);
					dy = ((GLfloat)yl2 - (GLfloat)yl1);
					lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
					dx /= lineLength;
					dy /= lineLength;

					if ((linesLength + lineLength) >= currentLength) {
						break;
					}

					linesLength += lineLength;
				}

				dashCoords[dashPoints*2+0] = xl1 + (currentLength - linesLength) * (xl2 - xl1) / lineLength;
				dashCoords[dashPoints*2+1] = yl1 + (currentLength - linesLength) * (yl2 - yl1) / lineLength;
				dashPoints++;
			}

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashCoords[dashPoints*2+0] = x2;
			dashCoords[dashPoints*2+1] = y2;
			dashPoints++;
		}

	} else {
		dashPoints = (numberPoints-1)*2;
		dashCoords = new float[dashPoints*2];

		for(int index = 0; index < (numberPoints-1); index++) {
			dashCoords[index*4+0] = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+0];
			dashCoords[index*4+1] = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+1];
			dashCoords[index*4+2] = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+2];
			dashCoords[index*4+3] = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+3];
		}
	}

	int renderIndex = 0;
	double xPoints[4], yPoints[4], uPoints[4], vPoints[4];


	// calculate u,v texture bounds
	float x = 0.0, y = 0.0, minX = 1.0e6, minY = 1.0e6, maxX = -1.0e6, maxY = -1.0e6;

	for(int index = 0; index < dashPoints-1; index += 2) {
		dx = ((GLfloat)dashCoords[index*2+2] - (GLfloat)dashCoords[index*2+0]);
		dy = ((GLfloat)dashCoords[index*2+3] - (GLfloat)dashCoords[index*2+1]);
		lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
		dx /= lineLength;
		dy /= lineLength;

		xPoints[0] = dashCoords[index*2+0] - (dy * _renderStroke->width / 2.0);
		yPoints[0] = dashCoords[index*2+1] + (dx * _renderStroke->width / 2.0);
		xPoints[1] = dashCoords[index*2+2] + (dy * _renderStroke->width / 2.0);
		yPoints[1] = dashCoords[index*2+3] - (dx * _renderStroke->width / 2.0);
		xPoints[2] = dashCoords[index*2+0] + (dy * _renderStroke->width / 2.0);
		yPoints[2] = dashCoords[index*2+1] - (dx * _renderStroke->width / 2.0);
		xPoints[3] = dashCoords[index*2+2] - (dy * _renderStroke->width / 2.0);
		yPoints[3] = dashCoords[index*2+3] + (dx * _renderStroke->width / 2.0);

		for(int index1 = 0; index1 < 4; index1++) {
			x += xPoints[index1];
			y += yPoints[index1];
			renderIndex ++;

			if (xPoints[index1] < minX) {
				minX = xPoints[index1];
			}
			if (xPoints[index1] > maxX) {
				maxX = xPoints[index1];
			}

			if (yPoints[index1] < minY) {
				minY = yPoints[index1];
			}
			if (yPoints[index1] > maxY) {
				maxY = yPoints[index1];
			}
		}
	}

	x /= renderIndex;
	y /= renderIndex;



	renderIndex = 0;
	for(int index = 0; index < dashPoints-1; index += 2) {
		dx = ((GLfloat)dashCoords[index*2+2] - (GLfloat)dashCoords[index*2+0]);
		dy = ((GLfloat)dashCoords[index*2+3] - (GLfloat)dashCoords[index*2+1]);
		lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
		dx /= lineLength;
		dy /= lineLength;

		xPoints[0] = dashCoords[index*2+0] - (dy * _renderStroke->width / 2.0);
		yPoints[0] = dashCoords[index*2+1] + (dx * _renderStroke->width / 2.0);
		xPoints[1] = dashCoords[index*2+2] + (dy * _renderStroke->width / 2.0);
		yPoints[1] = dashCoords[index*2+3] - (dx * _renderStroke->width / 2.0);
		xPoints[2] = dashCoords[index*2+0] + (dy * _renderStroke->width / 2.0);
		yPoints[2] = dashCoords[index*2+1] - (dx * _renderStroke->width / 2.0);
		xPoints[3] = dashCoords[index*2+2] - (dy * _renderStroke->width / 2.0);
		yPoints[3] = dashCoords[index*2+3] + (dx * _renderStroke->width / 2.0);


		for(int index1 = 0; index1 < 4; index1++) {
			if (minX == maxX) {
				uPoints[index1] = 0.0;
			} else {
				uPoints[index1] = (xPoints[index1] - minX) / (maxX - minX);
			}
			if (minY == maxY) {
				vPoints[index1] = 0.0;
			} else {
				vPoints[index1] = (yPoints[index1] - minY) / (maxY - minY);
			}
		}

		if (index > 0 && index < (dashPoints-2)) {

			//qDebug()  << "Graphics2D::renderDrawPolyline: before: " << xPoints[1] << " " << yPoints[1]  << " " << xPoints[3]  << " " << xPoints[3];

			if (dashCoords[index*2+4] == dashCoords[index*2+2] && dashCoords[index*2+5] == dashCoords[index*2+3]) {

				//qDebug()  << "Graphics2D::renderDrawPolyline: before: " << xPoints[1] << " " << yPoints[1]  << " " << xPoints[3]  << " " << xPoints[3];

				dx1 = ((GLfloat)dashCoords[index*2+6] - (GLfloat)dashCoords[index*2+4]);
				dy1 = ((GLfloat)dashCoords[index*2+7] - (GLfloat)dashCoords[index*2+5]);
				lineLength = sqrt (fabs(dx1 * dx1) + fabs(dy1 * dy1));
				dx1 /= lineLength;
				dy1 /= lineLength;

				xPoints[1] = dashCoords[index*2+2] + (dy * _renderStroke->width / 4.0) + (dy1 * _renderStroke->width / 4.0);
				yPoints[1] = dashCoords[index*2+3] - (dx * _renderStroke->width / 4.0) - (dx1 * _renderStroke->width / 4.0);
				xPoints[3] = dashCoords[index*2+2] - (dy * _renderStroke->width / 4.0) - (dy1 * _renderStroke->width / 4.0);
				yPoints[3] = dashCoords[index*2+3] + (dx * _renderStroke->width / 4.0) + (dx1 * _renderStroke->width / 4.0);

				//qDebug()  << "Graphics2D::renderDrawPolyline: after: " << xPoints[1] << " " << yPoints[1]  << " " << xPoints[3]  << " " << xPoints[3];
			}
		}

		//qDebug()  << "Graphics2D::renderDrawPolyline: : " << xPoints[0] << " " << yPoints[0]  << " " << xPoints[1] << " " << yPoints[1] << " "  << xPoints[2] << " " << yPoints[2]  << " " << xPoints[3]  << " " << xPoints[3];

		if (fabs(dy) > fabs(dx)) {
			_renderVertexCoords[renderIndex*8+0] = (GLfloat)xPoints[3];
			_renderVertexCoords[renderIndex*8+1] = (GLfloat)yPoints[3];
			_renderVertexCoords[renderIndex*8+2] = (GLfloat)xPoints[0];
			_renderVertexCoords[renderIndex*8+3] = (GLfloat)yPoints[0];
			_renderVertexCoords[renderIndex*8+4] = (GLfloat)xPoints[1];
			_renderVertexCoords[renderIndex*8+5] = (GLfloat)yPoints[1];
			_renderVertexCoords[renderIndex*8+6] = (GLfloat)xPoints[2];
			_renderVertexCoords[renderIndex*8+7] = (GLfloat)yPoints[2];

			_renderTextureCoords[renderIndex*8+0] = (GLfloat)uPoints[3];
			_renderTextureCoords[renderIndex*8+1] = (GLfloat)vPoints[3];
			_renderTextureCoords[renderIndex*8+2] = (GLfloat)uPoints[0];
			_renderTextureCoords[renderIndex*8+3] = (GLfloat)vPoints[0];
			_renderTextureCoords[renderIndex*8+4] = (GLfloat)uPoints[1];
			_renderTextureCoords[renderIndex*8+5] = (GLfloat)vPoints[1];
			_renderTextureCoords[renderIndex*8+6] = (GLfloat)uPoints[2];
			_renderTextureCoords[renderIndex*8+7] = (GLfloat)vPoints[2];
		} else {
			_renderVertexCoords[renderIndex*8+0] = (GLfloat)xPoints[2];
			_renderVertexCoords[renderIndex*8+1] = (GLfloat)yPoints[2];
			_renderVertexCoords[renderIndex*8+2] = (GLfloat)xPoints[1];
			_renderVertexCoords[renderIndex*8+3] = (GLfloat)yPoints[1];
			_renderVertexCoords[renderIndex*8+4] = (GLfloat)xPoints[0];
			_renderVertexCoords[renderIndex*8+5] = (GLfloat)yPoints[0];
			_renderVertexCoords[renderIndex*8+6] = (GLfloat)xPoints[3];
			_renderVertexCoords[renderIndex*8+7] = (GLfloat)yPoints[3];

			_renderTextureCoords[renderIndex*8+0] = (GLfloat)uPoints[2];
			_renderTextureCoords[renderIndex*8+1] = (GLfloat)vPoints[2];
			_renderTextureCoords[renderIndex*8+2] = (GLfloat)uPoints[1];
			_renderTextureCoords[renderIndex*8+3] = (GLfloat)vPoints[1];
			_renderTextureCoords[renderIndex*8+4] = (GLfloat)uPoints[0];
			_renderTextureCoords[renderIndex*8+5] = (GLfloat)vPoints[0];
			_renderTextureCoords[renderIndex*8+6] = (GLfloat)uPoints[3];
			_renderTextureCoords[renderIndex*8+7] = (GLfloat)vPoints[3];
		}

		renderIndex++;
		//qDebug()  << "Graphics2D::renderDrawPolyline: " << numberPoints << " " << index  << " " << renderIndex  << " " << (renderIndex*8 + 8)   << " " << MAX_VERTEX_COORDINATES;

		if ((renderIndex*8 + 8) > MAX_VERTEX_COORDINATES) {
			renderDrawTriangles(renderIndex, 4);

			renderIndex = 0;
		}
	}

	renderDrawTriangles(renderIndex, 4);

	if (dashCoords) {
		delete dashCoords;
	}

	//qDebug()  << "Graphics2D::renderDrawLine: " << _drawFloatIndices[commandCount*2+0]/2;
}

void Graphics2D::renderDrawTriangles(int renderCount, int renderPoints)
{
#ifdef GLES1
	glColor4f(_renderForegroundColor.red, _renderForegroundColor.green, _renderForegroundColor.blue, _renderForegroundColor.alpha);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, _renderVertexCoords);

	for(int index = 0; index < renderCount; index++) {
		glDrawArrays(GL_TRIANGLE_STRIP, index*renderPoints, renderPoints);
	}

	glDisableClientState(GL_VERTEX_ARRAY);

#elif defined(GLES2)

	if (_renderGradient) {

		//qDebug()  << "Graphics2D::renderDrawTriangles: _renderGradient";

	    glUseProgram(_polyGradientRenderingProgram);

	    GLint pmLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_projectionMatrix");
	    GLint mvmLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_modelViewMatrix");
		GLint positionLoc = glGetAttribLocation(_polyGradientRenderingProgram, "a_position");
		GLint texcoordLoc = glGetAttribLocation(_polyGradientRenderingProgram, "a_texcoord");

	    GLint segmentsLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_segments");
		GLint colorsLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_colors");
		GLint percentagesLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_percentages");
		GLint radiusLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_radius");
		GLint angleLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_angle");
		GLint originLoc = glGetUniformLocation(_polyGradientRenderingProgram, "u_origin");

		glUniformMatrix4fv(pmLoc, 1, GL_FALSE, _orthoMatrix);
		glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, _renderModelMatrix);

		glUniform1i(segmentsLoc, _renderGradient->segments);

		GLfloat gradientColors[400];
		for(int index = 0; index < _renderGradient->segments+1; index++) {
			gradientColors[index*4+0] = _renderGradient->colors[index].red;
			gradientColors[index*4+1] = _renderGradient->colors[index].green;
			gradientColors[index*4+2] = _renderGradient->colors[index].blue;
			gradientColors[index*4+3] = _renderGradient->colors[index].alpha;
		}
		glUniform4fv(colorsLoc, _renderGradient->segments+1, gradientColors);
		glUniform1fv(percentagesLoc, _renderGradient->segments*2+1, _renderGradient->percentages);
		glUniform1f(radiusLoc, _renderGradient->radius);
		glUniform1f(angleLoc, _renderGradient->angle);
		glUniform2f(originLoc, _renderGradient->originU, _renderGradient->originV);

		glEnableVertexAttribArray(positionLoc);
		glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderVertexCoords);

		glEnableVertexAttribArray(texcoordLoc);
		glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderTextureCoords);


		for(int index = 0; index < renderCount; index++) {
			glDrawArrays(GL_TRIANGLE_STRIP, index*renderPoints, renderPoints);
		}

		glDisableVertexAttribArray(texcoordLoc);
		glDisableVertexAttribArray(positionLoc);
	} else {
	    glUseProgram(_polyColorRenderingProgram);

	    GLint pmLoc = glGetUniformLocation(_polyColorRenderingProgram, "u_projectionMatrix");
	    GLint mvmLoc = glGetUniformLocation(_polyColorRenderingProgram, "u_modelViewMatrix");
		GLint colorLoc = glGetUniformLocation(_polyColorRenderingProgram, "u_color");
		GLint positionLoc = glGetAttribLocation(_polyColorRenderingProgram, "a_position");

		glUniformMatrix4fv(pmLoc, 1, GL_FALSE, _orthoMatrix);
		glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, _renderModelMatrix);
		glUniform4f(colorLoc, _renderForegroundColor.red, _renderForegroundColor.green, _renderForegroundColor.blue, _renderForegroundColor.alpha);

		glEnableVertexAttribArray(positionLoc);
		glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderVertexCoords);

		for(int index = 0; index < renderCount; index++) {
			glDrawArrays(GL_TRIANGLE_STRIP, index*renderPoints, renderPoints);
		}

		glDisableVertexAttribArray(positionLoc);
	}

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
}


// Render a line, using the current color, between the points (x1, y1) and (x2, y2) in this graphics context's coordinate system.
void Graphics2D::renderDrawFillArc(int commandCount)
{
	//qDebug()  << "Graphics2D::renderDrawFillArc: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1] << " " << _drawFloatIndices[commandCount*2+2] << " " << _drawFloatIndices[commandCount*2+3] << " " << _drawFloatIndices[commandCount*2+4] << " " << _drawFloatIndices[commandCount*2+5];

	double x, y, width, height, startAngle, arcAngle;
	double radiusX, radiusY, endAngle, drawAngle, endAngle1, drawAngle1, angleStep, temp;
	GLfloat dx, dy, dx1, dy1, dx2, dy2, length, length1;

	bool fillArc = false;
	if (_master2D->_drawCommands[commandCount] == RENDER_FILL_ARC) {
		fillArc = true;
	}

	x          = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	y          = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	width      = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	height     = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];
	startAngle = _drawFloats[_drawFloatIndices[commandCount*2+0]+4];
	arcAngle   = _drawFloats[_drawFloatIndices[commandCount*2+0]+5];

	radiusX = width / 2.0;
	radiusY = height / 2.0;

	endAngle = startAngle + arcAngle;
	if (startAngle < 0.0) {
		startAngle = 360.0 + startAngle;
	}
	if (endAngle < 0.0) {
		endAngle = 360.0 + endAngle;
	}
	if (endAngle < startAngle) {
		temp = endAngle;
		endAngle = startAngle;
		startAngle = temp;
	}

	length = (endAngle - startAngle) * M_PI * sqrt(radiusX * radiusX + radiusY * radiusY) / 180.0;

	angleStep = asin (1.0 / (height + width)) * 180.0 / M_PI;

	//qDebug()  << "Graphics2D::renderDrawArc: " << startAngle << " " << endAngle << " " << angleStep;

	double drawX, drawY, drawX1, drawY1, lastX, lastY, drawU, drawV, drawU1, drawV1, lastU, lastV, angle, angle1, currentLength, dashLength;
	int drawCount = 0;

	currentLength = 0.0;
	dashLength = 0.0;
	if (_renderStroke->dashCount > 1) {
		for(int index = 0; index < _renderStroke->dashCount; index++) {
			dashLength += _renderStroke->dash[index];
		}
	}


	float* dashCoords = NULL;
	int   dashPoints = 0;

	//qDebug()  << "Graphics2D::renderDrawArc: " << length;

	if (_renderStroke->dashCount > 1 && fillArc == false) {

		int dashIndex = -1;
		float currentLength = 0.0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashPoints++;
			}

			//qDebug()  << "Graphics2D::renderDrawArc: calculate array size " << currentLength << " " << dashIndex << " " << dashPoints;

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashPoints++;
		}

		//qDebug()  << "Graphics2D::renderDrawArc: " << currentLength << " " << dashPoints;

		dashCoords = new float[dashPoints];

		dashIndex = -1;
		currentLength = 0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}
		dashPoints = 0;

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashCoords[0] = startAngle;
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashCoords[0] = startAngle;
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashCoords[dashPoints] = startAngle + currentLength * (endAngle - startAngle) / length;
				dashPoints++;
			}

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashCoords[dashPoints] = endAngle;
			dashPoints++;
		}

	} else {
		dashPoints = 2;
		dashCoords = new float[dashPoints];
		dashCoords[0] = startAngle;
		dashCoords[1] = endAngle;
	}

	int renderIndex = 0;
	double xPoints[4], yPoints[4];

	for(int index = 0; index < dashPoints; index += 2) {
		drawAngle = dashCoords[index+0];
		endAngle = dashCoords[index+1];

		//qDebug()  << "Graphics2D::renderDrawArc: " << index << " " << drawAngle << " " << endAngle;

		drawCount = 0;

		do {
			angle = fmod(drawAngle, 360.0);
			if (angle < 0) {
				angle += 360.0;
			}

			if (angle >= 0.0 && angle <= 180.0) {
				drawX = cos(angle * M_PI / 180.0);
			} else {
				drawX = cos((360.0 - angle) * M_PI / 180.0);
			}

			if (angle >= 0.0 && angle <= 180.0) {
				drawY = sin(angle * M_PI / 180.0);
			} else {
				drawY = -sin((360.0 - angle) * M_PI / 180.0);
			}

			drawU = drawX * 0.5;
			drawV = drawY * 0.5;
			drawU += 0.5;
			drawV += 0.5;

			drawX *= width;
			drawY *= height;
			drawX += x;
			drawY += y;

			if (drawCount > 0) {
				dx = ((GLfloat)drawX - (GLfloat)lastX);
				dy = ((GLfloat)drawY - (GLfloat)lastY);
				length = sqrt (fabs(dx * dx) + fabs(dy * dy));
				dx /= length;
				dy /= length;

				currentLength += length;

				GLfloat dx1 = ((GLfloat)drawX - (GLfloat)x);
				GLfloat dy1 = ((GLfloat)drawY - (GLfloat)y);
				length1 = sqrt (fabs(dx1 * dx1) + fabs(dy1 * dy1));
				dx1 /= length1;
				dy1 /= length1;


				if (!(fabs(dx) < 1.0 || fabs(dy) < 1.0) || length > 20.0 || drawAngle == endAngle) {
					if (fillArc) {
						_renderVertexCoords[renderIndex*6+0] = (GLfloat)x;
						_renderVertexCoords[renderIndex*6+1] = (GLfloat)y;
						_renderVertexCoords[renderIndex*6+2] = (GLfloat)lastX;
						_renderVertexCoords[renderIndex*6+3] = (GLfloat)lastY;
						_renderVertexCoords[renderIndex*6+4] = (GLfloat)drawX;
						_renderVertexCoords[renderIndex*6+5] = (GLfloat)drawY;

						_renderTextureCoords[renderIndex*6+0] = (GLfloat)0.5;
						_renderTextureCoords[renderIndex*6+1] = (GLfloat)0.5;
						_renderTextureCoords[renderIndex*6+2] = (GLfloat)lastU;
						_renderTextureCoords[renderIndex*6+3] = (GLfloat)lastV;
						_renderTextureCoords[renderIndex*6+4] = (GLfloat)drawU;
						_renderTextureCoords[renderIndex*6+5] = (GLfloat)drawV;
					} else {
						//qDebug()  << "Graphics2D::renderDrawArc: renderIndex: " << renderIndex*8 << " " << (renderIndex*8+8);
						xPoints[0] = lastX - (dy * _renderStroke->width / 2.0);
						yPoints[0] = lastY + (dx * _renderStroke->width / 2.0);
						xPoints[1] = drawX + (dy * _renderStroke->width / 2.0);
						yPoints[1] = drawY - (dx * _renderStroke->width / 2.0);
						xPoints[2] = lastX + (dy * _renderStroke->width / 2.0);
						yPoints[2] = lastY - (dx * _renderStroke->width / 2.0);
						xPoints[3] = drawX - (dy * _renderStroke->width / 2.0);
						yPoints[3] = drawY + (dx * _renderStroke->width / 2.0);

						drawAngle1 = drawAngle + angleStep;
						if (drawAngle1 > endAngle) {
							drawAngle1 = endAngle;
						}

						//qDebug()  << "Graphics2D::renderDrawPolyline: before: " << xPoints[1] << " " << yPoints[1]  << " " << xPoints[3]  << " " << xPoints[3];

						angle1 = fmod(drawAngle1, 360.0);
						if (angle1 < 0) {
							angle1 += 360.0;
						}

						if (angle1 >= 0.0 && angle1 <= 180.0) {
							drawX1 = cos(angle1 * M_PI / 180.0);
						} else {
							drawX1 = cos((360.0 - angle1) * M_PI / 180.0);
						}

						if (angle1 >= 0.0 && angle1 <= 180.0) {
							drawY1 = sin(angle1 * M_PI / 180.0);
						} else {
							drawY1 = -sin((360.0 - angle1) * M_PI / 180.0);
						}

						drawU1 = drawX1 * 0.5;
						drawV1 = drawY1 * 0.5;
						drawU1 += 0.5;
						drawV1 += 0.5;

						drawX1 *= width;
						drawY1 *= height;
						drawX1 += x;
						drawY1 += y;

						if (drawAngle != endAngle) {
							dx2 = ((GLfloat)drawX1 - (GLfloat)drawX);
							dy2 = ((GLfloat)drawY1 - (GLfloat)drawY);
							length1 = sqrt (fabs(dx2 * dx2) + fabs(dy2 * dy2));
							dx2 /= length1;
							dy2 /= length1;

							xPoints[1] = drawX + (dy * _renderStroke->width / 4.0) + (dy2 * _renderStroke->width / 4.0);
							yPoints[1] = drawY - (dx * _renderStroke->width / 4.0) - (dx2 * _renderStroke->width / 4.0);
							xPoints[3] = drawX - (dy * _renderStroke->width / 4.0) - (dy2 * _renderStroke->width / 4.0);
							yPoints[3] = drawY + (dx * _renderStroke->width / 4.0) + (dx2 * _renderStroke->width / 4.0);
						}


						if (fabs(dy) > fabs(dx)) {
							_renderVertexCoords[renderIndex*8+0] = (GLfloat)xPoints[3];
							_renderVertexCoords[renderIndex*8+1] = (GLfloat)yPoints[3];
							_renderVertexCoords[renderIndex*8+2] = (GLfloat)xPoints[0];
							_renderVertexCoords[renderIndex*8+3] = (GLfloat)yPoints[0];
							_renderVertexCoords[renderIndex*8+4] = (GLfloat)xPoints[1];
							_renderVertexCoords[renderIndex*8+5] = (GLfloat)yPoints[1];
							_renderVertexCoords[renderIndex*8+6] = (GLfloat)xPoints[2];
							_renderVertexCoords[renderIndex*8+7] = (GLfloat)yPoints[2];
						} else {
							_renderVertexCoords[renderIndex*8+0] = (GLfloat)xPoints[2];
							_renderVertexCoords[renderIndex*8+1] = (GLfloat)yPoints[2];
							_renderVertexCoords[renderIndex*8+2] = (GLfloat)xPoints[1];
							_renderVertexCoords[renderIndex*8+3] = (GLfloat)yPoints[1];
							_renderVertexCoords[renderIndex*8+4] = (GLfloat)xPoints[0];
							_renderVertexCoords[renderIndex*8+5] = (GLfloat)yPoints[0];
							_renderVertexCoords[renderIndex*8+6] = (GLfloat)xPoints[3];
							_renderVertexCoords[renderIndex*8+7] = (GLfloat)yPoints[3];
						}
					}

					//qDebug()  << "Graphics2D::renderDrawArc: " << drawCount << " " << renderIndex << drawAngle << " " << endAngle << " " << drawX << " " << drawY << " " << lastX << " " << lastY;
					//qDebug()  << "Graphics2D::renderDrawArc: " << renderIndex << " " << _renderVertexCoords[renderIndex*8+0] << " " << _renderVertexCoords[renderIndex*8+1] << " " << _renderVertexCoords[renderIndex*8+2] << " " << _renderVertexCoords[renderIndex*8+3] << " " << _renderVertexCoords[renderIndex*8+4] << " " << _renderVertexCoords[renderIndex*8+5] << " " << _renderVertexCoords[renderIndex*8+6] << " " << _renderVertexCoords[renderIndex*8+7];

					lastX = drawX;
					lastY = drawY;

					lastU = drawU;
					lastV = drawV;

					renderIndex++;

					if ((renderIndex*8 + 8) > MAX_VERTEX_COORDINATES) {
						if (fillArc) {
							renderDrawTriangles(renderIndex, 3);
						} else {
							renderDrawTriangles(renderIndex, 4);
						}

						renderIndex = 0;
					}

				}
			} else {
				lastX = drawX;
				lastY = drawY;

				lastU = drawU;
				lastV = drawV;
			}

			drawCount++;

			if (drawAngle < endAngle) {
				drawAngle += angleStep;
				if (drawAngle > endAngle) {
					drawAngle = endAngle;
				}
			} else {
				drawAngle += angleStep;
			}
		} while (drawAngle <= endAngle);

	}

	if (fillArc) {
		renderDrawTriangles(renderIndex, 3);
	} else {
		renderDrawTriangles(renderIndex, 4);
	}

	if (dashCoords) {
		delete dashCoords;
	}

	//qDebug()  << "Graphics2D::renderDrawArc: " << _drawFloatIndices[commandCount*2+0]/2;
}

// Draws as much of the specified area of the specified image as is currently available, scaling it on the fly to fit inside the specified area of the destination drawable surface.
void Graphics2D::renderDrawImage(int commandCount)
{
	qDebug()  << "Graphics2D::renderDrawImage: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	GLuint  photo = 0;
	GLfloat imageVertices[8];
	GLfloat imageTexCoord[8];

	photo = _drawInts[_drawIntIndices[commandCount*2+0]+0];

	qDebug()  << "Graphics2D::renderDrawImage: " << photo;

	imageTexCoord[0] = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	imageTexCoord[1] = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	imageTexCoord[2] = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	imageTexCoord[3] = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];
	imageTexCoord[4] = _drawFloats[_drawFloatIndices[commandCount*2+0]+4];
	imageTexCoord[5] = _drawFloats[_drawFloatIndices[commandCount*2+0]+5];
	imageTexCoord[6] = _drawFloats[_drawFloatIndices[commandCount*2+0]+6];
	imageTexCoord[7] = _drawFloats[_drawFloatIndices[commandCount*2+0]+7];

	imageVertices[0] = _drawFloats[_drawFloatIndices[commandCount*2+0]+8];
	imageVertices[1] = _drawFloats[_drawFloatIndices[commandCount*2+0]+9];
	imageVertices[2] = _drawFloats[_drawFloatIndices[commandCount*2+0]+10];
	imageVertices[3] = _drawFloats[_drawFloatIndices[commandCount*2+0]+11];
	imageVertices[4] = _drawFloats[_drawFloatIndices[commandCount*2+0]+12];
	imageVertices[5] = _drawFloats[_drawFloatIndices[commandCount*2+0]+13];
	imageVertices[6] = _drawFloats[_drawFloatIndices[commandCount*2+0]+14];
	imageVertices[7] = _drawFloats[_drawFloatIndices[commandCount*2+0]+15];

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (photo > 0) {
#ifdef GLES1
		glEnable(GL_TEXTURE_2D);

		glColor4f(COLOR_WHITE.red, COLOR_WHITE.green, COLOR_WHITE.blue, COLOR_WHITE.alpha);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_FLOAT, 0, imageVertices);
		glTexCoordPointer(2, GL_FLOAT, 0, imageTexCoord);

		glBindTexture(GL_TEXTURE_2D, photo);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_TEXTURE_2D);

#elif defined(GLES2)

		qDebug()  << "Graphics2D::renderDrawImage: program: " << _polyTextureRenderingProgram;

	    //First render background and menu if it is enabled
	    glUseProgram(_polyTextureRenderingProgram);

		GLint positionLoc = glGetAttribLocation(_polyTextureRenderingProgram, "a_position");
		GLint texcoordLoc = glGetAttribLocation(_polyTextureRenderingProgram, "a_texcoord");
		GLint textureLoc = glGetUniformLocation(_polyTextureRenderingProgram, "u_texture");
	    GLint pmLoc = glGetUniformLocation(_polyTextureRenderingProgram, "u_projectionMatrix");
	    GLint mvmLoc = glGetUniformLocation(_polyTextureRenderingProgram, "u_modelViewMatrix");

		glBindBuffer(GL_ARRAY_BUFFER, 0);

	    glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, photo);
		glUniform1i(textureLoc, 0);

		glUniformMatrix4fv(pmLoc, 1, GL_FALSE, _orthoMatrix);
		glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, _renderModelMatrix);

		glEnableVertexAttribArray(positionLoc);
		glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, imageVertices);

		glEnableVertexAttribArray(texcoordLoc);
		glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, imageTexCoord);
		qDebug() << "imageTexCoord: " << imageTexCoord[0] << " " << imageTexCoord[1] << " " << imageTexCoord[2] << " " << imageTexCoord[3] << " " << imageTexCoord[4] << " " << imageTexCoord[5] << " " << imageTexCoord[6] << " " << imageTexCoord[7];

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(texcoordLoc);
		glDisableVertexAttribArray(positionLoc);

#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
	}

	glDisable(GL_BLEND);

	//qDebug()  << "Graphics2D::renderDrawImage: " << _drawFloatIndices[commandCount*2+0]/2;
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void Graphics2D::renderDrawFillRoundRect(int commandCount)
{
	//qDebug()  << "Graphics2D::renderDrawRoundRect: " << commandCount;

	double x, y, width, height, arcWidth, arcHeight, startAngle, arcAngle;
	double radiusX, radiusY, endAngle, drawAngle, angleStep, temp;
	GLfloat dx, dy, length;

	bool fill = false;
	if (_master2D->_drawCommands[commandCount] == RENDER_FILL_ROUNDRECT) {
		fill = true;
	}

	//qDebug()  << "Graphics2D::renderDrawRoundRect: " << fill;

	x          = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	y          = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];
	width      = _drawFloats[_drawFloatIndices[commandCount*2+0]+2];
	height     = _drawFloats[_drawFloatIndices[commandCount*2+0]+3];
	arcWidth   = _drawFloats[_drawFloatIndices[commandCount*2+0]+4];
	arcHeight  = _drawFloats[_drawFloatIndices[commandCount*2+0]+5];
	startAngle = 0.0;
	arcAngle   = 360.0;


	// calculate u,v texture bounds
	float minX = 1.0e6, minY = 1.0e6, maxX = -1.0e6, maxY = -1.0e6;

	if (fill) {
		minX = x;
		minY = y;
		maxX = x + width;
		maxY = y + height;
	} else {
		minX = x;
		minY = y;
		maxX = x + width;
		maxY = y + height;
		if (_renderStroke->width > 0) {
			minX -= _renderStroke->width / 2.0;
			minY -= _renderStroke->width / 2.0;
			maxX += _renderStroke->width / 2.0;
			maxY += _renderStroke->width / 2.0;
		}
	}

	radiusX = arcWidth;
	radiusY = arcHeight;

	endAngle = startAngle + arcAngle;
	if (startAngle < 0.0) {
		startAngle = 360.0 + startAngle;
	}
	if (endAngle < 0.0) {
		endAngle = 360.0 + endAngle;
	}
	if (endAngle < startAngle) {
		temp = endAngle;
		endAngle = startAngle;
		startAngle = temp;
	}

	float arcLength = 0.5 * M_PI * sqrt(radiusX * radiusX + radiusY * radiusY);
	float horizLength = width - arcWidth*2.0;
	float vertLength = height - arcHeight*2.0;

	length = 4.0 * arcLength + 2.0 * horizLength + 2.0 * vertLength;

	angleStep = asin (1.0 / (arcHeight + arcWidth)) * 180.0 / M_PI;

	double drawX, drawY, lastX, lastY, angle, currentLength, dashLength;
	int drawCount = 0;

	currentLength = 0.0;
	dashLength = 0.0;
	if (_renderStroke->dashCount > 1) {
		for(int index = 0; index < _renderStroke->dashCount; index++) {
			dashLength += _renderStroke->dash[index];
		}
	}


	float* dashCoords = NULL;
	int   dashPoints = 0;

	//qDebug()  << "Graphics2D::renderDrawRoundRect: " << length;

	if (_renderStroke->dashCount > 1 && fill == false) {

		int dashIndex = -1;
		float currentLength = 0.0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashPoints++;
			}

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashPoints++;
		}

		//qDebug()  << "Graphics2D::renderDrawRoundRect: " << currentLength << " " << dashPoints;

		dashCoords = new float[dashPoints];

		dashIndex = -1;
		currentLength = 0;
		if (_renderStroke->dashPhase != 0.0) {
			currentLength = -_renderStroke->dashPhase;
		}
		dashPoints = 0;

		do {
			if (dashIndex >= 0) {
				currentLength += _renderStroke->dash[dashIndex];
				if (currentLength > length) {
					currentLength = length;

					if (dashPoints == 0 && (dashIndex % 2 == 0)) {
						dashCoords[0] = 0;
						dashPoints++;
					}
					continue;
				}
			}

			if (dashPoints == 0 && currentLength == 0.0) {
				dashCoords[0] = 0;
				dashPoints++;
			}

			if (currentLength > 0.0) {
				dashCoords[dashPoints] = currentLength;
				dashPoints++;
			}

			dashIndex++;
			if (dashIndex == _renderStroke->dashCount) {
				dashIndex = 0;
			}

		} while (currentLength < length);

		if ((dashPoints % 2) == 1) {
			dashCoords[dashPoints] = length;
			dashPoints++;
		}

	} else {
		dashPoints = 16;
		dashCoords = new float[dashPoints];
		dashCoords[ 0] = 0.0;
		dashCoords[ 1] = arcLength;
		dashCoords[ 2] = arcLength;
		dashCoords[ 3] = arcLength + horizLength;
		dashCoords[ 4] = arcLength + horizLength;
		dashCoords[ 5] = arcLength*2.0 + horizLength;
		dashCoords[ 6] = arcLength*2.0 + horizLength;
		dashCoords[ 7] = arcLength*2.0 + horizLength + vertLength;
		dashCoords[ 8] = arcLength*2.0 + horizLength + vertLength;
		dashCoords[ 9] = arcLength*3.0 + horizLength + vertLength;
		dashCoords[10] = arcLength*3.0 + horizLength + vertLength;
		dashCoords[11] = arcLength*3.0 + 2.0*horizLength + vertLength;
		dashCoords[12] = arcLength*3.0 + 2.0*horizLength + vertLength;
		dashCoords[13] = arcLength*4.0 + 2.0*horizLength + vertLength;
		dashCoords[14] = arcLength*4.0 + 2.0*horizLength + vertLength;
		dashCoords[15] = arcLength*4.0 + 2.0*horizLength + 2.0*vertLength;
	}

	int renderIndex = 0;
	float drawLength, endLength;
	float x1, y1, x2, y2;

	for(int index = 0; index < dashPoints; index += 2) {
		drawLength = dashCoords[index+0];
		endLength = dashCoords[index+1];

		//qDebug()  << "Graphics2D::renderDrawRoundRect: " << index << " " << drawAngle << " " << endAngle;

		bool drawArc = false;
		int quadrant = 0;
		bool drawLine = false;
		bool drawSecondArc = false;

		if (drawLength >= 0.0 && endLength <= arcLength) {
			drawAngle = 90.0 * drawLength / arcLength;
			endAngle  = 90.0 *  endLength / arcLength;
			drawArc = true;
			quadrant = 0;
		} else if (drawLength >= 0.0 && drawLength < arcLength && endLength > arcLength && endLength <= (arcLength+horizLength)) {
			drawAngle = 90.0 * drawLength / arcLength;
			endAngle  = 90.0;
			drawArc = true;
			quadrant = 0;

			x1 = x + arcWidth + horizLength;
			y1 = y + height;
			x2 = x + arcWidth + horizLength - (endLength - arcLength);
			y2 = y + height;
			drawLine = true;
		} else if (drawLength >= arcLength && endLength <= (arcLength+horizLength)) {
			x1 = x + arcWidth + horizLength - (drawLength - arcLength);
			y1 = y + height;
			x2 = x + arcWidth + horizLength - (endLength - arcLength);
			y2 = y + height;
			drawLine = true;
		} else if (drawLength >= arcLength && drawLength < (arcLength+horizLength) && endLength > (arcLength+horizLength) && endLength <= (2.0*arcLength+horizLength)) {
			x1 = x + arcWidth + horizLength - (drawLength - arcLength);
			y1 = y + height;
			x2 = x + arcWidth;
			y2 = y + height;
			drawLine = true;

			drawAngle = 90.0;
			endAngle = 90.0 + 90.0 * (endLength - (arcLength+horizLength)) / arcLength;
			drawSecondArc = true;
			quadrant = 1;
		} else if (drawLength >= (arcLength+horizLength) && endLength <= (2.0*arcLength+horizLength)) {
			drawAngle = 90.0 + 90.0 * (drawLength - (arcLength+horizLength)) / arcLength;
			endAngle = 90.0 + 90.0 * (endLength - (arcLength+horizLength)) / arcLength;
			drawArc = true;
			quadrant = 1;
		} else if (drawLength >= (arcLength+horizLength) && drawLength < (2.0*arcLength+horizLength) && endLength > (2.0*arcLength+horizLength) && endLength <= (2.0*arcLength+horizLength+vertLength)) {
			drawAngle = 90.0 + 90.0 * (drawLength -(arcLength+horizLength)) / arcLength;
			endAngle  = 180.0;
			drawArc = true;
			quadrant = 1;

			x1 = x;
			y1 = y + height - arcHeight;
			x2 = x;
			y2 = y + height - arcHeight - (endLength - (2.0*arcLength+horizLength));
			drawLine = true;
		} else if (drawLength >= (2.0*arcLength+horizLength) && endLength <= (2.0*arcLength+horizLength+vertLength)) {
			x1 = x;
			y1 = y + height - arcHeight - (drawLength - (2.0*arcLength+horizLength));
			x2 = x;
			y2 = y + height - arcHeight - (endLength - (2.0*arcLength+horizLength));
			drawLine = true;
		} else if (drawLength >= (2.0*arcLength+horizLength) && drawLength < (2.0*arcLength+horizLength+vertLength) && endLength > (2.0*arcLength+horizLength+vertLength) && endLength <= (3.0*arcLength+horizLength+vertLength)) {
			x1 = x;
			y1 = y + height - arcHeight - (drawLength - (2.0*arcLength+horizLength));
			x2 = x;
			y2 = y + arcHeight;
			drawLine = true;

			drawAngle = 180.0;
			endAngle = 180.0 + 90.0 * (endLength - (arcLength*2.0+horizLength+vertLength)) / arcLength;
			drawSecondArc = true;
			quadrant = 2;
		} else if (drawLength >= (arcLength*2.0+horizLength+vertLength) && endLength <= (3.0*arcLength+horizLength+vertLength)) {
			drawAngle = 180.0 + 90.0 * (drawLength - (arcLength*2.0+horizLength+vertLength)) / arcLength;
			endAngle = 180.0 + 90.0 * (endLength - (arcLength*2.0+horizLength+vertLength)) / arcLength;
			drawArc = true;
			quadrant = 2;

		} else if (drawLength >= (arcLength*2.0+horizLength+vertLength) && drawLength < (3.0*arcLength+horizLength+vertLength) && endLength > (3.0*arcLength+horizLength+vertLength) && endLength <= (3.0*arcLength+2.0*horizLength+vertLength)) {
			drawAngle = 180.0 + 90.0 * (drawLength - (arcLength*2.0+horizLength+vertLength)) / arcLength;
			endAngle  = 270.0;
			drawArc = true;
			quadrant = 2;

			x1 = x + arcWidth;
			y1 = y;
			x2 = x + arcWidth + (endLength - (3.0*arcLength+horizLength+vertLength));
			y2 = y;
			if (!fill) {
				drawLine = true;
			}
		} else if (drawLength >= (3.0*arcLength+horizLength+vertLength) && endLength <= (3.0*arcLength+2.0*horizLength+vertLength)) {
			x1 = x + arcWidth + (drawLength - (3.0*arcLength+horizLength+vertLength));
			y1 = y;
			x2 = x + arcWidth + (endLength - (3.0*arcLength+horizLength+vertLength));
			y2 = y;
			if (!fill) {
				drawLine = true;
			}
		} else if (drawLength >= (3.0*arcLength+horizLength+vertLength) && drawLength < (3.0*arcLength+2.0*horizLength+vertLength) && endLength > (3.0*arcLength+2.0*horizLength+vertLength) && endLength <= (4.0*arcLength+2.0*horizLength+vertLength)) {
			x1 = x + arcWidth + (drawLength - (3.0*arcLength+horizLength+vertLength));
			y1 = y;
			x2 = x + arcWidth + horizLength;
			y2 = y;
			if (!fill) {
				drawLine = true;
			}

			drawAngle = 270.0;
			endAngle = 270.0 + 90.0 * (endLength - (3.0*arcLength+2.0*horizLength+vertLength)) / arcLength;
			drawSecondArc = true;
			quadrant = 3;
		} else if (drawLength >= (3.0*arcLength+2.0*horizLength+vertLength) && endLength <= (4.0*arcLength+2.0*horizLength+vertLength)) {
			drawAngle = 270.0 + 90.0 * (drawLength - (3.0*arcLength+2.0*horizLength+vertLength)) / arcLength;
			endAngle = 270.0 + 90.0 * (endLength - (3.0*arcLength+2.0*horizLength+vertLength)) / arcLength;
			drawArc = true;
			quadrant = 3;
		} else if (drawLength >= (3.0*arcLength+2.0*horizLength+vertLength) && drawLength < (4.0*arcLength+2.0*horizLength+vertLength) && endLength > (4.0*arcLength+2.0*horizLength+vertLength) && endLength <= (4.0*arcLength+2.0*horizLength+2.0*vertLength)) {
			drawAngle = 270.0 + 90.0 * (drawLength -(3.0*arcLength+2.0*horizLength+vertLength)) / arcLength;
			endAngle  = 360.0;
			drawArc = true;
			quadrant = 3;

			x1 = x + width;
			y1 = y + arcHeight;
			x2 = x + width;
			y2 = y + arcHeight + (endLength - (4.0*arcLength+2.0*horizLength+vertLength));
			drawLine = true;
		} else if (drawLength >= (4.0*arcLength+2.0*horizLength+vertLength)) {
		//} else if (drawLength >= (4.0*arcLength+2.0*horizLength+vertLength) && endLength <= (4.0*arcLength+2.0*horizLength+2.0*vertLength)) {
			x1 = x + width;
			y1 = y + arcHeight + (drawLength - (4.0*arcLength+2.0*horizLength+vertLength));
			x2 = x + width;
			y2 = y + arcHeight + (endLength - (4.0*arcLength+2.0*horizLength+vertLength));
			drawLine = true;
		}

		if (drawArc) {
			drawCount = 0;

			do {
				angle = fmod(drawAngle, 360.0);
				if (angle < 0) {
					angle += 360.0;
				}

				if (angle >= 0.0 && angle <= 180.0) {
					drawX = cos(angle * M_PI / 180.0);
				} else {
					drawX = cos((360.0 - angle) * M_PI / 180.0);
				}

				if (angle >= 0.0 && angle <= 180.0) {
					drawY = sin(angle * M_PI / 180.0);
				} else {
					drawY = -sin((360.0 - angle) * M_PI / 180.0);
				}

				drawX *= arcWidth;
				drawY *= arcHeight;

				switch (quadrant) {
				case 0:
					drawX += x + arcWidth + horizLength;
					drawY += y + arcHeight + vertLength;
					break;
				case 1:
					drawX += x + arcWidth;
					drawY += y + arcHeight + vertLength;
					break;
				case 2:
					drawX += x + arcWidth;
					drawY += y + arcHeight;
					break;
				case 3:
					drawX += x + arcWidth + horizLength;
					drawY += y + arcHeight;
					break;
				}

				if (drawCount > 0) {
					dx = ((GLfloat)drawX - (GLfloat)lastX);
					dy = ((GLfloat)drawY - (GLfloat)lastY);
					length = sqrt (fabs(dx * dx) + fabs(dy * dy));

					if (!(fabs(dx) < 1.0 || fabs(dy) < 1.0) || length > 20.0 || drawAngle == endAngle) {
						if (fill) {
							switch (quadrant) {
							case 0:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth + horizLength);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight + vertLength);
								break;
							case 1:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight + vertLength);
								break;
							case 2:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight);
								break;
							case 3:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth + horizLength);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight);
								break;
							}

							_renderVertexCoords[renderIndex*6+2] = (GLfloat)lastX;
							_renderVertexCoords[renderIndex*6+3] = (GLfloat)lastY;
							_renderVertexCoords[renderIndex*6+4] = (GLfloat)drawX;
							_renderVertexCoords[renderIndex*6+5] = (GLfloat)drawY;

							for(int index1 = 0; index1 < 6; index1 += 2) {
								if (minX == maxX) {
									_renderTextureCoords[renderIndex*6+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
								}
								if (minY == maxY) {
									_renderTextureCoords[renderIndex*6+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
								}
							}

						} else {
							if (fabs(dy) > fabs(dx)) {
								_renderVertexCoords[renderIndex*8+0] = (GLfloat)drawX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+1] = (GLfloat)drawY + (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+2] = (GLfloat)lastX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+3] = (GLfloat)lastY + (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+4] = (GLfloat)drawX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+5] = (GLfloat)drawY - (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+6] = (GLfloat)lastX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+7] = (GLfloat)lastY - (dx * _renderStroke->width / (2.0 * length));
							} else {
								_renderVertexCoords[renderIndex*8+0] = (GLfloat)lastX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+1] = (GLfloat)lastY - (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+2] = (GLfloat)drawX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+3] = (GLfloat)drawY - (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+4] = (GLfloat)lastX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+5] = (GLfloat)lastY + (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+6] = (GLfloat)drawX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+7] = (GLfloat)drawY + (dx * _renderStroke->width / (2.0 * length));
							}

							for(int index1 = 0; index1 < 8; index1 += 2) {
								if (minX == maxX) {
									_renderTextureCoords[renderIndex*8+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*8+index1] = (_renderVertexCoords[renderIndex*8+index1] - minX) / (maxX - minX);
								}
								if (minY == maxY) {
									_renderTextureCoords[renderIndex*8+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*8+index1+1] = (_renderVertexCoords[renderIndex*8+index1+1] - minY) / (maxY - minY);
								}
							}
						}

						lastX = drawX;
						lastY = drawY;
						renderIndex++;
					}
				} else {
					lastX = drawX;
					lastY = drawY;
				}

				drawCount++;

				if (drawAngle < endAngle) {
					drawAngle += angleStep;
					if (drawAngle > endAngle) {
						drawAngle = endAngle;
					}
				} else {
					drawAngle += angleStep;
				}
			} while (drawAngle <= endAngle);
		}

		if (drawLine) {
			dx = ((GLfloat)x2 - (GLfloat)x1);
			dy = ((GLfloat)y2 - (GLfloat)y1);
			length = sqrt (fabs(dx * dx) + fabs(dy * dy));


			if (fill) {
				//qDebug()  << "Graphics2D::renderDrawRoundRect: " << fill << ":" << x1 << ":" << y1 << ":" << x2 << ":" << y2;

				if (x2 < x1 && y1 == y2) {
					_renderVertexCoords[renderIndex*6+0] = (GLfloat)x2;
					_renderVertexCoords[renderIndex*6+1] = (GLfloat)y2 - height;
					_renderVertexCoords[renderIndex*6+2] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+3] = (GLfloat)y1;
					_renderVertexCoords[renderIndex*6+4] = (GLfloat)x2;
					_renderVertexCoords[renderIndex*6+5] = (GLfloat)y2;

					for(int index1 = 0; index1 < 6; index1 += 2) {
						if (minX == maxX) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
						}
						if (minY == maxY) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
						}
					}

					renderIndex++;

					_renderVertexCoords[renderIndex*6+0] = (GLfloat)x2;
					_renderVertexCoords[renderIndex*6+1] = (GLfloat)y2 - height;
					_renderVertexCoords[renderIndex*6+2] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+3] = (GLfloat)y1 - height;
					_renderVertexCoords[renderIndex*6+4] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+5] = (GLfloat)y1;

					for(int index1 = 0; index1 < 6; index1 += 2) {
						if (minX == maxX) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
						}
						if (minY == maxY) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
						}
					}
				}

				if (y2 < y1 && x1 == x2) {
					_renderVertexCoords[renderIndex*6+0] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+1] = (GLfloat)y1;
					_renderVertexCoords[renderIndex*6+2] = (GLfloat)x2;
					_renderVertexCoords[renderIndex*6+3] = (GLfloat)y2;
					_renderVertexCoords[renderIndex*6+4] = (GLfloat)x2 + arcWidth;
					_renderVertexCoords[renderIndex*6+5] = (GLfloat)y2;

					for(int index1 = 0; index1 < 6; index1 += 2) {
						if (minX == maxX) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
						}
						if (minY == maxY) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
						}
					}
					renderIndex++;

					_renderVertexCoords[renderIndex*6+0] = (GLfloat)x2 + arcWidth;
					_renderVertexCoords[renderIndex*6+1] = (GLfloat)y2;
					_renderVertexCoords[renderIndex*6+2] = (GLfloat)x1 + arcWidth;
					_renderVertexCoords[renderIndex*6+3] = (GLfloat)y1;
					_renderVertexCoords[renderIndex*6+4] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+5] = (GLfloat)y1;

					for(int index1 = 0; index1 < 6; index1 += 2) {
						if (minX == maxX) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
						}
						if (minY == maxY) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
						}
					}
				}

				if (y2 > y1 && x1 == x2) {
					_renderVertexCoords[renderIndex*6+0] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+1] = (GLfloat)y1;
					_renderVertexCoords[renderIndex*6+2] = (GLfloat)x2;
					_renderVertexCoords[renderIndex*6+3] = (GLfloat)y2;
					_renderVertexCoords[renderIndex*6+4] = (GLfloat)x2 - arcWidth;
					_renderVertexCoords[renderIndex*6+5] = (GLfloat)y2;

					for(int index1 = 0; index1 < 6; index1 += 2) {
						if (minX == maxX) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
						}
						if (minY == maxY) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
						}
					}
					renderIndex++;

					_renderVertexCoords[renderIndex*6+0] = (GLfloat)x2 - arcWidth;
					_renderVertexCoords[renderIndex*6+1] = (GLfloat)y2;
					_renderVertexCoords[renderIndex*6+2] = (GLfloat)x1 - arcWidth;
					_renderVertexCoords[renderIndex*6+3] = (GLfloat)y1;
					_renderVertexCoords[renderIndex*6+4] = (GLfloat)x1;
					_renderVertexCoords[renderIndex*6+5] = (GLfloat)y1;

					for(int index1 = 0; index1 < 6; index1 += 2) {
						if (minX == maxX) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
						}
						if (minY == maxY) {
							_renderTextureCoords[renderIndex*6+index1] = 0.0;
						} else {
							_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
						}
					}
				}
			} else {
				if (fabs(dy) > fabs(dx)) {
					_renderVertexCoords[renderIndex*8+0] = (GLfloat)x2 - (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+1] = (GLfloat)y2 + (dx * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+2] = (GLfloat)x1 - (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+3] = (GLfloat)y1 + (dx * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+4] = (GLfloat)x2 + (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+5] = (GLfloat)y2 - (dx * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+6] = (GLfloat)x1 + (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+7] = (GLfloat)y1 - (dx * _renderStroke->width / (2.0 * length));
				} else {
					_renderVertexCoords[renderIndex*8+0] = (GLfloat)x1 + (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+1] = (GLfloat)y1 - (dx * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+2] = (GLfloat)x2 + (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+3] = (GLfloat)y2 - (dx * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+4] = (GLfloat)x1 - (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+5] = (GLfloat)y1 + (dx * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+6] = (GLfloat)x2 - (dy * _renderStroke->width / (2.0 * length));
					_renderVertexCoords[renderIndex*8+7] = (GLfloat)y2 + (dx * _renderStroke->width / (2.0 * length));
				}

				for(int index1 = 0; index1 < 8; index1 += 2) {
					if (minX == maxX) {
						_renderTextureCoords[renderIndex*8+index1] = 0.0;
					} else {
						_renderTextureCoords[renderIndex*8+index1] = (_renderVertexCoords[renderIndex*8+index1] - minX) / (maxX - minX);
					}
					if (minY == maxY) {
						_renderTextureCoords[renderIndex*8+index1] = 0.0;
					} else {
						_renderTextureCoords[renderIndex*8+index1+1] = (_renderVertexCoords[renderIndex*8+index1+1] - minY) / (maxY - minY);
					}
				}
			}
			renderIndex++;
		}

		if (drawSecondArc) {
			drawCount = 0;

			do {
				angle = fmod(drawAngle, 360.0);
				if (angle < 0) {
					angle += 360.0;
				}

				if (angle >= 0.0 && angle <= 180.0) {
					drawX = cos(angle * M_PI / 180.0);
				} else {
					drawX = cos((360.0 - angle) * M_PI / 180.0);
				}

				if (angle >= 0.0 && angle <= 180.0) {
					drawY = sin(angle * M_PI / 180.0);
				} else {
					drawY = -sin((360.0 - angle) * M_PI / 180.0);
				}


				drawX *= arcWidth;
				drawY *= arcHeight;

				switch (quadrant) {
				case 0:
					drawX += x + arcWidth + horizLength;
					drawY += y + arcHeight + vertLength;
					break;
				case 1:
					drawX += x + arcWidth;
					drawY += y + arcHeight + vertLength;
					break;
				case 2:
					drawX += x + arcWidth;
					drawY += y + arcHeight;
					break;
				case 3:
					drawX += x + arcWidth + horizLength;
					drawY += y + arcHeight;
					break;
				}

				if (drawCount > 0) {
					dx = ((GLfloat)drawX - (GLfloat)lastX);
					dy = ((GLfloat)drawY - (GLfloat)lastY);
					length = sqrt (fabs(dx * dx) + fabs(dy * dy));

					if (!(fabs(dx) < 1.0 || fabs(dy) < 1.0) || length > 20.0 || drawAngle == endAngle) {
						if (fill) {
							switch (quadrant) {
							case 0:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth + horizLength);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight + vertLength);
								break;
							case 1:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight + vertLength);
								break;
							case 2:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight);
								break;
							case 3:
								_renderVertexCoords[renderIndex*6+0] = (GLfloat)(x + arcWidth + horizLength);
								_renderVertexCoords[renderIndex*6+1] = (GLfloat)(y + arcHeight);
								break;
							}

							_renderVertexCoords[renderIndex*6+2] = (GLfloat)lastX;
							_renderVertexCoords[renderIndex*6+3] = (GLfloat)lastY;
							_renderVertexCoords[renderIndex*6+4] = (GLfloat)drawX;
							_renderVertexCoords[renderIndex*6+5] = (GLfloat)drawY;

							for(int index1 = 0; index1 < 6; index1 += 2) {
								if (minX == maxX) {
									_renderTextureCoords[renderIndex*6+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*6+index1] = (_renderVertexCoords[renderIndex*6+index1] - minX) / (maxX - minX);
								}
								if (minY == maxY) {
									_renderTextureCoords[renderIndex*6+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*6+index1+1] = (_renderVertexCoords[renderIndex*6+index1+1] - minY) / (maxY - minY);
								}
							}
						} else {
							if (fabs(dy) > fabs(dx)) {
								_renderVertexCoords[renderIndex*8+0] = (GLfloat)drawX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+1] = (GLfloat)drawY + (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+2] = (GLfloat)lastX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+3] = (GLfloat)lastY + (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+4] = (GLfloat)drawX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+5] = (GLfloat)drawY - (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+6] = (GLfloat)lastX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+7] = (GLfloat)lastY - (dx * _renderStroke->width / (2.0 * length));
							} else {
								_renderVertexCoords[renderIndex*8+0] = (GLfloat)lastX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+1] = (GLfloat)lastY - (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+2] = (GLfloat)drawX + (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+3] = (GLfloat)drawY - (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+4] = (GLfloat)lastX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+5] = (GLfloat)lastY + (dx * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+6] = (GLfloat)drawX - (dy * _renderStroke->width / (2.0 * length));
								_renderVertexCoords[renderIndex*8+7] = (GLfloat)drawY + (dx * _renderStroke->width / (2.0 * length));
							}

							for(int index1 = 0; index1 < 8; index1 += 2) {
								if (minX == maxX) {
									_renderTextureCoords[renderIndex*8+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*8+index1] = (_renderVertexCoords[renderIndex*8+index1] - minX) / (maxX - minX);
								}
								if (minY == maxY) {
									_renderTextureCoords[renderIndex*8+index1] = 0.0;
								} else {
									_renderTextureCoords[renderIndex*8+index1+1] = (_renderVertexCoords[renderIndex*8+index1+1] - minY) / (maxY - minY);
								}
							}
						}

						lastX = drawX;
						lastY = drawY;
						renderIndex++;
					}
				} else {
					lastX = drawX;
					lastY = drawY;
				}

				drawCount++;

				if (drawAngle < endAngle) {
					drawAngle += angleStep;
					if (drawAngle > endAngle) {
						drawAngle = endAngle;
					}
				} else {
					drawAngle += angleStep;
				}
			} while (drawAngle <= endAngle);
		}
	}

	if (fill) {
		renderDrawTriangles(renderIndex, 3);
	} else {
		renderDrawTriangles(renderIndex, 4);
	}

	if (dashCoords) {
		delete dashCoords;
	}
}

// Draws the text given by the specified string, using this graphics context's current font and color.
void Graphics2D::renderDrawString(int commandCount)
{
	//qDebug()  << "Graphics2D::renderDrawString: " << commandCount << " : " << _drawFloatIndices[commandCount*2+0] << " " << _drawFloatIndices[commandCount*2+1];

	QString *text;
	wchar_t *wtext;
	double x, y;

	text = (QString*)_drawPointers[_drawPointerIndices[commandCount*2+0]+0];
	wtext = new wchar_t[text->size()+1];
	int length = text->toWCharArray(wtext);
	wtext[text->size()] = 0;

	x = _drawFloats[_drawFloatIndices[commandCount*2+0]+0];
	y = _drawFloats[_drawFloatIndices[commandCount*2+0]+1];

	//qDebug()  << "Graphics2D::renderDrawString: x,y: " << x << " : " << y;

    int i, j, c;

    if (!_renderFont) {
        qCritical() << "Graphics2D::renderDrawString: Font must not be null\n";
        return;
    }

    if (!_renderFont->initialized) {
    	qCritical() << "Graphics2D::renderDrawString: Font has not been loaded\n";
        return;
    }

    if (!text || !wtext) {
        return;
    }

    int textLength = wcslen(wtext);


    int charMapIndex;
	int previousCharMapIndex;
    float pen_x = 0.0f;


	// calculate u,v texture bounds
	float minX = 1.0e6, minY = 1.0e6, maxX = -1.0e6, maxY = -1.0e6;

	if (x < minX) {
		minX = x;
	}
	if (x > maxX) {
		maxX = x;
	}

	if (y < minY) {
		minY = y;
	}
	if (y > maxY) {
		maxY = y;
	}

    for(i = 0; i < textLength; ++i) {
		c = (int)wtext[i];

		charMapIndex = 0;
		for(j = 0; j < _renderFont->numberCharacters; j++) {
			if (_renderFont->charMap[j] == c) {
				charMapIndex = j;
				break;
			}
		}

		if (i > 0) {
			previousCharMapIndex = 0;
			for(j = 0; j < _renderFont->numberCharacters; j++) {
				if (_renderFont->charMap[j] == wtext[i-1]) {
					previousCharMapIndex = j;
					break;
				}
			}

			pen_x += _currentFont->kerning[previousCharMapIndex * _currentFont->numberCharacters + charMapIndex];
		}

		double charX = x + pen_x + _renderFont->offsetX[charMapIndex];
		double charY = y + _renderFont->offsetY[charMapIndex];
		double charMaxX = charX + _renderFont->width[charMapIndex];
		double charMaxY = charY + _renderFont->height[charMapIndex];

		if (charX < minX) {
			minX = charX;
		}
		if (charX > maxX) {
			maxX = charX;
		}

		if (charY < minY) {
			minY = charY;
		}
		if (charY > maxY) {
			maxY = charY;
		}

		if (charMaxX < minX) {
			minX = charMaxX;
		}
		if (charMaxX > maxX) {
			maxX = charMaxX;
		}

		if (charMaxY < minY) {
			minY = charMaxY;
		}
		if (charMaxY > maxY) {
			maxY = charMaxY;
		}

		//Assume we are only working with typewriter fonts
		pen_x += _renderFont->advance[charMapIndex];
    }
	//qDebug()  << "Graphics2D::renderDrawString: bounds: " << minX << " : " << maxX << " : " << minY << " : " << maxY;

    pen_x = 0.0f;
    for(i = 0; i < textLength; ++i) {
		c = (int)wtext[i];

		charMapIndex = 0;
		for(j = 0; j < _renderFont->numberCharacters; j++) {
			if (_renderFont->charMap[j] == c) {
				charMapIndex = j;
				break;
			}
		}

		if (i > 0) {
			previousCharMapIndex = 0;
			for(j = 0; j < _renderFont->numberCharacters; j++) {
				if (_renderFont->charMap[j] == wtext[i-1]) {
					previousCharMapIndex = j;
					break;
				}
			}

			pen_x += _currentFont->kerning[previousCharMapIndex * _currentFont->numberCharacters + charMapIndex];
		}

		double charX    = x + pen_x + _renderFont->offsetX[charMapIndex];
		double charY    = y +         _renderFont->offsetY[charMapIndex];
		double charMaxX = charX     + _renderFont->width[charMapIndex];
		double charMaxY = charY     + _renderFont->height[charMapIndex];
		//qDebug()  << "Graphics2D::renderDrawString: char bounds: " << charX << " : " << charY << " : " << charMaxX << " : " << charMaxY;

		_renderVertexCoords[8 * i + 0] = charX;
		_renderVertexCoords[8 * i + 1] = charY;
		_renderVertexCoords[8 * i + 2] = charMaxX;
		_renderVertexCoords[8 * i + 3] = charY;
		_renderVertexCoords[8 * i + 4] = charX;
		_renderVertexCoords[8 * i + 5] = charMaxY;
		_renderVertexCoords[8 * i + 6] = charMaxX;
		_renderVertexCoords[8 * i + 7] = charMaxY;
		//qDebug()  << "Graphics2D::renderDrawString: char coords: " << charX << " : " << charY << " : " << charMaxX << " : " << charY;
		//qDebug()  << "Graphics2D::renderDrawString: char coords: " << charX << " : " << charMaxY << " : " << charMaxX << " : " << charMaxY;

		_renderMaskTextureCoords[8 * i + 0] = _renderFont->texX1[charMapIndex];
		_renderMaskTextureCoords[8 * i + 1] = _renderFont->texY2[charMapIndex];
		_renderMaskTextureCoords[8 * i + 2] = _renderFont->texX2[charMapIndex];
		_renderMaskTextureCoords[8 * i + 3] = _renderFont->texY2[charMapIndex];
		_renderMaskTextureCoords[8 * i + 4] = _renderFont->texX1[charMapIndex];
		_renderMaskTextureCoords[8 * i + 5] = _renderFont->texY1[charMapIndex];
		_renderMaskTextureCoords[8 * i + 6] = _renderFont->texX2[charMapIndex];
		_renderMaskTextureCoords[8 * i + 7] = _renderFont->texY1[charMapIndex];

		if (minX == maxX) {
			_renderTextureCoords[8 * i + 0] = 0.0;
			_renderTextureCoords[8 * i + 2] = 0.0;
			_renderTextureCoords[8 * i + 4] = 0.0;
			_renderTextureCoords[8 * i + 6] = 0.0;
		} else {
			_renderTextureCoords[8 * i + 0] = (charX - minX) / (maxX - minX);
			_renderTextureCoords[8 * i + 2] = (charMaxX - minX) / (maxX - minX);
			_renderTextureCoords[8 * i + 4] = (charX - minX) / (maxX - minX);
			_renderTextureCoords[8 * i + 6] = (charMaxX - minX) / (maxX - minX);
		}
		if (minY == maxY) {
			_renderTextureCoords[8 * i + 1] = 0.0;
			_renderTextureCoords[8 * i + 3] = 0.0;
			_renderTextureCoords[8 * i + 5] = 0.0;
			_renderTextureCoords[8 * i + 7] = 0.0;
		} else {
			_renderTextureCoords[8 * i + 1] = (charY - minY) / (maxY - minY);
			_renderTextureCoords[8 * i + 3] = (charY - minY) / (maxY - minY);
			_renderTextureCoords[8 * i + 5] = (charMaxY - minY) / (maxY - minY);
			_renderTextureCoords[8 * i + 7] = (charMaxY - minY) / (maxY - minY);
		}

		_renderVertexIndices[i * 6 + 0] = 4 * i + 0;
		_renderVertexIndices[i * 6 + 1] = 4 * i + 1;
		_renderVertexIndices[i * 6 + 2] = 4 * i + 2;
		_renderVertexIndices[i * 6 + 3] = 4 * i + 2;
		_renderVertexIndices[i * 6 + 4] = 4 * i + 1;
		_renderVertexIndices[i * 6 + 5] = 4 * i + 3;

		//Assume we are only working with typewriter fonts
		pen_x += _renderFont->advance[charMapIndex];
    }

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

#ifdef GLES1
	glEnable(GL_TEXTURE_2D);

	glColor4f(_renderForegroundColor.red, _renderForegroundColor.green, _renderForegroundColor.blue, _renderForegroundColor.alpha);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, _renderVertexCoords);
	glTexCoordPointer(2, GL_FLOAT, 0, _renderMaskTextureCoords);
	glBindTexture(GL_TEXTURE_2D, _renderFont->fontTexture);

	glDrawElements(GL_TRIANGLES, 6 * textLength, GL_UNSIGNED_INT, _renderVertexIndices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);

#elif defined GLES2

	if (_renderGradient) {
		//Render text
		glUseProgram(_textGradientRenderingProgram);

		// Store the locations of the shader variables we need later
		GLint positionLoc = glGetAttribLocation(_textGradientRenderingProgram, "a_position");
		GLint maskTextcoordLoc = glGetAttribLocation(_textGradientRenderingProgram, "a_maskTexcoord");
		GLint texcoordLoc = glGetAttribLocation(_textGradientRenderingProgram, "a_texcoord");
		GLint textureLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_texture");
		GLint colorLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_color");
		GLint pmLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_projectionMatrix");
		GLint mvmLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_modelViewMatrix");

	    GLint segmentsLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_segments");
		GLint colorsLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_colors");
		GLint percentagesLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_percentages");
		GLint radiusLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_radius");
		GLint angleLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_angle");
		GLint originLoc = glGetUniformLocation(_textGradientRenderingProgram, "u_origin");

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _renderFont->fontTexture);
		glUniform1i(textureLoc, 0);

		glUniformMatrix4fv(pmLoc, 1, GL_FALSE, _orthoMatrix);
		glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, _renderModelMatrix);
		glUniform4f(colorLoc, _renderForegroundColor.red, _renderForegroundColor.green, _renderForegroundColor.blue, _renderForegroundColor.alpha);
		glUniform1i(segmentsLoc, _renderGradient->segments);

		GLfloat gradientColors[400];
		for(int index = 0; index < _renderGradient->segments+1; index++) {
			gradientColors[index*4+0] = _renderGradient->colors[index].red;
			gradientColors[index*4+1] = _renderGradient->colors[index].green;
			gradientColors[index*4+2] = _renderGradient->colors[index].blue;
			gradientColors[index*4+3] = _renderGradient->colors[index].alpha;
		}
		glUniform4fv(colorsLoc, _renderGradient->segments+1, gradientColors);
		glUniform1fv(percentagesLoc, _renderGradient->segments*2+1, _renderGradient->percentages);
		glUniform1f(radiusLoc, _renderGradient->radius);
		glUniform1f(angleLoc, _renderGradient->angle);
		glUniform2f(originLoc, _renderGradient->originU, _renderGradient->originV);


		glEnableVertexAttribArray(positionLoc);
		glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderVertexCoords);

		glEnableVertexAttribArray(maskTextcoordLoc);
		glVertexAttribPointer(maskTextcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderMaskTextureCoords);

		glEnableVertexAttribArray(texcoordLoc);
		glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderTextureCoords);

		   //Draw the string
		glDrawElements(GL_TRIANGLES, 6 * textLength, GL_UNSIGNED_INT, _renderVertexIndices);

		glDisableVertexAttribArray(texcoordLoc);
		glDisableVertexAttribArray(maskTextcoordLoc);
		glDisableVertexAttribArray(positionLoc);

	} else {
		//Render text
		glUseProgram(_textRenderingProgram);

		// Store the locations of the shader variables we need later
		GLint positionLoc = glGetAttribLocation(_textRenderingProgram, "a_position");
		GLint texcoordLoc = glGetAttribLocation(_textRenderingProgram, "a_texcoord");
		GLint textureLoc = glGetUniformLocation(_textRenderingProgram, "u_texture");
		GLint colorLoc = glGetUniformLocation(_textRenderingProgram, "u_color");
		GLint pmLoc = glGetUniformLocation(_textRenderingProgram, "u_projectionMatrix");
		GLint mvmLoc = glGetUniformLocation(_textRenderingProgram, "u_modelViewMatrix");


		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _renderFont->fontTexture);
		glUniform1i(textureLoc, 0);

		glUniformMatrix4fv(pmLoc, 1, GL_FALSE, _orthoMatrix);
		glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, _renderModelMatrix);
		glUniform4f(colorLoc, _renderForegroundColor.red, _renderForegroundColor.green, _renderForegroundColor.blue, _renderForegroundColor.alpha);

		glEnableVertexAttribArray(positionLoc);
		glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderVertexCoords);

		glEnableVertexAttribArray(texcoordLoc);
		glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, GL_FALSE, 0, _renderMaskTextureCoords);

		   //Draw the string
		glDrawElements(GL_TRIANGLES, 6 * textLength, GL_UNSIGNED_INT, _renderVertexIndices);

		glDisableVertexAttribArray(texcoordLoc);
		glDisableVertexAttribArray(positionLoc);
	}
#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif
	glDisable(GL_BLEND);
}

// Draws a sequence of connected lines defined by arrays of x and y coordinates.
void Graphics2D::renderFillPolygon(int commandCount)
{
	qDebug()  << "Graphics2D::renderFillPolygon: " << commandCount;

	GLfloat dx, dy, lineLength;

	int nPoints = _drawInts[_drawIntIndices[commandCount*2+0]+0];

	qDebug()  << "Graphics2D::renderFillPolygon: numberPoints: " << nPoints;

	float* xPoints = NULL;
	float* yPoints = NULL;
	float* uPoints = NULL;
	float* vPoints = NULL;

	xPoints = new float[nPoints];
	yPoints = new float[nPoints];
	uPoints = new float[nPoints];
	vPoints = new float[nPoints];

	float x = 0.0, y = 0.0, u, v, minX = 1.0e6, minY = 1.0e6, maxX = -1.0e6, maxY = -1.0e6;

	for(int index = 0; index < nPoints; index++) {
		xPoints[index] = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+0];
		yPoints[index] = _drawFloats[_drawFloatIndices[commandCount*2+0]+index*2+1];
		uPoints[index] = _drawFloats[_drawFloatIndices[commandCount*2+0]+nPoints*2+index*2+0];
		vPoints[index] = _drawFloats[_drawFloatIndices[commandCount*2+0]+nPoints*2+index*2+1];

		x += xPoints[index];
		y += yPoints[index];

		if (xPoints[index] < minX) {
			minX = xPoints[index];
		}
		if (xPoints[index] > maxX) {
			maxX = xPoints[index];
		}

		if (yPoints[index] < minY) {
			minY = yPoints[index];
		}
		if (yPoints[index] > maxY) {
			maxY = yPoints[index];
		}
	}

	x /= nPoints;
	y /= nPoints;

	if (minX == maxX) {
		u = 0.0;
	} else {
		u = (x - minX) / (maxX - minX);
	}
	if (minY == maxY) {
		v = 0.0;
	} else {
		v = (y - minY) / (maxY - minY);
	}

	int renderIndex = 0;

	switch (nPoints) {
	case 3:
		dx = ((GLfloat)xPoints[1] - (GLfloat)xPoints[0]);
		dy = ((GLfloat)yPoints[1] - (GLfloat)yPoints[0]);
		lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
		dx /= lineLength;
		dy /= lineLength;


		qDebug()  << "Graphics2D::renderFillPolygon: dx/dy: " << dx << " " << dy;

		if (fabs(dy) > fabs(dx)) {
			_renderVertexCoords[0] = (GLfloat)xPoints[1];
			_renderVertexCoords[1] = (GLfloat)yPoints[1];
			_renderVertexCoords[2] = (GLfloat)xPoints[0];
			_renderVertexCoords[3] = (GLfloat)yPoints[0];
			_renderVertexCoords[4] = (GLfloat)xPoints[2];
			_renderVertexCoords[5] = (GLfloat)yPoints[2];

			_renderTextureCoords[0] = (GLfloat)uPoints[1];
			_renderTextureCoords[1] = (GLfloat)vPoints[1];
			_renderTextureCoords[2] = (GLfloat)uPoints[0];
			_renderTextureCoords[3] = (GLfloat)vPoints[0];
			_renderTextureCoords[4] = (GLfloat)uPoints[2];
			_renderTextureCoords[5] = (GLfloat)vPoints[2];
		} else {
			_renderVertexCoords[2] = (GLfloat)xPoints[0];
			_renderVertexCoords[3] = (GLfloat)yPoints[0];
			_renderVertexCoords[0] = (GLfloat)xPoints[1];
			_renderVertexCoords[1] = (GLfloat)yPoints[1];
			_renderVertexCoords[4] = (GLfloat)xPoints[2];
			_renderVertexCoords[5] = (GLfloat)yPoints[2];

			_renderTextureCoords[2] = (GLfloat)uPoints[0];
			_renderTextureCoords[3] = (GLfloat)vPoints[0];
			_renderTextureCoords[0] = (GLfloat)uPoints[1];
			_renderTextureCoords[1] = (GLfloat)vPoints[1];
			_renderTextureCoords[4] = (GLfloat)uPoints[2];
			_renderTextureCoords[5] = (GLfloat)vPoints[2];
		}
		break;

	case 4:
		dx = ((GLfloat)xPoints[1] - (GLfloat)xPoints[0]);
		dy = ((GLfloat)yPoints[2] - (GLfloat)yPoints[1]);
		lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
		dx /= lineLength;
		dy /= lineLength;

		if (fabs(dy) > fabs(dx)) {
			_renderVertexCoords[0] = (GLfloat)xPoints[1];
			_renderVertexCoords[1] = (GLfloat)yPoints[1];
			_renderVertexCoords[2] = (GLfloat)xPoints[0];
			_renderVertexCoords[3] = (GLfloat)yPoints[0];
			_renderVertexCoords[4] = (GLfloat)xPoints[3];
			_renderVertexCoords[5] = (GLfloat)yPoints[3];
			_renderVertexCoords[6] = (GLfloat)xPoints[2];
			_renderVertexCoords[7] = (GLfloat)yPoints[2];

			_renderTextureCoords[0] = (GLfloat)uPoints[1];
			_renderTextureCoords[1] = (GLfloat)vPoints[1];
			_renderTextureCoords[2] = (GLfloat)uPoints[0];
			_renderTextureCoords[3] = (GLfloat)vPoints[0];
			_renderTextureCoords[4] = (GLfloat)uPoints[3];
			_renderTextureCoords[5] = (GLfloat)vPoints[3];
			_renderTextureCoords[6] = (GLfloat)uPoints[2];
			_renderTextureCoords[7] = (GLfloat)vPoints[2];
		} else {
			_renderVertexCoords[2] = (GLfloat)xPoints[0];
			_renderVertexCoords[3] = (GLfloat)yPoints[0];
			_renderVertexCoords[0] = (GLfloat)xPoints[1];
			_renderVertexCoords[1] = (GLfloat)yPoints[1];
			_renderVertexCoords[4] = (GLfloat)xPoints[3];
			_renderVertexCoords[5] = (GLfloat)yPoints[3];
			_renderVertexCoords[6] = (GLfloat)xPoints[2];
			_renderVertexCoords[7] = (GLfloat)yPoints[2];

			_renderTextureCoords[2] = (GLfloat)uPoints[0];
			_renderTextureCoords[3] = (GLfloat)vPoints[0];
			_renderTextureCoords[0] = (GLfloat)uPoints[1];
			_renderTextureCoords[1] = (GLfloat)vPoints[1];
			_renderTextureCoords[4] = (GLfloat)uPoints[3];
			_renderTextureCoords[5] = (GLfloat)vPoints[3];
			_renderTextureCoords[6] = (GLfloat)uPoints[2];
			_renderTextureCoords[7] = (GLfloat)vPoints[2];
		}
		break;

	default:
		for(int index = 0; index < nPoints-1; index ++) {
			dx = ((GLfloat)xPoints[index+1] - (GLfloat)xPoints[index]);
			dy = ((GLfloat)yPoints[index+1] - (GLfloat)yPoints[index]);
			lineLength = sqrt (fabs(dx * dx) + fabs(dy * dy));
			dx /= lineLength;
			dy /= lineLength;

			if (fabs(dy) > fabs(dx)) {
				_renderVertexCoords[renderIndex*6+0] = (GLfloat)xPoints[index+1];
				_renderVertexCoords[renderIndex*6+1] = (GLfloat)yPoints[index+1];
				_renderVertexCoords[renderIndex*6+2] = (GLfloat)xPoints[index];
				_renderVertexCoords[renderIndex*6+3] = (GLfloat)yPoints[index];
				_renderVertexCoords[renderIndex*6+4] = (GLfloat)x;
				_renderVertexCoords[renderIndex*6+5] = (GLfloat)y;

				_renderTextureCoords[renderIndex*6+0] = (GLfloat)uPoints[index+1];
				_renderTextureCoords[renderIndex*6+1] = (GLfloat)vPoints[index+1];
				_renderTextureCoords[renderIndex*6+2] = (GLfloat)uPoints[index];
				_renderTextureCoords[renderIndex*6+3] = (GLfloat)vPoints[index];
				_renderTextureCoords[renderIndex*6+4] = (GLfloat)u;
				_renderTextureCoords[renderIndex*6+5] = (GLfloat)v;
			} else {
				_renderVertexCoords[renderIndex*6+2] = (GLfloat)xPoints[index];
				_renderVertexCoords[renderIndex*6+3] = (GLfloat)vPoints[index];
				_renderVertexCoords[renderIndex*6+0] = (GLfloat)xPoints[index+1];
				_renderVertexCoords[renderIndex*6+1] = (GLfloat)vPoints[index+1];
				_renderVertexCoords[renderIndex*6+4] = (GLfloat)x;
				_renderVertexCoords[renderIndex*6+5] = (GLfloat)v;

				_renderTextureCoords[renderIndex*6+2] = (GLfloat)uPoints[index];
				_renderTextureCoords[renderIndex*6+3] = (GLfloat)vPoints[index];
				_renderTextureCoords[renderIndex*6+0] = (GLfloat)uPoints[index+1];
				_renderTextureCoords[renderIndex*6+1] = (GLfloat)vPoints[index+1];
				_renderTextureCoords[renderIndex*6+4] = (GLfloat)u;
				_renderTextureCoords[renderIndex*6+5] = (GLfloat)v;
			}

			renderIndex++;

			if ((renderIndex*6 + 6) > MAX_VERTEX_COORDINATES) {
				renderDrawTriangles(renderIndex, 3);

				renderIndex = 0;
			}

		}
		break;
	}

	switch (nPoints) {
	case 3:

		qDebug()  << "Graphics2D::renderFillPolygon: render for 3: ";
		renderDrawTriangles(1, 3);
		break;

	case 4:
		renderDrawTriangles(1, 4);
		break;

	default:
		renderDrawTriangles(renderIndex, 3);
		break;
	}

	if (xPoints) {
		delete xPoints;
	}
	if (yPoints) {
		delete yPoints;
	}
}


	}
}

