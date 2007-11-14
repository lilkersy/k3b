#ifndef _K3B_ACTION_H_
#define _K3B_ACTION_H_

#include <QtCore/QObject>
#include <KIcon>
#include <KAction>
#include <KActionCollection>

namespace K3b {
/**
 * Create a KAction the old (way cooler) KDE3 way. Porting becomes a simple replacement.
 */
    KAction* createAction( QObject* parent,
			   const QString& text, const QString& icon, const
			   QKeySequence& shortcut, QObject* receiver, const char* slot,
			   KActionCollection* actionCollection = 0,
			   const QString& actionName = QString() )
    {
	KAction* action = new KAction( parent );
	action->setText( text );
	if( !icon.isEmpty() ) {
	    action->setIcon( KIcon( icon ) );
	}
	action->setShortcut( shortcut );
	if( receiver ) {
	    QObject::connect( action, SIGNAL( triggered() ),
			      receiver, slot );
	}
	if( actionCollection ) {
	    actionCollection->addAction( actionName, action );
	}
	return action;
    }
}

#endif