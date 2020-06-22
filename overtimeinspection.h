#ifndef OVERTIMEINSPECTION_H
#define OVERTIMEINSPECTION_H

#include <QObject>
#include<QTimer>
#include<QtSql>

class overtimeInspection : public QObject
{
    Q_OBJECT
public:
    explicit overtimeInspection(QObject *parent = nullptr);
    void repeatCheck();
    void overtimeCheck();

private:
    QTimer *timer;
    QSqlTableModel *recordModel;
    QSqlTableModel *overtimeRecordModel;


signals:

};

#endif // OVERTIMEINSPECTION_H
