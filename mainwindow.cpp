//Includes from FusionCLient
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addgamedialog.h"
#include "fsettingsdialog.h"
#include "gameinfodialog.h"

//Includes from LibFusion
#include <libfusion.h>
#include <fgame.h>
#include <flauncher.h>
#include <fdb.h>
#include <f_dbg.h>

//Includes QT-Framework
#include <QDesktopWidget>
#include <QFontDatabase>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsPixmapItem>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cUpdater->writeVersion(VersionString, QDir::currentPath());

    updateInProgress = false;


    if(!db.init())
    {
        QMessageBox msg(QMessageBox::Warning, "Error!", "Couldn't init the database. Things may not work correctly. If this happened after an update, please submit a bug report.", QMessageBox::Ok, this);
        msg.exec();
        qDebug() << "Couldn't init DB.";
        //return;
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
           QMessageBox::warning(0, "Font Loader", (QString)"Cannot open font: " + *cIt + ".");
       }
       else
       {
           fID = QFontDatabase::addApplicationFontFromData(res.readAll());
           if (fID == -1)
               QMessageBox::warning(0, "Font Loader", (QString)"Cannot load font: " + *cIt + ".");
       }
   }

   //set Font
   if(fID >= 0) {
       QFontDatabase fodb;
       QFont latoFont = fodb.font("Lato Light", "Light", 11);
       qApp->setFont(latoFont);
    }



   //Build the Settings-Button
   settingsMenu = new QMenu(ui->pb_Settings);
   //: Entry for Settings-Menu
   settingsMenu->addAction(tr("Edit Game"), this, SLOT(sttngsBtn_edtGame_triggered()));
   //: Entry for Settings-Menu
   settingsMenu->addAction(tr("Add Game"), this, SLOT(sttngsBtn_addGame_triggered()));
   //: Entry for Settings-Menu
   settingsMenu->addAction(tr("Settings"), this, SLOT(sttngsBtn_opnSttngs_triggered()));
   //: Entry for Settings-Menu
   settingsMenu->addAction(tr("Report Bug"), this, SLOT(sttngsBtn_reportBug_triggered()));



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
    gameScrollLayout->setMargin(1);
    gameScrollLayout->setSpacing(0);
    ui->gameScrollArea->widget()->setLayout(gameScrollLayout);

    refreshList();

    //required to store WindowSize on Resize
    resizeTimer.setSingleShot( true );
    connect( &resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()) );

    dragEnabled = false;
    resizeHeightEnabled = false;
    resizeWidthEnabled = false;
    resizeWidthEnabledInv =false;


    //Shadow!
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(15);
    effect->setOffset(5,5);
    ui->pb_Settings->setGraphicsEffect(effect);

    QGraphicsDropShadowEffect* Leffect = new QGraphicsDropShadowEffect();
    Leffect->setBlurRadius(15);
    Leffect->setOffset(5,5);
    ui->pb_LaunchGame->setGraphicsEffect(Leffect);

    QGraphicsDropShadowEffect* scrollShadow = new QGraphicsDropShadowEffect();
    scrollShadow->setBlurRadius(15);
    scrollShadow->setOffset(5,0);
    ui->gameListWidget->setGraphicsEffect(scrollShadow);

    QGraphicsDropShadowEffect* menuEffect = new QGraphicsDropShadowEffect();
    menuEffect->setBlurRadius(15);
    menuEffect->setOffset(5,5);
    settingsMenu->setGraphicsEffect(menuEffect);

    QGraphicsDropShadowEffect* randEffect = new QGraphicsDropShadowEffect();
    randEffect->setBlurRadius(15);
    randEffect->setOffset(3,3);
    ui->pb_LaunchRandom->setGraphicsEffect(randEffect);

    createTrayIcon();
    checkForUpdates();
  //  loadLanguage(db.getTextPref("currentLanguage", "en"));
    loadLanguage("de");
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
    DBG_LANG("Try change lang to " + rLanguage);
    if(currentLanguage != rLanguage) {
        currentLanguage = rLanguage;
        QLocale locale = QLocale(currentLanguage);
        QLocale::setDefault(locale);
        switchTranslator(appTranslator, QString("FusionLang_%1.qm").arg(rLanguage));
     //   switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
    } else {
        DBG_LANG("Language '" + rLanguage + "' not found!");
    }
}

