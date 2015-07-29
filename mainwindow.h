#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "fgamewidget.h"

#include <QMainWindow>
#include <fgame.h>
#include <fdb.h>
#include <fcrawler.h>
#include <QListWidgetItem>
#include <QMenu>

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
    void addGame(FGame game);
    void on_actionSwitch_View_triggered();

    void onGameClick(FGame *game, QObject *sender = NULL);
    void onGameDoubleClicked(FGame *game, QObject *sender);
    void onGameRightClicked(FGame *game, QObject *sender);
    void on_GameInfoDialogFinished();

    void on_pb_Min_clicked();
    void on_pb_Max_clicked();
    void on_pb_Close_clicked();

    void on_tgw_GameIconButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void ShowSettingsContextMenu(const QPoint& pos);
    void on_pb_Settings_clicked();
    void on_pb_LaunchGame_clicked();


    void resizeDone();
    void setWatchedFolders(QList<QDir> folders);
    void reloadStylesheet();
    void refreshList();

    void on_launcherSet(FLauncher launcher);
    void sttngsBtn_opnSttngs_triggered();
    void sttngsBtn_mngLib_triggered();
    void sttngsBtn_addLnchr_triggered();
    void sttngsBtn_addGame_triggered();
    void sttngsBtn_edtGame_triggered();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);


private:
    Ui::MainWindow *ui;
    FGame *game;
    FCrawler crawler;
    FDB db;

    QList<FGame> gameList;
    QList<FGameWidget*> gameWidgetList;

    void changeView();

    QLayout *gameScrollLayout;
    QString currentStyle;
    QMenu *settingsMenu;

    int currentView;
    void setView();

    //Save GUI-Size on resize
    void resizeEvent(QResizeEvent *event);
    QTimer resizeTimer;

    //Frameless Moving
    QPoint dragPosition;
    QSize initSize;
    QRect initPos;
    bool dragEnabled;
    bool resizeHeightEnabled;
    bool resizeWidthEnabled;
    bool resizeWidthEnabledInv;
    void prepareResize(QMouseEvent *event);

    QGraphicsScene* sceneCover;
    QGraphicsPixmapItem* itemCover;
};

#endif // MAINWINDOW_H
