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
       qApp->setFont(fodb.font("Lato Light", "Light", 12));
    }

    currentView = db.getIntPref("lastView", 1);
    setView();


    reloadStylesheet();
    game = new FGame();

    //Scan for New Games First
    crawler.scanAllFolders();

    gameScrollLayout = new QVBoxLayout;

    ui->gameScrollArea->widget()->setLayout(gameScrollLayout);


    refreshList();

    //required to store WindowSize on Resize
    resizeTimer.setSingleShot( true );
    connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()) );
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
        this->resize(db.getIntPref("minviewWidth",360), db.getIntPref("minviewHeight", 600));

    } else {
        //Hide Small GUI
        ui->gameScrollArea->setVisible(false);

        //Show big Gui
        ui->defaultViewWidget->setVisible(true);

        //Show last Tab
        int lastTab = db.getIntPref("lastTab", 2);
        if(lastTab==1)
            on_tabButton_Store_clicked();
        else if (lastTab==2)
            on_tabButton_Games_clicked();
        else
            on_tabButton_Community_clicked();

        this->resize(db.getIntPref("defaultviewWidth", 700), db.getIntPref("defaultviewHeight", 400));
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
    ui->simpleGameList->clear();

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
    GameInfoDialog *dialog = new GameInfoDialog(game);
    connect(dialog, SIGNAL(finished(int)), this, SLOT(on_GameInfoDialogFinished(int)));
    dialog->exec();
}


void MainWindow::on_GameInfoDialogFinished(int r) {
    refreshList();
}

void MainWindow::on_tabButton_Store_clicked()
{
    ui->tabWidget_Community->setVisible(false);
    ui->tabWidget_Store->setVisible(true);
    ui->tabWidget_Games->setVisible(false);
    db.updateIntPref("lastTab", 1);
}

void MainWindow::on_tabButton_Games_clicked()
{
    ui->tabWidget_Community->setVisible(false);
    ui->tabWidget_Store->setVisible(false);
    ui->tabWidget_Games->setVisible(true);
    db.updateIntPref("lastTab", 2);
}

void MainWindow::on_tabButton_Community_clicked()
{
    ui->tabWidget_Community->setVisible(true);
    ui->tabWidget_Store->setVisible(false);
    ui->tabWidget_Games->setVisible(false);
    db.updateIntPref("lastTab", 3);
}

void MainWindow::on_tgw_GameIconButton_clicked()
{
    if(game->getType() == Steam) {
        ui->webView->setUrl("http://store.steampowered.com/app/" + game->getExe() + "/");
    } else if(game->getType() == Origin) {
        QString u("http://www.origin.com/en-us/store/browse?q=" + game->getName().replace(" ", "%20").replace("™", ""));
        ui->webView->setUrl(u);
    }


    ui->tabWidget_Community->setVisible(false);
    ui->tabWidget_Store->setVisible(true);
    ui->tabWidget_Games->setVisible(false);
}

void MainWindow::on_tgw_pb_Artwork_clicked()
{
    GameInfoDialog *dialog = new GameInfoDialog(game);
    connect(dialog, SIGNAL(finished(int)), this, SLOT(on_GameInfoDialogFinished(int)));
    dialog->exec();
}

void MainWindow::on_simpleGameList_itemClicked(QListWidgetItem * item)
{
    game = &gameList[ui->simpleGameList->row(item)];
    ui->tgw_GameTitle->setText(game->getName());

    if(game->getBoxart() != "") {
        ui->tgw_GameCover->setStyleSheet("#tgw_GameCover{border-image:url("+ game->getBoxart() +") 0 0 0 0 stretch stretch}");
    }

    if(game->getType() == Steam) {
        QPixmap pixmap(":/gfx/FGameType_Steam.png");
        QIcon ButtonIcon(pixmap);
        ui->tgw_GameIconButton->setIcon(ButtonIcon);
    } else if (game->getType() == Origin) {
        QPixmap pixmap(":/gfx/FGameType_Origin.png");
        QIcon ButtonIcon(pixmap);
        ui->tgw_GameIconButton->setIcon(ButtonIcon);
    } else {
        ui->tgw_GameIconButton->setIcon(QIcon());
    }


    if(game->getClearart() != "") {
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

void MainWindow::resizeEvent(QResizeEvent* event)
{
    //This event Fires everytime, the window is resized by a pixel.
    //using a timer to get some delay  & not flooding the DB.
    resizeTimer.start( 500 );
   QMainWindow::resizeEvent(event);
}






void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // is the height of the Header (blue Bar)
    if (event->button() == Qt::LeftButton && event->pos().y() <90) {
           dragPosition = event->globalPos() - frameGeometry().topLeft();
           event->accept();
       }
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

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // is the height of the Header (blue Bar)
    if (event->buttons() & Qt::LeftButton && event->pos().y() <90) {
           move(event->globalPos() - dragPosition);
           event->accept();
       }
}
