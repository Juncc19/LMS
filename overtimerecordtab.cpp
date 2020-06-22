#include "overtimerecordtab.h"

overtimeRecordTab::overtimeRecordTab(QWidget *parent) : QWidget(parent)
{
    model= new QSqlRelationalTableModel(this);
    model->setTable("overtimeRecord");
    model->setRelation(3,QSqlRelation("genres","id","genre"));  //set the relation between the tables
    model->setHeaderData(0,Qt::Horizontal,tr("图书ID"));
    model->setHeaderData(1,Qt::Horizontal,tr("书名"));
    model->setHeaderData(2,Qt::Horizontal,tr("作者"));
    model->setHeaderData(3,Qt::Horizontal,tr("图书分类"));
    model->setHeaderData(4,Qt::Horizontal,tr("用户ID"));
    model->setHeaderData(5,Qt::Horizontal,tr("用户名"));
    model->setHeaderData(6,Qt::Horizontal,tr("借书日期"));
    model->setHeaderData(7,Qt::Horizontal,tr("归还限期"));
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    view=new QTableView(this);
    view->setModel(model);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    idSearch=new QLineEdit(this);
    idSearch->setPlaceholderText("请输入图书ID");
    idSearch->setValidator(new QIntValidator(idSearch));
    idSearch->setAttribute(Qt::WA_InputMethodEnabled,false);

    nameSearch=new QLineEdit(this);
    nameSearch->setPlaceholderText("请输入书名");
    nameSearch->setMaxLength(MAX_NAME_LENGTH);
    nameSearch->setAttribute(Qt::WA_InputMethodEnabled,false);
    nameSearch->setValidator(new QRegExpValidator(QRegExp("[\\S]*"), nameSearch));

    idSearchButton=new QPushButton(tr("图书ID查询"),this);
    nameSearchButton=new QPushButton(tr("书名查询"),this);
    removeRowButton=new QPushButton(tr("删除选中行"),this);
    fulltableButton=new QPushButton(tr("显示全部"),this);
    connect(idSearchButton,&QPushButton::clicked,this,&overtimeRecordTab::searchID);
    connect(nameSearchButton,&QPushButton::clicked,this,&overtimeRecordTab::searchName);
    connect(removeRowButton,&QPushButton::clicked,this,&overtimeRecordTab::removeRow);
    connect(fulltableButton,&QPushButton::clicked,this,&overtimeRecordTab::fulltable);

    QGridLayout *searchLayout=new QGridLayout;
    searchLayout->addWidget(idSearch,0,0);
    searchLayout->addWidget(idSearchButton,0,1);
    searchLayout->addWidget(nameSearch,1,0);
    searchLayout->addWidget(nameSearchButton,1,1);
    searchLayout->addWidget(removeRowButton,0,3);
    searchLayout->addWidget(fulltableButton,1,3);
    searchLayout->setColumnStretch(0,3);
    searchLayout->setColumnStretch(1,1);
    searchLayout->setColumnStretch(2,1);
    searchLayout->setColumnStretch(3,1);
    searchLayout->setSpacing(10);

    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->addWidget(view,0,0);
    mainLayout->addLayout(searchLayout,1,0);
    mainLayout->setRowStretch(0,5);
    mainLayout->setRowStretch(1,1);
    mainLayout->setSpacing(20);

    setLayout(mainLayout);
}

void overtimeRecordTab::searchID()
{
    int id=idSearch->text().toInt();
    model->setFilter(QString("bookId='%1'").arg(id));
    model->select();

    if(model->rowCount()==0)
    {
        qCritical().noquote()<<QString("Fail to find ID: '%1'").arg(id);
        QMessageBox::warning(this,QString("找不到图书ID: '%1'").arg(id),tr("请重新输入图书ID!"));
    }

    idSearch->clear();
    return;
}

void overtimeRecordTab::searchName()
{
    QString name=nameSearch->text();
    model->setFilter(QString("bookName='%1'").arg(name));
    model->select();

    if(model->rowCount()==0)
    {
        qCritical().noquote()<<QString("Fail to find name: '%1'").arg(name);
        QMessageBox::warning(this,QString("找不到书名: '%1'").arg(name),tr("请重新输入书名!"));
    }

    nameSearch->clear();
    return;
}

void overtimeRecordTab::fulltable()
{
    model->setFilter("");
    model->select();
}

void overtimeRecordTab::removeRow()
{
    int row=view->currentIndex().row();
    if(row==-1)
    {
        QMessageBox::warning(this,tr("没有选中的行!"),tr("请重新选择要删除的行!"));
        return;
    }
    model->removeRow(row);
    int choice=QMessageBox::warning(this,tr("删除当前行!"),tr("确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
    if(choice==QMessageBox::Yes)
    {
        model->submitAll();
    }
    else model->revertAll();
}
