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
    void resetDatabase();
    void addGame(FGame game);
    void on_removeDatabaseAction_triggered();
    void on_refreshUIAction_triggered();
    void openStylesheetDialog();
    void resetStylesheet();
    void on_libAddGameAction_triggered();
    void on_libAddLibAction_triggered();
    void on_actionSwitch_View_triggered();
    void setWatchedFolders(QList<QDir> folders);


    void onGameClick(FGame *game, QObject *sender = NULL);
    void onGameDoubleClicked(FGame *game, QObject *sender);
    void onGameRightClicked(FGame *game, QObject *sender);
    void on_GameInfoDialogFinished(int r);


    void on_tgw_GameIconButton_clicked();
    void on_tgw_pb_Artwork_clicked();

    void on_tabWidget_currentChanged(int index);

    void ShowSettingsContextMenu(const QPoint& pos);
    void on_pb_Settings_clicked();

    void resizeDone();
    void on_SettingsMenueClicked(QAction *action);
    //http://doc.qt.io/qt-5/qtwidgets-widgets-shapedclock-example.html
protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
 //   void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
//    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


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
    QMenu *settingsMenu;

    int currentView;
    void setView();

    //Save GUI-Size on resize
    void resizeEvent(QResizeEvent *event);
    QTimer resizeTimer;

    //Frameless Moving
    QPoint dragPosition;
    QSize initSize;
    bool dragEnabled;
    bool resizeHeightEnabled;
    bool resizeWidthEnabled;
};

#endif // MAINWINDOW_H
