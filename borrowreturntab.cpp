#include "borrowreturntab.h"

borrowReturnTab::borrowReturnTab(QWidget *parent) : QWidget(parent)
{
    bookmodel=new QSqlTableModel(this);
    bookmodel->setTable("book");
    recordmodel=new QSqlTableModel(this);
    recordmodel->setTable("record");
    usermodel=new QSqlTableModel(this);
    usermodel->setTable("reader");
    overtimerecordmodel=new QSqlTableModel(this);
    overtimerecordmodel->setTable("overtimeRecord");

    QGroupBox *borrowGroup = new QGroupBox(tr("借阅"));
    QGroupBox *returnGroup = new QGroupBox(tr("归还"));

    userBorrow=new QLineEdit;
    userBorrow->setPlaceholderText(tr("请输入读者ID"));
    bookBorrow=new QLineEdit;
    bookBorrow->setPlaceholderText(tr("请输入书籍ID"));
    userReturn=new QLineEdit;
    userReturn->setPlaceholderText(tr("请输入读者ID"));
    bookReturn=new QLineEdit;
    bookReturn->setPlaceholderText(tr("请输入书籍ID"));

    borrowButton=new QPushButton(tr("确定"));
    returnButton=new QPushButton(tr("确定"));

    connect(borrowButton,&QPushButton::clicked,this,&borrowReturnTab::borrowBook);
    connect(returnButton,&QPushButton::clicked,this,&borrowReturnTab::returnBook);

    QGridLayout *borrowLayout=new QGridLayout;
    borrowLayout->addWidget(userBorrow,0,0);
    borrowLayout->addWidget(bookBorrow,1,0);
    borrowLayout->addWidget(borrowButton,1,1);
    borrowLayout->setColumnStretch(0,3);
    borrowLayout->setColumnStretch(1,1);
    borrowLayout->setSpacing(10);

    QGridLayout *returnLayout=new QGridLayout;
    returnLayout->addWidget(userReturn,0,0);
    returnLayout->addWidget(bookReturn,1,0);
    returnLayout->addWidget(returnButton,1,1);
    returnLayout->setColumnStretch(0,3);
    returnLayout->setColumnStretch(1,1);
    returnLayout->setSpacing(10);

    borrowGroup->setLayout(borrowLayout);
    returnGroup->setLayout(returnLayout);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(borrowGroup);
    mainLayout->addWidget(returnGroup);
    setLayout(mainLayout);

}

void borrowReturnTab::borrowBook()
{
    int userid=userBorrow->text().toInt();
    int bookid=bookBorrow->text().toInt();

    const QString oldUserFilter = usermodel->filter();
    usermodel->setFilter(QString("id = %1").arg(userid));
    usermodel->select();

    //check if this user exists
    if (usermodel->rowCount() == 0) {
        qCritical().noquote() << QString("Fail to find userID: '%1'").arg(userid);
        QMessageBox::warning(this,QString("找不到用户ID: '%1'").arg(userid),tr("请重新输入用户ID!"));
        return;
    }
    QSqlRecord userRecord= usermodel->record(0);

    const QString oldBookFilter = bookmodel->filter();
    bookmodel->setFilter(QString("bookId = %1").arg(bookid));
    bookmodel->select();

    //check if this book exists
    if (bookmodel->rowCount() == 0) {
        qCritical().noquote() << QString("Fail to find bookID: '%1'").arg(bookid);
        QMessageBox::warning(this,QString("找不到图书ID: '%1'").arg(bookid),tr("请重新输入图书ID!"));
        return;
    }

    //check if this book is in stock
    QSqlRecord bookRecord = bookmodel->record(0);
    if(bookRecord.value("state")!=0)
    {
        qCritical().noquote() << QString("This book is not in stock");
        QMessageBox::warning(this,tr("此书不在库中!"),tr("请输入其他书籍!"));
        return;
    }

    //change the bookState
    bookmodel->setData(bookmodel->index(0,4),1);    // 1 means being lent out

    //write a new record for this transaction
    int row=recordmodel->rowCount();
    QSqlRecord recordRecord = recordmodel->record();
    QDate currentTime=QDateTime::currentDateTime().date();
    recordRecord.setValue("recordId",row);
    recordRecord.setValue("bookId",bookRecord.value("bookId"));
    recordRecord.setValue("bookName",bookRecord.value("bookName"));
    recordRecord.setValue("author",bookRecord.value("author"));
    recordRecord.setValue("genre",bookRecord.value("genre"));
    recordRecord.setValue("userId",userRecord.value("id"));
    recordRecord.setValue("userName",userRecord.value("name"));
    recordRecord.setValue("borrowDate",currentTime.toString());
    //recordRecord.setValue("returnDate",currentTime.toString());   //test
    recordRecord.setValue("returnDate",currentTime.addMonths(LOAN_PERIOD).toString());
    recordRecord.setValue("recordState",0);
    recordmodel->insertRecord(row,recordRecord);

    if(bookmodel->submitAll()&&recordmodel->submitAll())
    {
        QMessageBox::information(this,tr("提示"),tr("借书成功!"));
    }
    else QMessageBox::warning(this,tr("提示"),tr("借书失败!"));

    //restore the filter
    bookmodel->setFilter(oldBookFilter);
    bookmodel->select();
    usermodel->setFilter(oldUserFilter);
    usermodel->select();

    userBorrow->clear();
    bookBorrow->clear();

}

