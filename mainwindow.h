#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QSqlDatabase>

class QTableView;
class QCheckBox;
class QListWidgetItem;
class QDockWidget;
class QCompleter;
class QSqlQueryModel;

class DatabaseAdapter;
class CodepointProxy;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupValidators();

    void setupGlyphNameAutocomplete();

    void readSettings();

private slots:
    void changeTopFont();
    void hexEntered();
    void decEntered();
    void glyphNameDoubleClicked(const QModelIndex &index);
    void appendCodepoint(quint32 codepoint);
    void addFirstReturnedResult();
    void fillInGlyphName(quint32 codepoint);

    void textentrySelectionChanged();

    void setDockVisible(bool visible);
    void setCompleterActive(bool visible);

    void setupQueryModel();
    void updateQueryModel();
    void setKeepWindowOnTop(bool stayOnTop);
    void setShowCodepoints(bool show);

    void changeSort(int logicalIndex, Qt::SortOrder order);
    void detailedResultsChanged(int state);
    void useDisplaySizeChanged(int state);

protected:
    void closeEvent(QCloseEvent *event);

private:
    QTableView *mNameView;
    Ui::MainWindow *ui;
    QDockWidget *cpDock;
    QCompleter *completer;
    QAction *stayOnTop, *showCodepoints;
    QCheckBox * mUseDisplaySize;

    QSqlQueryModel * mSubstringQueryModel;
    CodepointProxy * mProxyModel;

    void createDock();
    void setupOptionsMenu();
    void setDisplayFont(const QFont & font);
};

#endif // MAINWINDOW_H
