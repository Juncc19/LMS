#ifndef STUDENTINTERFACE_H
#define STUDENTINTERFACE_H

#include "passworddialog.h"
#include "accounttablemodel.h"
#include "booktablemodel.h"

#include "admininterface.h"
#include "recordtablemodel.h"
#include "userinterface.h"
namespace Ui {
class studentinterface;
}

class studentinterface : public UserInterface
{
    Q_OBJECT

public:
    explicit studentinterface(int id,QString name,QSharedPointer<AccountTableModel> adminTable,
                              QSharedPointer<BookTableModel> bookTableModel,
                              QSharedPointer<RecordTableModel> recordTable,QWidget *parent = nullptr);
    ~studentinterface();
public slots:
    void updatename(int id, QString name);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_5_clicked();

private:
    QVBoxLayout *lay;
    QTableView *view;
    QVBoxLayout *lay2;
    QTableView *view2;
    QWidget *parent;
    int id;
    QString name;
    PasswordDialog *pdialog;
    UserInfoDialog *infodia;
    QSharedPointer<BookTableModel> bookmodel;
    QSharedPointer<AccountTableModel> readermodel;
    QSharedPointer<RecordTableModel> recordmodel;
    Ui::studentinterface *ui;
};

#endif // STUDENTINTERFACE_H
