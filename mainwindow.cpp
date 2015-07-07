#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fgame.h>
#include <fdb.h>
#include <QMessageBox>
#include "addgamedialog.h"
#include "fsettingsdialog.h"
#include "gameinfodialog.h"
#include "watchedfoldersdialog.h"
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>



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
   for (QStringList::const_iterator cIt = list.constBegin(); cIt != list.constEnd(); ++cIt)
   {
       QFile res(":/fonts/" + *cIt);
       if (res.open(QIODevice::ReadOnly) == false)
       {
           QMessageBox::warning(0, "Font-Loader", (QString)"Cannot open font: " + *cIt + ".");
       }
       else
       {
           fID = QFontDatabase::addApplicationFontFromData(res.readAll());
           if (fID == -1)
               QMessageBox::warning(0, "Font-Loader", (QString)"Cannot load Font into System: " + *cIt + ".");
       }
   }

   //set Font
   if(fID >= 0) {
       QFontDatabase fodb;
       QFont latoFont = fodb.font("Lato Light", "Light", 12);
       qApp->setFont(latoFont);
    }

   //Shadow!
   QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
   effect->setBlurRadius(15);
   effect->setOffset(5,5);
   ui->pb_Settings->setGraphicsEffect(effect);


   //Shadow!
   settingsMenu = new QMenu(ui->gameDetailsSidebarWidget);
   settingsMenu->addAction("Edit Game");
   settingsMenu->addAction("Add Game");
   settingsMenu->addAction("Add Launcher");
   settingsMenu->addAction("Manage Library");
   settingsMenu->addAction("Settings");
   connect(settingsMenu, SIGNAL(triggered(QAction*)), this, SLOT(on_SettingsMenueClicked(QAction*)));
   QGraphicsDropShadowEffect* menuEffect = new QGraphicsDropShadowEffect();
   menuEffect->setBlurRadius(15);
   menuEffect->setOffset(5,5);
   settingsMenu->setGraphicsEffect(menuEffect);

   //Shadow!
   QGraphicsDropShadowEffect* scrollShadow = new QGraphicsDropShadowEffect();
   scrollShadow->setBlurRadius(15);
   scrollShadow->setOffset(5,0);
   ui->gameListWidget->setGraphicsEffect(scrollShadow);


    currentView = db.getIntPref("lastView", 1);
    ui->tabWidget->setCurrentIndex(currentView);
    //Resized to Stored Size
    this->resize(db.getIntPref("defaultviewWidth", 800), db.getIntPref("defaultviewHeight", 600));
    QRect screenRes = QApplication::desktop()->screenGeometry();


    if(this->height()> screenRes.height())
        this->resize(this->width(), screenRes.height()-20);
    if(this->width()> screenRes.width())
        this->resize(screenRes.width()-20, this->height());


    this->setWindowState((Qt::WindowState)db.getIntPref("windowState", Qt::WindowNoState));

    reloadStylesheet();
    game = new FGame();

    //Scan for New Games First
    if(db.getBoolPref("ScanLibsOnStartup", true))
        crawler.scanAllFolders();

    //Scrolling-List fo games
    gameScrollLayout = new QVBoxLayout;
    ui->gameScrollArea->widget()->setLayout(gameScrollLayout);

    refreshList();

    //required to store WindowSize on Resize
    resizeTimer.setSingleShot( true );
    connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()) );

    dragEnabled = false;
    resizeHeightEnabled = false;
    resizeWidthEnabled = false;
    resizeWidthEnabledInv =false;

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

    delete settingsMenu;
    delete ui;
}

