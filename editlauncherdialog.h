#ifndef EDITLAUNCHERDIALOG_H
#define EDITLAUNCHERDIALOG_H

#include <QDialog>

namespace Ui {
class EditLauncherDialog;
}

class EditLauncherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditLauncherDialog(QWidget *parent = 0);
    ~EditLauncherDialog();

private:
    Ui::EditLauncherDialog *ui;
};

#endif // EDITLAUNCHERDIALOG_H
