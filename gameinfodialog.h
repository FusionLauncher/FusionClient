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

private slots:
    void on_downloadArtButton_clicked();
    void gotData(QString data);
};

#endif // GAMEINFODIALOG_H
