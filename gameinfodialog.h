#ifndef GAMEINFODIALOG_H
#define GAMEINFODIALOG_H

#include <QDialog>
#include <fgame.h>
#include <thegamedbstorage.h>

namespace Ui {
class GameInfoDialog;
}

class GameInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameInfoDialog(FGame *g, QWidget *parent = 0);
    ~GameInfoDialog();
    FGame *game;
private:
    Ui::GameInfoDialog *ui;
    int runningDownloads;
    int totalDownloads;
    QString lastDir;
    void openFile(QString destFileName);
private slots:
    void on_downloadArtButton_clicked();
    void on_ShowArtworkFolder_clicked();

    void on_importBannerButton_clicked();
    void on_importClearartButton_clicked();
    void on_importCoverButton_clicked();
    void on_importFanartButton_clicked();

    void downloadFinished();
    void downloadStarted();
    void on_foundMultipleGames(QList<TheGameDBStorage *> Games);
    void on_gameSelected(TheGameDBStorage *selectedGame);
};

#endif // GAMEINFODIALOG_H