void MainWindow::switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if(translator.load(filename)) {
        qApp->installTranslator(&translator);
        DBG_LANG("Successfully changed lang.");
        ui->retranslateUi(this);
    }
}

void MainWindow::createTrayIcon()
{
    if(!db.getBoolPref("useTrayIcon", true))
        return;

    trayIcon = new QSystemTrayIcon(QIcon(":/gfx/FusionLogo45Deg.png"), this);
    QMenu *iconMenu = new QMenu;



    QList<FGame *> lastGames = db.getLatestLaunchedGames(8);
    for(FGame *g : lastGames) {
        QAction *gameAction = new QAction( g->getName(), trayIcon );
        connect( gameAction, SIGNAL(triggered()), this, SLOT(trayLaunchGame()) );
        iconMenu->addAction( gameAction );
    }

    QAction *exitAction = new QAction( "Exit", trayIcon );
    connect( exitAction, SIGNAL(triggered()), this, SLOT(on_pb_Close_clicked()) );
    iconMenu->addAction( exitAction );

    trayIcon->setContextMenu( iconMenu );
    trayIcon->show();
 }

void MainWindow::checkForUpdates()
{
   if(!db.getBoolPref("autoScanUpdates", true))
       return;

   qDebug() << "Getting Updates";


    FClientUpdater u;
    FusionVersion v = u.strToVersion(VersionString);
    FusionVersion o = u.getCRClientVersion();
    if(!(o==v) && o.initialized) {
        if(QMessageBox::information(this, tr("New Version available!"), "Version " + u.VersionToStr(o) + " is available. Do you want to Download it?", QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            #ifdef _WIN32
                QFile updater(QDir::currentPath() + "/FusionUpdater.exe");
                if(!updater.exists()) {
                    QMessageBox::warning(this, tr("Cannot find Updater!"), tr("Unable to find Updater in: ") + QDir::currentPath() + ".\n" + tr("Please update manually by visiting projFusion.com."));
                    return;
                }else {
                    bool launched = QDesktopServices::openUrl(QUrl("file:///" + updater.fileName(), QUrl::TolerantMode) );
                    if(!launched) {
                        QMessageBox::warning(this, tr("Cannot launch Updater!"), tr("Unable to launch Updater!") + "\n" + tr("Please update manually by visiting projFusion.com."));
                        return;
                    } else {
                         updateInProgress = true;
                         qApp->exit(0);
                    }
                }
            #elif __linux

            #endif
        }
    }
}

void MainWindow::changeView()
{

}

void MainWindow::launchRandomGame()
{
    int randomGame = qrand() % gameList.length();
    qDebug() << "Random launch:" << gameList[randomGame]->getName();
    FGame *g = gameList[randomGame];
    db.updateLastLaunched(g);
    g->execute();
}

void MainWindow::trayLaunchGame()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        qDebug() << action->text();

    for(FGame *g : gameList)
    {
        if(g->getName() == action->text())
        {
            g->execute();
            return;
        }
    }
}


void MainWindow::sttngsBtn_opnSttngs_triggered() {
    FSettingsDialog* dialog = new FSettingsDialog(&db, this);
    connect(dialog, SIGNAL(reloadStylesheet()), this, SLOT(reloadStylesheet()));
    connect(dialog, SIGNAL(reloadLibrary()), this, SLOT(refreshList()));
    dialog->exec();
}

