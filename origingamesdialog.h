#ifndef ORIGINGAMESDIALOG_H
#define ORIGINGAMESDIALOG_H

#include <QDialog>
#include <fcrawler.h>

namespace Ui {
class OriginGamesDialog;
}

class OriginGamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OriginGamesDialog(QWidget *parent = 0);
    OriginGamesDialog(QWidget *parent, QList<FGame> gameList);
    ~OriginGamesDialog();

private:
    Ui::OriginGamesDialog *ui;
    FCrawler crawler;
};

#endif // ORIGINGAMESDIALOG_H
