// Default empty project template
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QRectF>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeRequest>


#include "views/Views.hpp"
#include "VideoOverlay.hpp"

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
	InvokeManager _invokeManager;
	QString _dataPath;
	bb::cascades::Application *_app;
};


#endif /* ApplicationUI_HPP_ */