void MainWindow::sttngsBtn_addGame_triggered() {
    QList<FLauncher> launchers = db.getLaunchers();
    qDebug() << "List length: " << launchers.length();
    AddGameDialog* dialog = new AddGameDialog(this, &launchers);
    connect(dialog, SIGNAL(gameSet(FGame)), this, SLOT(addGame(FGame)));
    dialog->exec();
}
void MainWindow::sttngsBtn_edtGame_triggered() {
    GameInfoDialog *dialog = new GameInfoDialog(game, &db, this);
    connect(dialog, SIGNAL(reloadRequired()), this, SLOT(on_GameInfoDialogFinished()));
    dialog->exec();
}

void MainWindow::sttngsBtn_reportBug_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/FusionLauncher/FusionClient/issues/new"));
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
    QElapsedTimer timer;
    timer.start();


    gameList = db.getGameList();
    for(int i=0;i<gameWidgetList.length();i++)
        gameWidgetList.at(i)->deleteLater();
        //gameWidgetList.removeAt(i);
        //delete gameWidgetList[i];

    gameWidgetList.clear();

    if(!gameList.isEmpty())
    {
        for(int i = 0; i < gameList.length(); i++)
        {
            FGameWidget *gw = new FGameWidget(ui->gameScrollArea);
            gw->setGame(gameList[i]);
            connect(gw, SIGNAL(clicked(FGame*, QObject*)), this, SLOT(onGameClick(FGame*, QObject*)));
            connect(gw, SIGNAL(doubleClicked(FGame*,QObject*)), this, SLOT(onGameDoubleClicked(FGame*, QObject*)));
            connect(gw, SIGNAL(rightClicked(FGame*,QObject*)), this, SLOT(onGameRightClicked(FGame*, QObject*)));
            gameWidgetList.append(gw);
            gameScrollLayout->addWidget(gw);
        }
        //Select first game by default
        onGameClick(gameList[0], gameWidgetList[0]);


        QSpacerItem *vSpacer;
        vSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        gameScrollLayout->addItem(vSpacer);
    }



    qDebug() << "Time to load list:" << timer.elapsed();
}



void MainWindow::onGameRightClicked(FGame *game, QObject *sender)
{
    GameInfoDialog *dialog = new GameInfoDialog(game, &db, this);
    connect(dialog, SIGNAL(reloadRequired()), this, SLOT(on_GameInfoDialogFinished()));
    dialog->exec();
}


void MainWindow::on_GameInfoDialogFinished() {
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

    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    db.updateIntPref("lastView", index);
}

void MainWindow::ShowSettingsContextMenu(const QPoint &pos)
{
       QPoint globalPos = ui->pb_Settings->mapToGlobal(pos);

       //this is required, because (,i assume,) ti re-calculates the sizes based in on_show().
       //at least it doesn't work if the menu was never open.
       settingsMenu->show();
       settingsMenu->close();
       int deltaWidth = this->width() - this->minimumWidth();

       globalPos.setY(globalPos.y() - settingsMenu->height());
       globalPos.setX(globalPos.x() - settingsMenu->width() - deltaWidth - 150);

       qDebug() << "Final Pos:" << globalPos;

       settingsMenu->exec(globalPos);
}

void MainWindow::on_launcherSet(FLauncher launcher)
{
    db.addLauncher(launcher);
}

void MainWindow::on_launcherEdited(FLauncher launcher)
{
    qDebug() << "Launcher edited!";
    db.updateLauncher(launcher);
}

void MainWindow::on_pb_Settings_clicked()
{
    ShowSettingsContextMenu(ui->pb_Settings->pos());
}

void MainWindow::on_pb_LaunchGame_clicked()
{
    if(game != NULL) {
        db.updateLastLaunched(game);
        game->execute();
    }
}

void MainWindow::on_pb_LaunchRandom_clicked()
{
    launchRandomGame();
}


void MainWindow::onGameDoubleClicked(FGame *game, QObject *sender)
{

    db.updateLastLaunched(game);
    game->execute();
}

