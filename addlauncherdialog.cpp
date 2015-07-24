#include "addlauncherdialog.h"
#include "ui_addlauncherdialog.h"
#include "flauncher.h"
#include <QMessageBox>
#include <QFileDialog>

AddLauncherDialog::AddLauncherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLauncherDialog)
{
    ui->setupUi(this);
}

AddLauncherDialog::~AddLauncherDialog()
{
    delete ui;
}

void AddLauncherDialog::on_buttonBox_accepted()
{
    FLauncher launcher;
    if(ui->nameEdit->text().isEmpty())
    {
        showMessage("Please set a name.");
        return;
    }
    if(ui->pathEdit->text().isEmpty())
    {
        showMessage("Please set a path.");
        return;
    }
    launcher.setName(ui->nameEdit->text());
    launcher.setPath(ui->pathEdit->text());
    launcher.setArgs(ui->argEdit->text());
    emit AddLauncherDialog::launcherSet(launcher);
    this->accept();
}

void AddLauncherDialog::on_browsePathButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose launcher", ".", "*");
    ui->pathEdit->setText(path);
}

void AddLauncherDialog::showMessage(QString message)
{
    QMessageBox box(QMessageBox::NoIcon, "Error", message, QMessageBox::Ok, this);
    box.exec();
}
