#include "fsettingsdialog.h"
#include "ui_fsettingsdialog.h"
#include <QDebug>
#include <fartmanager.h>
#include <QMessageBox>
#include <QDesktopServices>


FSettingsDialog::FSettingsDialog(FDB *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FSettingsDialog)
{
    ui->setupUi(this);
    this->db = db;

    //Hast to be the Same order as the Stacked Widget.
   ui->listWidget->addItem("General");
   ui->listWidget->addItem("Database");
   ui->listWidget->addItem("Interface");
   ui->listWidget->addItem("Artwork");
   ui->listWidget->setCurrentRow(0);

   ui->le_Stylesheet->setText(db->getTextPref("stylesheet"));
   ui->cb_ScanLibOnStartup->setChecked(db->getBoolPref("ScanLibsOnStartup", true));


   ui->cb_Artwork_UseCache->setChecked(db->getBoolPref("useArtworkCache", true));


}

FSettingsDialog::~FSettingsDialog()
{
    delete ui;
}

void FSettingsDialog::on_listWidget_currentRowChanged(int i)
{
    ui->settingPages->setCurrentIndex(i);
}

void FSettingsDialog::on_pb_selectStylesheet_clicked()
{
    QString stylesheetFile = QFileDialog::getOpenFileName(this, "Choose stylesheet", QDir::homePath(), "*.qss");
    qDebug() << "Stylesheet: " << stylesheetFile;
    if(QFile::exists(stylesheetFile))
    {
        qDebug() << "New stylesheet added: " << stylesheetFile;
        db->updateTextPref("stylesheet", stylesheetFile);
        emit reloadStylesheet();
    }
}

void FSettingsDialog::on_pb_ResetStylesheet_clicked()
{
    db->updateTextPref("stylesheet", ":/stylesheet.qss");
    emit reloadStylesheet();
}

void FSettingsDialog::on_pb_ScanNow_clicked()
{
    FCrawler crawler;
    crawler.scanAllFolders();
    emit reloadLibrary();
}

void FSettingsDialog::on_btn_Artwork_DownloadAll_clicked() {
    if(QMessageBox::warning(this, "Please confirm!", "If Fusion is able to find Artwork, existing Artwork will be overwritten!",QMessageBox::Ok, QMessageBox::Cancel) ==QMessageBox::Cancel)
        return;


    runningDownloads = 0;
    totalDownloads = 0;


   gameList = db->getGameList();
   for(int i=0;i<gameList.length();++i) {
       FArtManager *artmanager = new FArtManager();
       connect(artmanager, SIGNAL(startedDownload()), this, SLOT(downloadStarted()));
       connect(artmanager, SIGNAL(finishedDownload()), this, SLOT(downloadFinished()));
       artmanager->getGameData(&gameList[i], "PC");
   }

}

void FSettingsDialog::downloadFinished() {
    --runningDownloads;
    ui->la_Artwork_DownloadStatus->setText("Running Downloads:" + QString::number(runningDownloads));
    if(runningDownloads<=0)
        QMessageBox::information(this, "Downloads finished", "Finished " + QString::number(totalDownloads) + " download(s)");
}

void FSettingsDialog::downloadStarted() {
    ++runningDownloads;
    ++totalDownloads;
    ui->la_Artwork_DownloadStatus->setText("Running Downloads:" + QString::number(runningDownloads));
}

void FSettingsDialog::on_btn_Artwork_openCache_clicked() {
    QDesktopServices::openUrl(FGame::getCacheDir());

}


void FSettingsDialog::on_btn_Artwork_ClearCache_clicked() {
    QDir cacheDir(FGame::getCacheDir());

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    cacheDir.setNameFilters(QStringList()<<"*.*");
    QStringList steamFiles = cacheDir.entryList();
    for(int i=0;i<steamFiles.length();++i) {
        cacheDir.remove(steamFiles[i]);
    }
#else
   cacheDir.removeRecursively();
#endif


}



void FSettingsDialog::on_buttonBox_accepted()
{
   db->updateBoolPref("ScanLibsOnStartup", (bool)ui->cb_ScanLibOnStartup->checkState());

   //Artwortk-Page
   db->updateBoolPref("useArtworkCache", (bool)ui->cb_Artwork_UseCache->checkState());
}
