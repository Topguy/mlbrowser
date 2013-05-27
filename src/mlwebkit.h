#ifndef MLWEBKIT_H
#define MLWEBKIT_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWebView>
#include <QWebPage>
#include <QWebFrame>

#ifdef QT_OPENGL_LIB
#include <QGLWidget>
#endif

#ifdef _INSPECTOR_
#include <QWebInspector>
#include <QGraphicsProxyWidget>
#endif

class MLWebKit
{
public:
	MLWebKit();
	~MLWebKit();

	void load(QUrl url);
	void show();
	void hide();

#if defined (_PLAYER_) || defined (_PROPERTYCHANGER_)
	void attach_object(QObject* pObject, QString _name_);
#endif

#ifdef _INSPECTOR_
	static MLWebKit* instance();
	void inspector();
#endif

private:
	QGraphicsView*		pView;
	QGraphicsScene*		pScene;
	QGraphicsWebView*	pWebview;
	QWebPage*		pPage;
	QWebFrame*		pFrame;

	QObject*		pObject;

#ifdef QT_OPENGL_LIB
	QGLWidget*		pWidget; // viewport
#endif

#ifdef _INSPECTOR_
	static MLWebKit*	pWebKit;
	QWebInspector*		pInspector;
	QGraphicsProxyWidget*	pProxyWidget;
#endif
};
#endif
