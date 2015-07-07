#include "fsettingsdialog.h"
#include "ui_fsettingsdialog.h"
#include <QDebug>

FSettingsDialog::FSettingsDialog(FDB *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FSettingsDialog)
{
    ui->setupUi(this);
    this->db = db;

    //Hast to be the Same order as the Stacked Widget.
   ui->listWidget->addItem("General");
   ui->listWidget->addItem("Database");
   ui->listWidget->addItem("Interface");
   ui->listWidget->setCurrentRow(0);

   ui->le_Stylesheet->setText(db->getTextPref("stylesheet"));
   ui->cb_ScanLibOnStartup->setChecked(db->getBoolPref("ScanLibsOnStartup", true));
}

FSettingsDialog::~FSettingsDialog()
{
    delete ui;
}

void FSettingsDialog::on_listWidget_currentRowChanged(int i)
{
    ui->settingPages->setCurrentIndex(i);
}

void FSettingsDialog::on_pb_selectStylesheet_clicked()
{
    QString stylesheetFile = QFileDialog::getOpenFileName(this, "Choose stylesheet", QDir::homePath(), "*.qss");
    qDebug() << "Stylesheet: " << stylesheetFile;
    if(QFile::exists(stylesheetFile))
    {
        qDebug() << "New stylesheet added: " << stylesheetFile;
        db->updateTextPref("stylesheet", stylesheetFile);
        emit reloadStylesheet();
    }
}

void FSettingsDialog::on_pb_ResetStylesheet_clicked()
{
    db->updateTextPref("stylesheet", ":/stylesheet.qss");
    emit reloadStylesheet();
}

void FSettingsDialog::on_pb_ScanNow_clicked()
{

    FCrawler crawler;
    crawler.scanAllFolders();
}



void FSettingsDialog::on_buttonBox_accepted()
{
   db->updateBoolPref("ScanLibsOnStartup", (bool)ui->cb_ScanLibOnStartup->checkState());
}
