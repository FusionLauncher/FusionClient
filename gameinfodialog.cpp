#include "gameinfodialog.h"
#include "ui_gameinfodialog.h"

#include <fartmanager.h>

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
    connect(artmanager, SIGNAL(gotData(QString)), this, SLOT(gotData(QString)));
    artmanager->getGameData(&game, "PC");

}


void GameInfoDialog::gotData(QString data) {

}
