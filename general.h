#ifndef GENERAL_H
#define GENERAL_H

#include <QVariant>
#include <QWidget>



class general : public QWidget
{
    Q_OBJECT
public:

    static void updateProperty(QWidget *obj, const char *prop, const QVariant value) {
        if(obj->property(prop) == value)
            return;

        obj->setProperty(prop, value);
   //    obj->style()->unpolish(obj);
   //    obj->style()->polish(obj);
        obj->update();
    }
};

#endif // GENERAL_H
