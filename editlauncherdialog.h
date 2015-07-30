#ifndef EDITLAUNCHERDIALOG_H
#define EDITLAUNCHERDIALOG_H

#include <QDialog>
#include <flauncher.h>

namespace Ui {
class EditLauncherDialog;
}

class EditLauncherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditLauncherDialog(QWidget *parent = 0, QList<FLauncher> *launchers = 0);
    ~EditLauncherDialog();

signals:
     void launcherEdited(FLauncher launcher);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditLauncherDialog *ui;
};

#endif // EDITLAUNCHERDIALOG_H
