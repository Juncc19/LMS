#include "overtimeinspection.h"

overtimeInspection::overtimeInspection(QObject *parent) : QObject(parent)
{
    recordModel=new QSqlTableModel(this);
    recordModel->setTable("record");
    overtimeRecordModel=new QSqlTableModel(this);
    overtimeRecordModel->setTable("overtimeRecord");

    overtimeCheck();
    timer = new QTimer(this);
    timer->setSingleShot(1);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,this,&overtimeInspection::repeatCheck);
    int msec=QTime::currentTime().msec();   //time calibration
    timer->start(24*3600*1000+2000-msec);   //execute at around 0:00:02
}

void overtimeInspection::repeatCheck()
{
    overtimeCheck();
    delete timer;

    //create a new timer
    timer = new QTimer(this);
    timer->setSingleShot(1);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer,&QTimer::timeout,this,&overtimeInspection::repeatCheck);  //check repeatly
    int msec=QTime::currentTime().msec();   //time calibration
    timer->start(24*3600*1000+2000-msec);   //execute at around 0:00:02
}

void overtimeInspection::overtimeCheck()
{
    QString currentDate=QDate::currentDate().toString();
    recordModel->setFilter("recordState = 0");
    recordModel->select();
    recordModel->setSort(8,Qt::AscendingOrder); //select the nearest returnDate
    recordModel->select();
    if(recordModel->rowCount()==0) return;

    //compare the returnDate with current date
    int cnt=0;
    while(recordModel->record(cnt).value(8).toString()<=currentDate)
    {
        recordModel->setData(recordModel->index(cnt,9),1);
        if(!recordModel->submitAll())
        {
            qCritical().noquote() << QString("Overtime-check:record failed");
            return;
        }
        QSqlRecord recordRecord = recordModel->record(cnt);

        //add an overtime-record
        int row=overtimeRecordModel->rowCount();
        QSqlRecord overtimerecordRecord = overtimeRecordModel->record();
        overtimerecordRecord.setValue("bookId",recordRecord.value("bookId"));
        overtimerecordRecord.setValue("bookName",recordRecord.value("bookName"));
        overtimerecordRecord.setValue("author",recordRecord.value("author"));
        overtimerecordRecord.setValue("genre",recordRecord.value("genre"));
        overtimerecordRecord.setValue("userId",recordRecord.value("userId"));
        overtimerecordRecord.setValue("userName",recordRecord.value("userName"));
        overtimerecordRecord.setValue("borrowDate",recordRecord.value("borrowDate"));
        overtimerecordRecord.setValue("returnDate",recordRecord.value("returnDate"));
        overtimeRecordModel->insertRecord(row,overtimerecordRecord);
        if(!overtimeRecordModel->submitAll())
        {
            qCritical().noquote() << QString("Overtime-check:overtimeRecord failed");
            return;
        }

        ++cnt;
        if(cnt==recordModel->rowCount()) break; //break the checking function if all of the record are already checked
    }

}
