#include "bookadminwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>

BookAdminWidget::BookAdminWidget(QSharedPointer<BookTableModel> bookTable,
                                 QSharedPointer<RecordTableModel> recordTable, QWidget *parent)
    : QWidget(parent), bookTableModel(bookTable), recordTableModel(recordTable)
{
    main = new QStackedWidget;
    tabIconList = new QListWidget;

    scaleBox = new QComboBox;
    timeScales << "三天内" << "一周内" << "一月内" << "三月内" << "一年内" << "所有";
    scaleBox->addItems(timeScales);
    /*图书管理主页布局start*/
    homePage = new QWidget;
    recordPage = new QWidget;
    bookFilterDlg = new FilterDialog((QSharedPointer<QSqlRelationalTableModel>)(dynamic_cast<QSqlRelationalTableModel *>(&*bookTableModel)), this);
    addNewDlg = new AddNewDialog(this);
    recordFilterDlg = new FilterDialog(QSharedPointer<QSqlRelationalTableModel>(dynamic_cast<QSqlRelationalTableModel *>(&*recordTableModel)), this);

    QTableView *bookPanel = new QTableView;

    bookPanel->setModel(dynamic_cast<QSqlRelationalTableModel *>(&*bookTable));
    bookPanel->setMinimumWidth(900);
    bookPanel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookPanel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookPanel->resizeColumnToContents(BookTableModel::fieldName::genre);
    bookPanel->resizeColumnToContents(BookTableModel::fieldName::bookName);


    QPushButton *addNew = new QPushButton;
    addNew->setText("新书入库");
    addNew->setIcon(QIcon(":/image/newBookIcon.png"));
    QPushButton *deleteBook = new QPushButton;
    deleteBook->setText("删除");
    deleteBook->setIcon(QIcon(":/image/deleteIcon.png"));


    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *bookSearchBtn = new QPushButton(QIcon(":/image/searchIcon.png"),"查询");
    btnLayout->addStretch(0);
    btnLayout->addWidget(bookSearchBtn);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(addNew);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(deleteBook);
//    btnLayout->addSpacing(20);

    QVBoxLayout *homePageLayout = new QVBoxLayout;
    homePageLayout->addLayout(btnLayout);
    homePageLayout->addWidget(bookPanel);
    homePage->setLayout(homePageLayout);

    /*图书管理主页布局end*/

    /*管理主页与外借记录页切换列start*/
    tabIconList->setViewMode(QListView::IconMode);
    tabIconList->setMovement(QListView::Static);
    tabIconList->setSpacing(20);

    QListWidgetItem *homeItem = new QListWidgetItem(QIcon(":/image/homeIcon.png"), "管理主页");
    homeItem->setTextAlignment(Qt::AlignCenter);
    homeItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *recordItem = new QListWidgetItem(QIcon(":/image/recordIcon.png"), "外借记录");
    recordItem->setTextAlignment(Qt::AlignCenter);
    recordItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    tabIconList->addItem(homeItem);
    tabIconList->addItem(recordItem);

    QGroupBox *tabIconBox = new QGroupBox;
    tabIconBox->setTitle("页面选择");
    QVBoxLayout *tabIconBoxLayout = new QVBoxLayout;
    tabIconBox->setFixedWidth(140);
    tabIconBox->setFixedHeight(tabIconBox->height());
    tabIconBoxLayout->addWidget(tabIconList);

    tabIconBox->setLayout(tabIconBoxLayout);

    QVBoxLayout *layoutContainBox = new QVBoxLayout;
    layoutContainBox->addWidget(tabIconBox);
    layoutContainBox->addStretch(0);
    /*管理主页与外借记录页切换列end*/


    /*图书管理外借记录页面布局start*/
    QTableView *recordPanel = new QTableView;
    recordPanel->setModel(dynamic_cast<QSqlRelationalTableModel *>(&*recordTableModel));
    recordPanel->setMinimumWidth(900);
//    recordPanel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    recordPanel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recordPanel->resizeColumnToContents(RecordTableModel::fieldName::genre);
    recordPanel->resizeColumnToContents(RecordTableModel::fieldName::bookName);

    QHBoxLayout *recordSearchLayout = new QHBoxLayout;
    QPushButton *recordSearchBtn = new QPushButton(QIcon(":/image/searchIcon.png"),"查询");
    recordSearchLayout->addStretch(0);
    recordSearchLayout->addWidget(recordSearchBtn);
    recordSearchLayout->addWidget(scaleBox);
    recordSearchLayout->addSpacing(20);

    QVBoxLayout *recordPageLayout = new QVBoxLayout;
    recordPageLayout->addLayout(recordSearchLayout);
    recordPageLayout->addWidget(recordPanel);

    recordPage->setLayout(recordPageLayout);
    /*图书管理外借记录页面布局end*/

    /*图书管理总布局start*/
    main->addWidget(homePage);
    main->addWidget(recordPage);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(main);
    mainLayout->addLayout(layoutContainBox);
    setLayout(mainLayout);
    /*图书管理总布局end*/

    /**/
    connect(recordFilterDlg, &FilterDialog::filter, this, [=](const QString filter){
        recordTableModel->updateFilter(filter);
    });
    typedef void (QComboBox::*currentIndexChangedInt)(int);
    connect(scaleBox, static_cast<currentIndexChangedInt>(&QComboBox::currentIndexChanged), this, [=](int scale){
        recordTableModel->updateFilter(scale);
    });
    connect(recordSearchBtn, &QAbstractButton::clicked, recordFilterDlg, &QDialog::show);
    connect(addNew, &QAbstractButton::clicked, addNewDlg, &QDialog::show);
    connect(bookSearchBtn, &QAbstractButton::clicked, bookFilterDlg, &QDialog::show);
    connect(tabIconList, &QListWidget::currentItemChanged, this, &BookAdminWidget::changePage);
    connect(bookFilterDlg, &FilterDialog::filter, &*bookTableModel, &BookTableModel::executeFilter);
    connect(addNewDlg, &AddNewDialog::newBookRtned, &*bookTableModel, &BookTableModel::addEntry);

    /**/
//    connect(recordSearchBtn, &QAbstractButton::clicked, &*recordTableModel, [=](){
//        recordTableModel->addEntry(0,0);
//    });
    /**/
}

