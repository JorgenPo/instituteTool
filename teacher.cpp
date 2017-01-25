#include "teacher.h"

#include <QSqlQuery>
#include <QSqlError>

#include <iostream>
#include "mainwindow.h"

using namespace std;

Teacher::Teacher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Teacher)
{
    ui->setupUi(this);
}

Teacher::~Teacher()
{
    delete ui;
}

void Teacher::on_addButton_clicked()
{
    submitTeacher();
    emit submitted();
}

void Teacher::on_cancelButton_clicked()
{
    emit submitted();
}

void Teacher::submitTeacher()
{
    QSqlQuery q;
    q.prepare("INSERT INTO teachers (employer_id, contract_from, contract_to, experience, teacher_load) "
              "VALUES ( ?, ?, ?, ?, ? );");

    q.addBindValue(ui->id->text());
    q.addBindValue(ui->contract_start->date());
    q.addBindValue(ui->contract_end->date());
    q.addBindValue(ui->expirience->text());
    q.addBindValue(ui->load->text());

    cout << q.executedQuery().toStdString() << endl;

    if ( !q.exec() ) {
        cerr << "Error inserting teacher data: " << q.lastError().text().toStdString() << endl;
    } else {
        cout << "Teacher " << ui->id->value() << " inserted to database!" << endl;
    }
}
