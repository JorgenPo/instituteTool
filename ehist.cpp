#include "ehist.h"
#include "ui_ehist.h"

Ehist::Ehist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ehist)
{
    ui->setupUi(this);

}

Ehist::~Ehist()
{
    delete ui;
}

void Ehist::on_nextButton_clicked()
{
    emit completed();
}
