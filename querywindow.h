#ifndef QUERYWINDOW_H
#define QUERYWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include "database.h"
#include <memory>
#include <QLineEdit>

class NumberBox: public QWidget{
    Q_OBJECT
public:
    explicit NumberBox(QWidget* parent = nullptr);
    ~NumberBox() = default;
    QString getData();
    void clear();
    void setNum(const QString& number);

private:
    std::unique_ptr<QLineEdit> TextBox;
    std::unique_ptr<QComboBox> ComboBox;
    std::unique_ptr<QHBoxLayout> layout;
    std::unique_ptr<QIntValidator> validator;
};

class AgeBox : public QWidget{
    Q_OBJECT
public:
    explicit AgeBox(QWidget* parent = nullptr);
    ~AgeBox();
    QString getData();
    void clear();
    void setAge(int age);

public slots:
    void ChangeText();

private:
    std::unique_ptr<QSlider> slider;
    std::unique_ptr<QLabel> Info;
    std::unique_ptr<QVBoxLayout> layout;

};

class QueryWindow: public QWidget
{
    Q_OBJECT
public:
    QueryWindow(QWidget* parent = nullptr);
    ~QueryWindow();

    void setData(const Person& data);

public slots:
    void SentData();

signals:
    void Submitted(const Person& data);
    void Closed();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    std::unique_ptr<QLabel> l_first;
    std::unique_ptr<QLabel> l_second;
    std::unique_ptr<QLabel> l_age;
    std::unique_ptr<QLabel> l_phone;

    std::unique_ptr<QTextEdit> t_first;
    std::unique_ptr<QTextEdit> t_second;
    std::unique_ptr<AgeBox> slider_age;

    std::unique_ptr<NumberBox> NumBox;

    std::unique_ptr<QPushButton> button;

    std::unique_ptr<QGridLayout> layout;
};

#endif // QUERYWINDOW_H
