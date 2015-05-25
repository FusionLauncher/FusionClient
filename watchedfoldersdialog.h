#ifndef WATCHEDFOLDERS_H
#define WATCHEDFOLDERS_H

#include <QDialog>
#include <QDir>

namespace Ui {
    class WatchedFoldersDialog;
}

class WatchedFoldersDialog : public QDialog
{
    Q_OBJECT

signals:
    folderSet(QList<QDir> folders);
public:
    explicit WatchedFoldersDialog(QWidget *parent = 0);
    ~WatchedFoldersDialog();

private:
    Ui::WatchedFoldersDialog *ui;
    QList<QDir> folder;
private slots:
    void on_pb_AddFolder_clicked();
    void on_pb_RemoveFolder_clicked();
    void on_pb_Save_clicked();
    void on_pb_Close_clicked();
};

#endif // WATCHEDFOLDERS_H
