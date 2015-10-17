#include "fsettingsdialog.h"
#include "ui_fsettingsdialog.h"
#include <QDebug>
#include <fartmanager.h>
#include <libfusion.h>
#include <QMessageBox>
#include <QDesktopServices>


FSettingsDialog::FSettingsDialog(FDB *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FSettingsDialog)
{
    ui->setupUi(this);
    this->db = db;

    //Hast to be the Same order as the Stacked Widget.
   ui->listWidget->addItem(tr("General"));
   ui->listWidget->addItem(tr("Database"));
   ui->listWidget->addItem(tr("Interface"));
   ui->listWidget->addItem(tr("Artwork"));
   ui->listWidget->addItem(tr("Watched Folders"));
   ui->listWidget->addItem(tr("Launchers"));
//   ui->listWidget->addItem(tr("Savegame-Sync"));
   ui->listWidget->setCurrentRow(0);

   ui->le_Stylesheet->setText(db->getTextPref("stylesheet"));
   ui->cb_ScanLibOnStartup->setChecked(db->getBoolPref("ScanLibsOnStartup", true));


   ui->cb_Artwork_UseCache->setChecked(db->getBoolPref("useArtworkCache", true));

   ui->lbl_gen_Version->setText(VersionString);

   ui->cb_int_laguage->addItem("English", "en");
   ui->cb_int_laguage->addItem("German", "de");
   int idx = ui->cb_int_laguage->findData(db->getTextPref("currentLanguage", "en"));
   ui->cb_int_laguage->setCurrentIndex(idx);

    //##########################
    //WatchedFolders
    QList<FWatchedFolder> tmpList = db->getWatchedFoldersList();
    ui->lw_Folder_FolderList->clear();
    for(int i=0;i<tmpList.length();++i)
    {
        watchedFolders.insert(tmpList[i].getDirectory().absolutePath(), tmpList[i]);
        ui->lw_Folder_FolderList->addItem(tmpList[i].getDirectory().absolutePath());
    }

    //##########################
    // LAUNCHERS

    loadLaunchers();


    //##########################
    //Savegame-Sync
    ui->le_sync_path->setText(db->getTextPref("sync_target", ""));
    ui->sb_sync_backups->setValue(db->getIntPref("sync_No_of_Backups", 0));

    ui->cb_gen_ScanForUpdates->setChecked(db->getBoolPref("autoScanUpdates", true));
    ui->cb_gen_StartWithSystem->setChecked(db->getBoolPref("StartWithSystem", true));
    ui->cb_gen_useTrayIcon->setChecked(db->getBoolPref("useTrayIcon", true));


}

FSettingsDialog::~FSettingsDialog()
{
    delete ui;
}

void FSettingsDialog::loadLaunchers()
{
    QList<FLauncher> tmpLnchr = db->getLaunchers();
    ui->lw_launcher_launchers->clear();
    ui->cb_Folder_LauncherList->clear();
    ui->cb_Folder_LauncherList->blockSignals(true);
    ui->lw_launcher_launchers->blockSignals(true);
    for(int i = 0; i < tmpLnchr.length(); i++)
    {
        launchers.insert(tmpLnchr[i].getName(), tmpLnchr[i]);
        ui->lw_launcher_launchers->addItem(tmpLnchr[i].getName());
        //this is for the WatchedfFolders
        ui->cb_Folder_LauncherList->addItem(tmpLnchr[i].getName(), QVariant(tmpLnchr[i].getDbId()));
    }

    ui->cb_Folder_LauncherList->blockSignals(false);
    ui->lw_launcher_launchers->blockSignals(false);
    selectedLauncher = NULL;
}

void FSettingsDialog::on_listWidget_currentRowChanged(int i)
{
    ui->settingPages->setCurrentIndex(i);
}

