#include "infoapp.h"


InfoApp::InfoApp(const Person &pers, QWidget *parent): QWidget(parent),
    f_name(new QLabel(QString("<span style='font-weight: bold'>Name: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(pers.first_name == "" ? "NONE" : pers.first_name))),
    s_name(new QLabel(QString("<span style='font-weight: bold'>Second Name: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(pers.second_name == "" ? "NONE" : pers.second_name))),
    age(new QLabel(QString("<span style='font-weight: bold'>Age: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(QString::number(pers.age)))),
    number(new QLabel(QString("<span style='font-weight: bold'>Number: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(pers.number))),
    layout(new QGridLayout),
    deleteUser(new QPushButton),
    closeUser(new QPushButton),
    editUser(new QPushButton),
    data(pers),
    query(nullptr)
{
    layout->addWidget(f_name.get(), 0, 0);
    layout->addWidget(s_name.get(), 0, 1);
    layout->addWidget(age.get(), 1, 0);
    layout->addWidget(number.get(), 1, 1);
    layout->addWidget(deleteUser.get(), 2, 0);
    layout->addWidget(closeUser.get(), 2, 1);
    layout->addWidget(editUser.get(), 2, 2);

    deleteUser->setText("Delete");
    deleteUser->setFont(QFont("Lifehack Medium"));
    deleteUser->setStyleSheet("font-size: 14px;");
    deleteUser->setFixedHeight(30);
    QObject::connect(deleteUser.get(), &QPushButton::clicked, this, &InfoApp::DeletePressed);

    closeUser->setText("Close");
    closeUser->setFont(QFont("Lifehack Medium"));
    closeUser->setStyleSheet("font-size: 14 px;");
    closeUser->setFixedHeight(30);
    QObject::connect(closeUser.get(), &QPushButton::clicked, this, &InfoApp::CloseApp);

    editUser->setText("Edit");
    editUser->setFont(QFont("Lifehack Medium"));
    editUser->setStyleSheet("font-size: 14px;");
    editUser->setFixedHeight(30);
    QObject::connect(editUser.get(), &QPushButton::clicked, this, &InfoApp::Edit);

    this->setLayout(layout.get());
    this->resize(QSize(200, 200));

}

void InfoApp::DeletePressed()
{
    this->hide();
    emit Delete();
}

void InfoApp::CloseApp()
{
    this->close();
    emit Close();
}

void InfoApp::Edit()
{
    if(!query.get())
        query = std::make_unique<QueryWindow>();
    query->setData(this->data);
    QObject::connect(query.get(), &QueryWindow::Submitted, this, &InfoApp::SentEdit);
    query->show();
    this->hide();
}

void InfoApp::SentEdit(const Person &to)
{
    emit ToEdit(this->data, to);
    this->f_name->setText(QString("<span style='font-weight: bold'>Name: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(to.first_name == "" ? "NONE" : to.first_name));
    this->s_name->setText(QString("<span style='font-weight: bold'>Name: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(to.second_name == "" ? "NONE" : to.second_name));
    this->age->setText(QString("<span style='font-weight: bold'>Name: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(QString::number(to.age) == "" ? "NONE" : QString::number(to.age)));
    this->number->setText(QString("<span style='font-weight: bold'>Name: </span><span style='font-family: Lifehack Medium; font-size: 12pt'>%1</span>").arg(to.number));
    this->show();
}