void BookAdminWidget::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(!current){
        current = previous;
    }
    main->setCurrentIndex(tabIconList->row(current));
}

void BookAdminWidget::slotExecuteBorrowRequest(int bookId, int userId)
{
    if(recordTableModel->addEntry(bookId, userId) == RecordTableModel::promptBorrowMessage::success){
        QMessageBox::information(this, "消息提示", "借书成功!");
        return;
    }
    if(recordTableModel->addEntry(bookId, userId) == RecordTableModel::promptBorrowMessage::borrow_bookNotExist){
        QMessageBox::warning(this, "异常操作警告", "该书不存在，请管理员核查。");
        return;
    }
    if(recordTableModel->addEntry(bookId, userId) == RecordTableModel::promptBorrowMessage::bookNotAvailable){
        QMessageBox::warning(this, "异常操作警告", "该书已外借或丢失，无法再次借出，请管理员核查。");
        return;
    }
    if(recordTableModel->addEntry(bookId, userId) == RecordTableModel::promptBorrowMessage::borrow_readerNotExist){
        QMessageBox::warning(this, "异常操作警告", "不存在该读者，请管理员核查。");
        return;
    }
    if(recordTableModel->addEntry(bookId, userId) == RecordTableModel::promptBorrowMessage::insertFail){
        QMessageBox::warning(this, "错误", "数据库出错，无法添加该记录。");
        return;
    }
}

void BookAdminWidget::slotExecuteReturnRequest(int bookId, int userId)
{
    if(recordTableModel->changeReturnState(bookId, userId) == RecordTableModel::promptReturnMessage::normalReturn){
        QMessageBox::information(this, "消息提示", "还书成功!");
        return;
    }
    if(recordTableModel->changeReturnState(bookId, userId) == RecordTableModel::promptReturnMessage::chargeOvertimeFine){
        QMessageBox::information(this, "消息提示", "借书超时，请管理员收取滞纳金。");
        return;
    }
    if(recordTableModel->changeReturnState(bookId, userId) == RecordTableModel::promptReturnMessage::recordNotExist){
        QMessageBox::information(this, "异常操作警告", "该书未借出，请管理员核查。");
        return;
    }
    if(recordTableModel->changeReturnState(bookId, userId) == RecordTableModel::promptReturnMessage::databaseError){
        QMessageBox::warning(this, "错误", "数据库出错。");
        return;
    }
    if(recordTableModel->changeReturnState(bookId, userId) == RecordTableModel::promptReturnMessage::return_readerNotExist){
        QMessageBox::information(this, "异常操作警告", "不存在该读者，请管理员核查。");
        return;
    }
    if(recordTableModel->changeReturnState(bookId, userId) == RecordTableModel::promptReturnMessage::return_bookNotExist){
        QMessageBox::information(this, "异常操作警告", "该书不存在，请管理员核查。");
        return;
    }
}
