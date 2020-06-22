#include "loginwidget.h"
#include "constants.h"

LoginWidget::LoginWidget(QSharedPointer<AccountTableModel> readerTable,
                         QSharedPointer<AccountTableModel> adminTable,
                         QWidget *parent)
    : QDialog(parent), readerTable(readerTable), adminTable(adminTable)
{
    QLabel *welcomeLabel = new QLabel(tr("欢迎"), this);
    QLabel *idLabel = new QLabel(tr("账号ID"), this);
    QLabel *passwordLabel = new QLabel(tr("密码"), this);

    QSizePolicy buttonPolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    typeSelected = new QButtonGroup(this);
    QRadioButton *readerButton = new QRadioButton(tr("读者登录"), this);
    QRadioButton *adminButton = new QRadioButton(tr("管理员登录"), this);
    readerButton->setSizePolicy(buttonPolicy);
    adminButton->setSizePolicy(buttonPolicy);
    typeSelected->addButton(readerButton);
    typeSelected->addButton(adminButton);
    typeSelected->setId(readerButton, 0);
    typeSelected->setId(adminButton, 1);
    readerButton->setChecked(true); // select reader by default 默认选择读者登录

    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText(tr("账号ID"));
    idEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    //force to input numbers only 强制输入数字
    idEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), idEdit));
    idEdit->setMaxLength(MAX_ID_LENGTH);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText(tr("密码"));
    passwordEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    passwordEdit->setEchoMode(QLineEdit::Password);
    //force not to input space 禁止输入空格
    passwordEdit->setValidator(new QRegExpValidator(QRegExp("[\\S]*"), passwordEdit));
    passwordEdit->setMaxLength(MAX_PASSWORD_LENGTH);

    QPushButton *loginButton = new QPushButton(tr("登录"), this);
    QPushButton *registerButton = new QPushButton(tr("注册"), this);
    registerButton->setSizePolicy(buttonPolicy);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(welcomeLabel, 0, Qt::AlignLeft);
    headerLayout->addWidget(registerButton, 0, Qt::AlignRight);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(readerButton, 0, Qt::AlignRight);
    buttonLayout->addWidget(adminButton, 0, Qt::AlignLeft);

    QGridLayout *editLayout = new QGridLayout;
    editLayout->addWidget(idLabel, 0, 0);
    editLayout->addWidget(idEdit, 0, 1);
    editLayout->addWidget(passwordLabel, 1, 0);
    editLayout->addWidget(passwordEdit, 1, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(editLayout);
    mainLayout->addWidget(loginButton, 0, Qt::AlignHCenter | Qt::AlignTop);
    // mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    // 使对话框不可放缩

    setLayout(mainLayout);
    setWindowTitle(tr("登录"));
    resize(INITIAL_DIALOG_SIZE);

    registerWidget = new RegisterWidget(readerTable, this);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::judgeLogin);
    connect(registerButton, &QPushButton::clicked, registerWidget, &RegisterWidget::show);
    connect(registerButton, &QPushButton::clicked, this, &LoginWidget::hide);
    connect(readerButton, &QRadioButton::clicked, registerButton, &QPushButton::setEnabled);
    connect(adminButton, &QRadioButton::clicked, registerButton, &QPushButton::setDisabled);
}

void LoginWidget::judgeLogin()
{
    if (idEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("未输入ID"), tr("请输入ID"));
        return;
    }
    if (passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("未输入密码"), tr("请输入密码"));
        return;
    }

    QSharedPointer<AccountTableModel> &tablePointer = (typeSelected->checkedId() ==
                                                       AccountTableModel::reader? readerTable : adminTable);
    int id = QVariant(idEdit->text()).toInt();

    const QString password = passwordEdit->text();
    tablePointer->setFilter(QString("id = %1").arg(id));
    tablePointer->select();
    if  (tablePointer->rowCount() == 0) {
        QMessageBox::warning(this, tr("ID不存在"), tr("请重新输入ID"));
        return;
    } else if (tablePointer->rowCount() != 1) {
        qCritical().noquote() << QString("Fail to judge login: Id %1 is duplicate in "
                                         "%2.").arg(id).arg(tablePointer->tableName());
        QMessageBox::critical(this, tr("程序出错"), tr("检测到数据库中存在重复id，暂时无法登录。"
                                                   "请联系管理员或开发者获取支持。"));
        return;
    }
    const QString correctPassword = tablePointer->data(tablePointer->index(0, 1)).toString();

    tablePointer->setFilter("");
    tablePointer->select();
    // restore

    if (correctPassword != password) {
        QMessageBox::warning(this, tr("密码错误"), tr("请重新输入密码"));
        return;
    } else {
        this->hide();
        idEdit->clear();
        passwordEdit->clear();
        emit loginSuccessfully(id, typeSelected->checkedId());
        //... 打开主界面 通过typeSelected判断类型
    }
}

void LoginWidget::autoFill(int id, const QString &password)
{
    idEdit->setText(QVariant(id).toString());
    passwordEdit->setText(password);
}













