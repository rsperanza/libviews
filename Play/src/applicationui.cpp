// Default empty project template

#include <bb/cascades/Application>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Window>

#include "applicationui.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app), _app(app), _invokeManager(new InvokeManager())
{
	// register some new types for QML
    qmlRegisterType < VideoOverlay >      ("com.example.views", 1, 0, "VideoOverlay");

	_dataPath = QDir::current().absoluteFilePath("data");

/*
	//m_pphoto3DView = new Photo3DView(DISPLAY_DEVICE);
	//m_pphoto3DView_HDMI = new Photo3DView(DISPLAY_DEVICE);

	_photoView = new PhotoView(DISPLAY_DEVICE);
	_videoOverlayView = new VideoOverlay(DISPLAY_DEVICE);
	//_videoOverlayView2 = new VideoOverlay(DISPLAY_DEVICE);
	_videoOverlayView2 = new VideoOverlay(DISPLAY_HDMI);
	_sketchPad = new SketchPad();
	_signPad = new SketchPad();
	_graphView = new Test2D(DISPLAY_DEVICE);
	_mouseTest = new MouseTest(DISPLAY_DEVICE);
	_graphView_HDMI = new Test2D(DISPLAY_HDMI);
	_baseView_HDMI = new View(DISPLAY_HDMI);
	_videoOverlayView_HDMI = new VideoOverlay(DISPLAY_HDMI);
	_mouseTest_HDMI = new MouseTest(DISPLAY_HDMI);

	qml->setContextProperty("_photo", _photoView);
	qml->setContextProperty("_video", _videoOverlayView);
	qml->setContextProperty("_graph", _graphView);
	//qml->setContextProperty("_photo_HDMI", m_pphoto3DView_HDMI);


	QString mainWindowGroupId = _app->mainWindow()->groupId();

 	m_pphoto3DView->setWindowGroup(mainWindowGroupId);
	m_pphoto3DView->setWindowID("applicationuiID1");
	m_pphoto3DView->setPosition(0, 0);
	m_pphoto3DView->setSize(1280, 720);
	m_pphoto3DView->setDisplayMode(VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT);
	m_pphoto3DView->setLeftPhotoFilename("app/native/assets/images/DSC_0081.jpg");
	m_pphoto3DView->setRightPhotoFilename("app/native/assets/images/DSC_0080.jpg");
	m_pphoto3DView->setEnabled(true);
	m_pphoto3DView->add();

	//m_pphotoView_HDMI->setWindowGroup(mainWindowGroupId);
	//m_pphotoView_HDMI->setWindowID("applicationuiID1");
	m_pphotoView_HDMI->setPosition(0, 0);
	m_pphotoView_HDMI->setSize(720, 405);
	m_pphotoView_HDMI->setPhotoFilename("app/native/assets/images/DSC_0081.jpg");
	m_pphotoView_HDMI->setEnabled(true);
	m_pphotoView_HDMI->add();

	m_pphoto3DView_HDMI->setPosition(0, 0);
	m_pphoto3DView_HDMI->setSize(1280, 720);
	m_pphoto3DView_HDMI->setDisplayMode(VIEW_3D_TYPE_TOP_BOTTOM_LEFT_RIGHT);
	m_pphoto3DView_HDMI->setLeftPhotoFilename("app/native/assets/images/DSC_0081.jpg");
	m_pphoto3DView_HDMI->setRightPhotoFilename("app/native/assets/images/DSC_0080.jpg");
	m_pphoto3DView_HDMI->setEnabled(true);
	m_pphoto3DView_HDMI->add();

	_graphView->setWindowGroup(mainWindowGroupId);
	_graphView->setWindowID("applicationuiID1");
	_graphView->setZ(-1);
	_graphView->setPosition(0, 0);
	_graphView->setSize(720, 1280);
	_graphView->setEnabled(false);
	//_graphView->add();

	QVariantList backgroundColor;
	backgroundColor << COLOR_WHITE.red << COLOR_WHITE.green << COLOR_WHITE.blue << COLOR_WHITE.alpha;
	QVariantList strokeColor;
	strokeColor << COLOR_BLACK.red << COLOR_BLACK.green << COLOR_BLACK.blue << COLOR_BLACK.alpha;

	_sketchPad->setWindowGroup(mainWindowGroupId);
	_sketchPad->setWindowID("applicationuiID1");
	_sketchPad->setZ(1);
	_sketchPad->setPosition(0, 0);
	_sketchPad->setSize(720, 1280);

	_sketchPad->setBackgroundImage("app/native/assets/images/DSC_0081.jpg");
	_sketchPad->setStrokeColor(strokeColor);
	_sketchPad->setStrokeWidth(10.0);
	_sketchPad->setTool("scroll");
	_sketchPad->setEnabled(false);
	_sketchPad->add();

	_signPad->setWindowGroup(mainWindowGroupId);
	_signPad->setWindowID("applicationuiID2");
	_signPad->setZ(1);
	_signPad->setPosition(0, 0);
	_signPad->setSize(720, 1280);

	_signPad->setBackgroundColor(backgroundColor);
	_signPad->setStrokeColor(strokeColor);
	_signPad->setStrokeWidth(10.0);
	_signPad->setTool("freedraw");
	_signPad->setEnabled(false);
	_signPad->add();

	_mouseTest->setWindowGroup(mainWindowGroupId);
	_mouseTest->setWindowID("applicationuiID8");
	_mouseTest->setZ(1);
	_mouseTest->setPosition(0, 0);
	_mouseTest->setSize(768, 1280);
	_mouseTest->setEnabled(false);
	//_mouseTest->add();

	_videoOverlayView->setWindowGroup(mainWindowGroupId);
	_videoOverlayView->setCreateWindowGroup(true);
	_videoOverlayView->setWindowID("applicationuiID2");
	_videoOverlayView->setZ(3);
	_videoOverlayView->setPosition(0, 0);
	_videoOverlayView->setSize(768, 432);
	QString mediaURL = QString("file:///") + QDir().absoluteFilePath("app/native/assets/media/VID_00000007.mp4");
	//_videoOverlayView->setMediaURL(mediaURL);
	_videoOverlayView->setEnabled(true);
	_videoOverlayView->setAltered(true);
	//_videoOverlayView->add();

	_photoView->setWindowGroup(mainWindowGroupId);
	_photoView->setWindowID("applicationuiID3");
	_photoView->setZ(7);
	_photoView->setPosition(0, 250);
	_photoView->setSize(720, 405);
	//_photoView->setPhotoFilename(QDir().absoluteFilePath("app/native/assets/images/DSC_0081.jpg"));
	_photoView->setPhotoFilename(QDir().absoluteFilePath("app/native/assets/images/IMG_00000005.jpg"));
	//_photoView->setPhotoFilename(QDir().absoluteFilePath("app/native/assets/images/IMG_00000010.jpg"));
	_photoView->setAltered(true);
	//_photoView->add();

	_baseView_HDMI->setWindowGroup("Play-HDMI");
	_baseView_HDMI->setCreateFullWindow(true);
	//_baseView_HDMI->setCreateWindowGroup(true);
	_baseView_HDMI->setWindowID("applicationuiID4");
	_baseView_HDMI->setPosition(0, 0);
	//_baseView_HDMI->setSize(1920, 1080);
	_baseView_HDMI->setZ(0);
	_baseView_HDMI->setTransparency(SCREEN_TRANSPARENCY_NONE);
	//_baseView_HDMI->setSize(1280, 720);
	_baseView_HDMI->setEnabled(true);
	Graphics2D* graphics2D = new Graphics2D(DISPLAY_HDMI);
	_baseView_HDMI->registerGraphics(graphics2D);
	//_baseView_HDMI->add();


	_graphView_HDMI->setWindowGroup("Play-HDMI");
	//_graphView_HDMI->setCreateWindowGroup(true);
	_graphView_HDMI->setCreateFullWindow(true);
	_graphView_HDMI->setWindowID("applicationuiID6");
	_graphView_HDMI->setZ(0);
	_graphView_HDMI->setPosition(0, 0);
	//_graphView_HDMI->setSize(1280, 720);
	//_graphView_HDMI->setSize(1920, 1080);
	_graphView_HDMI->setEnabled(true);
	_graphView_HDMI->setAltered(true);
	//_graphView_HDMI->add();

	_mouseTest_HDMI->setWindowGroup("Play-HDMI");
	_mouseTest_HDMI->setCreateWindowGroup(true);
	_mouseTest_HDMI->setCreateFullWindow(true);
	_mouseTest_HDMI->setWindowID("applicationuiID9");
	_mouseTest_HDMI->setZ(0);
	_mouseTest_HDMI->setPosition(0, 0);
	//_mouseTest_HDMI->setSize(1920, 1080);
	_mouseTest_HDMI->setEnabled(true);
	_mouseTest_HDMI->setAltered(true);
	//_mouseTest_HDMI->add();


	_videoOverlayView_HDMI->setWindowGroup("WorkOrPlay-HDMI");
	_videoOverlayView_HDMI->setCreateFullWindow(true);
	_videoOverlayView_HDMI->setCreateWindowGroup(true);
	_videoOverlayView_HDMI->setWindowID("applicationuiID5");
	_videoOverlayView_HDMI->setPosition(0, 0);
	//_videoOverlayView_HDMI->setSize(1920, 1080);
	_videoOverlayView_HDMI->setZ(0);
	//_videoOverlayView_HDMI->setSize(1280, 720);
	mediaURL = QString("http://vevoplaylist-live.hls.adaptive.level3.net/vevo/ch1/appleman.m3u8");
	//mediaURL = QString("file:///") + QDir().absoluteFilePath("app/native/assets/media/VID_00000007.mp4");
	_videoOverlayView_HDMI->setMediaURL(mediaURL);
	//_videoOverlayView_HDMI->setMediaSize(1920, 1080);
	_videoOverlayView_HDMI->setEnabled(true);
	_videoOverlayView_HDMI->add();

	//_videoOverlayView2->setWindowGroup(mainWindowGroupId);
	_videoOverlayView2->setWindowGroup("WorkOrPlay-HDMI");
	//_videoOverlayView2->setCreateFullWindow(true);
	//_videoOverlayView2->setCreateWindowGroup(true);
	_videoOverlayView2->setWindowID("applicationuiID7");
	_videoOverlayView2->setZ(5);
	//_videoOverlayView2->setPosition(400, 170);
	//_videoOverlayView2->setSize(250, 250);
	_videoOverlayView2->setPosition(750, 780);
	_videoOverlayView2->setSize(350, 250);
	mediaURL = QString("file:///") + QDir().absoluteFilePath("app/native/assets/media/VID_00000007.mp4");
	//_videoOverlayView2->setMediaURL(mediaURL);
	_videoOverlayView2->setMediaSize(1920, 1080);
	_videoOverlayView2->setEnabled(true);
	_videoOverlayView2->setAltered(true);
	//_videoOverlayView2->add();


			QString mainWindowGroupId = Application::instance()->mainWindow()->groupId();

			m_pGoodCitizen->setWindowGroup(mainWindowGroupId);
			m_pGoodCitizen->setWindowID("goodCitizenCascadesAppID1");
 *
*/


   // create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	if (!qml->hasErrors()) {

		qml->setContextProperty("_app", this);

		// create root object for the UI
		Page *root = qml->createRootObject<Page>();
		 if (root) {
			 //ViewControl* viewControl = root->findChild<ViewControl*>("playVC");

			// set created root object as a scene
			app->setScene(root);
		 }
	 }
}

