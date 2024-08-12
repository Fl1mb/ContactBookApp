#include "database.h"
#include <QDebug>
#include <QList>
#include <QHeaderView>


DataBase::DataBase(QWidget *parent) : QWidget(parent)
{
    if(!CreateDataBase())
        qDebug() << "Error of creating dataBase";

    this->CreateTable();

    model = std::unique_ptr<QSqlTableModel>(new QSqlTableModel(this, db));
    model->setTable("AddressBook");
    model->setSort(model->fieldIndex("user_id"), Qt::AscendingOrder);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    view = std::unique_ptr<QTableView>(new QTableView(this));
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout);
    layout->addWidget(view.get());

    this->setLayout(layout.get());


}

DataBase::~DataBase()
{
    db.close();
}

void DataBase::CreateTable()
{

    QSqlQuery query;
    QString str = "CREATE TABLE  IF NOT EXISTS AddressBook("
                  "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "first_name VARCHAR(20) ,"
                  "second_name VARCHAR(20) ,"
                  "age INTEGER,"
                  "number VARCHAR(30)"
                  ");";


    if(!query.exec(str))
        qDebug() << "Error of creating table" << query.lastError().text();
}

bool DataBase::AddPerson(Person person)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM AddressBook WHERE first_name = :first_name AND second_name = :second_name");
    query.bindValue(":first_name", person.first_name);
    query.bindValue(":second_name", person.second_name);

    if (query.exec()) {
        if (query.next()) {
            // Person with the same first_name and second_name already exists
            return false;
        }
    } else {
        qDebug() << query.lastError();
        return false;
    }

    query.prepare("INSERT INTO AddressBook(first_name, second_name, age, number) "
                  "VALUES(:first_name, :second_name, :age, :number);");
    query.bindValue(":first_name", person.first_name);
    query.bindValue(":second_name", person.second_name);
    query.bindValue(":age", person.age);
    query.bindValue(":number", person.number);

    if (!query.exec())
        qDebug() << query.lastError();

    model->select(); // Refresh the model's data
    view->reset();

    return true;
}

void DataBase::ExecQuery(const QString &str)
{
    QSqlQuery query;
    if(!query.exec(str))
        qDebug() << query.lastError();
    model->select();
    view->reset();
}


void DataBase::ShowTable()
{
    model->setSort(model->fieldIndex("first_name"), Qt::AscendingOrder);
    view->setModel(model.get());
    model->select(); // Refresh the model
    view->reset(); // Update the view
    this->show();
}

void DataBase::deleteUser(const Person &data)
{
    QSqlQuery query;
    QString strF = "DELETE FROM AddressBook "
                  "WHERE first_name = ? AND second_name = ?";
    query.prepare(strF);
    query.addBindValue(data.first_name);
    query.addBindValue(data.second_name);

    if(!query.exec())
        qDebug() << query.lastError();
}

void DataBase::ChangeValues(const Person &from, const Person &to)
{
    QSqlQuery query;
    QString updateQuery = "UPDATE AddressBook SET "
                           "first_name = :new_first_name, "
                           "second_name = :new_second_name, "
                           "age = :new_age, "
                           "number = :new_number "
                           "WHERE (first_name = :old_first_name AND second_name = :old_second_name)";

    query.prepare(updateQuery);
    query.bindValue(":new_first_name", to.first_name);
    query.bindValue(":new_second_name", to.second_name);
    query.bindValue(":new_age", to.age);
    query.bindValue(":new_number", to.number);
    query.bindValue(":old_first_name", from.first_name);
    query.bindValue(":old_second_name", from.second_name);

    if (!query.exec()) {
        qDebug() << "Error updating person: " << query.lastError().text();
        return;
    }
}


QSqlDatabase DataBase::getDB()
{
    return this->db;
}

void DataBase::MakeQuare(const QString &str)
{
    ExecQuery(str);
}
bool DataBase::CreateDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("addressbook.db");

    if(!db.open()){
        qDebug() << "Error";
        return false;
    }return true;
}

Person::Person(QString first, QString second, int age_, QString num): first_name(first),
    second_name(second),
    age(age_),
    number(num)
{}

bool Person::operator==(const Person &other)
{
    if(this->first_name == other.first_name &&
            this->second_name == other.second_name &&
            this->age == other.age &&
            this->number == other.number)
        return true;
    return false;
}
