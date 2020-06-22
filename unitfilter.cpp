#include "unitfilter.h"

UnitFilter::UnitFilter(QSharedPointer<QSqlTableModel> model, QWidget *parent)
    :QWidget(parent), model(model)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    checkBox = new QCheckBox(this);
    mainLayout->addWidget(checkBox);

    const QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    const QRegExpValidator *numberValidator = new QRegExpValidator(QRegExp("[0-9]*"), this);
    // set parent to this to prevent from deletion
    const QSqlRecord &record = model->record(); // used to judge column type and name

    for (int index = 0; index < model->columnCount(); ++index) {
        const QString text = model->headerData(index, Qt::Horizontal, Qt::DisplayRole).toString();

        QLabel *label = new QLabel(text + ":", this);

        editors.push_back(new QLineEdit(this));
        QLineEdit* lineEdit = dynamic_cast<QLineEdit *>(editors.back());
        lineEdit->setSizePolicy(sizePolicy);
        if (record.field(index).type() == QVariant::Int)
            lineEdit->setValidator(numberValidator); // only number allowed to input

        mainLayout->addWidget(label);
        mainLayout->addWidget(editors.back());
    }

    setLayout(mainLayout);
}

bool UnitFilter::isChecked() { return checkBox->isChecked(); }

QString UnitFilter::getFilter()
{
    QString filter;
    bool firstFlag = true; // judge whether to add "and"
    const QSqlRecord &record = model->record();

    for (int index = 0; index < editors.size(); ++index) {
        if (strcmp(editors[index]->metaObject()->className(), "QLineEdit") == 0) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editors[index]);

            if (!lineEdit->text().isEmpty()) {
                QVariant::Type dataType = record.field(index).type(); // get type of specific index

                if (dataType == QVariant::Int) {
                    filter += (firstFlag ? firstFlag = false, "" : " and ") + record.field(index).name()
                                + "=" + lineEdit->text();
                } else if (dataType == QVariant::String) {
                    filter += (firstFlag ? firstFlag = false, "" : " and ") + record.field(index).name()
                                + " like \'"  + processFilterWord(lineEdit->text()) + "\'";
                } else {
                    qWarning() << "In table" << QString(model->tableName()) << ":" << "Unknown type"
                               << QString(dataType);
                }
            }
        } else {
            qWarning() << "Illegal pointer in unit filter";
        }
    }

    return firstFlag ? "" : "(" + filter + ")";
}

UnitFilter* UnitFilter::generateNullCopy(QWidget *parent)
{
    return new UnitFilter(model, parent);
}

QString UnitFilter::processFilterWord(QString &&word)
{
    QString ret = "%";
    for (auto &ch : word) {
        ret.push_back(ch);
        ret.push_back("%");
    }
    return ret;
}

UnitFilterForRelation::UnitFilterForRelation(QSharedPointer<QSqlRelationalTableModel> model, QWidget *parent)
        :UnitFilter(parent), model(model)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    checkBox = new QCheckBox(this);
    mainLayout->addWidget(checkBox);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QRegExpValidator *numberValidator = new QRegExpValidator(QRegExp("[0-9]*"), this);
    // set parent to this to prevent from deletion
    const QSqlRecord &record = model->record();

    for (int index = 0; index < model->columnCount(); ++index) {
        const QString text = model->headerData(index, Qt::Horizontal, Qt::DisplayRole).toString();
        QLabel *label = new QLabel(text + ":");

        if (model->relation(index).isValid()) { // if is foreign key
            QSqlTableModel* const &relationModel = model->relationModel(index);

            if (!relationModel) {
                qWarning() << "Column" << index << "in" << model->tableName() <<
                              "is not a foreign key.";
                continue;
            }

            QStringList itemList;
            for (int id = 0; id < relationModel->rowCount(); ++id)
                itemList << relationModel->data(relationModel->index(id, 1)).toString();

            editors.push_back(new QComboBox(this));
            QComboBox* comboBox = dynamic_cast<QComboBox *>(editors.back());
            comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
            comboBox->addItems(itemList);
            comboBox->setCurrentIndex(-1); //reserve negative for select all
            comboBox->setSizePolicy(sizePolicy);

            mainLayout->addWidget(label);
            mainLayout->addWidget(comboBox);
        } else { // if not
            editors.push_back(new QLineEdit(this));
            QLineEdit* lineEdit = dynamic_cast<QLineEdit *>(editors.back());
            lineEdit->setSizePolicy(sizePolicy);
            if (record.field(index).type() == QVariant::Int)
                lineEdit->setValidator(numberValidator); // only number allowed to input

            mainLayout->addWidget(label);
            mainLayout->addWidget(editors.back());
        }
    }

    setLayout(mainLayout);
}

QString UnitFilterForRelation::getFilter()
{
    QString filter;
    bool firstFlag = true; // judge whether to add "and"
    const QSqlRecord &record = model->record();

    for (int index = 0; index < editors.size(); ++index) {
        if (strcmp(editors[index]->metaObject()->className(), "QLineEdit") == 0) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(editors[index]);

            if (!lineEdit->text().isEmpty()) {
                QVariant::Type dataType = record.field(index).type(); // get type of specific index

                if (dataType == QVariant::Int) {
                    filter += (firstFlag ? firstFlag = false, "" : " and ") + record.field(index).name()
                                + "=" + lineEdit->text();
                } else if (dataType == QVariant::String) {
                    filter += (firstFlag ? firstFlag = false, "" : " and ") + record.field(index).name()
                                + " like \'"  + processFilterWord(lineEdit->text()) + "\'";
                } else {
                    qWarning() << "In table" << QString(model->tableName()) << ":" << "Unknown type"
                               << QString(dataType);
                }
            }
        } else if (strcmp(editors[index]->metaObject()->className(), "QComboBox") == 0) {
            QComboBox *comboBox = dynamic_cast<QComboBox *>(editors[index]);
            QSqlRelation relation = model->relation(index);

            if (comboBox->currentIndex() >= 0) { // use alias of relation table
                filter += (firstFlag ? firstFlag = false, "relTblAl_" : " and relTblAl_") + QVariant(index).toString() + "."
                            + relation.indexColumn() + "=" + QVariant(comboBox->currentIndex()).toString();
            }
        } else {
            qWarning() << "Illegal pointer in unit filter";
        }
    }

    return firstFlag ? "" : "(" + filter + ")";
}

UnitFilter* UnitFilterForRelation::generateNullCopy(QWidget *parent)
{
    return new UnitFilterForRelation(model, parent);
}
