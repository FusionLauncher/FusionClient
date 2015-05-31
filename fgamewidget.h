#ifndef FGAMEWIDGET_H
#define FGAMEWIDGET_H

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
private:
    Ui::FGameWidget *ui;

    FGame *game;
    QPixmap *pix_Cover;
    QPixmap *pix_Type;
    void updateProperty(QWidget *obj, const char *prop, const QVariant value);
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

signals:
    void clicked(FGame*, QObject*);
    void doubleClicked(FGame*, QObject*);
    void rightClicked(FGame*, QObject*);
};

#endif // FGAMEWIDGET_H
