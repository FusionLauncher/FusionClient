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
    FArtManager artmanager;
    artmanager.getGameData(game.getName(), "PC");

}
