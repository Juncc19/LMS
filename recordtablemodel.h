#ifndef RECORDTABLEMODEL_H
#define RECORDTABLEMODEL_H

#include <QObject>
#include <QtSql>
#include "booktablemodel.h"
#include "accounttablemodel.h"
#include <QTimer>

class RecordTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    enum fieldName{
        recordId, bookId, bookName, author, genre,
        userId, userName, borrowDate, returnDate, recordState
    };
    enum timeScaleType{
        within_3_Days, within_1_Week, within_1_Month,
        within_3_Months, within_1_year, all
    };
    enum recordStates{
        normal, overTime, returned
    };
    enum promptBorrowMessage{
        success, borrow_bookNotExist, bookNotAvailable, insertFail, borrow_readerNotExist
    };
    enum promptReturnMessage{
        normalReturn, chargeOvertimeFine, recordNotExist, databaseError, return_bookNotExist, return_readerNotExist
    };

    explicit RecordTableModel(QSharedPointer<BookTableModel> bookTable,
                              QSharedPointer<AccountTableModel> readerTable, QObject *parent = nullptr);
    int addEntry(int bookId, int userId);
    void updateDateState();//更新有关时间记录状态(overtime or still normal)
    int changeReturnState(int bookId, int userId);//改变有关借出/归还的记录状态
    void updateFilter(int scale);//更新当前filter
    void updateFilter(const QString& filter);//更新当前filter
    void executeFilter();//执行当前filter
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;

signals:

private:
    QSharedPointer<BookTableModel> bookTable;
    QSharedPointer<AccountTableModel> readerTable;
    QTimer *timer;
    QString baseFilter;
    int timeScale;


};

#endif // RECORDTABLEMODEL_H
