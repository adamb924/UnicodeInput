#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "databaseadapter.h"
#include "characterwidget.h"
#include "codepointproxy.h"

#include <QtGui>
#include <QtSql>

#include <QtDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DatabaseAdapter::initializeDatabase();

    setWindowFlags(Qt::Window);

    createDock();

    setupValidators();

    setupOptionsMenu();

    connect(ui->textEntry,SIGNAL(selectionChanged()),this,SLOT(textentrySelectionChanged()));
    connect(ui->textEntry,SIGNAL(textChanged(QString)),ui->characterWidget,SLOT(updateText(QString)));
    connect(ui->hex,SIGNAL(returnPressed()),this,SLOT(hexEntered()));
    connect(ui->decimalEdit,SIGNAL(returnPressed()),this,SLOT(decEntered()));
    connect(ui->glyphName, SIGNAL(textChanged(const QString &)), this, SLOT(updateQueryModel()));
    connect(ui->glyphName,SIGNAL(returnPressed()),this,SLOT(addFirstReturnedResult()));
    connect(mNameView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(glyphNameDoubleClicked(const QModelIndex &)));
    connect(ui->detailedResults, SIGNAL(clicked(bool)), this, SLOT(updateQueryModel()));
    connect(ui->substringSearch, SIGNAL(clicked(bool)), this, SLOT(updateQueryModel()));
    connect(ui->textEntry,SIGNAL(cursorPositionChanged(int,int)),ui->characterWidget,SLOT(cursorPosition(int,int)));
    connect(ui->characterWidget,SIGNAL(characterDoubleClicked(quint32)),this,SLOT(fillInGlyphName(quint32)));
    connect(mNameView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(changeSort(int, Qt::SortOrder)));
    connect(ui->detailedResults, SIGNAL(stateChanged(int)), this, SLOT(detailedResultsChanged(int)));
    connect(mUseDisplaySize, SIGNAL(stateChanged(int)), this, SLOT(useDisplaySizeChanged(int)));

    setupGlyphNameAutocomplete();
    setupQueryModel();
    readSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupValidators()
{
    QRegExpValidator *hexval = new QRegExpValidator(QRegExp("[1234567890abcdefABCDEF]*"),this);
    ui->hex->setValidator(hexval);

    QIntValidator *intval = new QIntValidator(0,0x10FFFF,this);
    ui->decimalEdit->setValidator( intval );
}

void MainWindow::setupGlyphNameAutocomplete()
{
    QSqlTableModel * autocompleteModel = new QSqlTableModel(this, QSqlDatabase::database() );
    autocompleteModel->setTable( "names" );
    autocompleteModel->select();

    completer = new QCompleter(autocompleteModel, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionColumn(1);
    ui->glyphName->setCompleter(completer);
}

void MainWindow::readSettings()
{
    QSettings settings("AdamBaker", "UnicodeInput");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    stayOnTop->setChecked( settings.value("keepWindowOnTop",false).toBool() );
    showCodepoints->setChecked( settings.value("showCodepoints",false).toBool() );
    mUseDisplaySize->setChecked( settings.value("useDisplaySize",false).toBool() );

    QFont f;
    f.fromString( settings.value("font").toString() );
    setDisplayFont(f);

    detailedResultsChanged(ui->detailedResults->checkState());
}

void MainWindow::setupOptionsMenu()
{
    QMenu * optionsMenu = new QMenu(ui->optionsButton);
    optionsMenu->addAction("Change Font...",this,SLOT(changeTopFont()));
    ui->optionsButton->setMenu(optionsMenu);

    stayOnTop = new QAction("Keep window on top",optionsMenu);
    stayOnTop->setCheckable(true);
    stayOnTop->setChecked(false);
    optionsMenu->addAction(stayOnTop);
    connect(stayOnTop,SIGNAL(toggled(bool)), this, SLOT(setKeepWindowOnTop(bool)) );

    showCodepoints = new QAction("Show codepoints",optionsMenu);
    showCodepoints->setCheckable(true);
    showCodepoints->setChecked(true);
    optionsMenu->addAction(showCodepoints);
    connect(showCodepoints,SIGNAL(toggled(bool)), this, SLOT(setShowCodepoints(bool)) );
}

void MainWindow::setDisplayFont(const QFont &font)
{
    ui->textEntry->setFont(font);
    mProxyModel->setFont(font);
    setFixedHeight(sizeHint().height());
}

void MainWindow::changeTopFont()
{
    QFont newFont = QFontDialog::getFont(nullptr, ui->textEntry->font(), this);
    ui->textEntry->setFont(newFont);
    setDisplayFont( newFont );
    mNameView->viewport()->update();
    mNameView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::appendCodepoint(quint32 codepoint)
{
    if( codepoint == 0xFFFF )
    {
        return;
    }
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
    ui->glyphNameLabel->setText( DatabaseAdapter::nameFromCodepoint(character) );
    appendCodepoint(character);
}

void MainWindow::decEntered()
{
    quint32 character = ui->decimalEdit->text().toUInt();
    ui->glyphNameLabel->setText( DatabaseAdapter::nameFromCodepoint(character) );
    appendCodepoint(character);
}

void MainWindow::glyphNameDoubleClicked(const QModelIndex &index)
{
    appendCodepoint(DatabaseAdapter::uintFromHexCodepoint( mSubstringQueryModel->record( index.row() ).value(2).toString() ));
}

void MainWindow::createDock()
{
    // Codepoint Name Doc
    cpDock = new QDockWidget("Search Results",this);
    cpDock->setObjectName("CodepointNames");
    cpDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    cpDock->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::MinimumExpanding));
    QWidget *cpWidget = new QWidget(this);
    mNameView = new QTableView;

    mNameView->setSortingEnabled(true);
    mNameView->horizontalHeader()->setSortIndicatorShown(true);
    mNameView->verticalHeader()->hide();
    mNameView->setSelectionBehavior( QAbstractItemView::SelectRows);
    mNameView->setSelectionMode( QAbstractItemView::SingleSelection );
    mNameView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    mUseDisplaySize = new QCheckBox(tr("Use same font size as the text input window"), cpWidget);

    QVBoxLayout  *cpLayout = new QVBoxLayout;
    cpLayout->addWidget(mNameView);
    cpLayout->addWidget(mUseDisplaySize);

    cpWidget->setLayout(cpLayout);

    cpDock->setWidget(cpWidget);
    cpDock->setFloating(true);
    cpDock->resize(500,300);
    cpDock->setVisible(false);
    addDockWidget(Qt::LeftDockWidgetArea,cpDock);

    // make the substring search dock appear and disappear
    connect(ui->detailedResults, SIGNAL(clicked(bool)), this, SLOT(setDockVisible(bool)));
    connect(cpDock, SIGNAL(visibilityChanged(bool)), ui->detailedResults, SLOT(setChecked(bool)) );
    connect(cpDock, SIGNAL(visibilityChanged(bool)), this, SLOT(setCompleterActive(bool)) );
}

