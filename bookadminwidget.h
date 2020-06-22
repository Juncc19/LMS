#ifndef BOOKADMINWIDGET_H
#define BOOKADMINWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QGroupBox>
#include "booktablemodel.h"
#include "filterdialog.h"
#include "addnewdialog.h"
#include "recordtablemodel.h"

class BookAdminWidget : public QWidget
{
    Q_OBJECT
public:
    enum stack{
        bookSearchPanel, recordPanel
    };
    explicit BookAdminWidget(QSharedPointer<BookTableModel> bookTableModel,
                             QSharedPointer<RecordTableModel> recordTableModel, QWidget *parent = nullptr);
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void slotExecuteBorrowRequest(int bookId, int userId);
    void slotExecuteReturnRequest(int bookId, int userId);

//    void slotExcecuteFilter(const QString)

private:
    QStackedWidget *main;

    QSharedPointer<BookTableModel> bookTableModel;
    QSharedPointer<RecordTableModel> recordTableModel;
    QWidget *homePage;
    QWidget *recordPage;
    QListWidget *tabIconList;

    /**/
    FilterDialog *bookFilterDlg;
    AddNewDialog *addNewDlg;
    FilterDialog *recordFilterDlg;
    /**/
    QComboBox *scaleBox;
    QStringList timeScales;
signals:
//    void filterCombinedWithScale(const QString& filter, int scale);

};

#endif // BOOKADMINWIDGET_H