void MainWindow::setWatchedFolders(QList<QDir> folders)
{
    db.updateWatchedFolders(folders);
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


void MainWindow::addGame(FGame game)
{
    db.addGame(game);
    refreshList();
}


void MainWindow::refreshList()
{
    gameList = db.getGameList();

    for(int i=0;i<gameWidgetList.length();++i)
        delete gameWidgetList[i];

    gameWidgetList.clear();


    if(!gameList.isEmpty())
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

void MainWindow::on_pb_Min_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_pb_Max_clicked()
{
    mouseDoubleClickEvent(NULL);
}

void MainWindow::on_pb_Close_clicked()
{
    this->close();
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

void MainWindow::on_tabWidget_currentChanged(int index)
{
    db.updateIntPref("lastView", index);
}

void MainWindow::ShowSettingsContextMenu(const QPoint &pos)
{
       QPoint globalPos = ui->pb_Settings->mapToGlobal(pos);

       //this is required, because (i assume) i re-calculates the sizes based on CSS, on show().
       //at least id soesn't work if the menu was never open.
       settingsMenu->show();
       settingsMenu->close();

       globalPos.setY(globalPos.y() - settingsMenu->height());
       globalPos.setX(globalPos.x() - settingsMenu->width() - 20);


       settingsMenu->exec(globalPos);
}
void MainWindow::on_SettingsMenueClicked(QAction* action) {

    if(action->text()=="Edit Game")
        showGameEditDialog();
    else if(action->text()=="Add Game")
        on_libAddGameAction_triggered();
    else if(action->text()=="Manage Library")
        on_libAddLibAction_triggered();
    else if(action->text()=="Settings")
        showSettingsDialog();
}

void MainWindow::showGameEditDialog()
{
    GameInfoDialog *dialog = new GameInfoDialog(game);
    connect(dialog, SIGNAL(finished(int)), this, SLOT(on_GameInfoDialogFinished(int)));
    dialog->exec();
}


void MainWindow::showSettingsDialog() {
    FSettingsDialog* dialog = new FSettingsDialog(&db, this);
    connect(dialog, SIGNAL(reloadStylesheet()), this, SLOT(reloadStylesheet()));
    dialog->exec();
}

void MainWindow::on_pb_Settings_clicked()
{
    ShowSettingsContextMenu(ui->pb_Settings->pos());
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
       this->setWindowTitle("FusionLauncher - " + game->getName());
       this->game = game;

       ui->tgw_GameTitle->setText(game->getName());

       if(game->getBoxart() != "") {
           ui->tgw_GameCover->setStyleSheet("#tgw_GameCover{border-image:url("+ game->getBoxart() +") 0 0 0 0 stretch stretch}");
       }


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
    if(this->windowState() != Qt::WindowNoState)
        return;


    // is the height of the Header (blue Bar)
    if (event->button() == Qt::LeftButton && event->pos().y() <90) {
           dragPosition = event->globalPos() - frameGeometry().topLeft();
           event->accept();
           dragEnabled = true;
           qDebug() << "Allow Window Drag";
    }
    else if (event->pos().y()>this->height()-15) {
        qDebug() << event->pos();
        this->setCursor(Qt::SizeVerCursor);
        dragPosition = event->globalPos();
        resizeHeightEnabled = true;
        initSize = this->size();
        qDebug() << "Allow Window Resize Height";
    }
    else if (event->pos().x()>this->width()-15) {
        qDebug() << event->pos();
        this->setCursor(Qt::SizeHorCursor);
        dragPosition = event->globalPos();
        resizeWidthEnabled = true;
        initSize = this->size();
        qDebug() << "Allow Window Resize Height";
    }
    else if (event->pos().x()<15) {
        qDebug() << event->pos();
        this->setCursor(Qt::SizeHorCursor);
        dragPosition = event->globalPos();
        resizeWidthEnabledInv = true;
        initSize = this->size();
        initPos =  frameGeometry();
        qDebug() << "Allow Window Resize Height";
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    dragEnabled = false;
    resizeHeightEnabled = false;
    resizeWidthEnabled = false;
    resizeWidthEnabledInv = false;
    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(this->windowState() != Qt::WindowMaximized)
        this->setWindowState(Qt::WindowMaximized);
    else
        this->setWindowState(Qt::WindowNoState);

    db.updateIntPref("windowState", this->windowState());
}


void MainWindow::resizeDone()
{
    /*
    if(currentView==0){
        db.updateIntPref("minviewWidth", this->width());
        db.updateIntPref("minviewHeight", this->height());
    } else */
    {
        db.updateIntPref("defaultviewWidth", this->width());
        db.updateIntPref("defaultviewHeight", this->height());
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && dragEnabled) {
           move(event->globalPos() - dragPosition);
           event->accept();
    }
    else if (event->buttons() & Qt::LeftButton && resizeHeightEnabled) {
        int yResize = event->globalPos().y() - dragPosition.y();
        int target = initSize.height()+yResize;
        this->resize(this->width(),target);
        qDebug() << "Resize H by: "<< yResize  << " to: " << target;
    }
    else if (event->buttons() & Qt::LeftButton && resizeWidthEnabled) {
        int xResize = event->globalPos().x() - dragPosition.x();
        int target = initSize.width()+xResize;
        this->resize(target, this->height());
        qDebug() << "Resize W by: "<< xResize  << " to: " << target;
    }
    else if (event->buttons() & Qt::LeftButton && resizeWidthEnabledInv) {
        int xResize = dragPosition.x()-event->globalPos().x();
        int target = initSize.width()+xResize;
        this->resize(target, this->height());
        QRect pos(initPos);
        pos.setX(initPos.x()-xResize);
        move(pos.topLeft());
        qDebug() << "Resize W Inc by: "<< xResize  << " to: " << target;
    }
}
