#ifndef UNITFILTER_H
#define UNITFILTER_H

#include <QtWidgets>
#include <QtSql>

class UnitFilter : public QWidget
{
    Q_OBJECT
public:
    UnitFilter(QSharedPointer<QSqlTableModel> model, QWidget *parent = nullptr);
    using QWidget::QWidget; // using parent's constructor for subclass

    virtual QString getFilter();
    virtual UnitFilter* generateNullCopy(QWidget *parent = nullptr); // for use of duplication in FilterDialog
    bool isChecked();

protected:
    QList<QWidget *> editors;
    QCheckBox *checkBox;

    QString processFilterWord(QString &&word); // add wildcard

private:
    QSharedPointer<QSqlTableModel> model;
};

class UnitFilterForRelation : public UnitFilter
{
    Q_OBJECT
public:
    UnitFilterForRelation(QSharedPointer<QSqlRelationalTableModel> model,  QWidget *parent = nullptr);
    virtual QString getFilter() override;
    virtual UnitFilter* generateNullCopy(QWidget *parent = nullptr) override;

private:
    QSharedPointer<QSqlRelationalTableModel> model;
};

#endif // UNITFILTER_H
