#include "mlplayer.h"

#include <QDebug>

Player::Player(QObject* object)
{
	Q_UNUSED(object);

	qDebug () << "create player";

	gst_init ( NULL, NULL );
//	gst_init ( &argc, &argv );
}

bool Player::load(QString _url_)
{
	qDebug () << "load url : " << _url_;

	url.clear();
	url = _url_;

//TODO:	dynamic allocation
	char pdef [500];
//TODO: verify
	sprintf(pdef,"gst-launch-0.10 videotestsrc ! queue ! eglglessink");

	//sprintf(pdef,"gst-launch-0.10 playbin2c uri=%s ! queue ! eglglessink", url.toLatin1().constData() );

	pbin = gst_parse_launch ( pdef, NULL );

	if ( pbin == NULL )
	{
		qDebug () << "cannot create pipeline (elements)";
		return false;
	}

//TODO: check return value
	gst_element_set_state ( pbin, GST_STATE_READY );

	gst_element_set_state ( pbin, GST_STATE_PAUSED );

	return true;
}

bool Player::start(void)
{
	qDebug () << "start playback";
 
	if ( pbin == NULL )
	{
		qDebug () << "cannot start playback";
		return false;
	}

//TODO: check return value
	gst_element_set_state ( pbin, GST_STATE_PLAYING );

	return true;
}

bool Player::pause(void)
{
	qDebug () << "pause playback";

	if ( pbin == NULL )
	{
		qDebug () << "cannot pause playback";
		return false;
	}

//TODO: check return value
	gst_element_set_state ( pbin, GST_STATE_PAUSED );

	return true;
}

bool Player::resume(void)
{
	qDebug () << "resume playback";

	if ( pbin == NULL )
	{
		qDebug () << "cannot cannot resume playback";
		return false;
	}

//TODO: check return value
	gst_element_set_state ( pbin, GST_STATE_PLAYING );

	return true;
}

bool Player::stop(void)
{
	qDebug () << "stop playback";

	if ( pbin == NULL )
		qDebug () << "cannot stop playback";
		return false;

//TODO: check return value

	pause();

	gst_element_set_state ( pbin, GST_STATE_READY );

	return true;
}

bool Player::quit(void)
{
	qDebug () << "quit player";

	stop();

//TODO: check return value
	gst_element_set_state ( pbin, GST_STATE_NULL );

	pbin = NULL;

	url.clear();

	return true;
}

Player::~Player()
{
	quit();
}

