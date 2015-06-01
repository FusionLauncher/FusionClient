#include "gamedbartselectordialog.h"
#include "ui_gamedbartselectordialog.h"

GameDBArtSelectorDialog::GameDBArtSelectorDialog(QList<TheGameDBStorage*> Games, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameDBArtSelectorDialog)
{
    ui->setupUi(this);
    Gamelist = Games;

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(OnClick()));


    for(int i=0;i<Gamelist.length();++i)
        ui->listWidget->addItem(Gamelist[i]->gameName);
}

GameDBArtSelectorDialog::~GameDBArtSelectorDialog()
{
    delete ui;
}

void GameDBArtSelectorDialog::OnClick() {
    {
        TheGameDBStorage* tgd  = Gamelist[ui->listWidget->selectionModel()->selectedIndexes()[0].row()];
        emit gameSelected(tgd);
    }
}
