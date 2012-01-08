#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QLabel;
class QFontDialog;
class QMessageBox;
class QRegExpValidator;
class QDockWidget;
class QListWidget;
class QSqlQuery;
class QSqlError;
class QCheckBox;
class QScrollBar;
class QListWidgetItem;
QT_END_NAMESPACE

#include "characterwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLineEdit *top, *hex, *glyphName;
    CharacterWidget *cw;
    QListWidget *nameList;
    QSqlDatabase db;

    QCheckBox *substringSearch;
    QLineEdit *numberToReturn;
    QLabel *glyphNameLabel;
    QCheckBox *sortByCodepoint;

public:
    bool unrecoverableError;

private slots:
    void searchGlyphName();
    void changeTopFont();
    void hexEntered();
    void glyphNameDoubleClicked(QListWidgetItem *item);
    quint32 uintFromHexCodepoint(QString codepoint);
    void appendCodepoint(quint32 codepoint);
    void addFirstReturnedResult();

    void textentrySelectionChanged();

public:
    MainWindow();
    ~MainWindow();

    QString nameFromCodepoint(quint32 character);

};

#endif // MAINWINDOW_H
