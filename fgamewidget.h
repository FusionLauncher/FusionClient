/**
 * @class FGameWidget
 *
 * @brief This Widget is displayed in the Main GUI as a List-Entry
 *
 *
 */

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

    /**
     * @fn setGame
     * @brief Set the reference to the game which will be displayed
     *
     * @param game Game, which will be displayed
     */
    void setGame(FGame *g);

    /**
     * @fn setActive
     * @brief This function is used to create en optical "activated"-effect
     *
     * This effect is archied by settings a CSS-property. The actual
     * color or effect is defined in the stylesheet.
     *
     * @param state True/False as in is active / not active
     *
     * @todo This needs to be implemented in the Stylesheet
     */
    void setActive(bool state);

    /**
     * @fn setFontSize
     * @brief Used to set the Font-Size of the Game-Title
     *
     * @param size Size in PT of the font-Size
     */
    void setFontSize(int size);
private:
    Ui::FGameWidget *ui;

    FGame *game;
    QGraphicsScene* sceneBanner;
    QGraphicsPixmapItem* itemBanner;


    QGraphicsScene* sceneCover;
    QGraphicsPixmapItem* itemCover;

    /**
     * @fn updateProperty
     * @brief This is used to actually set or remove a CSS-Property
     *
     * @todo Merge this into the setActive-Function, since we use it only there.
     *
     */
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
