// Default empty project template
#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include "applicationui.hpp"

// include JS Debugger / CS Profiler enabler
// this feature is enabled by default in the debug build only
#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // this is where the server is started etc
	bb::cascades::Application app(argc, argv);

    // localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString( "Play%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator( &translator );
    }

    ApplicationUI applicationUI(&app);

	QObject::connect(
			&app,
			SIGNAL(invisible()),
			&applicationUI,
			SLOT(onInvisible()));
	QObject::connect(
			&app,
			SIGNAL(fullscreen()),
			&applicationUI,
			SLOT(onFullscreen()));
	QObject::connect(
			&app,
			SIGNAL(thumbnail()),
			&applicationUI,
			SLOT(onThumbnail()));
	QObject::connect(
			&app,
			SIGNAL(aboutToQuit()),
			&applicationUI,
			SLOT(onAboutToQuit()));


    // we complete the transaction started in the app constructor and start the client event loop here
    return app.exec();
    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
