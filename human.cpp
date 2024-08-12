#include "human.h"


Human::Human(const Person& pers, QPushButton* parent) : QPushButton(parent),
        data(pers)
{
    configLabels();
}

Human::Human(const Human &other): QPushButton(other.parentWidget()),
    data(other.data)
{
    configLabels();
}

Human& Human::operator =(const Human& other){
    if(this != &other) {
        this->data = other.data;
    }
    configLabels();
    return *this;
}

void Human::configLabels()
{
    // Set font and color for labels
    QFont labelFont("Arial", 14);
    labelFont.setBold(true);
    QColor labelColor(255, 255, 255); // White color
    QString text = data.first_name + " " + data.second_name;
    this->setText(text);


    this->setStyleSheet("background-color: #111111;"); // Darker gray background for widget
    this->setStyleSheet("border: 2px solid white;");
    this->setFont(labelFont);
    this->setFixedSize(250, 50); // Set fixed size for widget
    this->setStyleSheet("QPushButton {"
                        "background-color: #444444;" /* основной цвет кнопки */
                        "border-radius: 10px;" /* радиус скругления */
                        "}"
                        "QPushButton:hover {"
                        "background-color: #222222;" /* цвет кнопки при наведении */
                        "}"
                        "QPushButton:pressed {"
                        "background-color: #333333;" /* цвет кнопки при нажатии */
                        "}");


    Info = nullptr;

    QObject::connect(this, &Human::clicked, this, &Human::OpenInfo);

}

void Human::setData(const Person &dat){
    this->data = dat;
    this->setText(data.first_name + " " + data.second_name);
}

void Human::OpenInfo()
{
    Info = std::make_unique<InfoApp>(this->data);
    Info->show();

    QObject::connect(Info.get(), &InfoApp::Close, this, &Human::CloseInfo);
    QObject::connect(Info.get(), &InfoApp::Delete, this, [=](){emit DeleteThis(this);});
    QObject::connect(Info.get(), &InfoApp::ToEdit, this, &Human::EditHuman);
}

void Human::CloseInfo()
{
    QObject::disconnect(Info.get(), &InfoApp::destroyed, this, &Human::CloseInfo);
    QObject::disconnect(Info.get(), &InfoApp::ToEdit, this, &Human::EditHuman);
    Info->hide();
    Info.reset();
    Info = nullptr;

}

