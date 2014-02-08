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
#include <pthread.h>

#include "Graphics.hpp"
#include "View.hpp"

#include <QDebug>

using namespace bb::cascades;
using namespace views::base;

namespace views {
	namespace graphics {

QMutex     Graphics::_renderMutex;
bool       Graphics::_eglInitialized = false;
EGLConfig  Graphics::_eglConfig;
EGLDisplay Graphics::_eglDeviceDisplay;
EGLDisplay Graphics::_eglHDMIDisplay;

Graphics::Graphics(int display, Graphics *master = NULL) : _width(0), _height(0)
{
	qDebug()  << "Graphics: Graphics ";

	if (master) {
		_master = master;
	} else {
		_master = this;
	}

	_eglDisplay = getDisplay(display);

	_renderedImage = NULL;
}

Graphics::~Graphics() {
    if (_eglDisplay != EGL_NO_DISPLAY) {
    	releaseGLContext();
		if (_eglContext != EGL_NO_CONTEXT) {
			eglDestroyContext(_eglDisplay, _eglContext);
			_eglContext = EGL_NO_CONTEXT;
		}
		if (_eglSurface != EGL_NO_SURFACE) {
			eglDestroySurface(_eglDisplay, _eglSurface);
			_eglSurface = EGL_NO_SURFACE;
		}
    }
}

int Graphics::initialize(screen_window_t screenWindow)
{
    EGLBoolean status;

#ifdef GLES1
    _eglContext = eglCreateContext(_eglDisplay, _eglConfig, EGL_NO_CONTEXT, NULL);
#elif defined(GLES2)
    EGLint attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    _eglContext = eglCreateContext(_eglDisplay, _eglConfig, EGL_NO_CONTEXT, attributes);
#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif

    if (_eglContext == EGL_NO_CONTEXT) {
		eglPrintError("eglCreateContext");
        return EXIT_FAILURE;
    }

	qDebug()  << "Graphics::initialize:eglCreateContext "<< _eglContext << ":" << _eglDisplay << ":" << _eglConfig << ":" << screenWindow;
	_eglSurface = eglCreateWindowSurface(_eglDisplay, _eglConfig, screenWindow, NULL);
	if (_eglSurface == EGL_NO_SURFACE) {
		eglPrintError("eglCreateWindowSurface");
		return EXIT_FAILURE;
	}

	getGLContext();

    EGLint interval = 1;
    status = eglSwapInterval(_eglDisplay, interval);
	if (status != EGL_TRUE) {
		eglPrintError("eglSwapInterval");
		return EXIT_FAILURE;
	}

    status = eglQuerySurface(_eglDisplay, _eglSurface, EGL_WIDTH, &_surfaceWidth);
	if (status != EGL_TRUE) {
		perror("query surface width");
		return EXIT_FAILURE;
	}

    status = eglQuerySurface(_eglDisplay, _eglSurface, EGL_HEIGHT, &_surfaceHeight);
	if (status != EGL_TRUE) {
		perror("query surface height");
		return EXIT_FAILURE;
	}

	qDebug()  << "Graphics::initialize: " << screenWindow << ":"<< _eglContext << ":" << _eglDisplay << ":" << _eglSurface << ":" << _surfaceWidth << ":" << _surfaceHeight;

	return EXIT_SUCCESS;
}

int Graphics::initializeEGL() {
    int returnCode;
    int numConfigs;

	qDebug()  << "Graphics::initializeEGL ";

    EGLint attribList[]= { EGL_RED_SIZE,        8,
                           EGL_GREEN_SIZE,      8,
                           EGL_BLUE_SIZE,       8,
                           EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
                           EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
                           EGL_NONE};

    _eglDeviceDisplay = EGL_NO_DISPLAY;
    _eglHDMIDisplay = EGL_NO_DISPLAY;

    _eglDeviceDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (_eglDeviceDisplay == EGL_NO_DISPLAY) {
        eglPrintError("eglGetDisplay");
        return EXIT_FAILURE;
    }

    returnCode = eglInitialize(_eglDeviceDisplay, NULL, NULL);
    if (returnCode != EGL_TRUE) {
        eglPrintError("eglInitialize");
        return EXIT_FAILURE;
    }

	returnCode = eglBindAPI(EGL_OPENGL_ES_API);

    if (returnCode != EGL_TRUE) {
        eglPrintError("eglBindAPI");
        return EXIT_FAILURE;
    }


#ifdef GLES1
    attribList[9] = EGL_OPENGL_ES_BIT;
#elif defined(GLES2)
    attribList[9] = EGL_OPENGL_ES2_BIT;
#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif

    if(!eglChooseConfig(_eglDeviceDisplay, attribList, &_eglConfig, 1, &numConfigs)) {
        perror("eglChooseConfig");
        return EXIT_FAILURE;
    }

	_eglInitialized = true;

    return EXIT_SUCCESS;
}

void Graphics::cleanupEGL() {
	qDebug()  << "Graphics::cleanupEGL ";

    if (_eglDeviceDisplay != EGL_NO_DISPLAY) {
        eglTerminate(_eglDeviceDisplay);
        _eglDeviceDisplay = EGL_NO_DISPLAY;
    }

    if (_eglHDMIDisplay != EGL_NO_DISPLAY) {
        eglTerminate(_eglHDMIDisplay);
        _eglHDMIDisplay = EGL_NO_DISPLAY;
    }

    eglReleaseThread();


	_renderMutex.lock();

    _eglInitialized = false;

	_renderMutex.unlock();
}

bool Graphics::eglInitialized()

{
	bool initialized = false;

	_renderMutex.lock();

	initialized = _eglInitialized;

	_renderMutex.unlock();

	return initialized;
}

/*
void Graphics::setEGLInitialized(bool initialized)
{
	_renderMutex.lock();

	_egl_initialized = initialized;

	_renderMutex.unlock();
}
*/

EGLDisplay Graphics::getDisplay(int display)
{
	EGLDisplay eglDisplay = EGL_NO_DISPLAY;

	switch (display) {
		case DISPLAY_DEVICE:
			eglDisplay = _eglDeviceDisplay;
		break;

		case DISPLAY_HDMI:
			// HDMI display also uses the default EGL display
			//eglDisplay = _eglHDMIDisplay;
			eglDisplay = _eglDeviceDisplay;
		break;

		case DISPLAY_UNKNOWN:
		break;
	}

	return eglDisplay;
}

/*
bool ViewsThread::isDisplayAttached(ViewDisplay display)
{
	EGLDisplay egl_display = EGL_NO_DISPLAY;

	while (!eglInitialized()) {
		usleep(1);
	};

	switch (display) {
		case DISPLAY_DEVICE:
			egl_display = _egl_disp;
		break;

		case DISPLAY_HDMI:
			egl_display = _egl_disp_hdmi;
		break;

		case DISPLAY_UNKNOWN:
		break;
	}

	return false;
}
*/

int Graphics::regenerateCleanup()
{
	EGLBoolean status;

    status = eglQuerySurface(_eglDisplay, _eglSurface, EGL_WIDTH, &_surfaceWidth);
	if (status != EGL_TRUE) {
		perror("query surface width");
		return EXIT_FAILURE;
	}

    status = eglQuerySurface(_eglDisplay, _eglSurface, EGL_HEIGHT, &_surfaceHeight);
	if (status != EGL_TRUE) {
		perror("query surface height");
		return EXIT_FAILURE;
	}

	status = eglMakeCurrent(_eglDisplay, NULL, NULL, NULL);
	if (status != EGL_TRUE) {
		eglPrintError("eglMakeCurrent");
		return EXIT_FAILURE;
	}

	status = eglDestroySurface(_eglDisplay, _eglSurface);
	if (status != EGL_TRUE) {
		eglPrintError("eglMakeCurrent");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int Graphics::regenerate(screen_window_t screenWindow)
{
	EGLBoolean status;
	EGLint interval = 1;

	_eglSurface = eglCreateWindowSurface(_eglDisplay, _eglConfig, screenWindow, NULL);
	if (_eglSurface == EGL_NO_SURFACE) {
		eglPrintError("eglCreateWindowSurface");
		return EXIT_FAILURE;
	}

	getGLContext();

    status = eglSwapInterval(_eglDisplay, interval);
	if (status != EGL_TRUE) {
		eglPrintError("eglSwapInterval");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void Graphics::lockRendering()
{
	_renderMutex.lock();
}

void Graphics::unlockRendering()
{
	_renderMutex.unlock();
}

void Graphics::renderSafe(bool saveRender)
{
	if (saveRender) {
		if (_renderedImage) {
			delete _renderedImage;
		}

		_renderedImage = new ImageData(bb::PixelFormat::RGBA_Premultiplied, _captureWidth, _captureHeight);
	}

	lockRendering();

	getGLContext();

	render();

	if (saveRender) {
		glReadPixels(_captureX, _captureY, _captureWidth, _captureHeight,  GL_RGBA,  GL_UNSIGNED_BYTE,  _renderedImage->pixels());
	}

	swapBuffers();

	unlockRendering();
}

int Graphics::setCaptureRect(int x, int y, int width, int height)
{
	_captureX = x;
	_captureY = y;
	_captureWidth = width;
	_captureHeight = height;
}

/* Finds the next power of 2 */
int Graphics::nextp2(int x)
{
    int val = 1;
    while(val < x) val <<= 1;
    return val;
}

void Graphics::setSize(int width, int height)
{
	qDebug()  << "Graphics: setSize : " << width << "x" << height;

	_width = width;
	_height = height;
}

int Graphics::getNativeWindowUsage()
{
	int usage;

#ifdef GLES1
    usage = SCREEN_USAGE_OPENGL_ES1 | SCREEN_USAGE_ROTATION;
#elif defined(GLES3)
    usage = SCREEN_USAGE_OPENGL_ES3 | SCREEN_USAGE_ROTATION;
#elif defined(GLES2)
    usage = SCREEN_USAGE_OPENGL_ES2 | SCREEN_USAGE_ROTATION;
#else
#error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif

	return usage;
}

void Graphics::getGLContext()
{
	EGLBoolean status;

	if (_eglContext != eglGetCurrentContext()) {
		status = eglMakeCurrent(_eglDisplay, _eglSurface, _eglSurface, _eglContext);
		if (status != EGL_TRUE) {
			eglPrintError("getGLContext (eglMakeCurrent)");
		}
	}
}

void Graphics::releaseGLContext()
{
	EGLBoolean status;

	if (_eglContext != NULL) {
		status = eglMakeCurrent(_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (status != EGL_TRUE) {
			eglPrintError("releaseGLContext (eglMakeCurrent)");
		}
	}
}


ImageData* Graphics::loadImage(const QString& filename)
{
    ImageData* image = loadFullImage(filename);
    ImageData* adjustImage = NULL;

    if (image->isValid()) {
        qDebug() << "Graphics::loadImage: " << image->width() << ":" << image->height() << ":" << image->bytesPerLine();

    	adjustImage = getAdjustedImage(image);

    	qDebug() << "Graphics::loadImage: adjusted: " << adjustImage->width() << ":" << adjustImage->height() << ":" << adjustImage->bytesPerLine();

    	delete image;
    }

    return adjustImage;
}

int Graphics::saveImage(const ImageData* image, const QString& filename)
{
	int returnCode = EXIT_FAILURE;

    if (image->isValid()) {
        qDebug() << "Graphics::saveImage: " << image->width() << ":" << image->height() << ":" << image->bytesPerLine();

        QString imageFilePath = (new QDir())->absoluteFilePath(filename);

        if (ImageConverter::encode(QUrl::fromLocalFile(imageFilePath), *image, 100)) {
            returnCode = EXIT_SUCCESS;
        }
    }

    return returnCode;
}

ImageData* Graphics::loadFullImage(const QString& filename)
{
    QString imageFilePath = (new QDir())->absoluteFilePath(filename);

    ImageData image = ImageConverter::decode(QUrl::fromLocalFile(imageFilePath));

    //qDebug() << "Graphics::loadFullImage: Image attributes: " << imageFilePath << ":" << image.width() << ":" << image.height() << ":" << image.format();

    return new ImageData(image);
}

void Graphics::sampleImageBuffer(ImageData* image, float x, float y, float* rgba)
{
	if (x >= 0.0 && y >= 0.0 && x <= image->width() && y <= image->height()) {
		int xMin = (int)fabs(x);
		int yMin = (int)fabs(y);
		int xMax;
		int yMax;

		if (xMin >= (image->width()-1)) {
			xMin = image->width()-1;
			xMax = xMin;
		} else {
			xMax = xMin + 1;
		}
		if (yMin >= (image->height()-1)) {
			yMin = image->height()-1;
			yMax = yMin;
		} else {
			yMax = yMin + 1;
		}

		float xStep = (x - xMin);
		float yStep = (y - yMin);

		float cornersRGBA[4][4];

		cornersRGBA[0][0] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMin*4 + 0));
		cornersRGBA[0][1] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMin*4 + 1));
		cornersRGBA[0][2] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMin*4 + 2));
		cornersRGBA[0][3] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMin*4 + 3));

