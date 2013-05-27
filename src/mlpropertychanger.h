#ifndef MLPROPERTYCHANGER_H
#define MLPROPERTYCHANGER_H

#include <QObject>

class PropertyChanger : public QObject
{
	Q_OBJECT

public:
	PropertyChanger(QObject* object);
	~PropertyChanger();

	Q_INVOKABLE     bool set_repaint_throttle(QString value); // "NoThrottling", "Legacy", "Minimal", "Medium", "Heavy"
private:
	//QWebFrame*	pFrame;
};
#endif
