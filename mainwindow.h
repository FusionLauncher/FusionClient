#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "fgamewidget.h"

#include <QMainWindow>
#include <fgame.h>
#include <fdb.h>
#include <fcrawler.h>
#include <QListWidgetItem>

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
    void resetDatabase();
    void addGame(FGame game);
    void on_removeDatabaseAction_triggered();
    void on_refreshUIAction_triggered();
    void on_setStylesheetAction_triggered();
    void on_libAddGameAction_triggered();
    void on_libAddLibAction_triggered();
    void on_actionSwitch_View_triggered();
    void setWatchedFolders(QList<QDir> folders);


    void onGameClick(FGame *game, QObject *sender = NULL);
    void onGameDoubleClicked(FGame *game, QObject *sender);
    void onGameRightClicked(FGame *game, QObject *sender);
    void on_GameInfoDialogFinished(int r);

    void on_tabButton_Store_clicked();
    void on_tabButton_Games_clicked();
    void on_tabButton_Community_clicked();

    void on_tgw_GameIconButton_clicked();
    void on_tgw_pb_Artwork_clicked();

    void on_simpleGameList_itemClicked(QListWidgetItem * item);
private:
    Ui::MainWindow *ui;
    FGame *game;
    FCrawler crawler;
    FDB db;

    QList<FGame> gameList;
    QList<FGameWidget*> gameWidgetList;

    void refreshList();
    void reloadStylesheet();
    void changeView();

    QLayout *gameScrollLayout;
    QString currentStyle;

    int currentView;
    void setView();
};

#endif // MAINWINDOW_H