		cornersRGBA[1][0] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMax*4 + 0));
		cornersRGBA[1][1] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMax*4 + 1));
		cornersRGBA[1][2] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMax*4 + 2));
		cornersRGBA[1][3] = (float)(*(image->pixels() + yMin*image->bytesPerLine() + xMax*4 + 3));

		cornersRGBA[2][0] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMin*4 + 0));
		cornersRGBA[2][1] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMin*4 + 1));
		cornersRGBA[2][2] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMin*4 + 2));
		cornersRGBA[2][3] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMin*4 + 3));

		cornersRGBA[3][0] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMax*4 + 0));
		cornersRGBA[3][1] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMax*4 + 1));
		cornersRGBA[3][2] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMax*4 + 2));
		cornersRGBA[3][3] = (float)(*(image->pixels() + yMax*image->bytesPerLine() + xMax*4 + 3));

		float xStepActual = xStep + ((0.01 * (float)rand() / (float)RAND_MAX) - 0.005);
		float yStepActual = yStep + ((0.01 * (float)rand() / (float)RAND_MAX) - 0.005);
		if (xStepActual <= 0.0) {
			xStepActual = 0.0;
		}
		if (xStepActual >= 1.0) {
			xStepActual = 1.0;
		}
		if (yStepActual <= 0.0) {
			yStepActual = 0.0;
		}
		if (yStepActual >= 1.0) {
			yStepActual = 1.0;
		}
		//float xStepActual = 0.5 + 0.7 * (xStep - 0.5) + 0.3 * (yStep - 0.5);
		//float yStepActual = 0.5 - 0.3 * (xStep - 0.5) + 0.7 * (yStep - 0.5);

		rgba[0] = (cornersRGBA[0][0] * (1.0 - xStepActual) * (1.0 -yStepActual)) + (cornersRGBA[1][0] * xStepActual * (1.0 -yStepActual))
				+ (cornersRGBA[2][0] * (1.0 - xStepActual) * yStepActual)        + (cornersRGBA[3][0] * xStepActual * yStepActual);
		rgba[1] = (cornersRGBA[0][1] * (1.0 - xStepActual) * (1.0 -yStepActual)) + (cornersRGBA[1][1] * xStepActual * (1.0 -yStepActual))
				+ (cornersRGBA[2][1] * (1.0 - xStepActual) * yStepActual)        + (cornersRGBA[3][1] * xStepActual * yStepActual);
		rgba[2] = (cornersRGBA[0][2] * (1.0 - xStepActual) * (1.0 -yStepActual)) + (cornersRGBA[1][2] * xStepActual * (1.0 -yStepActual))
				+ (cornersRGBA[2][2] * (1.0 - xStepActual) * yStepActual)        + (cornersRGBA[3][2] * xStepActual * yStepActual);
		rgba[3] = (cornersRGBA[0][3] * (1.0 - xStepActual) * (1.0 -yStepActual)) + (cornersRGBA[1][3] * xStepActual * (1.0 -yStepActual))
				+ (cornersRGBA[2][3] * (1.0 - xStepActual) * yStepActual)        + (cornersRGBA[3][3] * xStepActual * yStepActual);

	}
}

