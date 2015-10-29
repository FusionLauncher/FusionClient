#ifndef FGAMEWIDGET_H
#define FGAMEWIDGET_H

#include <QGraphicsScene>
#include <QWidget>
#include <fgame.h>
#include <qevent.h>
#include "general.h"

namespace Ui {
class FGameWidget;
}

class FGameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FGameWidget(QWidget *parent = 0);
    ~FGameWidget();

    void setGame(FGame *g);
    void setActive(bool state);
    void setFontSize(int size);
private:
    Ui::FGameWidget *ui;

    FGame *game;
    QGraphicsScene* sceneBanner;
    QGraphicsPixmapItem* itemBanner;


    QGraphicsScene* sceneCover;
    QGraphicsPixmapItem* itemCover;

    void updateProperty(QWidget *obj, const char *prop, const QVariant value);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void clicked(FGame*, QObject*);
    void doubleClicked(FGame*, QObject*);
    void rightClicked(FGame*, QObject*);
};

#endif // FGAMEWIDGET_H
