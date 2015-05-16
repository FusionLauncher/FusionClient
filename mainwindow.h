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
    void on_chooseGameButton_clicked();

    void refreshUi();

    void on_setNameButton_clicked();

    void on_launchGameButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void resetDatabase();

    void on_actionRefresh_UI_triggered();

    void on_actionRemove_database_2_triggered();

    void on_addGameButton_clicked();

    void addGame(FGame game);

    void on_removeGameButton_clicked();

private:
    Ui::MainWindow *ui;
    FGame *game;
    FDB db;
    QList<FGame> gameList;
    void refreshList();
};

#endif // MAINWINDOW_H
