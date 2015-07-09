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
private slots:
    void on_listWidget_currentRowChanged(int i);

    void on_pb_selectStylesheet_clicked();
    void on_pb_ResetStylesheet_clicked();
    void on_pb_ScanNow_clicked();



    void on_buttonBox_accepted();
signals:
    void reloadStylesheet();
    void reloadLibrary();
};

#endif // FSETTINGSDIALOG_H
