#include "addnewdialog.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

AddNewDialog::AddNewDialog(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout *nameLayout = new QHBoxLayout;
    QLabel *nameLabel = new QLabel("书名：");
    nameLine = new QLineEdit;
    nameLine->setPlaceholderText("请输入新增图书书名...");
    nameLine->setMinimumWidth(250);
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLine);

    QHBoxLayout *authorLayout = new QHBoxLayout;
    QLabel *authorLabel = new QLabel("书名：");
    authorLine = new QLineEdit;
    authorLine->setMinimumWidth(250);
    authorLine->setPlaceholderText("请输入作者...");
    authorLayout->addWidget(authorLabel);
    authorLayout->addWidget(authorLine);

    QHBoxLayout *genreLayout = new QHBoxLayout;
    QLabel *genreLabel = new QLabel("类别：");
    genreBox = new QComboBox;
    genres = new QSqlTableModel(this);
    genres->setTable("genres");
    genres->select();
    QStringList genresList;
    for(int i = 0; i < genres->rowCount(); ++i){
        genresList << genres->data(genres->index(i, 1)).toString();
    }
    genreBox->addItems(genresList);
    genreLayout->addWidget(genreLabel);
    genreLayout->addWidget(genreBox);

    QPushButton *okBtn = new QPushButton;
    okBtn->setText("确定");
    QPushButton *cancelBtn = new QPushButton;
    cancelBtn->setText("取消");
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch(0);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(authorLayout);
    mainLayout->addLayout(genreLayout);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    resize(sizeHint());

    connect(cancelBtn, &QAbstractButton::clicked, this, &QDialog::close);
    connect(okBtn, &QAbstractButton::clicked, this, &AddNewDialog::judge);
}

void AddNewDialog::closeEvent(QCloseEvent * event)
{
    nameLine->clear();
    authorLine->clear();
    genreBox->setCurrentIndex(0);
    QDialog::closeEvent(event);
}

void AddNewDialog::judge()
{
    if(nameLine->text().isEmpty()||authorLine->text().isEmpty()||genreBox->currentText().isNull()){
        QMessageBox::information(this,"消息提示","请输入完整信息");
        return;
    }
    const QString& name = nameLine->text();
    const QString& author = authorLine->text();
    int genre = genreBox->currentIndex();
    emit newBookRtned(name, author, genre);
    this->close();
}
