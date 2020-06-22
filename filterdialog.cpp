#include "filterdialog.h"
#include "constants.h"

FilterDialog::FilterDialog(QWidget *parent) :QDialog(parent)
{
    QPushButton *addButton = new QPushButton(tr("添加"), this);
    deleteButton = new QPushButton(tr("删除"), this);
    clearButton = new QPushButton(tr("清空"), this);
    QVBoxLayout *rightButtonLayout = new QVBoxLayout;
    rightButtonLayout->addWidget(addButton, 0, Qt::AlignTop);
    rightButtonLayout->addWidget(deleteButton);
    rightButtonLayout->addWidget(clearButton);
    rightButtonLayout->addStretch();

    QPushButton *confirmButton = new QPushButton(tr("确认"), this);
    confirmButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *filterWidget = new QWidget(scrollArea);
    filterLayout = new QVBoxLayout;
    filterLayout->addStretch();
    filterWidget->setLayout(filterLayout);
    scrollArea->setWidget(filterWidget);
    scrollArea->setWidgetResizable(true);

    QHBoxLayout *upLayout = new QHBoxLayout;
    upLayout->addWidget(scrollArea);
    upLayout->addLayout(rightButtonLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(upLayout);
    mainLayout->addWidget(confirmButton, 0, Qt::AlignHCenter);
    setLayout(mainLayout);

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &FilterDialog::updateButtons);

    connect(addButton, &QPushButton::clicked, this, &FilterDialog::addFilter);
    connect(deleteButton, &QPushButton::clicked, this, &FilterDialog::removeFilter);
    connect(clearButton, &QPushButton::clicked, this, &FilterDialog::clearFilter);
    connect(confirmButton, &QPushButton::clicked, this, &FilterDialog::confirmSelect);
}

FilterDialog::FilterDialog(QSharedPointer<QSqlTableModel> model, QWidget *parent)
    :FilterDialog(parent)
{
    this->model = model;
    nullFilter = new UnitFilter(model);
    this->resize(std::min(nullFilter->sizeHint().width() + deleteButton->sizeHint().width(), MAX_FILTER_DIALOG_WIDTH),
                 std::min(nullFilter->sizeHint().height() * 6 + deleteButton->sizeHint().height(), MAX_FILTER_DIALOG_HEIGHT));
}

FilterDialog::FilterDialog(QSharedPointer<QSqlRelationalTableModel> model, QWidget *parent)
    :FilterDialog(parent)
{
    this->model = model;
    nullFilter = new UnitFilterForRelation(model);
    this->resize(std::min(nullFilter->sizeHint().width() + deleteButton->sizeHint().width(), MAX_FILTER_DIALOG_WIDTH),
                 std::min(nullFilter->sizeHint().height() * 6 + deleteButton->sizeHint().height(), MAX_FILTER_DIALOG_HEIGHT));
}

void FilterDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    timer->start();
}

void FilterDialog::addFilter()
{
    filters.push_back(nullFilter->generateNullCopy());
    filterLayout->insertWidget(filterLayout->count() - 1, filters.back(), 0, Qt::AlignTop); // update layout
}

void FilterDialog::removeFilter()
{
    auto filterIterator = filters.begin();
    while (filterIterator != filters.end()) {
        if ((*filterIterator)->isChecked()) { // delete checked filter
            filterLayout->removeWidget(*filterIterator);
            delete *filterIterator;
            filterIterator = filters.erase(filterIterator);
        } else {
            filterIterator++;
        }
    }
}

void FilterDialog::clearFilter()
{
    auto filterIterator = filters.begin();
    while (filterIterator != filters.end()) {
        filterLayout->removeWidget(*filterIterator);
        delete *filterIterator;
        filterIterator++;
    }
    filters.clear();
}

void FilterDialog::updateButtons()
{
    if (deleteButton->isEnabled()) {
        for (auto &filter : filters) {
            if (filter->isChecked())
                return; // if some is checked, not to change the state
        }
        deleteButton->setEnabled(false);
    } else {
        for (auto &filter : filters) {
            if (filter->isChecked()) {
                deleteButton->setEnabled(true);
                return; // if some is checked, set button enabled
            }
        }
    }
}

void FilterDialog::confirmSelect()
{
    QString filterText;

    bool firstFlag = true;
    for (auto &filter : filters) {
        const QString subFilter = filter->getFilter();

        if (!subFilter.isEmpty()) {
            if (firstFlag) {
                filterText += subFilter;
                firstFlag = false;
            } else {
                filterText += " or " + subFilter;
            }
        }
    }

    emit filter(filterText);
    timer->stop();
    this->close();
}
