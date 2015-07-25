#include "addgamedialog.h"
#include "ui_addgamedialog.h"
#include "fgame.h"
#include <QFileDialog>
#include "fdb.h"
#include "flauncher.h"

AddGameDialog::AddGameDialog(QWidget *parent, QList<FLauncher> *launchers) :
    QDialog(parent),
    ui(new Ui::AddGameDialog)
{
    this->launchers = launchers;
    if(launchers == 0)
    {
        return;
    }
    ui->setupUi(this);
    game = FGame();
    if(launchers->length() > 0)
    {
        launchersAvailable = true;
        for(int i = 0; i < launchers->length(); i++)
        {
            FLauncher launcher = launchers->at(i);
            ui->launcherComboBox->addItem(launcher.getName(), QVariant(i));
            qDebug() << "Name:" << launcher.getName();
        }
    }

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
    if(ui->launcherEnabledCheckBox->isChecked())
    {
        game.setLauncher(launchers->at(ui->launcherComboBox->itemData(ui->launcherComboBox->currentIndex()).toInt()));
    }
    qDebug() << "Name: " << game.getName() << "command: " << game.getCommand() << "commandEdit: " << ui->gameCommandEdit->text() << game.getLauncher().getName();
    emit AddGameDialog::gameSet(game);
}

void AddGameDialog::on_gameCommandBrowse_clicked()
{
    QString command = QFileDialog::getOpenFileName(this, "Choose command");
    if(command.isEmpty())
        return;
    ui->gameCommandEdit->setText(command);
}

void AddGameDialog::on_launcherEnabledCheckBox_clicked()
{

    ui->launcherComboBox->setEnabled(ui->launcherEnabledCheckBox->isChecked());
}
