#include "gameinfodialog.h"
#include "ui_gameinfodialog.h"
#include "gamedbartselectordialog.h"

#include <fartmanager.h>

#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QFileInfo>
#include <fdb.h>


GameInfoDialog::GameInfoDialog(FGame *g, FDB *database, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameInfoDialog)
{
    ui->setupUi(this);
    game = g;
    this->db = database;

    ui->le_Title->setText(game->getName());
    ui->le_Exec->setText(game->getExe());
    ui->le_Directory->setText(game->getPath());
    ui->le_Type->setText(FGame::FGameTypeToStr(game->getType()));

    QString args;
    for(int i=0;i<game->getArgs().length();++i)
        args += " " + game->getArgs()[i];

    ui->le_Params->setText(args);


    runningDownloads = 0;
    totalDownloads = 0;
    lastDir =  "/";


    ui->aw_la_Cover->setStyleSheet("#aw_la_Cover{border-image:url("+ game->getArt(FArtBox) +") 0 0 0 0 stretch stretch}");
    ui->aw_la_Banner->setStyleSheet("#aw_la_Banner{border-image:url("+ game->getArt(FArtBanner) +") 0 0 0 0 repeat stretch}");
    ui->aw_la_Clearart->setStyleSheet("#aw_la_Clearart{border-image:url("+ game->getArt(FArtClearart) +") 0 0 0 0 stretch stretch}");
    ui->aw_la_Fanart->setStyleSheet("#aw_la_Fanart{border-image:url("+ game->getArt(FArtFanart) +") 0 0 0 0 stretch stretch}");

    QList<FLauncher> launchers = db->getLaunchers();
    for(int i = 0; i < launchers.length(); i++)
    {
        FLauncher launcher = launchers.at(i);
        ui->launcherComboBox->addItem(launcher.getName(), QVariant(launcher.getDbId()));
    }
    if(game->doesUseLauncher())
    {
        qDebug() << "Current launcher:" << game->getLauncher().getName() << ", id:" << game->getLauncher().getDbId();
        qDebug() << "Game is using launcher.";
        ui->launcherCheckBox->setChecked(true);
        ui->launcherComboBox->setEnabled(true);
        ui->launcherComboBox->setCurrentIndex(game->getLauncher().getDbId()-1);
    }

    //Savegame-Sync
    /*
    if(game->savegameSyncEndabled()) {
        ui->cb_useSavegameSync->setChecked(true);
        ui->le_savegameDir->setEnabled(true);
        ui->le_savegameDir->setText(game->getSavegameDir().absolutePath());
        ui->pb_setSavegameDir->setEnabled(true);
    }
    else {
        ui->cb_useSavegameSync->setChecked(false);
        ui->le_savegameDir->setEnabled(false);
        ui->pb_setSavegameDir->setEnabled(false);
    }
*/
}



void GameInfoDialog::on_chooseGameDirButton_clicked()
{

    QDir gameDir = QFileDialog::getExistingDirectory(this, "Choose the game directory", ui->le_Directory->text());
    if(gameDir.dirName()!=".")
        ui->le_Directory->setText(gameDir.absolutePath());
}

void GameInfoDialog::on_pb_deleteGame_clicked()
{
    QMessageBox::StandardButton btn = QMessageBox::warning(this, "Really delete game?", "Are you sure you want to delete \""  + game->getName() + "\"?", QMessageBox::Yes|QMessageBox::No);
    if(btn == QMessageBox::Yes) {
        db->removeGameById(game->dbId);
        emit reloadRequired();
        this->close();
    }
}

void GameInfoDialog::on_chooseGameExecutableButton_clicked()
{
    QString file;
    QDir gameDir = QDir(ui->le_Directory->text());
    file = QFileDialog::getOpenFileName(this, "Choose executable", gameDir.absolutePath());
    if(file.isEmpty())
        return;
    file = gameDir.relativeFilePath(file);
    ui->le_Exec->setText(file);
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

    ui->label_2->setText("Searching for artwork...");


}

void GameInfoDialog::on_ShowArtworkFolder_clicked()
{
    QDesktopServices::openUrl(game->getArtworkDir());
}

void GameInfoDialog::openFile(QString destFileName) {

    QFileInfo fi;
    QString file = QFileDialog::getOpenFileName(this, "Choose artwork", lastDir, "Images (*.png *.jpg)");
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
    openFile(FGame::FGameArtToStr(FArtBanner));

}

void GameInfoDialog::on_importClearartButton_clicked()
{
    openFile(FGame::FGameArtToStr(FArtClearart));
}

void GameInfoDialog::on_importCoverButton_clicked()
{
    openFile(FGame::FGameArtToStr(FArtBox));
}

void GameInfoDialog::on_importFanartButton_clicked()
{
    openFile(FGame::FGameArtToStr(FArtFanart));
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

void GameInfoDialog::on_buttonBox_accepted()
{
    game->setName(ui->le_Title->text());
    game->setExe(ui->le_Exec->text());
    game->setPath(ui->le_Directory->text());
    qDebug() << "Updating game.";
    if(ui->launcherCheckBox->isChecked())
    {
        game->setLauncher(db->getLauncher(ui->launcherComboBox->itemData(ui->launcherComboBox->currentIndex()).toInt()));
        qDebug() << "Launcher set, id:" <<  game->getLauncher().getDbId() << ", name:" << game->getLauncher().getName() << ", boxId:" << ui->launcherComboBox->itemData(ui->launcherComboBox->currentIndex()).toInt();
    }
    else
    {
        game->disableLauncher();
    }
    game->setArgs(QStringList(ui->le_Params->text()));


    if(ui->cb_useSavegameSync->checkState()) {
        game->setSavegameDir(ui->le_savegameDir->text());
    } else {
        game->setSavegameDir("");
    }

    db->updateGame(game);
    emit reloadRequired();
}

void GameInfoDialog::downloadFinished() {
    --runningDownloads;
    ui->label_2->setText("Running downloads:" + QString::number(runningDownloads));
    if(runningDownloads<=0)
        QMessageBox::information(this, "Downloads finished", "Finished " + QString::number(totalDownloads) + " download(s)");
}

void GameInfoDialog::downloadStarted() {
    ++runningDownloads;
    ++totalDownloads;
    ui->label_2->setText("Running downloads:" + QString::number(runningDownloads));
}


void GameInfoDialog::on_launcherCheckBox_clicked()
{
    ui->launcherComboBox->setEnabled(ui->launcherCheckBox->isChecked());
}


void GameInfoDialog::on_pb_setSavegameDir_clicked()
{
    QDir gameDir = QFileDialog::getExistingDirectory(this, "Choose the Savegame-Directory", ui->le_Directory->text());
    if(gameDir.dirName()!=".")
        ui->le_savegameDir->setText(gameDir.absolutePath());
}

void GameInfoDialog::on_cb_useSavegameSync_clicked()
{
    ui->cb_useSavegameSync->setChecked(ui->cb_useSavegameSync->checkState());
    ui->le_savegameDir->setEnabled(ui->cb_useSavegameSync->checkState());
  //  ui->pb_setSavegameDir->setEnabled(ui->cb_useSavegameSync->checkState());
}


