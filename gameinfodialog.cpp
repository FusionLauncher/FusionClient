#include "gameinfodialog.h"
#include "ui_gameinfodialog.h"
#include "GameDBArtSelectorDialog.h"
#include <fartmanager.h>
#include <QMessageBox>
GameInfoDialog::GameInfoDialog(FGame g, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameInfoDialog)
{
    ui->setupUi(this);
    game = g;
    ui->lineEdit->setText(game.getName());
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
    artmanager->getGameData(&game, "PC");

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
    artmanager->getGameData(&game, selectedGame);
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
