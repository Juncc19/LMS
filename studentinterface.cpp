#include "studentinterface.h"
#include "ui_studentinterface.h"
#include <QMessageBox>
#include <QTableView>
studentinterface::studentinterface(int id,QString name,QSharedPointer<AccountTableModel> readerTable,
                                   QSharedPointer<BookTableModel> bookTable,
                                   QSharedPointer<RecordTableModel> recordTable,QWidget *parent)
    : UserInterface(id,name,parent), ui(new Ui::studentinterface)
{   this->parent=parent;
    this->id=id;this->name=name;
    this->readermodel=readerTable;
    this->bookmodel= bookTable;
    this->recordmodel=recordTable;
    ui->setupUi(this);
    this->ui->listWidget_2->addItem(QString("ID：%1").arg(id));
    this->ui->listWidget_2->addItem(QString("姓名：%1").arg(name));
    this->pdialog=new PasswordDialog(id,readerTable,this);
    this->infodia=new UserInfoDialog(id,name,this);
    //tableview设置;
    lay=new QVBoxLayout();
    view=new QTableView();
        lay->addWidget(view);
        this->ui->widget->setLayout(lay);
        view->hide();
    lay2=new QVBoxLayout();
    view2=new QTableView();
        lay2->addWidget(view2);
        this->ui->widget_2->setLayout(lay2);
        view2->hide();


    connect(this->infodia, &UserInfoDialog::infoRtned, &*readerTable, &AccountTableModel::changeName);
    connect(infodia, &UserInfoDialog::infoRtned, this, &studentinterface::updatename);

}

studentinterface::~studentinterface()
{
    delete ui;
}

void studentinterface::updatename(int id, QString name)
{

    this->ui->listWidget_2->setCurrentRow(1);
    this->ui->listWidget_2->currentItem()->setText(QString("姓名：%1").arg(name));
}

void studentinterface::on_pushButton_2_clicked()
{

    this->pdialog->show();

}

void studentinterface::on_pushButton_4_clicked()
{
//注销qaq
    if(QMessageBox::Yes==QMessageBox::question(this,"question",tr("确定删除此账户所有信息？"))){
            //qDebug()<<"Yes";
            this->readermodel->deleteEntry(id);
        this->close();
        this->parent->show();

    }
    else{
        //qDebug()<<"no";
    }
}

void studentinterface::on_pushButton_clicked()
{
this->infodia->show();
}

void studentinterface::on_pushButton_3_clicked()
{
//查询书籍

        if(this->ui->radioButton->isChecked()){
        this->bookmodel->setFilter(QString("bookName = '%1'").arg(this->ui->lineEdit->text()));
         bookmodel->select();
         view->setModel(&*bookmodel);
         view->show();

        }else if(this->ui->radioButton_2->isChecked()){
            this->bookmodel->setFilter(QString("author = '%1'").arg(this->ui->lineEdit->text()));
             bookmodel->select();
             //QTableView *view=new QTableView(this->ui->tableWidget);
             view->setModel(&*bookmodel);
             view->show();

        }else if(this->ui->radioButton_3->isChecked()){
            this->bookmodel->setFilter(QString("bookId = '%1'").arg(this->ui->lineEdit->text()));
             bookmodel->select();
              //QTableView *view=new QTableView(this->ui->tableWidget);
             view->setModel(&*bookmodel);
             view->show();
        }
        else{
            QMessageBox::warning(this,"wrong choice","please choose one type above");
        }
    \

}
//清空状态
void studentinterface::on_tabWidget_currentChanged(int index)
{
    if(index==1){
    view->hide();
    this->ui->radioButton->setChecked(true);
    this->ui->lineEdit->clear();
   }else{
    view2->hide();
    }
}

void studentinterface::on_pushButton_5_clicked()
{
//查询记录qaq
    this->recordmodel->setFilter(QString("userId=%1").arg(id));
    recordmodel->select();
    if(recordmodel->rowCount()==0){
        QMessageBox::information(this,"information","your account have not borrowed books yet, just try to borrow!");
    }
    else{this->view2->setModel(&*recordmodel);
    view2->show();}
}
