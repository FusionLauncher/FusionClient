#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fgame.h>
#include <fdb.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db.init();
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
    ui->gameIdBox->setMaximum(db.getGameCount());
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
    ui->gamePathLabel->setText(path);
    ui->gameNameLabel->setText(name);
    ui->gameExeLabel->setText(exe);
    ui->gameIdBox->setMaximum(db.getGameCount());
}

void MainWindow::generateGameList() {
    //TODO
    QItemSelectionModel *sm = ui->tableView->selectionModel();
    connect(sm, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(on_tableView_doubleClicked(QModelIndex));
}

void MainWindow::on_setNameButton_clicked()
{
    game->setName(ui->nameEdit->text());
    refreshUi();
}

void MainWindow::on_launchGameButton_clicked()
{
    game->execute();
}

void MainWindow::on_chooseExecutableButton_clicked()
{
    QString file;
    QDir gameDir = QDir(game->getPath());
    file = QFileDialog::getOpenFileName(this, "Choose executable", gameDir.absolutePath());
    if(file.isEmpty())
        return;
    file = gameDir.relativeFilePath(file);
    game->setExe(file);
    refreshUi();
}

void MainWindow::on_saveButton_clicked()
{
    db.addGame(*game);
    refreshUi();
}
void MainWindow::on_loadButton_clicked()
{
    game = db.getGame(ui->gameIdBox->value());
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

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    //TODO, get game index, run game
    //GUI todo: make each item large and leave space for logo / nice game name
}
