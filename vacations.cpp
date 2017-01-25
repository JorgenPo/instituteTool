#include "vacations.h"
#include "ui_vacations.h"

#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>

#include <iostream>

using namespace std;

Vacations::Vacations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vacations)
{
    ui->setupUi(this);
}

Vacations::~Vacations()
{
    delete ui;
}

void Vacations::setEmployeeId(int id)
{
    ui->id->setValue(id);

    QSqlQuery q;
    q.prepare("SELECT begins, ends FROM vacations WHERE employer_id = :id");
    q.bindValue(":id", id);

    QDate begins, ends;
    begins.setDate(0, 0, 0);
    ends.setDate(0, 0, 0);

    q.exec();

    while (q.next()) {
        begins = q.value(0).toDate();
        ends = q.value(1).toDate();
    }

    ui->start->setDate(begins);
    ui->end->setDate(ends);
}

void Vacations::on_cancelButton_clicked()
{
    emit submitted();
}

void Vacations::on_saveButton_clicked()
{
    // Passport insert
    QSqlQuery query;
    query.prepare("INSERT INTO vacations (employer_id, begins, ends)"
                  " VALUES (:id, :begins, :ends)"
                  " ON DUPLICATE KEY UPDATE begins = :begins, ends = :ends;");

    query.bindValue(":id",      ui->id->value());
    query.bindValue(":begins",  ui->start->date());
    query.bindValue(":ends",    ui->end->date());

    if ( !query.exec() ) {
        cerr << "Error inserting vacation data: " << query.lastError().text().toStdString() << endl;
    } else {
        cout << "Vacation for " << ui->id->value()
             << " inserted to database!" << endl;
    }

    emit submitted();
}

void Vacations::on_id_valueChanged(int value)
{
    setEmployeeId(value);
}
