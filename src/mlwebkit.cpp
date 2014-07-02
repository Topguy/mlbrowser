#include <QGraphicsSceneContextMenuEvent>
#include <QWebSettings>
#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QWebFrame>

#include "mlwebkit.h"


#include <QMetaObject>


#ifdef _PLAYER_
#include "mlplayer.h"
#endif


#ifdef QT_OPENGL_LIB
#undef QT_OPENGL_LIB
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

        qDebug () << "received a context event";

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

    		qDebug () << "user agent : " << "mlwebkit/1.0";
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

MLWebKit::MLWebKit(int overscanw, int overscanh, qreal zoom, int rotationMode)
{
#ifdef _INSPECTOR_
    pWebKit = this;
#endif

    // Create elements

    pScene = new QGraphicsScene();

    if ( pScene == NULL ) return;

    pView = new QGraphicsView(pScene);

    pWebview = new GraphicsWebView();

    pPage =  new WebPage();

    pFrame = pPage->mainFrame();

    pFrame->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff );
    pFrame->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );

    QApplication* pApp = (QApplication *)QApplication::instance();

    QDesktopWidget* pDesktop = QApplication::desktop();

    if ( pScene == NULL || pView == NULL || pWebview == NULL || pPage == NULL || pFrame == NULL || pApp == NULL || pDesktop == NULL )
    {
        qDebug () << "unable to construct browser (elements)";
        return;
    }

    // Rotate View according to rotationMode
    pView->rotate(90*rotationMode);

#ifdef QT_OPENGL_LIB
    pWidget = NULL;
//	pWidget = new QGLWidget();
//	pWidget = new QGLWidget(pView);
#endif

#ifdef _INSPECTOR_
    pInspector = new QWebInspector;
    pInspector->setPage(pPage);
    pInspector->resize(QApplication::desktop()->screenGeometry().size());
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
    palette.setColor(QPalette::Active, QPalette::Window, QColor(0, 0, 0x80, 255));
    // Nice light grey default backgroundcolor. Change to bright red if you want to see overscan borders.
    palette.setColor(QPalette::Active, QPalette::Base, QColor(0xC0, 0xc0, 0xc0, 255));
    palette.setColor(QPalette::Inactive, QPalette::Window, QColor(0, 0, 0, 255));
    palette.setColor(QPalette::Inactive, QPalette::Base, QColor(0, 0, 0, 255));

    pApp->setPalette(palette);


//TODO: implement check
    QSizeF screenSize = pDesktop->screenGeometry().size();
    qDebug () << "screen geometry : " << screenSize;

    QSizeF displaySize;
    switch (rotationMode) {
        break;
        case 1:
        case 3:
            displaySize = QSizeF(
                screenSize.height() - overscanw,
                screenSize.width() - overscanh
            );
        break;
        case 0:
        case 2:
        default:
            displaySize = QSizeF(
                screenSize.width() - overscanw,
                screenSize.height() - overscanh
            );
    }
    qDebug () << "display geometry : " << displaySize;

    pWebview->resize(displaySize);
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
//	pSettings->setAttribute(QWebSettings::WebSecurityEnabled, false);
    pSettings->setAttribute(QWebSettings::SpatialNavigationEnabled, false);

    // Overrule the cache settings
    /*
    	pSettings->setMaximumPagesInCache(0);
    	pSettings->setObjectCacheCapacities(0, 0, 0);
    	pSettings->QWebSettings::clearMemoryCaches();
    */

    // Finalize
#ifdef QT_OPENGL_LIB
//	pView->setViewport(pWidget);
    pView->setViewport(new QGLWidget(QGL::DirectRendering | QGL::DoubleBuffer));
#endif

    pScene->addItem(pWebview);

#ifdef _INSPECTOR_
    pProxyWidget = pScene->addWidget(pInspector);
#endif

    // Set visibility

#ifdef _INSPECTOR_
//	pInspector->hide();
    pProxyWidget->hide();
#endif

    // Set zoomfactor. (help readability)
    pWebview->setZoomFactor(zoom);
    //pWebview->setTextSizeMultiplier(zoom);

    pWebview->show();
}

MLWebKit::~MLWebKit()
{
    qDebug () << "clean up browser (elements)";

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
    qDebug () << "load ( url ) : " << url;

    if (pWebview != NULL)
        pWebview->load(url);
}

void MLWebKit::show()
{
    qDebug () << "show ()";

    if (pView != NULL)
        pView->show();
}

void MLWebKit::hide()
{
    qDebug () << "hide ()";

    if (pView != NULL)
        pView->hide();
}


#if defined (_PLAYER_) || defined (_PROPERTYCHANGER_)
void MLWebKit::attach_object(QObject* _pObject_, QString _name_)
{
    qDebug () << "attach_player()";

    if ( pFrame != NULL)
    {
        pObject = _pObject_;

        qDebug () << "change (NULL) parent to pFrame";
        pObject->setParent(pFrame);

        qDebug () << "add webkit bridge for object " << pObject;
//TODO: connect to slot to keep the object accessible when page has changed
        pFrame->addToJavaScriptWindowObject(_name_, pObject);


    }
}
#endif

#ifdef _INSPECTOR_
void MLWebKit::inspector()
{
    qDebug () << "toggle web inspector";

    if(pInspector != NULL && pWebview != NULL)
    {

        if ( pInspector->isVisible() == false )
        {
            qDebug () << "show webinspector";

            pWebview->hide();
            pWebview->setEnabled(false);
            pProxyWidget->show();
            pProxyWidget->setEnabled(true);
        }
        else
        {
            qDebug () << "hide webinspector";

            pProxyWidget->hide();
            pProxyWidget->setEnabled(false);
            pWebview->show();
            pWebview->setEnabled(true);
        }
    }
    else
        qDebug () << "some elements do not exist";
}
#endif
