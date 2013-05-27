#include "mlpropertychanger.h"

#include <QDebug>
#include <QWebFrame>
#include <QWebPage>

PropertyChanger::PropertyChanger(QObject* pObject)
{
	Q_UNUSED(pObject);
	qDebug () << "parent :" << this->parent();
}

PropertyChanger::~PropertyChanger()
{
}

bool PropertyChanger::set_repaint_throttle(QString value)
{
	QWebFrame* pFrame = static_cast < QWebFrame* >  ( this->parent() );
	QWebPage* pPage = NULL;

	if ( pFrame != NULL )
		pPage = pFrame->page();
	else
		return false;

	if ( pPage == NULL )
		return false;

//TODO: check values
	pPage->setProperty("_q_RepaintThrottlingPreset", value);

	return true;
}

