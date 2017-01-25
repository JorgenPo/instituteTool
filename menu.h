#ifndef MENU_H
#define MENU_H

#include <QWidget>

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

signals:
    void onEntered();

private slots:

    void on_exitButton_clicked();

    void on_enterButton_clicked();

private:
    Ui::Menu *ui;
};

#endif // MENU_H
