#include <QGraphicsSceneContextMenuEvent>
#include <QWebSettings>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>

#include "mlwebkit.h"


#include <QMetaObject>


#ifdef _PLAYER_
#include "mlplayer.h"
#endif

class GraphicsWebView : public QGraphicsWebView
{
public:
	GraphicsWebView()
	{
	}

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* ev)
	{

		qDebug () << "METROLOGICAL : received a context event";

		if ( ev != NULL )
			ev->ignore();	

/*
QContextMenuEvent::Mouse
QContextMenuEvent::Keyboard
QContextMenuEvent::Other
*/

	}
};

class WebPage : public QWebPage
{
public:

protected:
	void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source)
	{
		QString logEntry = source + " [" + QString::number(lineNumber) + "]:" + message.toUtf8().constData();
		qDebug()<<logEntry;
	}

	void javaScriptAlert(QWebFrame*, const QString& message)
	{
		QString logEntry = message.toUtf8().constData();
		qDebug()<<logEntry;
	}

	bool shouldInterruptJavaScript()
	{
		return false;
	}

/*
	virtual QString userAgentForUrl(const QUrl &url ) const
	{
		Q_UNUSED(url);

		qDebug () << "METROLOGICAL : user agent : " << "mlwebkit/1.0";
		return QString("mlwebkit/1.0");
	}
*/
};

#ifdef _INSPECTOR_
MLWebKit* MLWebKit::pWebKit = NULL;

MLWebKit* MLWebKit::instance()
{
	return pWebKit;
} 
#endif

