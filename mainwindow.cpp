#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseadapter.h"
#include "characterwidget.h"

#include <QApplication>

#include <QFontDialog>
#include <QDockWidget>
#include <QScrollBar>
#include <QCompleter>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QListView>
#include <QLineEdit>

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

    ui->characterWidget->updateCharacterDisplayFont( ui->textEntry->font() );

    connect(ui->textEntry,SIGNAL(selectionChanged()),this,SLOT(textentrySelectionChanged()));
    connect(ui->textEntry,SIGNAL(textChanged(QString)),ui->characterWidget,SLOT(updateText(QString)));
    connect(ui->changeFont,SIGNAL(clicked(bool)),this,SLOT(changeTopFont()));
    connect(ui->hex,SIGNAL(returnPressed()),this,SLOT(hexEntered()));
    connect(ui->glyphName, SIGNAL(textChanged(const QString &)), this, SLOT(updateQueryModel()));
    connect(ui->glyphName,SIGNAL(returnPressed()),this,SLOT(addFirstReturnedResult()));
    connect(mNameView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(glyphNameDoubleClicked(const QModelIndex &)));
    connect(ui->substringSearch, SIGNAL(clicked(bool)), this, SLOT(updateQueryModel()));
    connect(ui->textEntry,SIGNAL(cursorPositionChanged(int,int)),ui->characterWidget,SLOT(cursorPosition(int,int)));
    connect(mSortByCodepoint, SIGNAL(clicked()), this, SLOT(updateQueryModel()));
    connect(ui->characterWidget,SIGNAL(characterDoubleClicked(quint32)),this,SLOT(fillInGlyphName(quint32)));

    // make the substring search dock appear and disappear
    connect(ui->substringSearch, SIGNAL(clicked(bool)), this, SLOT(setDockVisible(bool)));
    connect(cpDock, SIGNAL(visibilityChanged(bool)), ui->substringSearch, SLOT(setChecked(bool)) );
    connect(cpDock, SIGNAL(visibilityChanged(bool)), this, SLOT(setCompleterActive(bool)) );

    QSqlTableModel * model = new QSqlTableModel(this, QSqlDatabase::database() );
    model->setTable( "names" );
    model->select();

    mQueryModel = new QSqlQueryModel;

    completer = new QCompleter(model, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(1);
    ui->glyphName->setCompleter(completer);

    setFixedHeight(sizeHint().height());
}

MainWindow::~MainWindow()
{

}

void MainWindow::changeTopFont()
{
    QFont newFont = QFontDialog::getFont(nullptr, ui->textEntry->font());
    ui->textEntry->setFont(newFont);
    ui->characterWidget->updateCharacterDisplayFont( newFont );
}

void MainWindow::appendCodepoint(quint32 codepoint)
{
    int pos = ui->textEntry->cursorPosition();
    QVector<quint32> array;
    array = ui->textEntry->text().toUcs4();
    array.insert(pos,codepoint);
    ui->textEntry->setText(ui->textEntry->text().fromUcs4(static_cast<quint32*>(array.data()),array.size()));
    ui->textEntry->setCursorPosition(pos+1);
}

void MainWindow::hexEntered()
{
    quint32 character = DatabaseAdapter::uintFromHexCodepoint(ui->hex->text());
    ui->glyphNameLabel->setText( mDbAdapter->nameFromCodepoint(character) );
    appendCodepoint(character);
}


void MainWindow::glyphNameDoubleClicked(const QModelIndex &index)
{
    appendCodepoint(DatabaseAdapter::uintFromHexCodepoint( mQueryModel->record( index.row() ).value(2).toString() ));
}

void MainWindow::createDock()
{
    // Codepoint Name Doc
    cpDock = new QDockWidget("Codepoint Names",this);
    cpDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    QWidget *cpWidget = new QWidget(this);
    mNameView = new QListView;
    mNameView->setModelColumn(0);
    mSortByCodepoint = new QCheckBox(tr("Sort by codepoint order"));
    mSortByCodepoint->setChecked(true);
    QVBoxLayout  *cpLayout = new QVBoxLayout;
    cpLayout->addWidget(mNameView);
    cpLayout->addWidget(mSortByCodepoint);

    cpWidget->setLayout(cpLayout);

    cpDock->setWidget(cpWidget);
    cpDock->setFloating(true);
    cpDock->resize(500,300);
    cpDock->setVisible(false);
    addDockWidget(Qt::LeftDockWidgetArea,cpDock);
}

void MainWindow::addFirstReturnedResult()
{
    appendCodepoint( mDbAdapter->codepointFromName( ui->glyphName->text() ) );
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

void MainWindow::setDockVisible(bool visible)
{
    cpDock->setVisible(visible);
    setCompleterActive(visible);
}

void MainWindow::setCompleterActive(bool visible)
{
    // if the dock is visible, disable autocomplete
    if(visible) {
        ui->glyphName->setCompleter(nullptr);
    }
    else {
        ui->glyphName->setCompleter(completer);
    }
}

void MainWindow::updateQueryModel()
{
    QSqlQuery q(QSqlDatabase::database());
    QString searchString = ui->glyphName->text().toUpper();

    if( ui->substringSearch->isChecked() ) // then the dock is visible
    {
        if( mSortByCodepoint->isChecked() )
        {
            q.prepare("SELECT name||' (U+'||codepoint||')',name,codepoint FROM names WHERE name LIKE ?||'%' "
                      "UNION "
                      "SELECT name||' (U+'||codepoint||')',name,codepoint FROM names WHERE name LIKE '%_'||?||'%' ORDER BY codepoint;");
        }
        else
        {
            q.prepare("SELECT name||' (U+'||codepoint||')',name,codepoint,1 as ordering FROM names WHERE name LIKE ?||'%' "
                      "UNION "
                      "SELECT name||' (U+'||codepoint||')',name,codepoint,2 as ordering FROM names WHERE name LIKE '%_'||?||'%' ORDER BY ordering ASC;"
                      );
        }
        q.bindValue(0, searchString );
        q.bindValue(1, searchString );
    }
    else
    {
        q.prepare("select * from names where 1=2;");
    }

    if(!q.exec()) {
        qDebug() << q.lastError();
    }
    mQueryModel->setQuery(q);
    mNameView->setModel(mQueryModel);
}

