#ifndef INFOAPP_H
#define INFOAPP_H
#include <QtWidgets>
#include <memory>
#include "database.h"
#include "querywindow.h"

class InfoApp : public QWidget
{
    Q_OBJECT
public:
    InfoApp(const Person& pers,QWidget* parent = 0);

public  slots:
    void DeletePressed();
    void CloseApp();
    void Edit();
    void SentEdit(const Person& to);

signals:
    void Delete();
    void Close();
    void ToEdit(const Person& from, const Person& To);
private:
    std::unique_ptr<QLabel> f_name;
    std::unique_ptr<QLabel> s_name;
    std::unique_ptr<QLabel> age;
    std::unique_ptr<QLabel> number;
    std::unique_ptr<QGridLayout> layout;
    std::unique_ptr<QPushButton> deleteUser;
    std::unique_ptr<QPushButton> closeUser;
    std::unique_ptr<QPushButton> editUser;

    std::unique_ptr<QueryWindow> query;
    Person data;
};

#endif // INFOAPP_H