ImageData* Graphics::getAdjustedImage(ImageData *image)
{
	//variables to pass to get info
	int imageWidth, imageHeight;
	PixelFormat::Type imageFormat;

	imageWidth = image->width();
	imageHeight = image->height();
	imageFormat = image->format();

	// photos need to be mapped to a (float)_widthx((float)_height / 2.0) (or for width x half height for other resolutions) area of the screen so calculate scaling factor to fill this area
	float imageScale = 1.0;
	float adjustX, adjustY, adjustWidth, adjustHeight;

	adjustWidth = imageWidth;
	adjustHeight = imageHeight;
	adjustX = 0.0;
	adjustY = 0.0;

	// TODO: fix this later
	if (adjustHeight > 2000) {
		adjustHeight = (float)2000;
		imageScale = (float)adjustHeight / imageHeight;
		adjustWidth = (float)((int)((float)imageWidth * imageScale));
	}
	if (adjustWidth > 2000) {
		adjustX = (adjustWidth - (float)2000) / 2.0;
		adjustWidth =(float)2000;
	}
/*
	if (adjustWidth < _width) {
		adjustWidth = (float)_width;
		imageScale = (float)adjustWidth / imageWidth;
		adjustHeight = (float)((int)((float)imageHeight * imageScale));
		adjustX = 0.0;
		adjustY = (adjustHeight - (float)_height) / 2.0;
		adjustHeight = (float)_height;
	}
*/
	qDebug() << "Graphics::getAdjustedImage: adjust Image attributes: " << ":" << adjustX << ":" << adjustY << ":" << adjustWidth << ":" << adjustHeight;
	qDebug() << "Graphics::getAdjustedImage: adjust Image attributes: " << ":" << imageScale << ":" << (adjustWidth*adjustHeight);

	ImageData* adjustImage = NULL;
	if ((adjustY != 0 || adjustX != 0) || (adjustWidth != imageWidth || adjustHeight != imageHeight))
	{
	   adjustImage = new ImageData(bb::PixelFormat::RGBA_Premultiplied, adjustWidth, adjustHeight);

		unsigned char *dstLine = adjustImage->pixels();

		float rgba[4];
		int xi, yi;

		//qDebug() << "Graphics::getAdjustedImage: sample X range: " << ":" << (adjustX + (0.0 / imageScale)) << ":" << (adjustX + ((adjustImage->width()-1) / imageScale));
		//qDebug() << "Graphics::getAdjustedImage: sample Y range: " << ":" << (adjustY + (0.0 / imageScale)) << ":" << (adjustY + ((adjustImage->height()-1) / imageScale));

		for (yi = 0; yi < adjustImage->height(); yi++) {
			unsigned char * dst = dstLine;

			for (xi = 0; xi < adjustImage->width(); xi++) {
				// for sampling not implemented yet, produce a transparent image
				rgba[0] = 0.0f;
				rgba[1] = 0.0f;
				rgba[2] = 0.0f;
				rgba[3] = 0.0f;

				float sampleX = (adjustX + (float)xi) / imageScale;
				float sampleY = (adjustY + (float)yi) / imageScale;

				sampleImageBuffer(image, sampleX, sampleY, rgba);

				//qDebug() << "Graphics::getAdjustedImage: sample: " << ":" << sampleX << ":" << sampleY << ": rgb: " << rgba[0] << " " << rgba[1] << " " << rgba[2] << " " << rgba[3];

				if (imageFormat == PixelFormat::RGBX) {
					rgba[3] = 255.0f;
				}

				*dst++ = (char)(rgba[0]);
				*dst++ = (char)(rgba[1]);
				*dst++ = (char)(rgba[2]);
				*dst++ = (char)(rgba[2]);
			}

			dstLine += adjustImage->bytesPerLine();
		}
	} else {
		adjustImage = new ImageData(*image);
	}

    return adjustImage;
}

