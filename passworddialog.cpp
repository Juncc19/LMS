#include "passworddialog.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>


PasswordDialog::PasswordDialog(int id, QSharedPointer<AccountTableModel> user, QWidget *parent)
    : QDialog(parent), id(id), userTable(user)
{
    this->setWindowTitle("修改密码");
    QHBoxLayout *dlgBtnLayout = new QHBoxLayout;
    dlgBtnLayout->addStretch(0);
    saveBtn = new QPushButton;
    saveBtn->setText("保存");
    cancelBtn = new QPushButton;
    cancelBtn->setText("取消");
    dlgBtnLayout->addWidget(saveBtn);
    dlgBtnLayout->addWidget(cancelBtn);

    QHBoxLayout *oldPasswordBuddy = new QHBoxLayout;
    QLabel *oldPasswordLabel = new  QLabel;
    oldPasswordLabel->setText("原密码");
    oldPasswordLine = new QLineEdit;
    oldPasswordLine->setPlaceholderText("请输入原密码");
    oldPasswordLine->setEchoMode(QLineEdit::Password);
    oldPasswordLine->setMinimumWidth(250);
    oldPasswordBuddy->addWidget(oldPasswordLabel);
    oldPasswordBuddy->addWidget(oldPasswordLine);

    QHBoxLayout *newPasswordBuddy = new QHBoxLayout;
    QLabel *newPasswordLabel = new  QLabel;
    newPasswordLabel->setText("新密码");
    newPasswordLine = new QLineEdit;
    newPasswordLine->setPlaceholderText("请输入新密码");
    newPasswordLine->setEchoMode(QLineEdit::Password);
    newPasswordLine->setMinimumWidth(300);
    newPasswordBuddy->addWidget(newPasswordLabel);
    newPasswordBuddy->addWidget(newPasswordLine);

    QHBoxLayout *newPasswordAgainBuddy = new QHBoxLayout;
    QLabel *newPasswordAgainLabel = new  QLabel;
    newPasswordAgainLabel->setText("新密码");
    newPasswordAgainLine = new QLineEdit;
    newPasswordAgainLine->setPlaceholderText("请再次输入新密码");
    newPasswordAgainLine->setEchoMode(QLineEdit::Password);
    newPasswordAgainLine->setMinimumWidth(250);
    newPasswordAgainBuddy->addWidget(newPasswordAgainLabel);
    newPasswordAgainBuddy->addWidget(newPasswordAgainLine);

    QVBoxLayout *diaglogLayout = new QVBoxLayout;
    diaglogLayout->addLayout(oldPasswordBuddy);
    diaglogLayout->addLayout(newPasswordBuddy);
    diaglogLayout->addLayout(newPasswordAgainBuddy);
    diaglogLayout->addLayout(dlgBtnLayout);
    this->setLayout(diaglogLayout);

    this->resize(sizeHint());

    this->setModal(true);

    connect(cancelBtn,&QAbstractButton::clicked,this,&PasswordDialog::close);
    connect(saveBtn, &QAbstractButton::clicked, this, &PasswordDialog::judge);

}
void PasswordDialog::judge()
{
    if(oldPasswordLine->text().isEmpty()){
        QMessageBox::information(this,"消息提示","未输入原密码");
        return;
    }
    if(newPasswordLine->text().isEmpty()){
        QMessageBox::information(this,"消息提示","未输入新密码");
        return;
    }
    if(newPasswordAgainLine->text().isEmpty()){
        QMessageBox::information(this,"消息提示","请输入新密码两次");
        return;
    }

    const QString& inputOldPassword = PasswordDialog::oldPasswordLine->text();
    userTable->setFilter(QString("id = %1").arg(id));
    userTable->select();
    if(userTable->rowCount()!=1){
        QMessageBox::warning(this,"错误","系统错误");
        userTable->setFilter("");
        userTable->select();
        return;
    }
    const QString& correctPassword = userTable->data(userTable->index(0,UserInterface::password)).toString();

    if(inputOldPassword!=correctPassword){
        QMessageBox::information(this,"消息提示","原密码错误");
        userTable->setFilter("");
        userTable->select();
        this->close();
        return;
    }

    const QString& newPassword = PasswordDialog::newPasswordLine->text();
    const QString& newPasswordAgain = PasswordDialog::newPasswordAgainLine->text();

    if(newPassword!=newPasswordAgain){
        QMessageBox::information(this,"消息提示","两次输入密码不相同，请重试");
        newPasswordLine->clear();
        newPasswordAgainLine->clear();
        userTable->setFilter("");
        userTable->select();
        return;
    }

    if(newPassword==newPasswordAgain){
        userTable->changePassword(id,newPassword);
        QMessageBox::information(this,"消息提示","修改成功");
        userTable->setFilter("");
        userTable->select();
        this->close();
        return;
    }
}

void PasswordDialog::closeEvent(QCloseEvent *event)
{
    oldPasswordLine->clear();
    newPasswordLine->clear();
    newPasswordAgainLine->clear();
    return QDialog::closeEvent(event);
}
