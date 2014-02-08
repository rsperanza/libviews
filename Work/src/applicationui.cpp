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
    qmlRegisterType < PieChart >      ("com.example.views", 1, 0, "PieChart");

	_dataPath = QDir::current().absoluteFilePath("data");

   // create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	if (!qml->hasErrors()) {

		qml->setContextProperty("_app", this);

		// create root object for the UI
		Page *root = qml->createRootObject<Page>();
		if (root) {
			ViewControl* viewControl = root->findChild<ViewControl*>("workVC");

			//if (viewControl) {
			//	 _signaturePad = root->findChild<SignaturePad*>("signature");
			//}

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

