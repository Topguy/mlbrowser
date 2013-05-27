#include "mleventlistener.h"

#include <QDebug>
#include <QEvent>

bool EventListener::eventFilter(QObject* pObject, QEvent* pEvent)
{
	switch ( pEvent->type() )
	{
		case QEvent::KeyPress :
		case QEvent::Paint		: // 12	Screen update necessary (QPaintEvent).
		case QEvent::UpdateLater	: // 78	The widget should be queued to be repainted at a later time.
		case QEvent::UpdateRequest	: // 77	The widget should be repainted.
		{
			qDebug () << "METROLOGICAL : QObject : " << pObject << " : event type : " << pEvent->type();
		}
		default :;
	}

	return QObject::eventFilter(pObject, pEvent);
} 
