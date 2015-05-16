#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fgame.h>
#include <fdb.h>
#include "addgamedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!db.init())
    {
        return;
    }
    reloadStylesheet();
    game = new FGame();
    refreshList();
    //ui->gameIdBox->setMaximum(db.getGameCount());
}

void MainWindow::reloadStylesheet()
{
    QString stylesheet = db.getTextPref("stylesheet");
    if(!stylesheet.isNull())
    {
        qDebug("Stylesheet set. Trying to load... ("+stylesheet.toLatin1()+")");
        QFile stylesheetFile(stylesheet);
        stylesheetFile.open(QFile::ReadOnly);
        QString stylesheetContents = QLatin1String(stylesheetFile.readAll());
        if(!stylesheetContents.isNull())
        {
            qDebug("Stylesheet loaded.");
            qApp->setStyleSheet(stylesheetContents);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_launchGameButton_clicked()
{
    if(gameList.isEmpty() || ui->gameListWidget->currentRow() == -1 || !ui->gameListWidget->currentItem()->isSelected())
    {
        return;
    }
    qDebug("Launching game!");
    game = new FGame(gameList.at(ui->gameListWidget->currentRow()));
    game->execute();
}

void MainWindow::resetDatabase()
{
    db.resetDatabase();
    refreshList();
}

void MainWindow::on_addGameButton_clicked()
{
    AddGameDialog* dialog = new AddGameDialog(this);
    connect(dialog, &AddGameDialog::gameSet, this, &MainWindow::addGame);
    dialog->exec();
}

void MainWindow::addGame(FGame game)
{
    //qDebug("Game added, YAY!");
    db.addGame(game);
    refreshList();
}

void MainWindow::refreshList()
{
    ui->gameListWidget->clear();
    gameList = db.getGameList();
    //FGame game = list.first();
    if(gameList.isEmpty())
    {
        ui->gameListWidget->addItem("NOTHING TO SEE HERE. Use the \"Add game\" button to add a new game.");
    }
    else
    {
        for(int i = 0; i < gameList.size(); i++)
        {
            ui->gameListWidget->addItem(gameList[i].getName());
        }
    }
}

void MainWindow::on_removeGameButton_clicked()
{
    if(gameList.isEmpty() || ui->gameListWidget->currentRow() == -1 || !ui->gameListWidget->currentItem()->isSelected())
    {
        return;
    }
    db.removeGameById(gameList.at(ui->gameListWidget->currentRow()).dbId);
    refreshList();
}

void MainWindow::on_removeDatabaseAction_triggered()
{
    resetDatabase();
}

void MainWindow::on_refreshUIAction_triggered()
{
    refreshList();
}

void MainWindow::on_setStylesheetAction_triggered()
{
    QString stylesheetFile = QFileDialog::getOpenFileName(this, "Choose stylesheet", QDir::homePath(), "*.qss");
    if(QDir(stylesheetFile).exists())
    {
        if(db.getTextPref("stylesheet").isNull())
        {
            db.addTextPref("stylesheet", stylesheetFile);
        }
        else
        {
            db.updateTextPref("stylesheet", stylesheetFile);
        }
        reloadStylesheet();
    }
}
