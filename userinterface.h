#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>
#include <QGroupBox>
#include <QListWidget>
#include <QDialog>

class UserInterface : public QMainWindow
{
    Q_OBJECT
public:
    enum headerData{
        id, password, Name
    };
    explicit UserInterface(int id, const QString &name, QWidget *parent = nullptr);
protected:
    int ID;
    QString name;

signals:

};

class UserInformation: public QGroupBox{
    Q_OBJECT
public:
    UserInformation(int id, const QString& name, QWidget *parent = 0);
    void addItem(QListWidgetItem *item);
    void changeName(const QString& name);
private:
    QListWidget *info;
    QListWidgetItem *nameItem;
};

class UserInfoDialog : public QDialog{
    Q_OBJECT
public:
    explicit UserInfoDialog(int id, const QString& name,QWidget *parent = 0);
    void judge();
    void closeEvent(QCloseEvent *) override;
private:
    int id;
    QString name;
    QLineEdit *nameLine;
    QPushButton *saveBtn;
    QPushButton *cancelBtn;
signals:
    void infoRtned(int id, QString name);
};

#endif // USERINTERFACE_H
