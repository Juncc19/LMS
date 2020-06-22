#ifndef BORROWRETURNTAB_H
#define BORROWRETURNTAB_H

#include <QObject>
#include <QWidget>
#include<QtWidgets>
#include<QtSql>
#include<QSqlRecord>
#include"constants.h"
#include"initdb.h"

class borrowReturnTab : public QWidget
{
    Q_OBJECT
public:
    explicit borrowReturnTab(QWidget *parent = nullptr);

signals:

private slots:
    void borrowBook();
    void returnBook();
private:
    QSqlTableModel *bookmodel;
    QSqlTableModel *recordmodel;
    QSqlTableModel *overtimerecordmodel;
    QSqlTableModel *usermodel;
    QLineEdit *userBorrow;
    QLineEdit *bookBorrow;
    QLineEdit *userReturn;
    QLineEdit *bookReturn;
    QPushButton *borrowButton;
    QPushButton *returnButton;
};

#endif // BORROWRETURNTAB_H
