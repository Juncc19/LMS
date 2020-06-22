#ifndef ADMININTERFACE_H
#define ADMININTERFACE_H

#include <QListWidget>
#include <QStackedWidget>
#include <QCloseEvent>

#include "userinterface.h"
#include "bookadminwidget.h"
#include "passworddialog.h"
#include "accounttablemodel.h"
#include "readermanagementwidget.h"
class AdminInformation;

class AdminInterface : public UserInterface
{
    Q_OBJECT
public:
    explicit AdminInterface(int id, const QString& name, QSharedPointer<AccountTableModel> adminTable,
                            QSharedPointer<BookTableModel> bookTableModel,
                            QSharedPointer<AccountTableModel> readerTable,
                            QSharedPointer<RecordTableModel> recordTable,
                            QWidget *parent = nullptr);
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
    int Id;
    QString name;
    AdminInformation *info;
    QListWidget *tabIcon;
    QStackedWidget *main;
    BookAdminWidget *bookAdmin;
    readermanagementwidget *readerAdmin;
    PasswordDialog *passwordDialog;
    UserInfoDialog *userInfoDlg;


signals:

};
//用于展示用户基本信息
class AdminInformation: public UserInformation
{
    Q_OBJECT
public:
    AdminInformation(int id, const QString& name, QWidget *parent = 0);
    void update(int id, const QString& name);
};
#endif // ADMININTERFACE_H
