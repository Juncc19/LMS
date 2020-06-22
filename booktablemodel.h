#ifndef BOOKTABLEMODEL_H
#define BOOKTABLEMODEL_H
#include <QtSql>

class BookTableModel: public QSqlRelationalTableModel
{
public:
    enum bookStates {
        Stored, Borrowed, Lost
    };
    enum fieldName{
        bookId, bookName, author, genre, state
    };

    explicit BookTableModel(QObject* parent = 0);
    int addEntry(const QString& name, const QString& author, int genre);
    int deleteEntry(int bookId);
    const QSqlRecord searchId(int id);
    QList<QSqlRecord> searchName(const QString& name);
    QList<QSqlRecord> searchAuthor(const QString& authorName);
    QList<QSqlRecord> searchGenre(int genre);
    bool changeName(int id, const QString& name);
    bool changeAuthor(int id, const QString& authorName);
    bool changeGenre(int id, int genre);
    bool changeState(int id, int state);

    int getState(int bookId);
    int getGenre(int bookId);
    QString getName(int bookId);
    bool isExist(int bookId);

    void executeFilter(const QString& filter);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
private:
    QSqlTableModel * genres;
    QSqlTableModel * states;
    //QStringList fieldName;
};

#endif // BOOKTABLEMODEL_H
