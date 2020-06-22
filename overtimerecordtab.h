#ifndef OVERTIMERECORDTAB_H
#define OVERTIMERECORDTAB_H

#include <QObject>
#include <QWidget>
#include<QtWidgets>
#include<QtSql>
#include"constants.h"

class overtimeRecordTab : public QWidget
{
    Q_OBJECT
public:
    explicit overtimeRecordTab(QWidget *parent = nullptr);

signals:

private slots:
    void searchID();
    void searchName();
    void fulltable();
    void removeRow();

private:
    QSqlTableModel *model;
    QTableView *view;
    QLineEdit *idSearch;
    QLineEdit *nameSearch;
    QPushButton *idSearchButton;
    QPushButton *nameSearchButton;
    QPushButton *fulltableButton;
    QPushButton *removeRowButton;
};

#endif // OVERTIMERECORDTAB_H
