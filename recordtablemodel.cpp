#include "recordtablemodel.h"
#include <QDate>
#include <QMessageBox>

RecordTableModel::RecordTableModel(QSharedPointer<BookTableModel> bookTableModel,
                                   QSharedPointer<AccountTableModel> readerTableModel, QObject *parent)
    : QSqlRelationalTableModel(parent), bookTable(bookTableModel), readerTable(readerTableModel), baseFilter(""), timeScale(0)
{
    setTable("record");
    select();

    timer = new QTimer(this);
    timer->setInterval(600000);//in order to update every 10 minutes
    timer->start();


    setHeaderData(fieldName::recordId,Qt::Horizontal, "记录编号", Qt::DisplayRole);
    setHeaderData(fieldName::bookId,Qt::Horizontal, "图书ID", Qt::DisplayRole);
    setHeaderData(fieldName::bookName, Qt::Horizontal, "图书名", Qt::DisplayRole);
    setHeaderData(fieldName::author, Qt::Horizontal, "作者", Qt::DisplayRole);
    setHeaderData(fieldName::genre, Qt::Horizontal, "类别", Qt::DisplayRole);
    setHeaderData(fieldName::userId, Qt::Horizontal, "借书人ID", Qt::DisplayRole);
    setHeaderData(fieldName::userName, Qt::Horizontal, "借书人姓名", Qt::DisplayRole);
    setHeaderData(fieldName::borrowDate, Qt::Horizontal, "借书时间", Qt::DisplayRole);
    setHeaderData(fieldName::returnDate, Qt::Horizontal, "归还时间", Qt::DisplayRole);
    setHeaderData(fieldName::recordState, Qt::Horizontal, "记录状态", Qt::DisplayRole);

    setRelation(fieldName::genre, QSqlRelation("genres", "id", "genre"));
    setRelation(fieldName::recordState, QSqlRelation("recordStates", "id", "state"));
    select();

    /*test start*/

    /*test end*/
    connect(timer, &QTimer::timeout, this, &RecordTableModel::updateDateState);

}

int RecordTableModel::addEntry(int bookId, int userId)
{
    if(!readerTable->isExist(userId))
        return promptBorrowMessage::borrow_readerNotExist;

    QSqlRecord record = this->record();

    QSqlRecord bookRecord = bookTable->searchId(bookId);

    if(bookRecord.isEmpty()) {
        //图书不存在
        return promptBorrowMessage::borrow_bookNotExist;
    }

    QSqlQuery query;
    query.exec(QString("select * from book where bookId = '%1'").arg(bookId));
    query.next();
    int genre = query.value(BookTableModel::fieldName::genre).toInt();
    int bookState = query.value(BookTableModel::fieldName::state).toInt();

    if(bookState != BookTableModel::bookStates::Stored){
        //图书借出或丢失
        return promptBorrowMessage::bookNotAvailable;
    }
    const QString oldFilter = filter();
    setFilter("");
    select();
    int total = rowCount();

    record.setValue(fieldName::recordId, total);
    record.setValue(fieldName::bookId, bookRecord.value(BookTableModel::fieldName::bookId).toInt());
    record.setValue(fieldName::bookName, bookRecord.value(BookTableModel::fieldName::bookName).toString());//
    record.setValue(fieldName::author, bookRecord.value(BookTableModel::fieldName::author).toString());//

    record.setValue(fieldName::genre, genre);
    record.setValue(fieldName::userId, userId);
    record.setValue(fieldName::userName, readerTable->getName(userId));//
    record.setValue(fieldName::borrowDate, QDate::currentDate().toString("yyyy-MM-dd"));//
    record.setValue(fieldName::returnDate, QDate::currentDate().addMonths(2).toString("yyyy-MM-dd"));//两月内归还
    record.setValue(fieldName::recordState, recordStates::normal);//

    bool ok = insertRecord(total, record);
    bool ok_0 = submitAll();

    if(!ok||!ok_0) {
        setFilter(oldFilter);
        select();
        qDebug() << ok;
        qDebug() << ok_0;
        qDebug() << "fail to insert record in addEntry function";
        qDebug() << lastError();
        return promptBorrowMessage::insertFail;
    }
    else{
        setFilter(oldFilter);
        select();//
        if(!bookTable->changeState(bookId, BookTableModel::bookStates::Borrowed)){
            qDebug() << "fail to change state";
            return -1;
        }
        /**/
        return promptBorrowMessage::success;
    }

}

