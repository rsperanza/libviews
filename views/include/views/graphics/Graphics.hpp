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


#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <screen/screen.h>

#include <EGL/egl.h>
#ifdef GLES1
#include <GLES/gl.h>
#include <GLES/glext.h>
#elif defined(GLES2)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif defined(GLES3)
#include <GLES2/gl3.h>
#include <GLES2/gl2ext.h>
#else
error libviews should be compiled with either GLES1 or GLES2 -D flags.
#endif

#include <QObject>

#include <bb/ImageData>
#include <bb/PixelFormat>
#include <bb/utility/ImageConverter>

using namespace bb;
using namespace bb::utility;

namespace views {
	namespace graphics {

class Q_DECL_EXPORT Graphics : public QObject {

Q_OBJECT

	friend class View;

public:
	Graphics(int display, Graphics *master);

	virtual ~Graphics();

	virtual int initialize(screen_window_t screenWindow);

	virtual void render() = 0;
	virtual void clear();
	virtual void renderSafe(bool saveRender);

	void setSize(int width, int height);

	int regenerateCleanup();
	int regenerate(screen_window_t screenWindow);


	// image utility functions
	ImageData* loadImage(const QString& filename);
	ImageData* loadFullImage(const QString& filename);
	void sampleImageBuffer(ImageData* image, float x, float y, float* rgba);
	ImageData* getAdjustedImage(ImageData *image);

	ImageData* getRenderedImage();
	int setCaptureRect(int x, int y, int width, int height);
	int saveImage(const ImageData* image, const QString& filename);


    // create a 2D texture from image data
	int createTexture2D(ImageData* image, int* width, int* height, float* tex_x, float* tex_y, unsigned int *tex);

#ifdef GLES2
	GLuint loadShader(const char* vSource, const char* fSource);
#endif

	//  utility functions for GL rendering - derived from bb_util.c
	/**
	 * Swaps default window surface to the screen
	 */
	void swapBuffers();

	/*
	 * makes this view's EGL context curent
	 */
	void getGLContext();

	/*
	 * releases the view's EGL context from being current
	 */
	void  releaseGLContext();



	static int getNativeWindowUsage();

	static int initializeEGL();
	static void cleanupEGL();

	static bool eglInitialized();

	static void lockRendering();
	static void unlockRendering();

	static EGLDisplay getDisplay(int display);

	// handy print error function derived from bb_util.c
	static void eglPrintError(const char *msg);

protected:
	int nextp2(int x);

	Graphics* _master;

	ImageData* _renderedImage;
	int _captureX;
	int _captureY;
	int _captureWidth;
	int _captureHeight;

	int _width;
	int _height;


	bool _initialized;
	static bool _eglInitialized;

    int _numberDisplays;
    screen_display_t *_screen_dpy;

	// EGL parameters
	EGLSurface _eglSurface;
	EGLint _surfaceWidth;
	EGLint _surfaceHeight;

	EGLContext _eglContext;
	EGLDisplay _eglDisplay;
	static EGLConfig  _eglConfig;

	static EGLDisplay _eglDeviceDisplay;
	static EGLDisplay _eglHDMIDisplay;

	// mutex for controlling render access across all views
	static QMutex _renderMutex;
};

	}
}

#endif /* GRAPHICS_HPP */
