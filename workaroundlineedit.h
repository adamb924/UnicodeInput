#ifndef WORKAROUNDLINEEDIT_H
#define WORKAROUNDLINEEDIT_H

#include <QLineEdit>

class WorkaroundLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit WorkaroundLineEdit(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent * e);
};

#endif // WORKAROUNDLINEEDIT_H
