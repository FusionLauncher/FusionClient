#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fgame.h>
#include <fdb.h>
#include "addgamedialog.h"
#include "gameinfodialog.h"
#include "watchedfoldersdialog.h"

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

    //Scan for New Games First
    crawler.scanAllFolders();

    gameScrollLayout = new QVBoxLayout;

    ui->gameScrollArea->widget()->setLayout(gameScrollLayout);


    refreshList();
    //ui->gameIdBox->setMaximum(db.getGameCount());

  //  QString homeLocation = QStandardPaths::locate(QStandardPaths::AppDataLocation, QString(), QStandardPaths::LocateDirectory);
}

void MainWindow::reloadStylesheet()
{
    QString stylesheet = db.getTextPref("stylesheet");
    if(!stylesheet.isNull())
    {
        qDebug("Stylesheet set. Trying to load... ("+stylesheet.toLatin1()+")");
        QFile stylesheetFile(stylesheet);
        stylesheetFile.open(QFile::ReadOnly);
        currentStyle = QLatin1String(stylesheetFile.readAll());
        if(!currentStyle.isNull())
        {
            qDebug("Stylesheet loaded.");
            qApp->setStyleSheet(currentStyle);
        }
    }
}

MainWindow::~MainWindow()
{
    for(int i=0;i<gameWidgetList.length();++i)
        delete gameWidgetList[i];

    delete ui;
}


void MainWindow::resetDatabase()
{
    db.resetDatabase();
    refreshList();
}


void MainWindow::addGame(FGame game)
{
    db.addGame(game);
    refreshList();
}

void MainWindow::setWatchedFolders(QList<QDir> folders)
{
    db.updateWatchedFolders(folders);
}

void MainWindow::refreshList()
{
    //ui->gameListWidget->clear();
    gameList = db.getGameList();

    for(int i=0;i<gameWidgetList.length();++i)
        delete gameWidgetList[i];
    gameWidgetList.clear();

    if(gameList.isEmpty())
    {
   //     ui->gameListWidget->addItem("NOTHING TO SEE HERE. Use the \"Add game\" button to add a new game.");
    }
    else
    {
        for(int i = 0; i < gameList.length(); i++)
        {
            FGameWidget *gw = new FGameWidget(ui->gameScrollArea);
            gw->setGame(&gameList[i]);
            connect(gw, SIGNAL(clicked(FGame*, QObject*)), this, SLOT(onGameClick(FGame*, QObject*)));
            connect(gw, SIGNAL(doubleClicked(FGame*,QObject*)), this, SLOT(onGameDoubleClicked(FGame*, QObject*)));
            connect(gw, SIGNAL(rightClicked(FGame*,QObject*)), this, SLOT(onGameRightClicked(FGame*, QObject*)));
            gameWidgetList.append(gw);
            gameScrollLayout->addWidget(gw);
        }
    }
}



void MainWindow::onGameRightClicked(FGame *game, QObject *sender)
{
    GameInfoDialog *dialog = new GameInfoDialog(*game);
    dialog->exec();
}

void MainWindow::onGameDoubleClicked(FGame *game, QObject *sender)
{
    game->execute();
}

void MainWindow::onGameClick(FGame *game, QObject *sender)
{
    if(qobject_cast<FGameWidget*>(sender)) {
       for(int i=0;i<gameWidgetList.length();++i)
           gameWidgetList[i]->setActive(false);

       FGameWidget *widget = (FGameWidget*)sender;
       widget->setActive(true);
       qDebug() << "is FGameWidget";
       this->setWindowTitle("FusionLauncher - " + game->getName());
    }
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

void MainWindow::on_libAddGameAction_triggered()
{
    AddGameDialog* dialog = new AddGameDialog(this);
    connect(dialog, SIGNAL(gameSet(FGame)), this, SLOT(addGame(FGame)));
    dialog->exec();
}

void MainWindow::on_libAddLibAction_triggered()
{
    WatchedFoldersDialog* dialog =  new WatchedFoldersDialog(this);
    connect(dialog, SIGNAL(folderSet(QList<QDir>)), this, SLOT(setWatchedFolders(QList<QDir>)));
    dialog->exec();
}
