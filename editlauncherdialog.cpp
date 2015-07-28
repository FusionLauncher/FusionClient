#include "editlauncherdialog.h"
#include "ui_editlauncherdialog.h"

EditLauncherDialog::EditLauncherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditLauncherDialog)
{
    ui->setupUi(this);
}

EditLauncherDialog::~EditLauncherDialog()
{
    delete ui;
}
