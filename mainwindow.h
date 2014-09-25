#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>

class QListWidget;
class QCheckBox;
class QListWidgetItem;

class DatabaseAdapter;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool databaseError() const;

private slots:
    void searchGlyphName();
    void changeTopFont();
    void hexEntered();
    void glyphNameDoubleClicked(QListWidgetItem *item);
    void appendCodepoint(quint32 codepoint);
    void addFirstReturnedResult();

    void textentrySelectionChanged();

private:
    QListWidget *mNameList;
    QCheckBox *mSortByCodepoint;
    Ui::MainWindow *ui;

    const DatabaseAdapter * mDbAdapter;

    void createDock();
};

#endif // MAINWINDOW_H
