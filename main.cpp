#include "loginwidget.h"

#include <QApplication>
#include <QSharedPointer>
#include "initdb.h"
#include "accounttablemodel.h"
#include "loginwidget.h"
#include "admininterface.h"
#include "studentinterface.h"
#include "recordtablemodel.h"
#include"overtimeinspection.h"
initDb ini;
QSharedPointer<AccountTableModel> readerTable;
QSharedPointer<AccountTableModel> adminTable;
QSharedPointer<BookTableModel> bookTable;
QSharedPointer<RecordTableModel> recordTable;
LoginWidget *loginWidget;
UserInterface *mainInterface;
overtimeInspection *overtimeinspection;

void initial();
void clear();
void openInterface(int id, int type);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("图书管理系统");
    initial();

    QObject::connect(loginWidget, &LoginWidget::loginSuccessfully, &openInterface);

    loginWidget->show();

    if(a.exec()){
        clear();
    }
    return 0;
}
void initial(){
    QSqlError error = ini();
    if (error.type() != QSqlError::NoError) {
        qWarning() <<  "Error in SQL initialization: " << error;
        // 建议在需要输出调试/错误消息时这样写，方便后面进行日志的格式化与管理
    }
    overtimeinspection = new overtimeInspection;
    readerTable = (QSharedPointer<AccountTableModel>)new AccountTableModel("reader");
    adminTable = (QSharedPointer<AccountTableModel>)new AccountTableModel("admin");
    bookTable = (QSharedPointer<BookTableModel>)new BookTableModel;
    recordTable=(QSharedPointer<RecordTableModel>)new RecordTableModel(bookTable, readerTable);
    loginWidget = new LoginWidget(readerTable, adminTable);

}

void clear(){
    delete loginWidget;
    delete mainInterface;
    delete overtimeinspection;
}

void openInterface(int id, int type)
{
    if(type == AccountTableModel::admin){
        adminTable->setFilter(QString("id = %1").arg(id));
        adminTable->select();
        const QString &name = adminTable->data(adminTable->index(0,UserInterface::Name)).toString();
        adminTable->setFilter("");//还原
        adminTable->select();
        mainInterface = new AdminInterface(id, name, adminTable, bookTable, readerTable, recordTable);
        mainInterface->show();
    }
    else if(type==AccountTableModel::reader){
        readerTable->setFilter(QString("id = %1").arg(id));
        readerTable->select();
        const QString &name=readerTable->data(readerTable->index(0,2)).toString();
        readerTable->setFilter("");
        readerTable->select();
        mainInterface=new studentinterface(id, name, readerTable, bookTable, recordTable);
        mainInterface->show();
    }

}
