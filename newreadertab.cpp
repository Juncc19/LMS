#include "newreadertab.h"

newReaderTab::newReaderTab(QWidget *parent) : QWidget(parent)
{
    model= new ReaderManagementModel("newReader",this);

    view=new QTableView(this);
    view->setModel(model);
    view->setColumnHidden(1,true);
    //view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    idSearch=new QLineEdit(this);
    idSearch->setPlaceholderText("请输入您的ID");
    idSearch->setValidator(new QIntValidator(idSearch));
    idSearch->setAttribute(Qt::WA_InputMethodEnabled,false);

    nameSearch=new QLineEdit(this);
    nameSearch->setPlaceholderText("请输入您的姓名");
    nameSearch->setMaxLength(MAX_NAME_LENGTH);
    nameSearch->setAttribute(Qt::WA_InputMethodEnabled,false);
    nameSearch->setValidator(new QRegExpValidator(QRegExp("[\\S]*"), nameSearch));

    idSearchButton=new QPushButton(tr("ID查询"),this);
    nameSearchButton=new QPushButton(tr("姓名查询"),this);
    fulltableButton=new QPushButton(tr("显示全部"),this);
    emptyButton=new QPushButton(tr("清除记录"),this);
    connect(idSearchButton,&QPushButton::clicked,this,&newReaderTab::searchID);
    connect(nameSearchButton,&QPushButton::clicked,this,&newReaderTab::searchName);
    connect(fulltableButton,&QPushButton::clicked,this,&newReaderTab::fulltable);
    connect(emptyButton,&QPushButton::clicked,this,&newReaderTab::emptytable);


    QGridLayout *searchLayout=new QGridLayout;
    searchLayout->addWidget(idSearch,0,0);
    searchLayout->addWidget(idSearchButton,0,1);
    searchLayout->addWidget(nameSearch,1,0);
    searchLayout->addWidget(nameSearchButton,1,1);
    searchLayout->addWidget(fulltableButton,1,3);
    searchLayout->addWidget(emptyButton,0,3);
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

void newReaderTab::searchID()
{
    model->searchID(idSearch->text().toInt());
    idSearch->clear();
}

void newReaderTab::searchName()
{
    model->searchName(nameSearch->text());
    nameSearch->clear();
}

void newReaderTab::fulltable()
{
    model->fulltable();
    view->setModel(model);
    view->setColumnHidden(1,true);
}

void newReaderTab::emptytable()
{
    model->removeRows(0,model->rowCount());
    model->submitAll();
}
