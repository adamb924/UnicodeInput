#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H


#include <QFont>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QtWidgets/QWidget>
#include <QVector>

class QMouseEvent;
class QPaintEvent;
class MainWindow;

class CharacterWidget : public QWidget
{
    Q_OBJECT

public:
    CharacterWidget(QWidget *parent = 0);
    QSize sizeHint() const;
    QFont font();

public slots:
    void updateFont(const QFont &font);
    void cursorPosition(int old, int newpos);
    void updateSelection(int left,int length);
    void updateHasSelection(bool hasSelection);

signals:
    void characterSelected(const QString &character);

protected:
    void paintEvent(QPaintEvent *event);
    MainWindow *mwParent;

    QList<QRect> aRects;

private:
//    QString theString;
    QVector<quint32> theString;
    bool hasSelection;
    QFont displayFont;
    int squareWidth, squareHeight;
    int selectionLeft, selectionLength;

    int topTextMargin;
    int bottomTextMargin;
    int leftTextMargin;
    int rightmargin;
    int rectPadding;
    int cursor;

    bool event(QEvent *event);

    int whichGlyph(QPoint pos);

private slots:
    void updateText(QString str);
};

#endif
