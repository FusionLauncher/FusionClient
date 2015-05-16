#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fgame.h>
#include <fdb.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_launchGameButton_clicked();

    void resetDatabase();

    void on_addGameButton_clicked();

    void addGame(FGame game);

    void on_removeGameButton_clicked();

    void on_removeDatabaseAction_triggered();

    void on_refreshUIAction_triggered();

    void on_setStylesheetAction_triggered();

private:
    Ui::MainWindow *ui;
    FGame *game;
    FDB db;
    QList<FGame> gameList;
    void refreshList();
    void reloadStylesheet();
};

#endif // MAINWINDOW_H
