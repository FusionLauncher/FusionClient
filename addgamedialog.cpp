#include "addgamedialog.h"
#include "ui_addgamedialog.h"
#include <fgame.h>
#include <QFileDialog>

AddGameDialog::AddGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGameDialog)
{
    ui->setupUi(this);
    game = FGame();
}

AddGameDialog::~AddGameDialog()
{
    delete ui;
}

void AddGameDialog::on_chooseGameDirButton_clicked()
{
    QDir gameDir = QFileDialog::getExistingDirectory(this, "Choose the game directory", QDir::homePath());
    game.setPath(gameDir.absolutePath());
    ui->gamePathLabel_2->setText(gameDir.absolutePath());
}

void AddGameDialog::on_chooseExecutableButton_clicked()
{
    QString file;
    QDir gameDir = QDir(game.getPath());
    file = QFileDialog::getOpenFileName(this, "Choose executable", gameDir.absolutePath());
    if(file.isEmpty())
        return;
    file = gameDir.relativeFilePath(file);
    game.setExe(file);
    ui->gameExeLabel_2->setText(file);
}

void AddGameDialog::on_buttonBox_accepted()
{
    game.setName(ui->nameEdit_2->text());
    emit AddGameDialog::gameSet(game);
}
