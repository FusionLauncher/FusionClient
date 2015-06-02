#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fgame.h>
#include <fdb.h>
#include <QMessageBox>
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

    currentView = db.getIntPref("lastView");
    setView();


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
    } else {
        //saveDefault stylesheet
        db.addTextPref("stylesheet", ":/stylesheet.qss");
        reloadStylesheet();
    }
}

void MainWindow::changeView()
{
    if(currentView==0){ //is now BiGView
        db.updateIntPref("minviewWidth", this->width());
        db.updateIntPref("minviewHeight", this->height());
        currentView = 1;

    } else { //is now smallview
        db.updateIntPref("defaultviewWidth", this->width());
        db.updateIntPref("defaultviewHeight", this->height());
        currentView = 0;
    }

    setView();
}


void MainWindow::setView() {
    if(currentView==0){
        ui->gameScrollArea->setVisible(true);
        ui->defaultViewWidget->setVisible(false);
        this->resize(db.getIntPref("minviewWidth"), db.getIntPref("minviewHeight"));

    } else {
        ui->gameScrollArea->setVisible(false);
        ui->defaultViewWidget->setVisible(true);
        this->resize(db.getIntPref("defaultviewWidth"), db.getIntPref("defaultviewHeight"));
    }
    db.updateIntPref("lastView", currentView);
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
            ui->simpleGameList->addItem(gameList[i].getName());
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
    connect(dialog, SIGNAL(finished(int)), this, SLOT(on_GameInfoDialogFinished(int)));
    dialog->exec();
}


void MainWindow::on_GameInfoDialogFinished(int r) {
    refreshList();
}

void MainWindow::on_simpleGameList_itemClicked(QListWidgetItem * item)
{
    game = &gameList[ui->simpleGameList->row(item)];
    ui->tgw_GameTitle->setText(game->getName());
    ui->tgw_GameCover->setPixmap( game->getBoxart()->scaled(90,125,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    if(game->getClearart() != "") {
     //   qDebug("Set BG: background-image:url("+ game->getClearart().toLatin1() +")");
        ui->gameDetailsWidget->setStyleSheet("#gameDetailsWidget{background-image:url("+ game->getClearart() +")}");
    } else  if(game->getFanart() != ""){
        ui->gameDetailsWidget->setStyleSheet("#gameDetailsWidget{background-image:url("+ game->getFanart() +")}");

    } else {
        ui->gameDetailsWidget->setStyleSheet("#gameDetailsWidget{background-image:none}");

    }
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

void MainWindow::on_actionSwitch_View_triggered()
{
    changeView();
}