MLWebKit::MLWebKit() 
{
#ifdef _INSPECTOR_
	pWebKit = this;
#endif

	// Create elements

	pScene = new QGraphicsScene();

	if ( pScene == NULL ) return;

	pView = new QGraphicsView(pScene);

	pWebview = new GraphicsWebView();

#ifdef QT_OPENGL_LIB
	pWidget = new QGLWidget();
#endif
	pPage =  new WebPage();

	pFrame = pPage->mainFrame();

	QApplication* pApp = (QApplication *)QApplication::instance();

	QDesktopWidget* pDesktop = QApplication::desktop();

	if ( pScene == NULL || pView == NULL || pWebview == NULL || pPage == NULL || pFrame == NULL || pApp == NULL || pDesktop == NULL )
	{
		qDebug () << "METROLOGICAL : unable to construct browser (elements)";
		return;
	}

#ifdef QT_OPENGL_LIB
	pWidget = new QGLWidget(pView);
#endif

#ifdef _INSPECTOR_
	pInspector = new QWebInspector;
	pInspector->setPage(pPage);
        pInspector->resize(QApplication::desktop()->screenGeometry().size());
#endif

#ifdef QT_OPENGL_LIB
	pView->setViewport(pWidget);
#endif

	// Configuration, settings and alike
//	pScene->setItemIndexMethod( QGraphicsScene::NoIndex);
//	pView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//	pView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
//	pView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

	// Disable some 'browser features / elements'
	pView->setFrameShape(QFrame::NoFrame);
	pView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	pView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

//	pView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	pView->setWindowFlags(Qt::FramelessWindowHint);
	pView->showFullScreen();

	// Overrule the 'foreground' and 'background' defaults with transparent colors
	QPalette palette;
	palette.setBrush(QPalette::Active, QPalette::Window, Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Base, Qt::SolidPattern);
	palette.setBrush(QPalette::Inactive, QPalette::Window, Qt::SolidPattern);
	palette.setBrush(QPalette::Inactive, QPalette::Base, Qt::SolidPattern);
	palette.setColor(QPalette::Active, QPalette::Window, QColor(0, 0, 0, 0));
	palette.setColor(QPalette::Active, QPalette::Base, QColor(0, 0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::Window, QColor(0, 0, 0, 0));
	palette.setColor(QPalette::Inactive, QPalette::Base, QColor(0, 0, 0, 0));

	pApp->setPalette(palette);

	qDebug () << "METROLOGICAL : geometry : " << pDesktop->screenGeometry().size();

	// Proper (re)sizing, full screen

//TODO: implement check
	pWebview->resize(QApplication::desktop()->screenGeometry().size());
	pWebview->setPage(pPage);

	// Set the keyboard and mouse focus
	pWebview->setFocus();

	// Some extra settings
	QWebSettings* pSettings = pWebview->settings();

#ifdef _INSPECTOR_
	pSettings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
#endif

	pSettings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
	pSettings->setAttribute(QWebSettings::WebGLEnabled, false);
	pSettings->setAttribute(QWebSettings::PluginsEnabled, false);
	pSettings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
	pSettings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
	pSettings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
//	pSettings->setAttribute(QWebSettings::FrameFlatteningEnabled, true);
	pSettings->setAttribute(QWebSettings::LocalStorageEnabled, true);
	pSettings->setAttribute(QWebSettings::WebSecurityEnabled, false);
	pSettings->setAttribute(QWebSettings::SpatialNavigationEnabled, false);

	// Overrule the cache settings
/*
	pSettings->setMaximumPagesInCache(0);
	pSettings->setObjectCacheCapacities(0, 0, 0);
	pSettings->QWebSettings::clearMemoryCaches();
*/

	// Finalize
	pScene->addItem(pWebview);

#ifdef _INSPECTOR_
	pProxyWidget = pScene->addWidget(pInspector);
#endif

	// Set visibility

#ifdef _INSPECTOR_
//	pInspector->hide();
	pProxyWidget->hide();
#endif

	pWebview->show();
}

MLWebKit::~MLWebKit()
{
	qDebug () << "METROLOGICAL : clean up browser (elements)";

/*
	if (pInspector != NULL)
		delete pInspector;
*/

	if (pWebview != NULL)
		delete pWebview;

	if (pView != NULL)
		delete pView;

	if (pScene != NULL)
		delete pScene;

	if (pObject != NULL)
		delete pObject;
}

void MLWebKit::load(QUrl url)
{
	qDebug () << "METROLOGICAL : load ( url ) : " << url;

	if (pWebview != NULL)
		pWebview->load(url);
}

void MLWebKit::show()
{
	qDebug () << "METROLOGICAL : show ()";

	if (pView != NULL)
		pView->show();
}

void MLWebKit::hide()
{
	qDebug () << "METROLOGICAL : hide ()";

	if (pView != NULL)
		pView->hide();
}


#if defined (_PLAYER_) || defined (_PROPERTYCHANGER_)
void MLWebKit::attach_object(QObject* _pObject_, QString _name_)
{
	qDebug () << "METROLOGICAL : attach_player()";

	if ( pFrame != NULL)
	{	
		pObject = _pObject_;

		qDebug () << "METROLOGICAL : change (NULL) parent to pFrame";
		pObject->setParent(pFrame);

		qDebug () << "METROLOGICAL : add webkit bridge for object " << pObject;
//TODO: connect to slot to keep the object accessible when page has changed
		pFrame->addToJavaScriptWindowObject(_name_, pObject);


	}
}
#endif

#ifdef _INSPECTOR_
void MLWebKit::inspector()
{
	qDebug () << "METROLOGICAL : toggle web inspector";

	if(pInspector != NULL && pWebview != NULL)
	{
 
 		if ( pInspector->isVisible() == false )
 		{
			qDebug () << "METROLOGICAL : show webinspector";

			pWebview->hide();
			pWebview->setEnabled(false);
			pProxyWidget->show();
			pProxyWidget->setEnabled(true);
 		}
 		else
 		{
			qDebug () << "METROLOGICAL : hide webinspector";

 			pProxyWidget->hide();
			pProxyWidget->setEnabled(false);
			pWebview->show();
			pWebview->setEnabled(true);
 		}
 	}
	else
		qDebug () << "METROLOGICAL : some elements do not exist";
}
#endif
