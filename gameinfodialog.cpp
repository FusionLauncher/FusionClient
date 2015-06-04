#include "gameinfodialog.h"
#include "ui_gameinfodialog.h"
#include "gamedbartselectordialog.h"

#include <fartmanager.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFileInfo>


GameInfoDialog::GameInfoDialog(FGame *g, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameInfoDialog)
{
    ui->setupUi(this);
    game = g;
    ui->lineEdit->setText(game->getName());

    runningDownloads = 0;
    totalDownloads = 0;
    lastDir =  "/";
}

GameInfoDialog::~GameInfoDialog()
{
    delete ui;
}

void GameInfoDialog::on_downloadArtButton_clicked()
{
    FArtManager *artmanager = new FArtManager();
    connect(artmanager, SIGNAL(startedDownload()), this, SLOT(downloadStarted()));
    connect(artmanager, SIGNAL(finishedDownload()), this, SLOT(downloadFinished()));
    connect(artmanager, SIGNAL(foundMultipleGames(QList<TheGameDBStorage*>)),  this, SLOT(on_foundMultipleGames(QList<TheGameDBStorage*>)));
    artmanager->getGameData(game, "PC");

}

void GameInfoDialog::on_ShowArtworkFolder_clicked()
{
    QDesktopServices::openUrl(game->getArtworkDir());
}

void GameInfoDialog::openFile(QString destFileName) {

    QFileInfo fi;
    QString file = QFileDialog::getOpenFileName(this, "Choose Artwork", lastDir, "Images (*.png *.jpg)");
    if(file.isEmpty())
        return;
    else {
        fi  = QFileInfo(file);
        lastDir = fi.absoluteFilePath();
        if(fi.exists()) {
            FArtManager fa(game);
            fa.importArtwork(fi, destFileName);
        }

    }
}

void GameInfoDialog::on_importBannerButton_clicked()
{
    openFile("banner");

}

void GameInfoDialog::on_importClearartButton_clicked()
{
    openFile("clearlogo");
}

void GameInfoDialog::on_importCoverButton_clicked()
{
    openFile("boxart");
}

void GameInfoDialog::on_importFanartButton_clicked()
{
    openFile("fanart");
}


void GameInfoDialog::on_foundMultipleGames(QList<TheGameDBStorage*> Games) {
    GameDBArtSelectorDialog *dialog = new GameDBArtSelectorDialog(Games, this);
    connect(dialog, SIGNAL(gameSelected(TheGameDBStorage*)), this, SLOT(on_gameSelected(TheGameDBStorage*)));
    dialog->exec();
}

void GameInfoDialog::on_gameSelected(TheGameDBStorage* selectedGame) {
    FArtManager *artmanager = new FArtManager();
    connect(artmanager, SIGNAL(startedDownload()), this, SLOT(downloadStarted()));
    connect(artmanager, SIGNAL(finishedDownload()), this, SLOT(downloadFinished()));
    connect(artmanager, SIGNAL(foundMultipleGames(QList<TheGameDBStorage*>)),  this, SLOT(on_foundMultipleGames(QList<TheGameDBStorage*>)));
    artmanager->getGameData(game, selectedGame);
}

void GameInfoDialog::downloadFinished() {
    --runningDownloads;
    ui->label_2->setText("Running Downloads:" + QString::number(runningDownloads));
    if(runningDownloads<=0)
        QMessageBox::information(this, "Downloads finished", "Finished " + QString::number(totalDownloads) + " download(s)");
}

void GameInfoDialog::downloadStarted() {
    ++runningDownloads;
    ++totalDownloads;
    ui->label_2->setText("Running Downloads:" + QString::number(runningDownloads));
}
