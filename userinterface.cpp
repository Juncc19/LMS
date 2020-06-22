#include "userinterface.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

UserInterface::UserInterface(int id, const QString &name, QWidget *parent)
    : QMainWindow(parent), ID(id), name(name)
{
}

UserInformation::UserInformation(int id, const QString &name, QWidget *parent)
    :QGroupBox(parent)
{
    setTitle("用户信息");
    setFixedWidth(140);
    setMaximumHeight(120);
    info = new QListWidget;
    QListWidgetItem *item0 = new QListWidgetItem(QString("ID：%1").arg(id));
    nameItem = new QListWidgetItem(QString("姓名：%1").arg(name));
    info->addItem(item0);
    info->addItem(nameItem);
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(info);
    setLayout(mainlayout);
}

void UserInformation::addItem(QListWidgetItem *item)
{
    info->addItem(item);
}

void UserInformation::changeName(const QString &name)
{
    nameItem->setText(QString("姓名：%1").arg(name));
}

UserInfoDialog::UserInfoDialog(int id, const QString& name,QWidget *parent)
    :QDialog(parent), id(id), name(name)
{
    setWindowTitle("修改个人信息");
    QHBoxLayout *nameLayout = new QHBoxLayout;
    QLabel *nameLabel = new QLabel;
    nameLabel->setText(tr("姓名："));
    nameLine = new QLineEdit;
    nameLine->setPlaceholderText(name);
    nameLine->setMinimumWidth(150);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLine);

    QHBoxLayout *dlgBtnLayout = new QHBoxLayout;
    dlgBtnLayout->addStretch(0);
    saveBtn = new QPushButton;
    saveBtn->setText("保存");
    cancelBtn = new QPushButton;
    cancelBtn->setText("取消");
    dlgBtnLayout->addWidget(saveBtn);
    dlgBtnLayout->addWidget(cancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(dlgBtnLayout);

    this->setLayout(mainLayout);
    this->resize(sizeHint());

    connect(cancelBtn,&QAbstractButton::clicked,this,&UserInfoDialog::close);
    connect(saveBtn, &QAbstractButton::clicked, this, &UserInfoDialog::judge);
}

void UserInfoDialog::judge(){
    if(nameLine->text().isEmpty()){
        QMessageBox::information(this,tr("消息提示"), tr("未输入任何字符"));
        return;
    }
    const QString& newName = nameLine->text();
    emit infoRtned(id, newName);
    QMessageBox::information(this, tr("消息提示"), tr("信息修改成功"));
    this->close();
}

void UserInfoDialog::closeEvent(QCloseEvent * event)
{
    nameLine->clear();
    return QDialog::closeEvent(event);
}
