#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fgame.h>
#include <fdb.h>
#include <QMessageBox>
#include "addgamedialog.h"
#include "gameinfodialog.h"
#include "watchedfoldersdialog.h"
#include <QFontDatabase>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!db.init())
    {
        return;
    }

   //load Fonts
   QStringList list;
   list << "Lato-Light.ttf";
   int fID;
   for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator)
   {
       QFile res(":/fonts/" + *constIterator);
       if (res.open(QIODevice::ReadOnly) == false)
       {
           QMessageBox::warning(0, "Font-Loader", (QString)"Cannot open font: " + *constIterator + ".");
       }
       else
       {
           fID = QFontDatabase::addApplicationFontFromData(res.readAll());
           if (fID == -1)
               QMessageBox::warning(0, "Font-Loader", (QString)"Cannot load Font into System: " + *constIterator + ".");
       }
   }

   //set Font
   if(fID >= 0) {
       QFontDatabase fodb;
       QFont latoFont = fodb.font("Lato Light", "Light", 12);
       qApp->setFont(latoFont);
    }

    currentView = db.getIntPref("lastView", 1);
    ui->tabWidget->setCurrentIndex(currentView);
    this->resize(db.getIntPref("defaultviewWidth", 800), db.getIntPref("defaultviewHeight", 600));

    reloadStylesheet();
    game = new FGame();

    //Scan for New Games First
    crawler.scanAllFolders();

    //Scrolling-List fo games
    gameScrollLayout = new QVBoxLayout;
    ui->gameScrollArea->widget()->setLayout(gameScrollLayout);

    refreshList();

    //required to store WindowSize on Resize
    resizeTimer.setSingleShot( true );
    connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()) );
    connect( ui->setStylesheetAction, SIGNAL(triggered()), this, SLOT(openStylesheetDialog()));
    connect( ui->resetStylesheetAction, SIGNAL(triggered()), this, SLOT(resetStylesheet()));
}

void MainWindow::reloadStylesheet()
{
    QString stylesheet = db.getTextPref("stylesheet", ":/stylesheet.qss");
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

}


void MainWindow::setView() {

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
            //MinimalView
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
    GameInfoDialog *dialog = new GameInfoDialog(game);
    connect(dialog, SIGNAL(finished(int)), this, SLOT(on_GameInfoDialogFinished(int)));
    dialog->exec();
}


void MainWindow::on_GameInfoDialogFinished(int r) {
    refreshList();
}


void MainWindow::on_tgw_GameIconButton_clicked()
{
    if(game->getType() == Steam) {
        ui->webView->setUrl("http://store.steampowered.com/app/" + game->getExe() + "/");
    } else if(game->getType() == Origin) {
        QString u("http://www.origin.com/en-us/store/browse?q=" + game->getName().replace(" ", "%20").replace("™", ""));
        ui->webView->setUrl(u);
    }
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_tgw_pb_Artwork_clicked()
{
    GameInfoDialog *dialog = new GameInfoDialog(game);
    connect(dialog, SIGNAL(finished(int)), this, SLOT(on_GameInfoDialogFinished(int)));
    dialog->exec();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    db.updateIntPref("lastView", index);
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

void MainWindow::resetStylesheet()
{
    db.deletePref("stylesheet");
    reloadStylesheet();
}

void MainWindow::openStylesheetDialog()
{
    QString stylesheetFile = QFileDialog::getOpenFileName(this, "Choose stylesheet", QDir::homePath(), "*.qss");
    qDebug() << "Stylesheet: " << stylesheetFile;
    if(QFile::exists(stylesheetFile))
    {
        qDebug() << "New stylesheet added: " << stylesheetFile;
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

void MainWindow::resizeEvent(QResizeEvent* event)
{
    //This event Fires everytime, the window is resized by a pixel.
    //using a timer to get some delay  & not flooding the DB.
    resizeTimer.start( 500 );
   QMainWindow::resizeEvent(event);
}




void MainWindow::resizeDone()
{
    if(currentView==0){
        db.updateIntPref("minviewWidth", this->width());
        db.updateIntPref("minviewHeight", this->height());
    } else {
        db.updateIntPref("defaultviewWidth", this->width());
        db.updateIntPref("defaultviewHeight", this->height());
    }
}
