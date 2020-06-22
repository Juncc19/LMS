//This is to display the notices of new readers and the basic reader's information.

#ifndef READERMANAGEMENTMODEL_H
#define READERMANAGEMENTMODEL_H
#include<QtSql>

class ReaderManagementModel: public QSqlTableModel
{
    Q_OBJECT
public:
    ReaderManagementModel(const QString &type,QObject *parent=0);
    void searchID(const int);
    void searchName(const QString &);
    void fulltable();

private:
    QString type;

};

#endif // READERMANAGEMENTMODEL_H
