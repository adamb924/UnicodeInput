#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>

class QListView;
class QCheckBox;
class QListWidgetItem;
class QDockWidget;
class QCompleter;
class QSqlQueryModel;

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

private slots:
    void changeTopFont();
    void hexEntered();
    void glyphNameDoubleClicked(const QModelIndex &index);
    void appendCodepoint(quint32 codepoint);
    void addFirstReturnedResult();
    void fillInGlyphName(quint32 codepoint);

    void textentrySelectionChanged();

    void setDockVisible(bool visible);

    void updateQueryModel();

private:
    QListView *mNameView;
    QCheckBox *mSortByCodepoint;
    Ui::MainWindow *ui;
    QDockWidget *cpDock;
    QCompleter *completer;

    const DatabaseAdapter * mDbAdapter;
    QSqlQueryModel * mQueryModel;

    void createDock();
};

#endif // MAINWINDOW_H
