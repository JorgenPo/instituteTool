#include "einfo.h"
#include "ui_einfo.h"

EInfo::EInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EInfo)
{
    ui->setupUi(this);
}

EInfo::~EInfo()
{
    delete ui;
}

void EInfo::on_addButton_clicked()
{
    emit completed();
}
