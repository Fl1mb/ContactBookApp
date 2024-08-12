#ifndef DATABASE_H
#define DATABASE_H
#include <QWidget>
#include <QtSql>
#include <QTableView>
#include <QVBoxLayout>
#include <memory>

struct Person{
    QString first_name;
    QString second_name;
    int age;
    QString number;

    Person(QString first, QString second, int age_, QString number);
    Person() = default;
    bool operator==(const Person& other);
    bool operator!=(const Person& other){return !(*this == other);}
};

class DataBase : public QWidget
{
    Q_OBJECT
public:
    explicit DataBase(QWidget* parent = nullptr);
    ~DataBase();

    void CreateTable();
    bool AddPerson(Person person);
    void ExecQuery(const QString& str);
    void ShowTable();
    void deleteUser(const Person& data);
    void ChangeValues(const Person& from, const Person& to);

    QSqlDatabase getDB();

public slots:
    void MakeQuare(const QString& str);

private:
    QSqlDatabase db;

    std::unique_ptr<QTableView> view;
    std::unique_ptr<QSqlTableModel> model;
    std::unique_ptr<QVBoxLayout> layout;

    QString strF;
    int NumOfUsers{1};
    bool CreateDataBase();
};

#endif // DATABASE_H
