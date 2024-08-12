#ifndef HUMAN_H
#define HUMAN_H

#include <QtWidgets>
#include "infoapp.h"

class Human : public QPushButton
{
    Q_OBJECT
public:
    Human(const Person& pers, QPushButton* parent = 0);
    Human(const Human& other);
    Human &operator=(const Human& other);

    void configLabels();

    void setData(const Person& dat);
    Person& getData(){return data;}



public slots:
    void OpenInfo();
    void CloseInfo();

signals:
    void DeleteThis(Human* hum);
    void EditHuman(const Person& from, const Person& to);
private:
    Person data;
    std::unique_ptr<InfoApp> Info;
};

#endif // HUMAN_H
