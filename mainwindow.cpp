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

    game = new FGame();
    refreshList();
    //ui->gameIdBox->setMaximum(db.getGameCount());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_chooseGameButton_clicked()
{
    QDir gameDir = QFileDialog::getExistingDirectory(this, "Choose the game directory", QDir::homePath());
    game->setPath(gameDir.absolutePath());
    refreshUi();
}

void MainWindow::refreshUi()
{
    QString path = game->getPath().isEmpty()?"No path":QDir::toNativeSeparators(game->getPath());
    QString name = game->getName().isEmpty()?"No name set":game->getName();
    QString exe = game->getExe().isEmpty()?"No game executable set.":game->getExe();
    //ui->gamePathLabel->setText(path);
    //ui->gameNameLabel->setText(name);
    //ui->gameExeLabel->setText(exe);
    //ui->gameIdBox->setMaximum(db.getGameCount());
}

void MainWindow::on_setNameButton_clicked()
{
    //game->setName(ui->nameEdit->text());
    refreshUi();
}

void MainWindow::on_launchGameButton_clicked()
{
    qDebug("Launching game!");
    game = new FGame(gameList.at(ui->listWidget->currentRow()));
    game->execute();
}

void MainWindow::on_saveButton_clicked()
{
    db.addGame(*game);
    refreshUi();
}
void MainWindow::on_loadButton_clicked()
{
    //game = db.getGame(ui->gameIdBox->value());
    if(!game)
    {
        game = new FGame();
    }
    refreshUi();
}

void MainWindow::resetDatabase()
{
    db.resetDatabase();
    refreshUi();
}

void MainWindow::on_actionRefresh_UI_triggered()
{
    refreshUi();
}

void MainWindow::on_actionRemove_database_2_triggered()
{
    resetDatabase();
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
    ui->listWidget->clear();
    gameList = db.getGameList();
    //FGame game = list.first();
    if(gameList.isEmpty())
    {
        ui->listWidget->addItem("NOTHING TO SEE HERE. Use the \"Add game\" button to add a new game.");
    }
    else
    {
        for(int i = 0; i < gameList.size(); i++)
        {
            ui->listWidget->addItem(gameList[i].getName());
        }
    }
}

void MainWindow::on_removeGameButton_clicked()
{
    db.removeGameById(gameList.at(ui->listWidget->currentRow()).dbId);
    refreshList();
}
