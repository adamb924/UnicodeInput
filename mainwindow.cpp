#include "mainwindow.h"



#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFontDialog>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QDockWidget>
#include <QListWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCheckBox>
#include <QScrollBar>
#include <QApplication>
#include <stdio.h>

#include <QtDebug>

MainWindow::MainWindow()
{
    unrecoverableError=0;

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
	QMessageBox::critical (0,"Fatal error", "The driver for the database is not available. It is unlikely that you will solve this on your own. Rather you had better contact the developer.");
	unrecoverableError=1;
	return;
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("names.db");
    if(!db.open())
    {
	    QMessageBox::information (this,"Error Message","There was a problem in opening the database. The program said: " + db.lastError().databaseText() + " It is unlikely that you will solve this on your own. Rather you had better contact the developer." );
	    unrecoverableError=1;
	    return;
    }
    QSqlQuery q;
    q.exec("select count(*) from names;");
    q.next();
    if(q.value(0).toInt()<21742)
    {
        QMessageBox::information (this,"Error Message","There was a problem in reading names.db. The ability to look up glyph names will either be impaired or completely unavailable." );
    }

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    QHBoxLayout *middleLayout = new QHBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QVBoxLayout *enterHexLayout = new QVBoxLayout;
    QVBoxLayout *enterGlyphNameLayout = new QVBoxLayout;

    cw = new CharacterWidget(this);

    top = new QLineEdit;
    top->setFont(QFont("Times New Roman",20));

    QPushButton *topFont = new QPushButton(tr("Change Font"),this);

    topLayout->addWidget(top);
    topLayout->addWidget(topFont);
    middleLayout->addWidget(cw);
    middleLayout->addStrut(10);
    enterHexLayout->addWidget(new QLabel(tr("Enter a hex value:")));
    enterGlyphNameLayout->addWidget(new QLabel(tr("Enter a glyph name:")));

    hex = new QLineEdit;
    glyphNameLabel = new QLabel;
    QRegExpValidator *hexval = new QRegExpValidator(QRegExp("[1234567890abcdefABCDEF]*"),this);
    hex->setValidator(hexval);

    glyphName = new QLineEdit;
    enterHexLayout->addWidget(hex);
    enterHexLayout->addWidget(glyphNameLabel);
    enterHexLayout->addStretch(99999);
    enterGlyphNameLayout->addWidget(glyphName);

    QHBoxLayout *numberLayout = new QHBoxLayout;
    numberToReturn = new QLineEdit("25");
    numberToReturn->setObjectName(QString::fromUtf8("numbertoreturn"));
    numberToReturn->setMaximumWidth(30);
    QValidator *validator = new QIntValidator(1, 100000, this);
    numberToReturn->setValidator(validator);
    numberToReturn->setEnabled(false);
    substringSearch = new QCheckBox(tr("Search for any substring"));
    numberLayout->addWidget(substringSearch);
    numberLayout->addSpacing(20);
    numberLayout->addWidget(new QLabel(tr("Returning up to")));
    numberLayout->addWidget(numberToReturn);
    numberLayout->addWidget(new QLabel(tr("results")));
    numberLayout->addStretch(99999);

    enterGlyphNameLayout->addLayout(numberLayout);

    bottomLayout->addLayout(enterHexLayout);
    bottomLayout->addLayout(enterGlyphNameLayout);

//    cw->updateFont(QFont("Unicode BMP Fallback SIL"));
    cw->updateFont(QFont());

    mainLayout->addLayout(topLayout,0);
    mainLayout->addLayout(middleLayout,0);
    mainLayout->addLayout(bottomLayout,0);
    mainLayout->addStretch(1000);

    // Codepoint Name Doc
    QDockWidget *cpDock = new QDockWidget("Codepoint Names",this);
    cpDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    QWidget *cpWidget = new QWidget(this);
    nameList = new QListWidget;
    sortByCodepoint = new QCheckBox(tr("Sort by codepoint order"));
    sortByCodepoint->setChecked(true);
    QVBoxLayout  *cpLayout = new QVBoxLayout;
    cpLayout->addWidget(nameList);
    cpLayout->addWidget(sortByCodepoint);

    cpWidget->setLayout(cpLayout);

    cpDock->setWidget(cpWidget);
    cpDock->setFloating(true);
    cpDock->resize(500,300);
    addDockWidget(Qt::LeftDockWidgetArea,cpDock);

    this->setMaximumHeight(225);

    widget->setLayout(mainLayout);
    this->setWindowTitle(tr("Unicode Input Utility (written by Adam Baker; released under GPL)"));

    connect(top,SIGNAL(selectionChanged()),this,SLOT(textentrySelectionChanged()));
    connect(top,SIGNAL(textChanged(QString)),cw,SLOT(updateText(QString)));
    connect(topFont,SIGNAL(clicked(bool)),this,SLOT(changeTopFont()));
    connect(hex,SIGNAL(returnPressed()),this,SLOT(hexEntered()));
    connect(glyphName, SIGNAL(textEdited(const QString &)), this, SLOT(searchGlyphName()));
    connect(glyphName,SIGNAL(returnPressed()),this,SLOT(addFirstReturnedResult()));
    connect(substringSearch, SIGNAL(clicked(bool)), numberToReturn, SLOT(setEnabled(bool)));
    connect(nameList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(glyphNameDoubleClicked(QListWidgetItem*)));
    connect(numberToReturn, SIGNAL(textEdited(const QString &)), this, SLOT(searchGlyphName()));
    connect(substringSearch, SIGNAL(clicked(bool)), this, SLOT(searchGlyphName()));
    connect(top,SIGNAL(cursorPositionChanged(int,int)),cw,SLOT(cursorPosition(int,int)));
//    connect(top,SIGNAL(cursorPositionChanged(int,int)),cw,SLOT(repaint()));
    connect(sortByCodepoint, SIGNAL(clicked()), this, SLOT(searchGlyphName()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::changeTopFont()
{
    top->setFont(QFontDialog::getFont(0, top->font()));
}

void MainWindow::appendCodepoint(quint32 codepoint)
{
    int pos = top->cursorPosition();
    QVector<quint32> array;
    array = top->text().toUcs4();
    array.insert(pos,codepoint);
    top->setText(top->text().fromUcs4((quint32*)array.data(),array.size()));
/*
    int positionIncrement = 1;
    if( codepoint > 0xffff )
	positionIncrement++;
    top->setCursorPosition(pos+positionIncrement);
*/
    top->setCursorPosition(pos+1);
}

quint32 MainWindow::uintFromHexCodepoint(QString codepoint)
{
    bool ok;
    return codepoint.toUInt(&ok,16);
}

void MainWindow::hexEntered()
{
    quint32 character = uintFromHexCodepoint(hex->text());
    glyphNameLabel->setText( nameFromCodepoint(character) );
    appendCodepoint(character);
}

QString MainWindow::nameFromCodepoint(quint32 character)
{
    QString unicode = QString("%1").arg(character,4,16,QLatin1Char('0')).toUpper();

    QString name="Codepoint not found in database!";
    QSqlQuery query(db);
    query.exec("select name from names where codepoint='"+ unicode +"';");
    if(query.next())
	name = query.value(0).toString();

    return name;
}


void MainWindow::glyphNameDoubleClicked(QListWidgetItem *item)
{
    // get the text from the widget item, and strip out everything but the hex codepoint
    QString name = item->text();
    name.replace(QRegExp("^.*\\+"),"");
    name.replace(")","");

    appendCodepoint(uintFromHexCodepoint(name));
}

void MainWindow::searchGlyphName()
{
    nameList->clear();

    QString maxcount;
    maxcount = numberToReturn->text();

    QString searchString = glyphName->text().toUpper();
    if(!searchString.length()) { return; }

    QString order("name");
    if( sortByCodepoint->isChecked() )
	order = "length(codepoint),codepoint";

    QSqlQuery query(db);
    query.exec("select name,codepoint from names where substr(name,1," + QString::number(searchString.length()) + ")='" + searchString  + "' order by "+order+";");
    while(query.next())
    {
	nameList->addItem(query.value(0).toString() + " (U+" + query.value(1).toString() + ")");
    }

    if(substringSearch->isChecked() && searchString.length()>1)
    {
	query.exec("select name,codepoint from names where name like '%_" + searchString  + "%' order by "+order+" limit " + maxcount + " ;");

	while(query.next())
	    nameList->addItem(query.value(0).toString() + " (U+" + query.value(1).toString() + ")");
    }

    nameList->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::addFirstReturnedResult()
{
    if(nameList->count() < 1)
	return;
    else
	glyphNameDoubleClicked(nameList->item(0));

}

void MainWindow::textentrySelectionChanged()
{
    cw->updateHasSelection(top->hasSelectedText());
    if( top->hasSelectedText() )
    {
	int length = top->selectedText().length();

	// oddly, characters greater than 0xffff are counted as two
	QVector<quint32> array = top->selectedText().toUcs4();
	for(int i=0; i<array.count(); i++)
	    if( array[i] > 0xffff )
		length--;

//	qDebug() << top->selectionStart() << top->selectedText().length() << length;

	cw->updateSelection(top->selectionStart(), length );
    }
}

