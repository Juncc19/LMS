/** This dialog is used to provide feature of advanced research.
 *  To use it, just connect filter signal to your slot receiving a QString,
 *  and call setFilter function in your model.
 *
 *  EXTRA: Print the filter and lastError in model is convenient for debug.
 * */

#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include "unitfilter.h"

#include <QtWidgets>

class FilterDialog : public QDialog
{
    Q_OBJECT
public:
    FilterDialog(QSharedPointer<QSqlTableModel> model, QWidget *parent = nullptr);
    FilterDialog(QSharedPointer<QSqlRelationalTableModel> model, QWidget *parent = nullptr);
    void showEvent(QShowEvent *) override;

signals:
    void filter(const QString filter);

private slots:
    void addFilter();
    void removeFilter();
    void clearFilter();
    void confirmSelect();
    void updateButtons(); // set delete and clear button enabled or disabled

private:
    QLinkedList<UnitFilter *> filters;
    QPushButton *deleteButton;
    QPushButton *clearButton;
    //QLabel *blankPage;
    QVBoxLayout *filterLayout;
    QTimer *timer;

    UnitFilter *nullFilter; // reserved for duplicate
    QSharedPointer<QSqlTableModel> model;

    FilterDialog(QWidget *parent = nullptr);
    //void filterCleared();
};

#endif // FILTERDIALOG_H
