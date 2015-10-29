#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "fgamewidget.h"

#include <QMainWindow>
#include <fgame.h>
#include <fdb.h>
#include <fcrawler.h>
#include <fclientupdater.h>
#include <QListWidgetItem>
#include <QSystemTrayIcon>
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
    bool updateInProgress;
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
    void on_pb_LaunchRandom_clicked();

    void resizeDone();
    void reloadStylesheet();
    void refreshList();

    void on_launcherSet(FLauncher launcher);
    void on_launcherEdited(FLauncher launcher);

    void sttngsBtn_opnSttngs_triggered();
    void sttngsBtn_addGame_triggered();
    void sttngsBtn_edtGame_triggered();
    void sttngsBtn_reportBug_triggered();

    void launchRandomGame();

    void trayLaunchGame();
    void on_ShowHideClicked();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void changeEvent(QEvent*);

private:

    Ui::MainWindow *ui;
    FGame *game;
    FCrawler crawler;
    FDB db;
    FClientUpdater *cUpdater = new FClientUpdater();

    QList<FGame*> gameList;
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

    QString currentLanguage;
    QTranslator appTranslator;

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
    void checkForUpdates();

    QSystemTrayIcon *trayIcon;
    void createTrayIcon();
    void switchTranslator(QTranslator &translator, const QString &filename);
    void loadLanguage(const QString &rLanguage);
};

#endif // MAINWINDOW_H
