#ifndef GAMEINFODIALOG_H
#define GAMEINFODIALOG_H

#include <QDialog>
#include <fgame.h>

namespace Ui {
class GameInfoDialog;
}

class GameInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameInfoDialog(FGame g, QWidget *parent = 0);
    ~GameInfoDialog();
    FGame game;
private:
    Ui::GameInfoDialog *ui;

    void on_downloadArtButton_clicked();
};

#endif // GAMEINFODIALOG_H
