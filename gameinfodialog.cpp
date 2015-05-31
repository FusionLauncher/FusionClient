#include "gameinfodialog.h"
#include "ui_gameinfodialog.h"

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
    artmanager->getGameData(&game, "PC");

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
