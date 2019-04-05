#include <QtWidgets>

#include <QtDebug>

#include "characterwidget.h"
#include "databaseadapter.h"

CharacterWidget::CharacterWidget(QWidget *parent)
    : QWidget(parent),
      mDbAdapter(nullptr)
{
    squareHeight = 30;
    squareWidth = 22;

    mScreenScale = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 120.0;

    topTextMargin = 2;
    bottomTextMargin = 4;
    leftTextMargin = 3;
    rightmargin = 5;
    rectPadding = 4;

    updateFont(QFont()); // default initialization

    setSizePolicy( QSizePolicy::Maximum , QSizePolicy::Minimum );
}

void CharacterWidget::setDbAdapter(const DatabaseAdapter *db)
{
    mDbAdapter = db;
}

void CharacterWidget::updateFont(const QFont &font)
{
    displayFont.setFamily(font.family());
    displayFont.setPixelSize(static_cast<int>(0.5f*squareHeight));
    adjustSize();
    update();
}

void CharacterWidget::updateCharacterDisplayFont(const QFont &font)
{
    characterDisplayFont = font;
    update();
}

QSize CharacterWidget::sizeHint() const
{
    return QSize(1000, topTextMargin + squareHeight + bottomTextMargin );
//    return QSize(1000, mScreenScale * ( topTextMargin + squareHeight + bottomTextMargin ) );
}

void CharacterWidget::updateText(QString str)
{
    theString = str.toUcs4();
    hasSelection = false; // a bit hackish, but logically necessary
    repaint();
}

void CharacterWidget::paintEvent(QPaintEvent *event)
{
    // set up the environment
    QPainter painter(this);

    transform = QTransform();
    transform.scale( mScreenScale, mScreenScale );
    painter.setWorldTransform(transform, false);

    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.setFont(displayFont);

    quint32 key;
    int cursorx;
    QFontMetrics fontMetrics(displayFont);
    QFontMetrics topFontFM(characterDisplayFont);

    // clear the memory of rects
    aRects.clear();

    for (int i=0; i < theString.count(); i++)
    {
        key = theString[i];

        if( topFontFM.inFont( theString[i] ) )
        {
            painter.setPen(QPen(Qt::black));
        }
        else
        {
            painter.setPen(QPen(Qt::red));
        }

        // temporary square width variable
        int sw = squareWidth;

        QString upperline, lowerline;

        if( key > 0xffff )
        {
            quint32 right = key & 0x000fff;
            quint32 left = (key & 0xfff00)>>12;
            lowerline = QString("%1").arg(left,3,16,QLatin1Char('0')).toUpper();
            upperline = QString("%1").arg(right,3,16,QLatin1Char('0')).toUpper();

            sw *= 1.5;
        }
        else // inside the BMP
        {
            quint32 right = key & 0x00ff;
            quint32 left = (key & 0xff00)>>8;
            lowerline = QString("%1").arg(left,2,16,QLatin1Char('0')).toUpper();
            upperline = QString("%1").arg(right,2,16,QLatin1Char('0')).toUpper();
        }

        painter.setClipRect(event->rect());

        int leftside;
        if(aRects.length() > 0) // or the edge of the last rect, plus a margin
            leftside = aRects.last().x() + aRects.last().width() + rightmargin;
        else
            leftside = leftTextMargin; // the left position is the leftTextMargin

        aRects << QRect( leftside , topTextMargin, sw, squareHeight);

        painter.drawRect(aRects.last());
//        painter.fillRect(aRects.last(), Qt::yellow);
        painter.drawText( aRects.last().left() + rectPadding ,
                          fontMetrics.height() - 2,
                          lowerline);
        painter.drawText( aRects.last().left() + rectPadding ,
                          2 * fontMetrics.height() - 6,
                          upperline);
    }

    if(theString.count())
    {
        painter.setPen(QPen(Qt::red));
        if(hasSelection) // draw a big box
        {
            painter.drawRect( aRects.at(selectionLeft).left() - static_cast<int>(0.5f*leftTextMargin) , 0 , aRects.at(selectionLeft+selectionLength-1).right() - aRects.at(selectionLeft).left() + leftTextMargin + static_cast<int>(0.5f*rightmargin) , 2*topTextMargin+squareHeight );
        }
        else // single cursor position
        {
            if( cursor < aRects.length() )
                cursorx = aRects.at(cursor).left() - static_cast<int>(0.25f*rightmargin);
            else
                cursorx = aRects.last().right() + static_cast<int>(0.75f*rightmargin);

            painter.drawLine(cursorx,topTextMargin,cursorx,topTextMargin+squareHeight);
        }
    }
}

void CharacterWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    int index = whichGlyph( transform.inverted().map( event->pos() ) );
    if (index != -1)
    {
        emit characterDoubleClicked(theString[index]);
    }
}

QFont CharacterWidget::font()
{
    return displayFont;
}

void CharacterWidget::cursorPosition(int old, int newpos)
{
    Q_UNUSED(old);

    cursor = newpos;

    // oddly, characters greater than 0xffff are counted as two
    for(int i=0; i<cursor && i>=0; i++)
    {
        if( theString[i] > 0xffff )
        {
            cursor--;
            cursor = qMax(cursor,0);
        }
    }

    repaint();
}

bool CharacterWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        int index = whichGlyph( transform.inverted().map( helpEvent->pos() ) );
        if (index != -1)
        {
            QToolTip::showText(helpEvent->globalPos(), mDbAdapter->nameFromCodepoint( theString[index] ) );
        }
        else
        {
            QToolTip::hideText();
            event->ignore();
        }
        return true;
    }
    return QWidget::event(event);
}

int CharacterWidget::whichGlyph(QPoint pos)
{
    for(int i=0; i < theString.count(); i++)
        if( aRects.at(i).contains(pos) )
            return i;
    return -1;
}

void CharacterWidget::updateSelection(int left,int length)
{
    // oddly, characters greater than 0xffff are counted as two
    for(int i=0; i<left && i>=0; i++)
    {
        if( theString[i] > 0xffff )
        {
            left--;
            left = qMax(left,0);
        }
    }

    selectionLeft = left;
    selectionLength = length;
    update();
}

void CharacterWidget::updateHasSelection(bool hasSelection)
{
    this->hasSelection = hasSelection;
    update();
}
