#ifndef GAMEINFODIALOG_H
#define GAMEINFODIALOG_H

#include <QDialog>
#include <fdb.h>
#include <fgame.h>
#include <thegamedbstorage.h>

namespace Ui {
class GameInfoDialog;
}

class GameInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameInfoDialog(FGame *g, FDB *database, QWidget *parent);
    ~GameInfoDialog();
private:
    Ui::GameInfoDialog *ui;
    int runningDownloads;
    int totalDownloads;
    QString lastDir;
    void openFile(QString destFileName);

    FGame *game;
    FDB *db;
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
    void on_buttonBox_accepted();
 signals:
    void reloadRequired();
};

#endif // GAMEINFODIALOG_H