void MainWindow::onGameClick(FGame *game, QObject *sender)
{
    if(qobject_cast<FGameWidget*>(sender))
    {
       for(int i=0;i<gameWidgetList.length();++i)
           gameWidgetList[i]->setActive(false);

       FGameWidget *widget = (FGameWidget*)sender;
       widget->setActive(true);
       this->setWindowTitle("FusionLauncher - " + game->getName());
       this->game = game;

       ui->tgw_GameTitle->setText(game->getName());

       QString art = "";
        if(game->getArt(FArtClearart) != "") { //Has clearart
            art = (game->getArt(FArtClearart, true, 125, FHeight));
            ui->tgw_GameTitle->setVisible(false);
        }
        else if (game->getArt(FArtBox) != "") { //Has Boxart
            art = (game->getArt(FArtBox, true, 125, FHeight));
            ui->tgw_GameTitle->setVisible(true);
        } else { //Has no Images
            ui->tgw_GameTitle->setVisible(true);
        }

        QString lastPlayed = game->getGameLastPlayed().toString(Qt::SystemLocaleShortDate);
        if(lastPlayed.length()>0)
            lastPlayed = tr("Last played: ") + lastPlayed;
        else
            //: This is used, when the games was never played
            lastPlayed = tr("Last played: -");

        ui->lbl_lastPlayed->setText(lastPlayed);


        QPixmap p(art);

        ui->gvCover->resize(p.width(), 125);
        ui->gvCover->setMaximumWidth(p.width());
        ui->gvCover->setMinimumWidth(p.width());
        sceneCover = new QGraphicsScene();
        ui->gvCover->setScene(sceneCover);

        itemCover = new QGraphicsPixmapItem(p);
        sceneCover->addItem(itemCover);

//       ui->tgw_GameCover->setStyleSheet("#tgw_GameCover{border-image:url("+ game->getArt(FArtBox) +") 0 0 0 0 stretch stretch}");
    }
}


void MainWindow::on_actionSwitch_View_triggered()
{
    changeView();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    //This event fires everytime, the window is resized by a pixel.
    //using a timer to get some delay  & not flooding the DB.
    resizeTimer.start( 500 );
   QMainWindow::resizeEvent(event);
}






void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(this->windowState() != Qt::WindowNoState)
        return;

    // is the height of the Header (blue Bar)
    if (event->button() == Qt::LeftButton && event->pos().y() <80) {
           dragPosition = event->globalPos() - frameGeometry().topLeft();
           event->accept();
           dragEnabled = true;
           qDebug() << "Allow Window Drag";
    }
    else if (event->pos().y()>this->height()-15) {
        this->setCursor(Qt::SizeVerCursor);
        prepareResize(event);
        resizeHeightEnabled = true;
    }
    else if (event->pos().x()>this->width()-15) {
        this->setCursor(Qt::SizeHorCursor);
        prepareResize(event);
        resizeWidthEnabled = true;
    }
    else if (event->pos().x()<15) {
        prepareResize(event);
        this->setCursor(Qt::SizeHorCursor);
        resizeWidthEnabledInv = true;
    }
}

void MainWindow::prepareResize(QMouseEvent *event) {
    dragPosition = event->globalPos();
    initSize = this->size();
    initPos =  frameGeometry();
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
  //      qDebug() << "Resize H by: "<< yResize  << " to: " << target;
    }
    else if (event->buttons() & Qt::LeftButton && resizeWidthEnabled) {
        int xResize = event->globalPos().x() - dragPosition.x();
        int target = initSize.width()+xResize;
        this->resize(target, this->height());
   //     qDebug() << "Resize W by: "<< xResize  << " to: " << target;
    }
    else if (event->buttons() & Qt::LeftButton && resizeWidthEnabledInv) {
        int xResize = dragPosition.x()-event->globalPos().x();
        int target = initSize.width()+xResize;
        this->resize(target, this->height());
        QRect pos(initPos);
        pos.setX(initPos.x()-xResize);
        move(pos.topLeft());
  //      qDebug() << "Resize W Inc by: "<< xResize  << " to: " << target;
    }
}