void RecordTableModel::updateDateState()
{
    const QString oldFilter = filter();
    setFilter("");
    select();
    QDate currentDate = QDate::currentDate();
    QDate returnDate;
    for(int i = 0; i < rowCount(); ++i){
        returnDate = data(index(i, RecordTableModel::returnDate)).toDate();
        if(currentDate > returnDate){
            setData(index(i, RecordTableModel::fieldName::recordState), RecordTableModel::recordStates::overTime);
        }
    }
    setFilter(oldFilter);
    select();
}

int RecordTableModel::changeReturnState(int bookId, int userId)
{
    if(!readerTable->isExist(userId))
        return promptReturnMessage::return_readerNotExist;
    if(!bookTable->isExist(bookId))
        return promptReturnMessage::return_bookNotExist;
    const QString oldFilter = filter();
    QString str = QString("bookId = '%1' and userId = '%2' "
                          "and (recordState = '%3' or recordState = '%4')").arg(bookId).arg(userId).arg(recordStates::normal).arg(recordStates::overTime);
    setFilter(str);
    select();
    if(rowCount() == 0){
        qDebug() << "rowcount = 0";
        setFilter(oldFilter);
        select();
        if(bookTable->getState(bookId) == BookTableModel::bookStates::Stored)
            return promptReturnMessage::recordNotExist;//无记录且未借出，不得归还
        else return promptReturnMessage::databaseError;
    }
    else{
        if(rowCount() != 1){
            setFilter(oldFilter);
            select();
            return promptReturnMessage::databaseError;//有多条借出记录，数据库出错
        }
        //仅有一条借出记录
        if(data(index(0, fieldName::recordState)).toInt() == recordStates::normal){
            bool ok = setData(index(0,fieldName::recordState), recordStates::returned);
            bool ok_0 = submitAll();
            if(!ok) qDebug() << "setData in return function failed";
            if(!ok_0) qDebug() << "submitAll failed in return function";
            bookTable->changeState(bookId, BookTableModel::bookStates::Stored);
            setFilter(oldFilter);
            select();
            return promptReturnMessage::normalReturn;
        }
        else if(data(index(0, fieldName::recordState)).toInt() == recordStates::overTime){
            setData(index(0,fieldName::recordState), recordStates::returned);
            setFilter(oldFilter);
            select();
            return promptReturnMessage::chargeOvertimeFine;
        }
        //图书状态出错
        else {
            return promptReturnMessage::databaseError;
        }
    }

}

void RecordTableModel::updateFilter(int scale)
{
    timeScale = scale;
    executeFilter();
}

void RecordTableModel::updateFilter(const QString &filter)
{
    baseFilter = filter;
    executeFilter();
}

void RecordTableModel::executeFilter()
{
    QString combinedFilter;
    QString timeFilter;
    //determine whether basefilter is empty
    if(baseFilter == "") {
        combinedFilter = "";
        timeFilter = "( borrowDate > '%1' )";
    }
    else{
        combinedFilter = "(";
        combinedFilter += baseFilter;
        combinedFilter +=")";
        timeFilter = " and ( borrowDate > '%1' )";
    }

    if(timeScaleType::all == timeScale){
            timeFilter = "";
    }
    else if(timeScaleType::within_3_Days == timeScale){
        timeFilter = timeFilter.arg(QDate::currentDate().addDays(-3).toString("yyyy-MM-dd"));
    }
    else if(timeScaleType::within_1_Week == timeScale){
        timeFilter = timeFilter.arg(QDate::currentDate().addDays(-7).toString("yyyy-MM-dd"));
    }
    else if(timeScaleType::within_1_Month == timeScale){
        timeFilter = timeFilter.arg(QDate::currentDate().addMonths(-1).toString("yyyy-MM-dd"));
    }
    else if(timeScaleType::within_3_Months == timeScale){
        timeFilter = timeFilter.arg(QDate::currentDate().addMonths(-3).toString("yyyy-MM-dd"));
    }
    else if(timeScaleType::within_1_year == timeScale){
        timeFilter = timeFilter.arg(QDate::currentDate().addYears(-1).toString("yyyy-MM-dd"));
    }
    else {
        qWarning() << "invalid scale";
    }

    combinedFilter += timeFilter;
    setFilter(combinedFilter);
    select();
}

QVariant RecordTableModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::TextAlignmentRole){
        return Qt::AlignCenter;
    }
    return QSqlRelationalTableModel::data(item, role);
}
