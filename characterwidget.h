#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include <QFont>
#include <QPoint>
#include <QSize>
#include <QString>
#include <QtWidgets/QFrame>
#include <QVector>

class QMouseEvent;
class QPaintEvent;
class MainWindow;
class DatabaseAdapter;

class CharacterWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CharacterWidget(QWidget *parent = nullptr);
    void setDbAdapter(const DatabaseAdapter * db);

    QSize sizeHint() const;
    QFont font();

public slots:
    void updateFont(const QFont &font);
    void updateCharacterDisplayFont(const QFont &font);
    void cursorPosition(int old, int newpos);
    void updateSelection(int left,int length);
    void updateHasSelection(bool hasSelection);

signals:
    void characterSelected(const QString &character);
    void characterDoubleClicked(quint32 character);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    const DatabaseAdapter * mDbAdapter;

    QTransform transform;
    QList<QRect> aRects;

private:
    qreal mScreenScale;
    QVector<quint32> theString;
    bool hasSelection;
    QFont displayFont, characterDisplayFont;
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
