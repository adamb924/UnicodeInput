#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupDatabase();
    createDock();

    QRegExpValidator *hexval = new QRegExpValidator(QRegExp("[1234567890abcdefABCDEF]*"),this);
    ui->hex->setValidator(hexval);

    QValidator *validator = new QIntValidator(1, 100000, this);
    ui->numberToReturn->setValidator(validator);

    connect(ui->textEntry,SIGNAL(selectionChanged()),this,SLOT(textentrySelectionChanged()));
    connect(ui->textEntry,SIGNAL(textChanged(QString)),ui->characterWidget,SLOT(updateText(QString)));
    connect(ui->changeFont,SIGNAL(clicked(bool)),this,SLOT(changeTopFont()));
    connect(ui->hex,SIGNAL(returnPressed()),this,SLOT(hexEntered()));
    connect(ui->glyphName, SIGNAL(textEdited(const QString &)), this, SLOT(searchGlyphName()));
    connect(ui->glyphName,SIGNAL(returnPressed()),this,SLOT(addFirstReturnedResult()));
    connect(ui->substringSearch, SIGNAL(clicked(bool)), ui->numberToReturn, SLOT(setEnabled(bool)));
    connect(nameList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(glyphNameDoubleClicked(QListWidgetItem*)));
    connect(ui->numberToReturn, SIGNAL(textEdited(const QString &)), this, SLOT(searchGlyphName()));
    connect(ui->substringSearch, SIGNAL(clicked(bool)), this, SLOT(searchGlyphName()));
    connect(ui->textEntry,SIGNAL(cursorPositionChanged(int,int)),ui->characterWidget,SLOT(cursorPosition(int,int)));
    connect(sortByCodepoint, SIGNAL(clicked()), this, SLOT(searchGlyphName()));

    setFixedHeight(sizeHint().height());
}

MainWindow::~MainWindow()
{

}

bool MainWindow::databaseError() const
{
    return mbDatabaseError;
}

void MainWindow::changeTopFont()
{
    ui->textEntry->setFont(QFontDialog::getFont(0, ui->textEntry->font()));
}

void MainWindow::appendCodepoint(quint32 codepoint)
{
    int pos = ui->textEntry->cursorPosition();
    QVector<quint32> array;
    array = ui->textEntry->text().toUcs4();
    array.insert(pos,codepoint);
    ui->textEntry->setText(ui->textEntry->text().fromUcs4((quint32*)array.data(),array.size()));
    ui->textEntry->setCursorPosition(pos+1);
}

quint32 MainWindow::uintFromHexCodepoint(QString codepoint)
{
    bool ok;
    return codepoint.toUInt(&ok,16);
}

void MainWindow::hexEntered()
{
    quint32 character = uintFromHexCodepoint(ui->hex->text());
    ui->glyphNameLabel->setText( nameFromCodepoint(character) );
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

void MainWindow::setupDatabase()
{
    mbDatabaseError=0;
    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
    {
        QMessageBox::critical (0,"Fatal error", "The driver for the database is not available. It is unlikely that you will solve this on your own. Rather you had better contact the developer.");
        mbDatabaseError=1;
        return;
    }
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("names.db");
    if(!db.open())
    {
        QMessageBox::information (this,"Error Message","There was a problem in opening the database. The program said: " + db.lastError().databaseText() + " It is unlikely that you will solve this on your own. Rather you had better contact the developer." );
        mbDatabaseError=1;
        return;
    }
    QSqlQuery q;
    q.exec("select count(*) from names;");
    q.next();
    if(q.value(0).toInt()<21742)
    {
        QMessageBox::information (this,"Error Message","There was a problem in reading names.db. The ability to look up glyph names will either be impaired or completely unavailable." );
    }
}

void MainWindow::createDock()
{
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
}

void MainWindow::searchGlyphName()
{
    nameList->clear();

    QString maxcount;
    maxcount = ui->numberToReturn->text();

    QString searchString = ui->glyphName->text().toUpper();
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

    if(ui->substringSearch->isChecked() && searchString.length()>1)
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
    ui->characterWidget->updateHasSelection(ui->textEntry->hasSelectedText());
    if( ui->textEntry->hasSelectedText() )
    {
        int length = ui->textEntry->selectedText().length();

        // oddly, characters greater than 0xffff are counted as two
        QVector<quint32> array = ui->textEntry->selectedText().toUcs4();
        for(int i=0; i<array.count(); i++)
            if( array[i] > 0xffff )
                length--;

        ui->characterWidget->updateSelection(ui->textEntry->selectionStart(), length );
    }
}

