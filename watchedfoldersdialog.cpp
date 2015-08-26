#include "watchedfoldersdialog.h"
#include "ui_watchedfoldersdialog.h"

#include <QFileDialog>
#include <fwatchedfolder.h>

WatchedFoldersDialog::WatchedFoldersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchedFoldersDialog)
{
    ui->setupUi(this);
    QList<FWatchedFolder> folders = db.getWatchedFoldersList();
    for(int i=0;i<folders.length();++i) {
        ui->listWidget->addItem(folders[i].getDirectory().absolutePath());
    //    folder.append(folders[i]);
    }
}

WatchedFoldersDialog::~WatchedFoldersDialog()
{
    delete ui;
}

void WatchedFoldersDialog::on_pb_AddFolder_clicked()
{
    QDir gameDir = QFileDialog::getExistingDirectory(this, "Choose the library folder (for example SteamApps)", QDir::homePath());
    ui->listWidget->addItem(gameDir.absolutePath());
    folder.append(gameDir);
}

void WatchedFoldersDialog::on_pb_RemoveFolder_clicked()
{
    int row = ui->listWidget->currentRow();
    folder.removeAt(row);
    ui->listWidget->clear();
    for(int i=0;i<folder.length();++i) {
        ui->listWidget->addItem(folder[i].absolutePath());
    }

}

void WatchedFoldersDialog::on_pb_Save_clicked()
{
    emit WatchedFoldersDialog::folderSet(folder);
    this->done(1);
}

void WatchedFoldersDialog::on_pb_Close_clicked()
{
    this->done(0);

}
