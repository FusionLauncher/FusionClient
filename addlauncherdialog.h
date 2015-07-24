#ifndef ADDLAUNCHERDIALOG_H
#define ADDLAUNCHERDIALOG_H

#include <QDialog>
#include "flauncher.h"

namespace Ui {
class AddLauncherDialog;
}

class AddLauncherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLauncherDialog(QWidget *parent = 0);
    ~AddLauncherDialog();

signals:
    void launcherSet(FLauncher launcher);

private slots:
    void on_buttonBox_accepted();

    void on_browsePathButton_clicked();

private:
    Ui::AddLauncherDialog *ui;
    void showMessage(QString message);
};

#endif // ADDLAUNCHERDIALOG_H
