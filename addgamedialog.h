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
    explicit AddGameDialog(QWidget *parent = 0, QList<FLauncher> *launchers = 0);
    ~AddGameDialog();

private slots:
    void on_chooseGameDirButton_clicked();
    void on_chooseGameExecutableButton_clicked();
    void on_buttonBox_accepted();

    void on_gameCommandBrowse_clicked();

    void on_launcherEnabledCheckBox_clicked();

private:
    Ui::AddGameDialog *ui;
    FGame game;
    bool launchersAvailable;
    QList<FLauncher>*launchers;
};

#endif // ADDGAMEDIALOG_H
