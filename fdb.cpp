#include "fdb.h"

FDB::FDB(QObject *parent) : QObject(parent)
{

}

bool FDB::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("fusion.db");
    bool ok = db.open();
    if(!ok)
    {
        // failed to open the db
        return false;
    }
    query = QSqlQuery(db);
    query.exec("CREATE TABLE IF NOT EXISTS preferences (key TEXT NOT NULL, valuetype INTEGER NOT NULL, number INTEGER, text TEXT);");
    query.exec("CREATE TABLE IF NOT EXISTS games (id INTEGER NOT NULL AUTO_INCREMENT, name TEXT NOT NULL, type INTEGER NOT NULL,");
    return true;
}
