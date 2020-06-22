#include "booktablemodel.h"
#include <QMessageBox>


BookTableModel::BookTableModel(QObject* parent)
    :QSqlRelationalTableModel(parent)
{
    setTable("book");
    select();


    genres = new QSqlTableModel(this);
    states = new QSqlTableModel(this);
    genres->setTable("genres");
    genres->select();
    states->setTable("bookStates");
    states->select();

    setHeaderData(BookTableModel::bookId,Qt::Horizontal, "图书ID", Qt::DisplayRole);
    setHeaderData(BookTableModel::bookName,Qt::Horizontal, "图书名", Qt::DisplayRole);
    setHeaderData(BookTableModel::author,Qt::Horizontal, "作者", Qt::DisplayRole);
    setHeaderData(BookTableModel::genre,Qt::Horizontal, "类别", Qt::DisplayRole);
    setHeaderData(BookTableModel::state,Qt::Horizontal, "图书状态", Qt::DisplayRole);

    setRelation(genre,QSqlRelation("genres", "id", "genre"));
    setRelation(state, QSqlRelation("bookStates", "id", "state"));
    select();


}

int BookTableModel::addEntry(const QString& name, const QString& author, int genre)
{
    const QString oldFilter = filter();
    setFilter("");
    select();
    int newId = rowCount();
    QSqlRecord record = this->record();
    record.setValue(fieldName::bookId, newId);
    record.setValue(fieldName::bookName, name);
    record.setValue(fieldName::author,author);
    record.setValue(fieldName::genre,genre);
    record.setValue(fieldName::state,BookTableModel::Stored);
    if(!insertRecord(newId,record)||!submitAll()){
        qCritical() << "Fail to insert record into bookTableModel";
        setFilter(oldFilter);
        select();
        return -1;
    }
    else{
        const QString genreText = genres->data(genres->index(genre,1)).toString();
        const QString stateText = states->data(states->index(BookTableModel::Stored,1)).toString();
        qCritical() << "Add to" << tableName()<<":"
                    <<QString("(%1,%2,%3,%4)").arg(newId).arg(name).arg(genreText).arg(stateText);
        setFilter(oldFilter);
        select();
        return newId;
    }
}

int BookTableModel::deleteEntry(int bookId)
{
    const QString oldFilter = this->filter();
    setFilter("");
    select();
    this->deleteRowFromTable(bookId);
    setFilter(oldFilter);
    select();
    return bookId;
}

const QSqlRecord BookTableModel::searchId(int id)
{
    const QString oldFilter = this->filter();
    setFilter("");
    select();
    if(id > (rowCount() - 1)){
        qCritical() << "book does not exist";
        return QSqlRecord();
    }
    setFilter(QString("bookId = %1").arg(id));
    select();

    QSqlRecord record = this->record(0);
    setFilter(oldFilter);
    select();//还原
    return record;
}

QList<QSqlRecord> BookTableModel::searchName(const QString &name)
{
    setFilter(QString("bookName like '%1'").arg(QString("%") + name + QString("%")));
    select();
    QList<QSqlRecord> result;
    for(int i = 0; i < rowCount(); ++i){
        result.append(record(i));
    }
    setFilter("");
    select();//还原
    return result;
}

QList<QSqlRecord> BookTableModel::searchAuthor(const QString &authorName)
{
    setFilter(QString("author like '%1'").arg(QString("%") + authorName + QString("%")));
    select();
    QList<QSqlRecord> result;
    for(int i = 0; i < rowCount(); ++i){
        result.append(record(i));
    }
    setFilter("");
    select();//还原
    return result;
}

QList<QSqlRecord> BookTableModel::searchGenre(int genre)
{
    setFilter(QString("genre = %1").arg(genre));
    select();
    QList<QSqlRecord> result;
    for(int i = 0; i < rowCount(); ++i){
        result.append(record(i));
    }
    setFilter("");
    select();//还原
    return result;
}

bool BookTableModel::changeName(int id, const QString &name)
{
    bool ok_0 = setData(index(id,fieldName::bookName),name);
    bool ok = submitAll();
    select();

    if(!ok_0||!ok)
        return false;
    return true;
    if(!setData(index(id,fieldName::bookId),name)||!submitAll())
        return false;
    return true;
}

bool BookTableModel::changeAuthor(int id, const QString& authorName)
{
    if(!setData(index(id,fieldName::author),authorName)||!submitAll())
        return false;
    return true;
}

bool BookTableModel::changeGenre(int id, int genre)
{
    bool ok_0 = setData(index(id,fieldName::genre),genre);
    bool ok = submitAll();
    select();

    if(!ok_0||!ok)
        return false;
    return true;
}

bool BookTableModel::changeState(int id, int state)
{
    bool ok_0 = setData(index(id,fieldName::state),state);
    bool ok = submitAll();
    select();
    if(!ok_0||!ok)
        return false;
//    qDebug() <<getState(id);
    return true;
}

int BookTableModel::getState(int bookId)
{
    QSqlQuery query;
    bool ok = query.exec(QString("select * from book where bookId = '%1'").arg(bookId));
    bool ok_0 = query.next();
    if(!ok||!ok_0){
        qDebug() << lastError();
        return -1;
    }
    int final = query.value(fieldName::state).toInt();

    return final;
}

int BookTableModel::getGenre(int bookId)
{
    QSqlQuery query;
    if(!query.exec(QString("select * from book where bookId = '%1'").arg(bookId))||!query.next()){
        qDebug() << lastError();
        return -1;
    }

    int genre = query.value(fieldName::state).toInt();

    return genre;
}

QString BookTableModel::getName(int bookId)
{
    const QString oldFilter = filter();
    setFilter(QString("bookId = '%1'").arg(bookId));
    select();
    QString bookName = data(index(0, fieldName::bookName)).toString();
    setFilter(oldFilter);
    select();
    return bookName;
}

bool BookTableModel::isExist(int bookId)
{
    const QString oldFilter = this->filter();
    setFilter(QString("bookId = '%1'").arg(bookId));
    select();
    if(rowCount() != 1) {
        setFilter(oldFilter);
        select();
        return false;
    }
    else {
        setFilter(oldFilter);
        select();
        return true;
    }
}

void BookTableModel::executeFilter(const QString &filter)
{
    setFilter(filter);
    select();

    /**/
    qDebug() << filter;
    qDebug() << lastError();
}

QVariant BookTableModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }
    return QSqlRelationalTableModel::data(item, role);
}
