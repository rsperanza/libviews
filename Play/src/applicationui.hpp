// Default empty project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QRectF>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeRequest>


#include "views/Views.hpp"
//#include "views/stereoscopic/Photo3DView.hpp"
#include "MouseTest.hpp"
#include "SketchPad.hpp"
#include "Test2D.hpp"
#include "VideoOverlay.hpp"

#include "SignaturePad.hpp"

namespace bb { namespace cascades { class Application; }}

using namespace bb::system;

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI : public QObject
{
    Q_OBJECT
public:
    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {};

public Q_SLOTS:
	void onInvisible();
	void onFullscreen();
	void onThumbnail();
	void onAboutToQuit();

#ifdef BBNDK_VERSION_AT_LEAST(10,2,0)
	void miracastShowOn();
	void miracastPlayOn();
#endif
	QString fileURL(QString filePath);

private:
	//NavigationPane  *m_navPane;
/*
	Photo3DView *_photo3DView;
	Photo3DView *_photo3DView_HDMI;
*/
	View *_baseView_HDMI;
	VideoOverlay *_videoOverlayView_HDMI;

	PhotoView *_photoView;
	VideoOverlay *_videoOverlayView;
	VideoOverlay *_videoOverlayView2;
	SketchPad *_sketchPad;
	SketchPad *_signPad;
	Test2D *_graphView;
	Test2D *_graphView_HDMI;
	MouseTest *_mouseTest;
	MouseTest *_mouseTest_HDMI;

	InvokeManager _invokeManager;

	SignaturePad* _signaturePad;
	QString _dataPath;
	bb::cascades::Application *_app;
};


#endif /* ApplicationUI_HPP_ */