void borrowReturnTab::returnBook()
{
    int userid=userReturn->text().toInt();
    int bookid=bookReturn->text().toInt();

    const QString oldUserFilter = usermodel->filter();
    usermodel->setFilter(QString("id = '%1'").arg(userid));
    usermodel->select();

    //check if this user exists
    if (usermodel->rowCount() == 0) {
        qCritical().noquote() << QString("Fail to find userID: '%1'").arg(userid);
        QMessageBox::warning(this,QString("找不到用户ID: '%1'").arg(userid),tr("请重新输入用户ID!"));
        return;
    }
    QSqlRecord userRecord= usermodel->record(0);

    const QString oldBookFilter = bookmodel->filter();
    bookmodel->setFilter(QString("bookId = '%1'").arg(bookid));
    bookmodel->select();

    //check if this book exists
    if (bookmodel->rowCount() == 0) {
        qCritical().noquote() << QString("Fail to find bookID: '%1'").arg(bookid);
        QMessageBox::warning(this,QString("找不到图书ID: '%1'").arg(bookid),tr("请重新输入图书ID!"));
        return;
    }

    //check if this book is lent out normally
    QSqlRecord bookRecord = bookmodel->record(0);
    if(bookRecord.value("state")!=1)
    {
        qCritical().noquote() << QString("This book is not lent out or is lost");
        QMessageBox::warning(this,tr("此书在库中或以丢失!"),tr("请输入其他书籍!"));
        return;
    }

    //search the record and check if it exists
    const QString oldRecordFilter = recordmodel->filter();
    recordmodel->setFilter(QString("bookId = '%1' and userId = '%2' and (recordState = 0 or recordState = 1)").arg(bookid).arg(userid));
    recordmodel->select();
    if (recordmodel->rowCount() == 0) {
        qCritical().noquote() << QString("Fail to find such borrowing record");
        QMessageBox::warning(this,tr("记录查询失败"),tr("没有找到此条记录!"));
        return;
    }

    //remove the overtime-record
    if(recordmodel->record(0).value(9)==1)
    {
        const QString oldovertimeRecordFilter = overtimerecordmodel->filter();
        overtimerecordmodel->setFilter(QString("bookId = '%1'").arg(bookid));
        overtimerecordmodel->select();
        overtimerecordmodel->removeRow(0);
        if(!overtimerecordmodel->submitAll())
        {
            qCritical().noquote() << QString("Failed to remove such overtimeRecord");
            QMessageBox::warning(this,tr("移除失败"),tr("未能移除超时记录!"));
            return;
        }
        overtimerecordmodel->setFilter(oldovertimeRecordFilter);
        overtimerecordmodel->select();
    }

    //change the bookState and the record's returnDate
    bookmodel->setData(bookmodel->index(0,4),0);
    recordmodel->setData(recordmodel->index(0,9),2);
    QDate currentDate=QDate::currentDate();
    recordmodel->setData(recordmodel->index(0,8),currentDate.toString());

    if(bookmodel->submitAll()&&recordmodel->submitAll())
    {
        QMessageBox::information(this,tr("提示"),tr("还书成功!"));
    }
    else QMessageBox::warning(this,tr("提示"),tr("还书失败!"));

    //restore the filter
    bookmodel->setFilter(oldBookFilter);
    bookmodel->select();
    usermodel->setFilter(oldUserFilter);
    usermodel->select();
    recordmodel->setFilter(oldRecordFilter);
    recordmodel->select();

    userReturn->clear();
    bookReturn->clear();
}
