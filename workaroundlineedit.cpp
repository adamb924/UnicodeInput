#include "workaroundlineedit.h"

#include <QKeyEvent>

WorkaroundLineEdit::WorkaroundLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void WorkaroundLineEdit::keyPressEvent(QKeyEvent *e)
{
    QLineEdit::keyPressEvent(e);
    if( e->key() == 0x200C )
    {
        insert( QChar(0x200C) );
    }
}
