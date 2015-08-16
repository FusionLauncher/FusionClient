#include "editlauncherdialog.h"
#include "ui_editlauncherdialog.h"

EditLauncherDialog::EditLauncherDialog(QWidget *parent, FLauncher *launcher) :
    QDialog(parent),
    ui(new Ui::EditLauncherDialog)
{
    ui->setupUi(this);
    ui->launcherNameEdit->setText(launcher->getName());
    ui->pathEdit->setText(launcher->getPath());
    ui->argEdit->setText(launcher->getArgs());
    dbId = launcher->getDbId();
}

EditLauncherDialog::~EditLauncherDialog()
{
    delete ui;
}

void EditLauncherDialog::on_buttonBox_accepted()
{
    FLauncher editedLauncher;
    editedLauncher.setName(ui->launcherNameEdit->text());
    editedLauncher.setPath(ui->pathEdit->text());
    editedLauncher.setArgs(ui->argEdit->text());
    editedLauncher.setDbId(dbId);
    accept();
    emit on_launcherEdited(editedLauncher);
}
