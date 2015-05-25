#include "watchedfoldersdialog.h"
#include "ui_watchedfoldersdialog.h"

#include <QFileDialog>

WatchedFoldersDialog::WatchedFoldersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WatchedFoldersDialog)
{
    ui->setupUi(this);
}

WatchedFoldersDialog::~WatchedFoldersDialog()
{
    delete ui;
}

void WatchedFoldersDialog::on_pb_AddFolder_clicked()
{
    QDir gameDir = QFileDialog::getExistingDirectory(this, "Choose the game directory", QDir::homePath());
    ui->listWidget->addItem(gameDir.absolutePath());
    folder.append(gameDir);
}

void WatchedFoldersDialog::on_pb_RemoveFolder_clicked()
{

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
