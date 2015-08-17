#ifndef FSETTINGSDIALOG_H
#define FSETTINGSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <fdb.h>
#include <fcrawler.h>
#include <QFileDialog>

namespace Ui {
class FSettingsDialog;
}

class FSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FSettingsDialog(FDB *db, QWidget *parent = 0);
    ~FSettingsDialog();

private:

    Ui::FSettingsDialog *ui;
    FDB *db;

    int runningDownloads;
    int totalDownloads;
    QList<FGame> gameList;
private slots:
    void on_listWidget_currentRowChanged(int i);

    void on_pb_selectStylesheet_clicked();
    void on_pb_ResetStylesheet_clicked();
    void on_pb_ScanNow_clicked();



    void on_buttonBox_accepted();
    void downloadStarted();
    void downloadFinished();
    void on_btn_Artwork_ClearCache_clicked();
    void on_btn_Artwork_openCache_clicked();
    void on_btn_Artwork_DownloadAll_clicked();
    void on_btn_Folder_Add_clicked();
    void on_btn_Folder_Delete_clicked();
    void on_cb_Folder_ForLauncher_clicked();
signals:
    void reloadStylesheet();
    void reloadLibrary();
};

#endif // FSETTINGSDIALOG_H
