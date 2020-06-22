#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QWidget>
#include <QLineEdit>
#include <QDialog>

#include"accounttablemodel.h"
#include "userinterface.h"

class PasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PasswordDialog(int id, QSharedPointer<AccountTableModel> user, QWidget *parent = nullptr);
    void judge();
    void closeEvent(QCloseEvent *event);
protected:
    QLineEdit *oldPasswordLine;
    QLineEdit *newPasswordLine;
    QLineEdit *newPasswordAgainLine;
    QPushButton *saveBtn;
    QPushButton *cancelBtn;
    int id;
    QSharedPointer<AccountTableModel> userTable;
signals:
    void passwordRtned(const QString& newPassword);

};

#endif // PASSWORDDIALOG_H
