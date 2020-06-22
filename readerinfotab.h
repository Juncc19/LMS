#ifndef READERINFOTAB_H
#define READERINFOTAB_H

#include <QObject>
#include <QWidget>
#include<QtWidgets>
#include<QTableView>
#include<QLineEdit>
#include"readermanagementmodel.h"
#include<QPushButton>
#include"constants.h"

class readerInfoTab : public QWidget
{
    Q_OBJECT
public:
    explicit readerInfoTab(QWidget *parent = nullptr);

signals:

private slots:
    void searchID();
    void searchName();
    void fulltable();

private:
    ReaderManagementModel *model;
    QTableView *view;
    QLineEdit *idSearch;
    QLineEdit *nameSearch;
    QPushButton *idSearchButton;
    QPushButton *nameSearchButton;
    QPushButton *fulltableButton;

};

#endif // READERINFOTAB_H
