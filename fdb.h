#ifndef FDB_H
#define FDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class FDB : public QObject
{
    Q_OBJECT
public:
    explicit FDB(QObject *parent = 0);
    bool init();

private:
    QSqlDatabase db;
    QSqlQuery query;

signals:

public slots:
};

#endif // FDB_H
