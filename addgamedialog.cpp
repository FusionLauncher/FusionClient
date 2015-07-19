#include "addgamedialog.h"
#include "ui_addgamedialog.h"
#include "fgame.h"
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
    ui->gameDirLabel->setText(gameDir.absolutePath());
}

void AddGameDialog::on_chooseGameExecutableButton_clicked()
{
    QString file;
    QDir gameDir = QDir(game.getPath());
    file = QFileDialog::getOpenFileName(this, "Choose executable", gameDir.absolutePath());
    if(file.isEmpty())
        return;
    file = gameDir.relativeFilePath(file);
    game.setExe(file);
    ui->gameExecutableLabel->setText(file);
}

void AddGameDialog::on_buttonBox_accepted()
{
    game.setName(ui->gameNameEdit->text());
    game.setCommand(ui->gameCommandEdit->text());
    game.setArgs(QStringList(ui->gameArgsEdit->text()));
    game.setType(Executable);
    qDebug() << "Name: " << game.getName() << "command: " << game.getCommand() << "commandEdit: " << ui->gameCommandEdit->text();
    emit AddGameDialog::gameSet(game);
}

void AddGameDialog::on_gameCommandBrowse_clicked()
{
    QString command = QFileDialog::getOpenFileName(this, "Choose command");
    if(command.isEmpty())
        return;
    ui->gameCommandEdit->setText(command);
}
