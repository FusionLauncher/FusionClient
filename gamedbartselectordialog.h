#ifndef GAMEDBARTSELECTORDIALOG_H
#define GAMEDBARTSELECTORDIALOG_H

#include <QDialog>
#include "thegamedbstorage.h"
#include <QAbstractButton>


namespace Ui {
class GameDBArtSelectorDialog;
}

class GameDBArtSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameDBArtSelectorDialog(QList<TheGameDBStorage *> Games, QWidget *parent = 0);
    ~GameDBArtSelectorDialog();

private:
    Ui::GameDBArtSelectorDialog *ui;
    QList<TheGameDBStorage*> Gamelist;
private slots:
    void OnClick();

signals:
    void gameSelected(TheGameDBStorage* );

};

#endif // GAMEDBARTSELECTORDIALOG_H
