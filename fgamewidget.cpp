#include "fgamewidget.h"
#include "ui_fgamewidget.h"

#include <QGraphicsPixmapItem>

FGameWidget::FGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FGameWidget)
{
    ui->setupUi(this);
    itemBanner = NULL;
    sceneBanner = NULL;
    itemCover = NULL;
    sceneCover = NULL;
    //ui->fgwDialog_launchButton->setVisible(false);
}

FGameWidget::~FGameWidget()
{
    if(itemBanner)
        delete itemBanner;

    if(sceneBanner)
        delete sceneBanner;


    if(itemCover)
        delete itemCover;

    if(sceneCover)
        delete sceneCover;

    delete ui;
}

void FGameWidget::setGame(FGame *g) {

 //   QElapsedTimer timer;
//    timer.start();


    game = g;
    ui->fgwDialog_GameTitle->setText(game->getName());

    if(game->getBanner() != "") {
        ui->viewOne->setVisible(false);
        sceneBanner = new QGraphicsScene();
        ui->graphicsView->setScene(sceneBanner);
        QPixmap p(game->getArt(FArtBanner, true, 300, FWidth));
        itemBanner = new QGraphicsPixmapItem(p);
        sceneBanner->addItem(itemBanner);
    } else {
        ui->graphicsView->setVisible(false);
        QPixmap p(game->getArt(FArtBox, true, 55, FHeight));
        ui->gvCover->resize(p.width(), 60);
        ui->gvCover->setMaximumWidth(p.width());
        ui->gvCover->setMinimumWidth(p.width());
        sceneCover = new QGraphicsScene();
        ui->gvCover->setScene(sceneCover);

        itemCover = new QGraphicsPixmapItem(p);
        sceneCover->addItem(itemCover);
    }

 //   qDebug() << timer.elapsed();

}

void FGameWidget::setActive(bool state)
{
    updateProperty(ui->fgwDialog_Background, "gameSelected", state);
 //   ui->fgwDialog_launchButton->setVisible(state);
}


void FGameWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked(game, this);
    } else if (event->button() == Qt::RightButton)
    {
        emit rightClicked(game, this);
    }
}

void FGameWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(game, this);
}


void FGameWidget::updateProperty(QWidget *obj, const char *prop, const QVariant value) {
    if(obj->property(prop) == value)
        return;

    obj->setProperty(prop, value);
    obj->style()->unpolish(obj);
    obj->style()->polish(obj);
    obj->update();
}
