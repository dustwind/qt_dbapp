#include "repository.h"

Repository::Repository(QObject *parent) : QObject(parent)
{
    this->mgr = new DBManager(parent);
    this->viewPosition = 0;
}

Repository::~Repository()
{
    delete this->viewQuery;
    delete this->mgr;
}

int Repository::recId()
{
    if (this->count()>0)
    {
        return this->viewQuery->value("id").toInt();
    }
    else
    {
        return -1;
    }
}

int Repository::recYear()
{
    if (this->count()>0)
    {
        return this->viewQuery->value("year").toInt();
    }
    else
    {
        return 0;
    }
}

int Repository::recPages()
{
    if (this->count()>0)
    {
        return this->viewQuery->value("page").toInt();
    }
    else
    {
        return 0;
    }
}

QString Repository::recAuthor()
{
    if (this->count()>0)
    {
        return this->viewQuery->value("author").toString();
    }
    else
    {
        return "";
    }
}

QString Repository::recName()
{
    if (this->count()>0)
    {
        return this->viewQuery->value("name").toString();
    }
    else
    {
        return "";
    }
}

void Repository::add(QString author, QString name, int year, int pages)
{
    QSqlQuery *q = new QSqlQuery();
    q->prepare("INSERT INTO book (author,page,year,name) VALUES (:author,:page,:year,:name)");
    QMap<QString, QVariant> p;
    p.insert(":author",author);
    p.insert(":name",name);
    p.insert(":year",year);
    p.insert(":page",pages);
    this->mgr->executeQuery(q,p);
    this->viewQuery->last();
    //Определяем и присваиваем позицию текущей записи
    this->viewPosition = this->count() - 1;
    //Обновляем запрос, отображающий содержимое БД
    this->refresh(this->viewPosition);
    delete q;
}

void Repository::update(int id, QString author, QString name, int year, int pages)
{
    QSqlQuery *q = new QSqlQuery();
    q->prepare("UPDATE book SET author = :author, page = :page, year = :year, name = :name WHERE id=:id");
    QMap<QString, QVariant> p;
    p.insert(":author",author);
    p.insert(":name",name);
    p.insert(":year",year);
    p.insert(":page",pages);
    p.insert(":id", id);
    this->mgr->executeQuery(q,p);
    this->refresh(this->viewPosition);
    delete q;
}

void Repository::remove(int id)
{
    QSqlQuery *q = new QSqlQuery();
    q->prepare("DELETE FROM book where id=:id");
    QMap<QString, QVariant> p;
    p.insert(":id",id);
    this->mgr->executeQuery(q,p);

    if (this->count() == 0)
      //Если количество равно 0, то БД пустая, устанавливаем
      //  значение -1 для поля fPos
      this->viewPosition = -1;
    //Если количество записей в БД соответствует значению поля fPos,
    if (this->viewPosition == this->count())
      //  тогда удаляли последнюю запись и перемещаем указатель
      //  на предпоследнюю
      this->viewPosition = this->count() - 1;
      //  иначе указатель будет указывать на следующую запись
    //Обновляем запрос, отображающий содержимое БД
    this->refresh(this->viewPosition);
    delete q;

}

void Repository::setManager(DBManager *manager)
{
    this->mgr = manager;
    this->mgr->open();
    this->viewQuery =  new  QSqlQuery();
    this->viewQuery->prepare("SELECT * FROM book");
    //Определяем количество записей в БД, если количество равно 0,
    if (this->count() == 0)
      // тогда устанавливаем значение -1 в поле fPos,
      this->viewPosition = -1;
    else
      // иначе 0, тем самым указываем на первую запись
      this->viewPosition = 0;
      //Обновляем запрос, отображающей содержимое БД
      this->refresh(this->viewPosition);
}

DBManager* Repository::manager()
{
    return this->mgr;
}

int Repository::count()
{
    return this->mgr->getSingleScalarResult("SELECT count(*) FROM book");
}

void Repository::prev()
{
    if (this->viewQuery->previous()) {
        //  тогда уменьшаем указатель fPos на единицу,
        this->viewPosition--;
      } else {
        //  иначе текущей была первая запись, тогда перемещаемся
        //  на последнюю запись БД
        this->viewQuery->last();
        //  и устанавливаем указатель на последнюю запись
        this->viewPosition = this->count() - 1;
      }
    this->refresh(this->viewPosition);
}

void Repository::next()
{
    if (this->viewQuery->next()) {
      //  тогда увеличиваем указатель fPos на единицу
      this->viewPosition++;
    } else {
      //  иначе текущей была последняя запись, тогда перемещаемся
      //  на первую запись БД
      this->viewQuery->first();
      //  и устанавливаем указатель на первую запись
      this->viewPosition = 0;
    }
    //Обновляем запрос, отображающей содержимое БД
    this->refresh(this->viewPosition);

}

void Repository::refresh(int position)
{
    if (position != -1) {
        //Если значение параметра Pos больше -1, тогда записи в БД
        //  есть, выполняем выборку, используя запрос в объекте fSelQuery
        this->mgr->executeQuery(*this->viewQuery);
        //Перемещаем указатель на позицию Pos
        if (!this->viewQuery->seek(position))
        {
            qDebug() << "Error while seeking position. " << QString(this->viewQuery->lastError().text());
        }
        emit recIdChanged(this->viewQuery->value("id").toInt());
        emit recAuthorChanged(this->viewQuery->value("author").toString());
        emit recNameChanged(this->viewQuery->value("name").toString());
        emit recYearChanged(this->viewQuery->value("year").toInt());
        emit recPagesChanged(this->viewQuery->value("page").toInt());
      } else {
        //Если значение параметра Pos равно -1, тогда в БД нет
        //  записей, вызываем сигналы для свойств со значениями,
        //  которые обозначают отсутствие данных
        emit recIdChanged(-1);
        emit recAuthorChanged("");
        emit recNameChanged("");
        emit recYearChanged(0);
        emit recPagesChanged(0);
      }

}
