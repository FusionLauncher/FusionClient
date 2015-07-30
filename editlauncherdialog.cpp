#include "editlauncherdialog.h"
#include "ui_editlauncherdialog.h"

EditLauncherDialog::EditLauncherDialog(QWidget *parent, QList<FLauncher> *launchers) :
    QDialog(parent),
    ui(new Ui::EditLauncherDialog)
{
    ui->setupUi(this);
    Iterator<FLauncher> launcher;
    for(launcher = launchers->begin(); launcher != launchers->end(); launcher++)
    {
        ui->chooseLauncherComboBox->addItem();
    }
}

EditLauncherDialog::~EditLauncherDialog()
{
    delete ui;
}

void EditLauncherDialog::on_buttonBox_accepted()
{
    FLauncher launcher;
    emit launcherEdited(launcher);
}