void FSettingsDialog::on_pb_selectStylesheet_clicked()
{
    QString stylesheetFile = QFileDialog::getOpenFileName(this, tr("Choose stylesheet"), QDir::homePath(), "*.qss");
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

void FSettingsDialog::on_lw_Folder_FolderList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current) {
        selectedFolder = &watchedFolders[current->text()];
        ui->cb_Folder_ForLauncher->setChecked(selectedFolder->forLauncher);
        ui->cb_Folder_LauncherList->setEnabled(selectedFolder->forLauncher);

        ui->cb_Folder_LauncherList->blockSignals(true);
        if(selectedFolder->forLauncher) {
            int indx = ui->cb_Folder_LauncherList->findData(selectedFolder->getLauncherID());
            if(indx>=0)
                ui->cb_Folder_LauncherList->setCurrentIndex(indx);
        }
        ui->cb_Folder_LauncherList->blockSignals(false);
    }
}

void FSettingsDialog::on_cb_Folder_LauncherList_currentIndexChanged(int index)
{
    if(selectedFolder) {
        QVariant lID = ui->cb_Folder_LauncherList->itemData(index);
        selectedFolder->setLauncherID(lID.toInt());
    }
}

void FSettingsDialog::on_btn_launcher_Add_clicked()
{
    FLauncher l;
    if(ui->le_launcher_nameEdit->text().isEmpty())
    {
       QMessageBox::information(this, tr("Error"), tr("Please set a name."), QMessageBox::Ok);
        return;
    }
    if(ui->le_launcher_pathEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("Error"), tr("Please set a path."), QMessageBox::Ok);
        return;
    }

    l.setName(ui->le_launcher_nameEdit->text());
    l.setPath(ui->le_launcher_pathEdit->text());
    l.setArgs(ui->le_launcher_argEdit->text());
    l.setFileEndings(ui->le_launcher_suffix->text());

    if(launchers.contains(l.getName())) {
        QMessageBox::information(this, tr("Error"), tr("This name already exists!"), QMessageBox::Ok);
        return;
    }

    launchers.insert(l.getName(), l);
    ui->lw_launcher_launchers->addItem(l.getName());
    ui->le_launcher_nameEdit->setText("");
    ui->le_launcher_pathEdit->setText("");
    ui->le_launcher_argEdit->setText("");
    ui->le_launcher_suffix->setText("");
    db->updateLaunchers(launchers.values());
    loadLaunchers();
}

void FSettingsDialog::on_btn_launcher_remove_clicked()
{

}

void FSettingsDialog::on_btn_launcher_browsePath_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Choose launcher"), ".", "*");
    ui->le_launcher_pathEdit->setText(path);
}

void FSettingsDialog::on_lw_launcher_launchers_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current) {
        selectedLauncher = &launchers[current->text()];
        ui->le_launcher_nameEdit->setText(selectedLauncher->getName());
        ui->le_launcher_pathEdit->setText(selectedLauncher->getPath());
        ui->le_launcher_argEdit->setText(selectedLauncher->getArgs());
        ui->le_launcher_suffix->setText(selectedLauncher->getFileEndings());
    }
}

void FSettingsDialog::on_le_launcher_nameEdit_editingFinished()
{ updateLauncher(); }
void FSettingsDialog::on_le_launcher_pathEdit_editingFinished()
{ updateLauncher(); }
void FSettingsDialog::on_le_launcher_argEdit_editingFinished()
{ updateLauncher(); }
void FSettingsDialog::on_le_launcher_suffix_editingFinished()
{ updateLauncher(); }

void FSettingsDialog::on_pb_sync_FolderDialog_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Target-Dir"));
    ui->le_sync_path->setText(path);

}

void FSettingsDialog::on_pb_gen_ScanForUpdates_clicked()
{

}


void FSettingsDialog::updateLauncher() {
    if(selectedLauncher) {
        selectedLauncher->setName(ui->le_launcher_nameEdit->text());
        selectedLauncher->setPath(ui->le_launcher_pathEdit->text());
        selectedLauncher->setArgs(ui->le_launcher_argEdit->text());
        selectedLauncher->setFileEndings(ui->le_launcher_suffix->text());
    }
}

