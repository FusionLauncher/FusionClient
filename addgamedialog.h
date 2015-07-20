#ifndef ADDGAMEDIALOG_H
#define ADDGAMEDIALOG_H

#include <QDialog>
#include <fgame.h>

namespace Ui {
class AddGameDialog;
}

class AddGameDialog : public QDialog
{
    Q_OBJECT

signals:
    void gameSet(FGame game);

public:
    explicit AddGameDialog(QWidget *parent = 0);
    ~AddGameDialog();

private slots:
    void on_chooseGameDirButton_clicked();
    void on_chooseGameExecutableButton_clicked();
    void on_buttonBox_accepted();

    void on_gameCommandBrowse_clicked();

private:
    Ui::AddGameDialog *ui;
    FGame game;
};

#endif // ADDGAMEDIALOG_H