ImageData* Graphics::getRenderedImage()
{
	ImageData* renderedImage;

	lockRendering();

	renderedImage = _renderedImage;

	unlockRendering();

	return renderedImage;
}

int Graphics::createTexture2D(ImageData* image, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex)
{
    GLuint format;

    if (!tex) {
        return EXIT_FAILURE;
    }

	if (image) {
		//variables to pass to get info
		int adjustWidth, adjustHeight;
		PixelFormat::Type adjustFormat;
		int texWidth, texHeight;

		adjustWidth = image->width();
		adjustHeight = image->height();
		adjustFormat = image->format();

    	switch (adjustFormat)
    	{
    		case PixelFormat::RGBA_Premultiplied:
    			format = GL_RGBA;
    			break;
    		case PixelFormat::RGBX:
    			format = GL_RGBA;
    			break;
    		default:
    			qCritical() << "Graphics::createTexture2D: Unsupported format (" << (int)image->format() << ") for image: " << image;
    			return EXIT_FAILURE;
    	}

		texWidth = nextp2(adjustWidth);
		texHeight = nextp2(adjustHeight);

		getGLContext();

		bool first = false;
		if ((*tex) == 0) {
			glGenTextures(1, tex);
			first = true;
			//qDebug() << "Graphics::createTexture2D: glGenTextures: " << tex << *tex;
		}
		glBindTexture(GL_TEXTURE_2D, (*tex));
		//qDebug() << "Graphics::createTexture2D: glBindTexture: " << tex << *tex;

		if (first) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, adjustWidth, adjustHeight, format, GL_UNSIGNED_BYTE, image->pixels());

		GLint err = glGetError();
	   if (err == 0) {
			//Return physical with and height of texture if pointers are not null
			if(width) {
				*width = adjustWidth;
			}
			if (height) {
				*height = adjustHeight;
			}
			//Return modified texture coordinates if pointers are not null
			if(tex_x) {
				*tex_x = ((float) adjustWidth - 0.5f) / ((float)texWidth);
			}
			if(tex_y) {
				*tex_y = ((float) adjustHeight - 0.5f) / ((float)texHeight);
			}

			qDebug() << "Graphics::createTexture2D: result: " << width << ":" << height << ":" << *tex_x << ":" << *tex_y;

		} else {
			qDebug() << "GL error " << err << "\n";
			return EXIT_FAILURE;
		}
	} else {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


#ifdef GLES2
GLuint Graphics::loadShader(const char* vSource, const char* fSource)
{
	GLint status;

	// Compile the vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	if (!vs) {
		qCritical() << "Failed to create vertex shader: " << glGetError();
		return 0;
	} else {
		glShaderSource(vs, 1, &vSource, 0);
		glCompileShader(vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		GLchar log[1024];
		glGetShaderInfoLog(vs, 256, NULL, log);
		qDebug() << "link vertex shader results: " << vSource << "\n:" << log;
		if (GL_FALSE == status) {

			qCritical() << "Failed to compile vertex shader: " << log;

			glDeleteShader(vs);

			return 0;
		}
	}

	// Compile the fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	if (!fs) {
		qCritical() << "Failed to create fragment shader: " << glGetError();
		return 0;
	} else {
		glShaderSource(fs, 1, &fSource, 0);
		glCompileShader(fs);
		glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
		GLchar log[1024];
		glGetShaderInfoLog(fs, 256, NULL, log);
		qDebug() << "link fragment shader results: " << fSource << "\n:" << log;
		if (GL_FALSE == status) {

			qCritical() << "Failed to compile fragment shader: " << log;

			glDeleteShader(vs);
			glDeleteShader(fs);

			return 0;
		}
	}

	// Create and link the program
	GLuint program = glCreateProgram();
	if (program)
	{
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &status);
		GLchar log[1024];
		glGetProgramInfoLog(program, 256, NULL, log);
		qDebug() << "link program results: " << program << "\n:" << log;

		if (status == GL_FALSE)    {
			qCritical() << "Failed to link shader program: "<< log;

			glDeleteProgram(program);
			program = 0;
		}
	} else {
		qCritical() << "Failed to create a shader program\n";

		glDeleteShader(vs);
		glDeleteShader(fs);
		return 0;
	}

	// We don't need the shaders anymore - the program is enough
	glDeleteShader(fs);
	glDeleteShader(vs);

	return program;
}
#endif



void Graphics::swapBuffers()
{
	EGLBoolean status;

    status = eglSwapBuffers(_eglDisplay, _eglSurface);
    if (status != EGL_TRUE) {
        eglPrintError("eglSwapBuffers");
    }
}

void Graphics::eglPrintError(const char *msg) {
    static const char *errmsg[] = {
        "function succeeded",
        "EGL is not initialized, or could not be initialized, for the specified display",
        "cannot access a requested resource",
        "failed to allocate resources for the requested operation",
        "an unrecognized attribute or attribute value was passed in an attribute list",
        "an EGLConfig argument does not name a valid EGLConfig",
        "an EGLContext argument does not name a valid EGLContext",
        "the current surface of the calling thread is no longer valid",
        "an EGLDisplay argument does not name a valid EGLDisplay",
        "arguments are inconsistent",
        "an EGLNativePixmapType argument does not refer to a valid native pixmap",
        "an EGLNativeWindowType argument does not refer to a valid native window",
        "one or more argument values are invalid",
        "an EGLSurface argument does not name a valid surface configured for rendering",
        "a power management event has occurred",
    };

    qCritical() << msg << ": " << errmsg[eglGetError() - EGL_SUCCESS] << "\n";
}

	}
}
