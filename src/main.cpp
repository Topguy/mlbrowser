#define _BUILD_TIME_ __TIME__
#define _BUILD_DATE_ __DATE__

#include <QApplication>
#include <QtGui>

#ifdef _KEYFILTER_
#include <QKeyEvent>
#include <QMouseEvent>
#endif

#ifdef _EVENTMONITORING_ 
#include "mleventlistener.h"
#endif

#ifdef _VERBOSE_
#include <iostream>
#endif

#ifdef _BROWSER_
#include "mlwebkit.h"
#endif

#ifdef _PLAYER_
#include "mlplayer.h"
#endif

#ifdef _PROPERTYCHANGER_
#include "mlpropertychanger.h"
#endif

#ifdef _KEYFILTER_
#include "mlkeyfilter.h"
#endif

#ifdef _VERBOSE_
void MessageHandler ( QtMsgType type, const QMessageLogContext& context, const QString& message )
{
/*
black - 30
red - 31
green - 32
brown - 33
blue - 34
magenta - 35
cyan - 36
lightgray - 37
*/

	std::cerr
		<< QCoreApplication::instance ()->applicationName().toStdString()
//		<< " : [ version ] "
//		<< context.version
		<< " : [ file ] "
		<< "\033[0;31m" << context.file << "\033[0m"
		<< " : [ line ] "
		<< "\033[0;35m" << context.line << "\033[0m"
//		<< " : [ category ] "
//		<< context.category
		<< " : [ type ] "
		<< "\033[0;36m";

	switch ( type )
	{
		case QtDebugMsg		: std::cerr << "debug"; break;
		case QtWarningMsg	: std::cerr << "warning"; break;
		case QtCriticalMsg	: std::cerr << "critical"; break;
		case QtFatalMsg		: std::cerr << "fatal"; break;
		default			:;
	}

	std::cerr
		<< "\033[0m"
		<< " : [ function ] "
		<<  "\033[0;34m" << context.function << "\033[0m"
		<< " : [ message ] "
		<< "\033[0;32m" << message.toStdString() << "\033[0m"
		<< std::endl;
}
#endif

int main(int argc, char * argv[])
{
	QApplication app(argc, argv);

#ifdef _VERBOSE_
	qInstallMessageHandler ( MessageHandler );
#endif

	qDebug () << "METROLOGICAL : browser release (" << _BUILD_DATE_ << _BUILD_TIME_ <<")";

#ifdef _PLAYER_
        Player player(NULL);
#endif

#ifdef _PROPERTYCHANGER_
	PropertyChanger propertychanger(NULL);
#endif

//REMARK: order of filters is important; last installed -> receives first
#ifdef _EVENTMONITORING_
        qDebug () << "METROLOGICAL : enable event monitoring";
	EventListener eventlistener;

	app.installEventFilter ( &eventlistener );
#endif

#ifdef _KEYFILTER_
	qDebug () << "METROLOGICAL : add keyboard filter";
	KeyFilter keyfilter;

	app.installEventFilter ( &keyfilter );
#endif

#ifdef _BROWSER_
	QUrl url;

        if (argc > 1)
                url = QUrl(argv[1]);
        else
                url = QUrl("");

#ifndef _MOUSE_
        qDebug () << "METROLOGICAL : hide mouse pointer";
	QApplication::setOverrideCursor ( QCursor ( Qt::BlankCursor ) );
#endif

        MLWebKit* browser = new MLWebKit();

#ifdef _PLAYER_
        qDebug () << "METROLOGICAL : add player";
	browser->attach_object(&player, "player");
#endif

#ifdef _PROPERTYCHANGER_
	qDebug () << "METROLOGICAL : add propertychanger"; 
	browser->attach_object(&propertychanger, "propertychanger");
#endif

	qDebug () << "METROLOGICAL : load and show page";

        browser->load(url);
        browser->show();
#endif

	return app.exec();

#ifdef _BROWSER_
	delete browser;
#endif
}
