#ifndef APPLICATION_H
#define APPLICATION_H
#include "human.h"
#include "querywindow.h"
#include "Trie.h"


class BookWidget;

class Application : public QWidget
{
    Q_OBJECT
public:
    explicit Application(QWidget* parent = 0);
    void AddHumans();
    void AddHuman(const Person& data);
    void DrawHumans(const std::vector<std::string>& SortedVector);

public slots:
    void ShowWindow();
    void MakeQuery(const Person& data);
    void DeleteHuman(Human* hum);
    void CloseWindow();
    void Edit(const Person& from, const Person& to);

private:
    std::unique_ptr<DataBase> base;
    std::unique_ptr<QPushButton> button;
    std::unique_ptr<QVBoxLayout> layout;
    std::unique_ptr<QScrollArea> scrollArea;
    std::unique_ptr<QWidget> BookWidget;
    std::unique_ptr<QVBoxLayout> BookWidgetLayout;
    std::unique_ptr<QueryWindow> win;
    std::unique_ptr<Trie> trie;
    QList<Human*> list;
    std::unordered_map<QChar, std::unique_ptr<QLabel>> labels;

    QSize size{300, 600};

};



#endif // APPLICATION_H
