#include "menu.h"
#include "ui_menu.h"

#include <QDesktopWidget>

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    this->move(QApplication::desktop()->availableGeometry().center()
               - this->rect().center());
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_exitButton_clicked()
{
    exit(0);
}

void Menu::on_enterButton_clicked()
{
    emit onEntered();
    this->hide();
}
