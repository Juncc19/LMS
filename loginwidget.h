#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "accounttablemodel.h"
#include "registerwidget.h"
#include "admininterface.h"

#include <QtWidgets>
#include <QSharedPointer>

class LoginWidget : public QDialog
{
    Q_OBJECT
    friend class RegisterWidget; // 声明为友元访问私有槽
public:
    LoginWidget(QSharedPointer<AccountTableModel> readerTable,
                QSharedPointer<AccountTableModel> adminTable,
                QWidget *parent = nullptr);
private slots:
    void autoFill(int id, const QString &password); // auto fill after account created 注册成功后自动填充
    void judgeLogin();
signals:
    void loginSuccessfully(int id, int type);

private:
    QButtonGroup *typeSelected;
    RegisterWidget *registerWidget;
    QSharedPointer<AccountTableModel> readerTable;
    QSharedPointer<AccountTableModel> adminTable;
    QLineEdit *idEdit, *passwordEdit;
};

#endif // LOGINWIDGET_H
