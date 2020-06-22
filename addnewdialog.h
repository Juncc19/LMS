#ifndef ADDNEWDIALOG_H
#define ADDNEWDIALOG_H

#include <QDialog>
#include "booktablemodel.h"
#include <QtSql>

class AddNewDialog : public QDialog
{
    Q_OBJECT
public:

    explicit AddNewDialog(QWidget *parent = nullptr);

public slots:
    void closeEvent(QCloseEvent *) override;
    void judge();

signals:
    void newBookRtned(const QString& bookName, const QString& author,
                      int genre);

private:
    QLineEdit *nameLine;
    QLineEdit *authorLine;
    QComboBox *genreBox;
    QSqlTableModel *genres;

};

#endif // ADDNEWDIALOG_H
