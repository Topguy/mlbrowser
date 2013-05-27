#include "mlkeyfilter.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>

#include "mlwebkit.h"

bool KeyFilter::eventFilter(QObject* pObject, QEvent* pEvent)
{
	if ( pEvent->type() == QEvent::KeyPress )
	{
//TODO: understand why a stream of keycode = 0 with alternating repeat occurs 

		QKeyEvent* pKeyEvent = static_cast< QKeyEvent* >( pEvent );

		/*
		pKeyEvent->count
		pKeyEvent->isAutoRepeat
		pKeyEvent->key
		pKeyEvent->matches
		pKeyEvent->modifiers
		pKeyEvent->nativeModifiers
		pKeyEvent->nativeScanCode
		pKeyEvent->nativeVirtualKey
		pKeyEvent->text
		*/

		if ( pKeyEvent->key() != 0 ) // avoid a (default) stream of output
		{
			qDebug() << "METROLOGICAL : key code (Qt::Key) : " << QString("%1").arg(pKeyEvent->key(), 0, 16) << " : unicode : " << pKeyEvent->text() << " : modifiers (or of Qt::KeyboardModifier's) : "<< pKeyEvent->modifiers() << " : keypress event (true/false) : " << true << " : key by autorepeat mechanism (true/false) : " << pKeyEvent->isAutoRepeat();
//TODO: check values

			switch ( pKeyEvent->key() )
			{
				case Qt::Key_Select : // 0x1010000
				{
					QKeyEvent keyevent ( QEvent::KeyPress, Qt::Key_Enter, pKeyEvent->modifiers(), "", pKeyEvent->isAutoRepeat(), 1 );
					qDebug () << "METROLOGICAL : send event : " << QCoreApplication::sendEvent ( reinterpret_cast<QObject*> (QApplication::focusWidget()), &keyevent ); 
					return true;
				}

				case Qt::Key_Menu : // 0x1000055
        	        	{
					QKeyEvent keyevent ( QEvent::KeyPress, Qt::Key_Backspace, pKeyEvent->modifiers(), "", pKeyEvent->isAutoRepeat(), 1 );
					qDebug () << "METROLOGICAL : send event : " << QCoreApplication::sendEvent ( reinterpret_cast<QObject*> (QApplication::focusWidget()), &keyevent ); 
					return true;
        		        } 

				case Qt::Key_HomePage : // 'Apps', 0x1000090
				{
					QKeyEvent keyevent ( QEvent::KeyPress, Qt::Key_F8, pKeyEvent->modifiers(), "", pKeyEvent->isAutoRepeat(), 1 );
					qDebug () << "METROLOGICAL : send event : " << QCoreApplication::sendEvent ( reinterpret_cast<QObject*> (QApplication::focusWidget()), &keyevent ); 
					return true;
				}
				case Qt::Key_Favorites : // 'FAV1', 0x1000091
				{
					QKeyEvent keyevent ( QEvent::KeyPress, Qt::Key_F10, pKeyEvent->modifiers(), "", pKeyEvent->isAutoRepeat(), 1 );
					qDebug () << "METROLOGICAL : send event : " << QCoreApplication::sendEvent ( reinterpret_cast<QObject*> (QApplication::focusWidget()), &keyevent ); 
					return true;
				}
#ifdef _INSPECTOR_
				case Qt::Key_Period : // '.' (period), 0x2e
				{
//TODO: move to the browser context menu event 

					// show / hide webinspector
					MLWebKit* pWebKit = MLWebKit::instance();

					if (pWebKit != NULL )
						pWebKit->inspector();

					return true;
				}

				case Qt::Key_VolumeMute : // 0x1000071
				{	
					QKeyEvent keyevent ( QEvent::KeyPress, Qt::Key_BracketRight, Qt::ControlModifier, "", pKeyEvent->isAutoRepeat(), 1 );
					qDebug () << "METROLOGICAL : send event : " << QCoreApplication::sendEvent ( reinterpret_cast<QObject*> (QApplication::focusWidget()), &keyevent ); 
					return true;
				}

#endif
				default:;
			}
		}
	}	

	return QObject::eventFilter(pObject, pEvent);
}
