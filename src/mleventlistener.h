#ifndef MLEVENTLISTENER_H
#define MLEVENTLISTENER_H

#include <QObject>

class EventListener : public QObject
{
protected:
	bool eventFilter(QObject* pObject, QEvent* pEvent);
}; 

#endif
