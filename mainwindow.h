#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
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

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    CharacterWidget *characterWidget;
    QListWidget *nameList;
    QSqlDatabase db;

    QCheckBox *sortByCodepoint;

    Ui::MainWindow *ui;

    bool mbDatabaseError;

    void setupDatabase();
    void createDock();

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
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool databaseError() const;

    QString nameFromCodepoint(quint32 character);

};

#endif // MAINWINDOW_H
