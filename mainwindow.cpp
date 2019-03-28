#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseadapter.h"
#include "characterwidget.h"

#include <QApplication>

#include <QListWidget>
#include <QFontDialog>
#include <QDockWidget>
#include <QScrollBar>

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mDbAdapter = new DatabaseAdapter;
    ui->characterWidget->setDbAdapter(mDbAdapter);

    createDock();

    QRegExpValidator *hexval = new QRegExpValidator(QRegExp("[1234567890abcdefABCDEF]*"),this);
    ui->hex->setValidator(hexval);

    QValidator *validator = new QIntValidator(1, 100000, this);
    ui->numberToReturn->setValidator(validator);

    ui->characterWidget->updateCharacterDisplayFont( ui->textEntry->font() );

    connect(ui->textEntry,SIGNAL(selectionChanged()),this,SLOT(textentrySelectionChanged()));
    connect(ui->textEntry,SIGNAL(textChanged(QString)),ui->characterWidget,SLOT(updateText(QString)));
    connect(ui->changeFont,SIGNAL(clicked(bool)),this,SLOT(changeTopFont()));
    connect(ui->hex,SIGNAL(returnPressed()),this,SLOT(hexEntered()));
    connect(ui->glyphName, SIGNAL(textEdited(const QString &)), this, SLOT(searchGlyphName()));
    connect(ui->glyphName,SIGNAL(returnPressed()),this,SLOT(addFirstReturnedResult()));
    connect(ui->substringSearch, SIGNAL(clicked(bool)), ui->numberToReturn, SLOT(setEnabled(bool)));
    connect(mNameList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(glyphNameDoubleClicked(QListWidgetItem*)));
    connect(ui->numberToReturn, SIGNAL(textEdited(const QString &)), this, SLOT(searchGlyphName()));
    connect(ui->substringSearch, SIGNAL(clicked(bool)), this, SLOT(searchGlyphName()));
    connect(ui->textEntry,SIGNAL(cursorPositionChanged(int,int)),ui->characterWidget,SLOT(cursorPosition(int,int)));
    connect(mSortByCodepoint, SIGNAL(clicked()), this, SLOT(searchGlyphName()));
    connect(ui->characterWidget,SIGNAL(characterDoubleClicked(quint32)),this,SLOT(fillInGlyphName(quint32)));

    setFixedHeight(sizeHint().height());
}

MainWindow::~MainWindow()
{

}

bool MainWindow::databaseError() const
{
    return mDbAdapter->databaseError();
}

void MainWindow::changeTopFont()
{
    QFont newFont = QFontDialog::getFont(0, ui->textEntry->font());
    ui->textEntry->setFont(newFont);
    ui->characterWidget->updateCharacterDisplayFont( newFont );
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

void MainWindow::hexEntered()
{
    quint32 character = DatabaseAdapter::uintFromHexCodepoint(ui->hex->text());
    ui->glyphNameLabel->setText( mDbAdapter->nameFromCodepoint(character) );
    appendCodepoint(character);
}


void MainWindow::glyphNameDoubleClicked(QListWidgetItem *item)
{
    // get the text from the widget item, and strip out everything but the hex codepoint
    QString name = item->text();
    name.replace(QRegExp("^.*\\+"),"");
    name.replace(")","");

    appendCodepoint(DatabaseAdapter::uintFromHexCodepoint(name));
}

void MainWindow::createDock()
{
    // Codepoint Name Doc
    QDockWidget *cpDock = new QDockWidget("Codepoint Names",this);
    cpDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    QWidget *cpWidget = new QWidget(this);
    mNameList = new QListWidget;
    mSortByCodepoint = new QCheckBox(tr("Sort by codepoint order"));
    mSortByCodepoint->setChecked(true);
    QVBoxLayout  *cpLayout = new QVBoxLayout;
    cpLayout->addWidget(mNameList);
    cpLayout->addWidget(mSortByCodepoint);

    cpWidget->setLayout(cpLayout);

    cpDock->setWidget(cpWidget);
    cpDock->setFloating(true);
    cpDock->resize(500,300);
    addDockWidget(Qt::LeftDockWidgetArea,cpDock);
}

void MainWindow::searchGlyphName()
{
    mNameList->clear();
    mNameList->addItems( mDbAdapter->searchGlyphName( ui->glyphName->text().toUpper(), ui->numberToReturn->text().toInt() , ui->substringSearch->isChecked(), mSortByCodepoint->isChecked() ) );
    mNameList->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::addFirstReturnedResult()
{
    if(mNameList->count() < 1)
        return;
    else
        glyphNameDoubleClicked(mNameList->item(0));

}

void MainWindow::fillInGlyphName(quint32 codepoint)
{
    ui->glyphName->setText(mDbAdapter->nameFromCodepoint( codepoint ));
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

