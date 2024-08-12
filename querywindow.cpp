#include "querywindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QLineEdit>


QueryWindow::QueryWindow(QWidget *parent) : QWidget(parent)
{
    //inizializing
    l_first = std::make_unique<QLabel>("Name: ", this);
    l_second = std::make_unique<QLabel>("Second Name:  ", this);
    l_age = std::make_unique<QLabel>("Age:  ", this);
    l_phone = std::make_unique<QLabel>("Number:  ", this);

    t_first = std::make_unique<QTextEdit>(this);
    t_second = std::make_unique<QTextEdit>(this);
    slider_age = std::make_unique<AgeBox>(this);
    NumBox = std::make_unique<NumberBox>(this);

    button = std::make_unique<QPushButton>("Submit",this);

    layout = std::make_unique<QGridLayout>(this);

    QFont font("Lifehack Medium");
    font.setPointSize(15);

    l_first->setFont(font);
    l_second->setFont(font);
    l_age->setFont(font);
    l_phone->setFont(font);

    t_first->setFixedHeight(25);
    t_first->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t_first->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t_first->setStyleSheet("border-radius: 10px;");

    t_second->setFixedHeight(25);
    t_second->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t_second->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set up the layout
   layout->addWidget(l_first.get(), 0, 0);
   layout->addWidget(t_first.get(), 0, 1);

   layout->addWidget(l_second.get(), 1, 0);
   layout->addWidget(t_second.get(), 1, 1);

   layout->addWidget(l_age.get(), 2, 0);
   layout->addWidget(slider_age.get(), 2, 1);

   layout->addWidget(l_phone.get(), 3, 0);
   layout->addWidget(NumBox.get(), 3, 1);

   layout->addWidget(button.get(), 4, 0, 1, 2);

   button->setText("Submit");

   // Set up the layout spacing
   layout->setContentsMargins(10, 10, 10, 10);
   layout->setSpacing(10);

   // Set the layout for the widget
   setLayout(layout.get());

   // Set the window title and size
   setWindowTitle("Enter Data");
   resize(300, 200);


   //connection
   QObject::connect(this->button.get(), SIGNAL(clicked()), this, SLOT(SentData()));
}

QueryWindow::~QueryWindow()
{
    QObject::disconnect(this->button.get(), SIGNAL(clicked()), this, SLOT(SentData()));
}

void QueryWindow::setData(const Person &data)
{
    t_first->setText(data.first_name);
    t_second->setText(data.second_name);
    NumBox->setNum(data.number);
    slider_age->setAge(data.age);

}


void QueryWindow::SentData()
{
    QString first_name = t_first->toPlainText();
    QString second_name = t_second->toPlainText();
    int age = slider_age->getData().toInt();
    QString number = NumBox->getData();
    
    if(number == ""){
        QMessageBox::warning(this, "Empty Number", "Enter Number");
        return;
    }
    if(first_name == ""){
        QMessageBox::warning(this, "Empty First Name", "Enter First Name");
        return;
    }
    
    emit Submitted(Person(first_name, second_name, age, number));
    this->hide();

    t_first->clear();
    t_second->clear();
    slider_age->clear();
    NumBox->clear();

}

void QueryWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, "Close Widget", "Are you sure?") == QMessageBox::No){
        event->ignore();
    }else
        emit Closed();
}

NumberBox::NumberBox(QWidget *parent): QWidget(parent),
    TextBox(new QLineEdit),
    ComboBox(new QComboBox),
    layout(new QHBoxLayout),
    validator(new QIntValidator)
{
    ComboBox->addItem("RU +7");
    ComboBox->addItem("US +1");
    ComboBox->addItem("De +49");
    ComboBox->addItem("FR +33");

    TextBox->setFixedHeight(50);
    TextBox->setValidator(validator.get());
    TextBox->setFixedHeight(18);
    TextBox->setFixedWidth(100);

    layout->addWidget(ComboBox.get());
    layout->addWidget(TextBox.get());

    this->setLayout(layout.get());

}

QString NumberBox::getData()
{
    QString code = ComboBox->currentText().mid(3);
    QString num = TextBox->text();
    if(num == ""){
        return "";
    }
    return code + num;
}

void NumberBox::clear()
{
    TextBox->clear();
}

void NumberBox::setNum(const QString &number)
{
    this->TextBox->setText(number.mid(3));
}


AgeBox::AgeBox(QWidget *parent) : QWidget(parent),
    slider(new QSlider(Qt::Horizontal, this)),
    Info(new QLabel),
    layout(new QVBoxLayout)
{
    slider->setMinimum(0);
    slider->setMaximum(120);
    slider->setValue(0);

    Info->setText(QString::number(slider->value()) + " year old");
    Info->setFont(QFont("Lifehack Medium"));
    Info->setStyleSheet("font-size: 14 px;");
    Info->setAlignment(Qt::AlignCenter);

    layout->addWidget(Info.get());
    layout->addWidget(slider.get());

    this->setLayout(layout.get());

    QObject::connect(slider.get(), &QSlider::valueChanged, this, &AgeBox::ChangeText);
}

AgeBox::~AgeBox()
{
    QObject::disconnect(slider.get(), &QSlider::valueChanged, this,  &AgeBox::ChangeText);
}

QString AgeBox::getData()
{
    return QString::number(slider->value());
}

void AgeBox::clear()
{
    slider->setValue(0);
}

void AgeBox::setAge(int age)
{
    this->slider->setValue(age);
}

void AgeBox::ChangeText()
{
    if(slider->value() != 1 && slider->value() != 0)
        Info->setText(QString::number(slider->value()) + " years old");
    else
        Info->setText(QString::number(slider->value()) + " year old");
}
