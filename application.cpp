#include "application.h"
#include <QIcon>

Application::Application(QWidget *parent) : QWidget(parent),
    base(new DataBase),
    button(new QPushButton),
    layout(new QVBoxLayout),
    scrollArea(new QScrollArea),
    BookWidget(new QWidget),
    BookWidgetLayout(new QVBoxLayout),
    win(new QueryWindow),
    trie(new Trie)
{

    BookWidget->setStyleSheet("border-radius: 10px;");
    BookWidget->setLayout(BookWidgetLayout.get()); // Set layout for BookWidget
    BookWidgetLayout->setSpacing(10);

    scrollArea->setWidget(BookWidget.get());
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout->addWidget(scrollArea.get());

    button->resize(QSize(50, 50));
    button->setText("Add");
    button->setFont(QFont("Lifehack Medium"));
    button->setStyleSheet("font-size: 14px;");

    layout->addWidget(button.get(), 0, Qt::AlignRight | Qt::AlignBottom);

    AddHumans();

    this->setLayout(layout.get());
    this->resize(size);
    this->setWindowIcon(QIcon(":/img/ContactBook.png"));
    //this->setStyleSheet("background-color: lightgray;");

    //connections:
    QObject::connect(button.get(), &QPushButton::clicked, this, &Application::ShowWindow);
}
void Application::AddHumans()
{
    QSqlDatabase db = base->getDB();
    if(!db.isOpen()){
        qDebug() << "DB IS NOT OPENED";
        return;
    }

    QSqlQuery query("SELECT * FROM AddressBook", db);
    if(!query.exec()){
        qDebug()<< "Error of exec";
        return;
    }

    while(query.next()){
        QSqlRecord record = query.record();
        QString first_name = record.value("first_name").toString();
        QString second_name  = record.value("second_name").toString();
        int age = record.value("age").toInt();
        QString number = record.value("number").toString();

        list.push_back(new Human(Person(first_name, second_name, age, number)));
        trie->AddWord(first_name.toStdString() + " " + second_name.toStdString());
    }

    BookWidgetLayout->setSpacing(5);
    BookWidgetLayout->setContentsMargins(14,0,3,0);

    DrawHumans(trie->TrieToVector());
    for(auto human : list){
            QObject::connect(human, &Human::DeleteThis, this, &Application::DeleteHuman);
            QObject::connect(human, &Human::EditHuman, this, &Application::Edit);
        }
    BookWidget->update();
}

void Application::AddHuman(const Person &data)
{
    list.push_back(new Human(data));
    trie->AddWord(data.first_name.toStdString() + data.second_name.toStdString());
    DrawHumans(trie->TrieToVector());


    QObject::connect(list.back(), &Human::DeleteThis, this, &Application::DeleteHuman);
    QObject::connect(list.back(), &Human::EditHuman, this, &Application::Edit);
}

void Application::DrawHumans(const std::vector<std::string> &SortedVector)
{
    BookWidgetLayout->deleteLater();
    BookWidgetLayout = std::make_unique<QVBoxLayout>();
    BookWidget->setLayout(BookWidgetLayout.get());

    QChar ch = QString::fromStdString(SortedVector[0])[0];
    labels[ch] = std::make_unique<QLabel>();
    labels[ch]->setText(QString(ch) + "<hr style='border: none; border-bottom: 1px solid white'>");
    labels[ch]->setStyleSheet("font-size: 14px;");
    labels[ch]->setFont(QFont("Lifehack Medium"));

    BookWidgetLayout->addWidget(labels[ch].get());


    for(const auto& str : SortedVector){
        QString fullName = QString::fromStdString(str);
        QString first_name = fullName.section(" ", 0, 0);
        QString second_name = fullName.section(" ", 1, 1);
        if(ch != fullName[0]){
            ch = fullName[0];
            labels[ch] = std::make_unique<QLabel>();
            labels[ch]->setText(QString(ch) + "<hr style='border: none; border-bottom: 1px solid white'>");
            BookWidgetLayout->addWidget(labels[ch].get());
            labels[ch]->setStyleSheet("font-size: 14px;");
            labels[ch]->setFont(QFont("Lifehack Medium"));
        }

        for(Human* hum : list){
            if(hum->getData().first_name == first_name && hum->getData().second_name == second_name){
                hum->resize(200, 75);
                BookWidgetLayout->addWidget(hum);
                BookWidgetLayout->addStretch(2);
                hum->show();
                break;
            }
        }
    }
    BookWidget->update();
}

void Application::ShowWindow()
{
    win->show();
    this->setEnabled(false);

    //connection
    QObject::connect(win.get(), &QueryWindow::Submitted, this, &Application::MakeQuery);
    QObject::connect(win.get(), &QueryWindow::Closed, this, &Application::CloseWindow);
}

void Application::MakeQuery(const Person &data)
{
    this->setEnabled(true);
    if(!this->base->AddPerson(data))
        return;
    AddHuman(data);
}

void Application::DeleteHuman(Human *hum)
{
    QObject::disconnect(hum, &Human::DeleteThis, this, &Application::DeleteHuman);
    QObject::disconnect(hum , &Human::EditHuman, this, &Application::Edit);
    BookWidgetLayout->removeWidget(hum);
    hum->hide();
    base->deleteUser(hum->getData());
    list.erase(std::find(list.begin(), list.end(), hum));
    trie->DeleteWord(hum->getData().first_name.toStdString() + " " + hum->getData().second_name.toStdString());
    DrawHumans(trie->TrieToVector());
}

void Application::CloseWindow()
{
    this->setEnabled(true);
}

void Application::Edit(const Person &from, const Person &to)
{
    base->ChangeValues(from, to);
    for(Human* hum : list){
        if(hum->getData() == from){
            hum->setData(to);
            return;
        }
    }
}

