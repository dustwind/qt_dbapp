#include "dbmanager.h"

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    this->database = QSqlDatabase::addDatabase("QSQLITE");
}

DBManager::~DBManager()
{
    this->database.close();
}

void DBManager::setDataBaseName(QString databaseName)
{
    this->dbName = databaseName;
    this->database.setDatabaseName(this->dbName);
}

bool DBManager::open()
{
    if (this->database.open())
    {
        return true;

    }
    else
    {
        qDebug() << "БД не открыта. " << this->database.lastError().text();
        emit dataBaseNameChanged(this->dbName);
        return false;
    }
}

QString DBManager::databaseName() const
{
    return this->dbName;
}

int DBManager::getSingleScalarResult(QString request)
{
    QSqlQuery query = this->database.exec(request);

        if (query.first())
        {
            QVariant val = query.value(0);
            return val.toInt();
        }
        else
        {
            qDebug() << "Error while executing request. " << QString(query.lastError().text());

            return 0;
        }
}

void DBManager::executeQuery(QSqlQuery *query, QMap<QString, QVariant> params)
{
    QMap<QString, QVariant>::iterator i;
    for (i = params.begin(); i != params.end(); i++)
    {
        query->bindValue(i.key(), i.value());
    }
    if (!query->exec())
    {
        qDebug() << "Error while executing request. " << QString(query->lastError().text());
    }
}

void DBManager::executeQuery(QSqlQuery &query)
{
    if (!query.exec())
    {
        qDebug() << "Error while executing request. " << QString(query.lastError().text());
    }
}
