/**
 * @class AddGameDialog
 *
 * @brief A Dialog to add Executables
 *
 * This Dialog allows the user to add Executables to his library.
 *
 */


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
    /**
     * @fn gameSet
     * @brief Is used to signal the MainWindow to add the game
     *
     * @param game Game, which will be added to the Database.
     *
     */
    void gameSet(FGame game);

public:
    explicit AddGameDialog(QWidget *parent = 0, QList<FLauncher> *launchers = 0);
    ~AddGameDialog();

private slots:

    /**
     * @fn on_chooseGameDirButton_clicked
     * @brief Fires, when the user clicks the Button to select the Game-Directory
     *
     */
    void on_chooseGameDirButton_clicked();

    /**
     * @fn on_chooseGameExecutableButton_clicked
     * @brief Fires, when the user clicks the Button to select the Game-Executable
     *
     */
    void on_chooseGameExecutableButton_clicked();

    /**
     * @fn on_buttonBox_accepted
     * @brief Fires, when the user clicks the OK-Button
     *
     */
    void on_buttonBox_accepted();


    /**
     * @fn on_gameCommandBrowse_clicked
     * @brief Fires, when the user clicks the Button to select a Launcher
     *
     */
    void on_gameCommandBrowse_clicked();


    /**
     * @fn on_chooseGameDirButton_clicked
     * @brief Fires, when the user clicks the Button to select the Game-Directory
     *
     */
    void on_launcherEnabledCheckBox_clicked();

private:
    Ui::AddGameDialog *ui; //!< Reference to the GUI.
    FGame game; //!< FGame-Template which is used to collect Information about the new Game.
    bool launchersAvailable; //!< Flag to check if the game will use a Launcher.
    QList<FLauncher>*launchers; //!< List of available Launcher.
};

#endif // ADDGAMEDIALOG_H
