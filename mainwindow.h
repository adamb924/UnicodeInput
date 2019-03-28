#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>

class QListWidget;
class QCheckBox;
class QListWidgetItem;
class QDockWidget;
class QCompleter;

class DatabaseAdapter;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool databaseError() const;

private slots:
    void searchGlyphName();
    void changeTopFont();
    void hexEntered();
    void glyphNameDoubleClicked(QListWidgetItem *item);
    void appendCodepoint(quint32 codepoint);
    void addFirstReturnedResult();
    void fillInGlyphName(quint32 codepoint);

    void textentrySelectionChanged();

    void setDockVisible(bool visible);

private:
    QListWidget *mNameList;
    QCheckBox *mSortByCodepoint;
    Ui::MainWindow *ui;
    QDockWidget *cpDock;
    QCompleter *completer;

    const DatabaseAdapter * mDbAdapter;

    void createDock();
};

#endif // MAINWINDOW_H
