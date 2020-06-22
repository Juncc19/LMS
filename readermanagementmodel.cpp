#include"readermanagementmodel.h"

ReaderManagementModel::ReaderManagementModel(const QString &type, QObject *parent)
    :QSqlTableModel(parent)
{
    this->type = type;
    setTable(type);
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("密码"));
    setHeaderData(2, Qt::Horizontal, tr("姓名"));
    select();
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void ReaderManagementModel::searchID(const int id)
{
    setFilter(QString("id='%1'").arg(id));
    select();

    if(rowCount()==0)
    {
        qCritical().noquote()<<QString("Fail to find : ID '%1'").arg(id);
    }

    return;
}

void ReaderManagementModel::searchName(const QString &name)
{
    setFilter(QString("name='%1'").arg(name));
    select();

    if(rowCount()==0)
    {
        qCritical().noquote()<<QString("Fail to find : Name '%1'").arg(name);
    }

    return;
}

void ReaderManagementModel::fulltable()
{
    setFilter("");
    select();
}