void MainWindow::addFirstReturnedResult()
{
    appendCodepoint( DatabaseAdapter::codepointFromName( ui->glyphName->text() ) );
}

void MainWindow::fillInGlyphName(quint32 codepoint)
{
    ui->glyphName->setText( DatabaseAdapter::nameFromCodepoint( codepoint ) );
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

void MainWindow::setupQueryModel()
{
    mSubstringQueryModel = new QSqlQueryModel(this);
    mProxyModel = new CodepointProxy(this);

    mSubstringQueryModel->setHeaderData(0,Qt::Horizontal,tr("Name"));
    mSubstringQueryModel->setHeaderData(1,Qt::Horizontal,tr("Character"));
    mSubstringQueryModel->setHeaderData(2,Qt::Horizontal,tr("Codepoint"));
    mProxyModel->setSourceModel(mSubstringQueryModel);
    mNameView->setModel(mProxyModel);
    mNameView->setColumnHidden(3,true);
    mNameView->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder );
    mNameView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::updateQueryModel()
{
    QSqlQuery q(QSqlDatabase::database());
    QString searchString = ui->glyphName->text().toUpper();

    if( ui->detailedResults->isChecked() && !searchString.isEmpty() ) // then the dock is visible
    {
        if( ui->substringSearch->isChecked() )
        {
            q.prepare("SELECT name,codepoint,codepoint,1 as ordering FROM names WHERE name LIKE ?||'%' "
                      "UNION "
                      "SELECT name,codepoint,codepoint,2 as ordering FROM names WHERE name LIKE '%_'||?||'%' ORDER BY ordering ASC;"
                      );
            q.bindValue(0, searchString );
            q.bindValue(1, searchString );
        }
        else
        {
            q.prepare("SELECT name,codepoint,codepoint FROM names WHERE name LIKE ?||'%';");
            q.bindValue(0, searchString );
        }
    }
    else
    {
        q.prepare("select name,codepoint,codepoint from names where 1=2;");
    }

    if(!q.exec()) {
        qDebug() << q.lastError();
    }
    mSubstringQueryModel->setQuery(q);

    mSubstringQueryModel->setHeaderData(0,Qt::Horizontal,tr("Name"));
    mSubstringQueryModel->setHeaderData(1,Qt::Horizontal,tr("Character"));
    mSubstringQueryModel->setHeaderData(2,Qt::Horizontal,tr("Codepoint"));
}

void MainWindow::setKeepWindowOnTop(bool stayOnTop)
{
    if( stayOnTop )
    {
        setWindowFlags(Qt::WindowStaysOnTopHint);
    }
    else
    {
        setWindowFlags(Qt::Window);
    }
    show();
    setDockVisible ( ui->substringSearch->isChecked() );
}

void MainWindow::setShowCodepoints(bool show)
{
    if( show )
    {
        ui->characterWidget->show();
    }
    else
    {
        ui->characterWidget->hide();
    }
    int oldWidth = width();
    adjustSize();
    resize( oldWidth, sizeHint().height() );
    setFixedHeight(sizeHint().height());
}

void MainWindow::changeSort(int logicalIndex, Qt::SortOrder order)
{
    mSubstringQueryModel->sort(logicalIndex, order);
    mProxyModel->setSourceModel(mSubstringQueryModel);
    mNameView->setModel(mProxyModel);
}

void MainWindow::detailedResultsChanged(int state)
{
    ui->substringSearch->setEnabled(state == Qt::Checked);
}

void MainWindow::useDisplaySizeChanged(int state)
{
    mProxyModel->setUseDisplaySize(state == Qt::Checked);
    mNameView->viewport()->update();
    mNameView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("AdamBaker", "UnicodeInput");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.setValue("keepWindowOnTop", static_cast<bool>(windowFlags() & Qt::WindowStaysOnTopHint) );
    settings.setValue("showCodepoints", ui->characterWidget->isVisible() );
    settings.setValue("font", ui->textEntry->font().toString() );
    settings.setValue("useDisplaySize", mUseDisplaySize->checkState() == Qt::Checked );
    QMainWindow::closeEvent(event);
}

