#include "fgamewidget.h"
#include "ui_fgamewidget.h"

#include <QGraphicsPixmapItem>

FGameWidget::FGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FGameWidget)
{
    ui->setupUi(this);
    item = NULL;
    scene = NULL;
    //ui->fgwDialog_launchButton->setVisible(false);
}

FGameWidget::~FGameWidget()
{
    if(item)
        delete item;

    if(scene)
        delete scene;

    delete ui;
}

void FGameWidget::setGame(FGame *g) {
    game = g;
    ui->fgwDialog_GameTitle->setText(game->getName());

    if(game->getBanner() != "") {
        ui->viewOne->setVisible(false);

        scene = new QGraphicsScene();
        ui->graphicsView->setScene(scene);
        QImage image(game->getBanner());
        QPixmap p = QPixmap::fromImage(image).scaledToWidth(300, Qt::SmoothTransformation);
        item = new QGraphicsPixmapItem(p);
        scene->addItem(item);

    } else {
        ui->graphicsView->setVisible(false);
    }

    if(game->getBoxart() != "") {
        ui->fgwDialog_Cover->setStyleSheet("#fgwDialog_Cover{border-image:url("+ game->getBoxart() +") 0 0 0 0 stretch stretch}");
    }


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
        qDebug() << "Clicked";
    } else if (event->button() == Qt::RightButton)
    {
        emit rightClicked(game, this);
        qDebug() << "RightClicked";
    }
}

void FGameWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClicked(game, this);
    qDebug() << "doubleClicked";
}


void FGameWidget::updateProperty(QWidget *obj, const char *prop, const QVariant value) {
    if(obj->property(prop) == value)
        return;

    obj->setProperty(prop, value);
    obj->style()->unpolish(obj);
    obj->style()->polish(obj);
    obj->update();
}
