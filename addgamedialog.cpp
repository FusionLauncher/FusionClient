#include "addgamedialog.h"
#include "ui_addgamedialog.h"
#include "fgame.h"
#include <QFileDialog>
#include <QMessageBox>
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
    QDir gameDir = QFileDialog::getExistingDirectory(this, tr("Choose the game directory"), QDir::homePath());
    game.setPath(gameDir.absolutePath());
    ui->gameDirLabel->setText(gameDir.absolutePath());
}

void AddGameDialog::on_chooseGameExecutableButton_clicked()
{
    QString file;
    QDir gameDir = QDir(game.getPath());
    file = QFileDialog::getOpenFileName(this, tr("Choose executable"), gameDir.absolutePath());
    if(file.isEmpty())
        return;
    file = gameDir.relativeFilePath(file);
    game.setExe(file);
    ui->gameExecutableLabel->setText(file);
}

void AddGameDialog::on_buttonBox_accepted()
{
    game.setName(ui->gameNameEdit->text());

    if(game.getName().length() <= 0) {
        QMessageBox::warning(this, tr("Please fill all fields!"), tr("You have to fill the name-field!"), QMessageBox::Ok);
        return;
    }

    if(game.getExe().length() <= 0) {
        QMessageBox::warning(this, tr("Please fill all fields!"), tr("You have to set an executable!"), QMessageBox::Ok);
        return;
    }

    if(game.getPath().length() <= 0) {
        QMessageBox::warning(this, tr("Please fill all fields!"), tr("You have to set a path!"), QMessageBox::Ok);
        return;
    }

    game.setCommand(ui->gameCommandEdit->text());
    game.setArgs(QStringList(ui->gameArgsEdit->text()));
    game.setType(Executable);
    if(ui->launcherEnabledCheckBox->isChecked())
    {
        if ((ui->launcherComboBox->itemText(ui->launcherComboBox->currentIndex()).isEmpty()) || (ui->launcherComboBox->itemText(ui->launcherComboBox->currentIndex()).isNull()))
        {

            ui->launcherEnabledCheckBox->setChecked(false);
        }
        else
        {
            game.setLauncher(launchers->at(ui->launcherComboBox->itemData(ui->launcherComboBox->currentIndex()).toInt()));
        }
    }

    emit AddGameDialog::gameSet(game);
    close();
}

void AddGameDialog::on_gameCommandBrowse_clicked()
{
    QString command = QFileDialog::getOpenFileName(this, tr("Choose command"));
    if(command.isEmpty())
        return;
    ui->gameCommandEdit->setText(command);
}

void AddGameDialog::on_launcherEnabledCheckBox_clicked()
{

    ui->launcherComboBox->setEnabled(ui->launcherEnabledCheckBox->isChecked());
}
