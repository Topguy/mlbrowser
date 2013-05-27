#ifndef MLKEYFILETR_H
#define MLKEYFILETR_H

#include <QObject>

class KeyFilter : public QObject
{
protected:
	bool eventFilter(QObject* pObject, QEvent* pEvent);
}; 

#endif
