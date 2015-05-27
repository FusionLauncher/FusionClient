#include "origingamesdialog.h"
#include "ui_origingamesdialog.h"

OriginGamesDialog::OriginGamesDialog(QWidget *parent, QList<FGame> gameList) :
    QDialog(parent),
    ui(new Ui::OriginGamesDialog)
{
    ui->setupUi(this);
    QList<FGame> detectedGames = crawler.getOriginGames();

    for(int i =0;i<detectedGames.length();++i) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setData( Qt::DisplayRole, detectedGames[i].getName() );


         if(gameList.contains(detectedGames[i]))
            item->setData( Qt::CheckStateRole, Qt::Checked );
        else
            item->setData( Qt::CheckStateRole, Qt::Unchecked );

        ui->listWidget->addItem( item );
    }
}

OriginGamesDialog::~OriginGamesDialog()
{
    delete ui;
}
