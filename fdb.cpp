#include <QSqlDatabase>
#include <QSqlQuery>

void init() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("fusion.db");
    bool ok = db.open();
    //ui->label->setText(ok ? "Connected!" : "Failed");
    if(!ok)
    {
        //show a popup window / alert about the error
        return;
    }
    QSqlQuery query;
    QSqlQuery queryPreferences;
    QSqlQuery queryGames;

    query.exec("CREATE TABLE IF NOT EXISTS prefs(key TINYTEXT NOT NULL, valuetype TINYINT NOT NULL, number TINYINT NOT NULL, text VARCHAR(255) NOT NULL);");
    queryPreferences.exec("SELECT * FROM prefs;");
    while(queryPreferences.next())
    {
        //get values here
    }
    //ui->label->setText(queryPreferences.value(1).toString());   for loop here?
    //(later) if clientToken doesnt exists, show login and run registerClient(), if no account, //run register()
    //(later) if lang is not set, set it to the default system language
    //Then get game list
    query.exec("CREATE TABLE IF NOT EXISTS games(gameName TEXT NOT NULL, gameType TINYINT NOT NULL , gameDirectory TEXT NOT NULL, relExecutablePath TEXT NOT NULL, PRIMARY KEY(gameName));");
    queryGames.exec("SELECT * FROM games");
    //TODO: get os and convert the full path  (Set by a #define OS)
    //TODO: add everything to list

}
