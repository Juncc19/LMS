/**This file is used to initialize database needed in this program
 * and provide some enums to make code clearer.
 **/

#ifndef TEMPDB_H
#define TEMPDB_H

#include <QtSql>
#include <QDebug>

enum recordStates {
    Normal, Timeout, Returned
};

enum genres {
    //...
};

class initDb
{
public:
    //functor 仿函数
    QSqlError operator()()
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("library.db");
        if (!db.open())
            return db.lastError();

        QStringList tables = db.tables();
        QSqlQuery q;
        QSqlError error;

        if (!tables.contains("recordStates")) {
            //prevent creating repeatedly 防止重复创建
            if (!q.exec(createRecordStates))
                return q.lastError();
            if (!q.prepare(insertRecordStates))
                return q.lastError();

            QStringList recordStates;
            recordStates << "正常" << "超时" << "归还";

            for (int id = 0; id < recordStates.size(); ++id) {
                //To start from 1, change id to id+1  如果从1开始就把id改成id+1
                error = addEntry(q, id, recordStates.at(id));
                if (error.type() != QSqlError::NoError)
                    return error;
            }
        }

        if (!tables.contains("bookStates")) {
            if (!q.exec(createBookStates))
                return q.lastError();
            if (!q.prepare(insertBookStates))
                return q.lastError();

            QStringList bookStates;
            bookStates << "在库" << "外借" << "丢失";

            for (int id = 0; id < bookStates.size(); ++id) {
                error = addEntry(q, id, bookStates.at(id));
                if (error.type() != QSqlError::NoError)
                    return error;
            }
        }

        if (!tables.contains("genres")) {
            if (!q.exec(createGenres))
                return q.lastError();
            if (!q.prepare(insertGenres))
                return q.lastError();

            QStringList genres;
            genres << "马克思主义、列宁主义、毛泽东思想" << "哲学" << "社会科学总论" << "政治、法律" << "军事" << "经济" << "文化、科学、教育、体育"
                   << "语言、文字" << "文学" << "艺术" << "历史、地理" << "自然科学总论" << "数理科学和化学" << "天文学、地球科学"
                   << "生物科学" << "医药、卫生" << "农业科学" << "工业技术" << "交通运输" << "航空、航天" << "环境科学"
                   << "综合性图书";

            for (int id = 0; id < genres.size(); ++id) {
                error = addEntry(q, id, genres.at(id));
                if (error.type() != QSqlError::NoError)
                    return error;
            }
        }

        if (!tables.contains("reader")) {
            if (!q.exec(createReader))
                return q.lastError();
        }

        if (!tables.contains("admin")) {
            if (!q.exec(createAdmin))
                return q.lastError();
        }

        if (!tables.contains("book")) {
            if (!q.exec(createBook))
                return q.lastError();
        }

        if (!tables.contains("record")) {
            if (!q.exec(createRecord))
                return q.lastError();
        }
        /*initial admin*/
        q.exec("insert into admin(id, password, name) values(0, 123456, 'admin')");
        /*initial admin*/

        if (!tables.contains("newReader")) {
            if (!q.exec(createNewReader))
                return q.lastError();
        }

        if (!tables.contains("overtimeRecord")) {
            if (!q.exec(createOvertimeRecord))
                return q.lastError();
        }

        q.exec("insert into book values(0, 'bookName', 'authorName', 0, 0)");  //insert test data
        q.exec("insert into book values(1, 'bookName', 'authorName', 0, 0)");  //insert test data
        q.exec("insert into book values(2, 'bookName', 'authorName', 0, 0)");  //insert test data

        return QSqlError();
    }

    void close() {
        db.close();
    }

private:
    //creat/insert command 创建插入指令
    const QString createReader = "create table reader(id integer primary key, password varchar, name varchar)";

    const QString createAdmin = "create table admin(id integer primary key, password varchar, name varchar)";

    const QString createBook = "create table book(bookId integer primary key, bookName varchar, author varchar, genre integer, state integer)";

    const QString createRecord = "create table record(recordId integer primary key, bookId integer, bookName varchar, author varchar, genre integer, "
                                                  "userId integer, userName varchar, borrowDate date, returnDate date, recordState integer)";

    const QString createRecordStates = "create table recordStates(id integer primary key, state varchar)";
    const QString insertRecordStates = "insert into recordStates(id, state) values(?, ?)";

    const QString createBookStates = "create table bookStates(id integer primary key, state varchar)";
    const QString insertBookStates = "insert into bookStates(id, state) values(?, ?)";

    const QString createGenres = "create table genres(id integer primary key, genre varchar)";
    const QString insertGenres = "insert into genres(id, genre) values(?, ?)";

    const QString createNewReader="create table newReader(id integer primary key,password varchar,name varchar)";

    const QString createOvertimeRecord="create table overtimeRecord(bookId integer primary key, bookName varchar, author varchar, genre integer, "
                                       "userId integer, userName varchar, borrowDate date, returnDate date)";
    QSqlDatabase db;

    //auxiliary function 辅助函数
    QSqlError addEntry(QSqlQuery &q, int id, const QString &var)
    {
        q.addBindValue(id);
        q.addBindValue(var);
        if (!q.exec())
            return q.lastError();
        return QSqlError();
    }
};

#endif // TEMPDB_H
