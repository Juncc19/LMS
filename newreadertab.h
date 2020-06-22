#ifndef NEWREADERTAB_H
#define NEWREADERTAB_H

#include <QObject>
#include <QWidget>
#include<QtWidgets>
#include"readermanagementmodel.h"
#include"constants.h"

class newReaderTab : public QWidget
{
    Q_OBJECT
public:
    explicit newReaderTab(QWidget *parent = nullptr);

signals:

private slots:
    void searchID();
    void searchName();
    void fulltable();
    void emptytable();

private:
    ReaderManagementModel *model;
    QTableView *view;
    QLineEdit *idSearch;
    QLineEdit *nameSearch;
    QPushButton *idSearchButton;
    QPushButton *nameSearchButton;
    QPushButton *fulltableButton;
    QPushButton *emptyButton;
};

#endif // NEWREADERTAB_H
