// Default empty project template

#include <bb/cascades/Application>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Window>

#include "applicationui.hpp"

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app)
: QObject(app), _app(app), _invokeManager(new InvokeManager())
{
	// register some new types for QML
    qmlRegisterType < PieChart >      ("views.graphs", 1, 0, "PieChart");
    qmlRegisterType < LineGraph >      ("views.graphs", 1, 0, "LineGraph");
    qmlRegisterType < SignaturePad >      ("views.controls", 1, 0, "SignaturePad");

	_dataPath = QDir::current().absoluteFilePath("data");

   // create scene document from main.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

	if (!qml->hasErrors()) {

		qml->setContextProperty("_app", this);

		// create root object for the UI
		TabbedPane *root = qml->createRootObject<TabbedPane>();
		if (root) {
			//ViewControl* viewControl = root->findChild<ViewControl*>("workVC");

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

QString ApplicationUI::getAbsoluteDataFilename(QString filename)
{
	qDebug()  << "ApplicationUI::getAbsoluteDataFilename";

	_dataPath = QDir::current().absoluteFilePath("data");
	return _dataPath.append("/").append(filename);
}

QString ApplicationUI::getFileURL(QString filename)
{
	qDebug()  << "ApplicationUI::getFileURL";

	_dataPath = QDir::current().absoluteFilePath("data");
	return QUrl::fromLocalFile(filename).toString();
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

