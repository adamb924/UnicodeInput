#include <QtWidgets>

#include "characterwidget.h"
#include "mainwindow.h"

CharacterWidget::CharacterWidget(QWidget *parent)
    : QWidget(parent)
{
    mwParent = qobject_cast<MainWindow*>(parent);

    squareHeight = 30;
    squareWidth = 22;

    topTextMargin = 2;
    bottomTextMargin = 4;
    leftTextMargin = 3;
    rightmargin = 5;

    rectPadding = 4;

    this->setMinimumHeight(squareHeight+topTextMargin+bottomTextMargin);
}

void CharacterWidget::updateFont(const QFont &font)
{
    displayFont.setFamily(font.family());
    displayFont.setPixelSize(0.5*squareHeight);
    adjustSize();
    update();
}

QSize CharacterWidget::sizeHint() const
{
    return QSize(1000, topTextMargin + squareHeight + bottomTextMargin);
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
    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.setFont(displayFont);

    quint32 key, cursorx;
    QFontMetrics fontMetrics(displayFont);
    painter.setPen(QPen(Qt::black));

    // clear the memory of rects
    aRects.clear();

    for (int i=0; i < theString.count(); i++)
    {
	key = theString[i];

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

	int leftside = leftTextMargin; // the left position is the leftTextMargin
	if(aRects.length() > 0) // or the edge of the last rect, plus a margin
	    leftside = aRects.last().right() + rightmargin;

	aRects << QRect( leftside , topTextMargin, sw, squareHeight);

	painter.drawRect(aRects.last());
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
	    painter.drawRect( aRects.at(selectionLeft).left() - 0.5*leftTextMargin , 0 , aRects.at(selectionLeft+selectionLength-1).right() - aRects.at(selectionLeft).left() + leftTextMargin + 0.5*rightmargin , 2*topTextMargin+squareHeight );
	}
	else // single cursor position
	{
	    if( cursor < aRects.length() )
		cursorx = aRects.at(cursor).left() - 0.25*rightmargin;
	    else
		cursorx = aRects.last().right() + 0.75*rightmargin;

	    painter.drawLine(cursorx,topTextMargin,cursorx,topTextMargin+squareHeight);
	}
    }
}

QFont CharacterWidget::font()
{
    return displayFont;
}

void CharacterWidget::cursorPosition(int old, int newpos)
{
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
	int index = whichGlyph(helpEvent->pos());
	if (index != -1)
	{
	    QToolTip::showText(helpEvent->globalPos(), mwParent->nameFromCodepoint( theString[index] ) );
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
}

void CharacterWidget::updateHasSelection(bool hasSelection)
{
    this->hasSelection = hasSelection;
}
