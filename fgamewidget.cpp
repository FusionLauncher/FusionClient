#include "fgamewidget.h"
#include "ui_fgamewidget.h"

FGameWidget::FGameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FGameWidget)
{
    ui->setupUi(this);
    ui->fgwDialog_launchButton->setVisible(false);
    ui->fgwDialog_removeButton->setVisible(false);
}

FGameWidget::~FGameWidget()
{    
    delete pix_Cover;
    delete ui;
}

void FGameWidget::setGame(FGame *g) {
    game = g;
    ui->fgwDialog_GameTitle->setText(game->getName());

    pix_Cover = game->getBoxart();
    ui->fgwDialog_Cover->setPixmap( pix_Cover->scaled(80,110,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    if(g->getType()==Steam) {
        pix_Type = new QPixmap(":/gfx/FGameType_Steam.png");
        ui->fgwDialog_TypeIcon->setPixmap( pix_Type->scaled(50,50,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else if(g->getType()==Origin) {
        pix_Type = new QPixmap(":/gfx/FGameType_Origin.png");
        ui->fgwDialog_TypeIcon->setPixmap( pix_Type->scaled(50,50,Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }

}

void FGameWidget::setActive(bool state)
{
    updateProperty(ui->fgwDialog_Background, "gameSelected", state);
    ui->fgwDialog_launchButton->setVisible(state);
    ui->fgwDialog_removeButton->setVisible(state);
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

void FGameWidget::on_fgwDialog_launchButton_clicked()
{
    game->execute();
}

void FGameWidget::on_fgwDialog_removeButton_clicked()
{

}

void FGameWidget::updateProperty(QWidget *obj, const char *prop, const QVariant value) {
    if(obj->property(prop) == value)
        return;

    obj->setProperty(prop, value);
    obj->style()->unpolish(obj);
    obj->style()->polish(obj);
    obj->update();
}
