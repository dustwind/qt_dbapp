#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QObject>
#include <dbmanager.h>
class Repository : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DBManager* manager READ manager
        WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(int recId READ recId NOTIFY recIdChanged)
    Q_PROPERTY(QString recAuthor READ recAuthor NOTIFY recAuthorChanged)
    Q_PROPERTY(QString recName READ recName NOTIFY recNameChanged)
    Q_PROPERTY(int recYear READ recYear NOTIFY recYearChanged)
    Q_PROPERTY(int recPages READ recPages NOTIFY recPagesChanged)
public:
    explicit Repository(QObject *parent = 0);
    DBManager* manager();
    int recId();
    QString recAuthor();
    QString recName();
    int recYear();
    int recPages();
    void setManager(DBManager *manager);
    ~Repository();

    Q_INVOKABLE void add(QString author, QString name, int year, int pages);
    Q_INVOKABLE void update(int id, QString author, QString name, int year, int pages);
    Q_INVOKABLE void remove(int id);
    Q_INVOKABLE void prev();
    Q_INVOKABLE void next();

protected:
    DBManager *mgr;
    int count();
    QSqlQuery *viewQuery;
    int viewPosition;
    void refresh(int position);
signals:
    void managerChanged(DBManager *manager);
    void recIdChanged(int Id);
    void recNameChanged(QString Name);
    void recYearChanged(int Year);
    void recPagesChanged(int Pages);
    void recAuthorChanged(QString Author);
};

#endif // REPOSITORY_H