void ApplicationUI::onInvisible()
{
	qDebug()  << "ApplicationUI::onInvisible";
}

void ApplicationUI::onFullscreen()
{
	qDebug()  << "ApplicationUI::onFullscreen";
}

void ApplicationUI::onThumbnail()
{
	qDebug()  << "ApplicationUI::onThumbnail";
}

void ApplicationUI::onAboutToQuit()
{
	qDebug()  << "ApplicationUI::onAboutToQuit";
	Views::shutdown();
}

QString ApplicationUI::fileURL(QString filePath)
{
	qDebug()  << "ApplicationUI::fileURL: " << filePath;

	return QString("file:///") + QDir().absoluteFilePath(filePath);
}

#ifdef BBNDK_VERSION_AT_LEAST(10,2,0)

void ApplicationUI::miracastShowOn() {
    InvokeRequest request;
    request.setTarget("sys.miracastviewer");
    request.setAction("bb.action.SHARE");
    request.setMimeType("application/vnd.rim.miracast.showon");

    _invokeManager.invoke(request);
}

void ApplicationUI::miracastPlayOn() {
    InvokeRequest request;
    request.setTarget("sys.miracastviewer");
    request.setAction("bb.action.SHARE");
    request.setMimeType("application/vnd.rim.miracast.playon");

    _invokeManager.invoke(request);
}

#endif