void FSettingsDialog::on_btn_Artwork_DownloadAll_clicked() {
    if(QMessageBox::warning(this, tr("Please confirm!"), tr("If artwork is found, existing artwork will be overwritten!"),QMessageBox::Ok, QMessageBox::Cancel) ==QMessageBox::Cancel)
        return;


    runningDownloads = 0;
    totalDownloads = 0;


   gameList = db->getGameList();
   for(int i=0;i<gameList.length();++i) {
       FArtManager *artmanager = new FArtManager();
       connect(artmanager, SIGNAL(startedDownload()), this, SLOT(downloadStarted()));
       connect(artmanager, SIGNAL(finishedDownload()), this, SLOT(downloadFinished()));
       artmanager->getGameData(gameList[i], "PC");
   }

}

void FSettingsDialog::on_btn_Folder_Add_clicked()
{
    QDir gameDir = QFileDialog::getExistingDirectory(this, tr("Choose the library folder"), QDir::homePath());
    ui->lw_Folder_FolderList->addItem(gameDir.absolutePath());

    FWatchedFolder w;
    w.setDirectory(gameDir);
    watchedFolders.insert(w.getDirectory().absolutePath(), w);
}

void FSettingsDialog::on_btn_Folder_Remove_clicked()
{
    if(watchedFolders.size() == 0)
    {
        return;
    }
    if(QMessageBox::warning(this, tr("Please confirm!"), tr("Do you really want to remove") + "\"" +selectedFolder->getDirectory().absolutePath()+ "\"?\r\n" + tr("The games inside won't be removed from your disk."),QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Ok) {
        watchedFolders.remove(selectedFolder->getDirectory().absolutePath());

        ui->lw_Folder_FolderList->clear();

        for(int i=0;i<watchedFolders.count();++i)
            ui->lw_Folder_FolderList->addItem(watchedFolders.values()[i].getDirectory().absolutePath());

    }

}

void FSettingsDialog::on_cb_Folder_ForLauncher_clicked()
{
    ui->cb_Folder_LauncherList->setEnabled(ui->cb_Folder_ForLauncher->checkState());

    if(selectedFolder != NULL)
        selectedFolder->forLauncher = (bool)ui->cb_Folder_ForLauncher->checkState();
}

void FSettingsDialog::downloadFinished() {
    --runningDownloads;
    ui->la_Artwork_DownloadStatus->setText(tr("Running downloads:") + QString::number(runningDownloads));
    if(runningDownloads<=0)
        QMessageBox::information(this, tr("Downloads finished"), tr("Finished %n download(s)",0,totalDownloads));
}

void FSettingsDialog::downloadStarted() {
    ++runningDownloads;
    ++totalDownloads;
    ui->la_Artwork_DownloadStatus->setText(tr("Running downloads:") + QString::number(runningDownloads));
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


   //##########################
   //WatchedFolders
    db->updateWatchedFolders(watchedFolders.values());

   //##########################
    //Launchers
    db->updateLaunchers(launchers.values());


    //##########################
    //Savegame-Sync
    db->updateTextPref("sync_target", ui->le_sync_path->text());
    db->updateIntPref("sync_No_of_Backups", ui->sb_sync_backups->value());


    db->updateBoolPref("autoScanUpdates", (bool)ui->cb_gen_ScanForUpdates->checkState());

    db->updateBoolPref("StartWithSystem", (bool)ui->cb_gen_StartWithSystem->checkState());
    db->updateBoolPref("useTrayIcon", (bool)ui->cb_gen_useTrayIcon->checkState());

    db->updateTextPref("currentLanguage", ui->cb_int_laguage->currentData().toString());

    #ifdef _WIN32
        QSettings bootUpSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        QString app_path = QCoreApplication::applicationFilePath().replace("/", "\\");

        if(db->getBoolPref("StartWithSystem", true)) {
            bootUpSettings.setValue("FusionLauncher", app_path);
        }else {
            bootUpSettings.remove("FusionLauncher");
        }
    #elif __linux

    #endif

}
