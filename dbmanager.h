#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>

class DBManager : public QObject
{
    Q_OBJECT
    //Свойство, хранящее имя БД. Свойство на запись можно использовать
    //  только один раз
    Q_PROPERTY(QString databaseName READ databaseName
        WRITE setDataBaseName NOTIFY dataBaseNameChanged)
public:
    explicit DBManager(QObject *parent = 0);
    //Деструктор
    ~DBManager();
    //Метод-чтения, возвращающий значение свойства
    void setDataBaseName(QString dataBaseName);
    QString databaseName() const;
    int getSingleScalarResult(QString request);
    void executeQuery(QSqlQuery &query);
    void executeQuery(QSqlQuery *query, QMap<QString,QVariant> params);
    bool open();
protected:
    QSqlDatabase database;
    QString dbName;

signals:
    //Сигнал, возникающий при изменении значения свойства dataBaseName
    void dataBaseNameChanged(QString DBName);

public slots:

};

#endif // DBMANAGER_H
