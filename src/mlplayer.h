#ifndef MLPLAYER_H
#define MLPLAYER_H

#include <QString>
#include <QObject>
#include <gst/gst.h>

class Player : public QObject
{
	Q_OBJECT

public:
	Player(QObject* object);
	~Player();

	Q_INVOKABLE	bool load(QString url);
	Q_INVOKABLE	bool start(void);
	Q_INVOKABLE	bool pause(void);
	Q_INVOKABLE	bool resume(void);
	Q_INVOKABLE	bool stop(void);
	Q_INVOKABLE	bool quit(void);

private:
	QString		url;
	GstElement*	pbin;
};

#endif
