#include "employee.h"
#include "passport.h"

#include "ui_employee.h"
#include "ui_ehist.h"
#include "ui_einfo.h"

#include <QSqlQuery>
#include <QSqlError>

#include <iostream>

using namespace std;

Employee::Employee(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Employee),
    m_ehistForm(),
    m_einfoForm()
{
    ui->setupUi(this);

    m_ehistForm = new Ehist;
    m_einfoForm = new EInfo;
}

Employee::~Employee()
{
    delete ui;
}

/**
 * @brief Employee::on_randomButton_clicked
 * Generate random passport values
 */
void Employee::on_randomButton_clicked()
{

    srand(time(NULL));
    Passport* pass = Passport::randomPassport();

    ui->id->setText(pass->id());
    ui->name->setText(pass->first());
    ui->last->setText(pass->last());
    ui->father->setText(pass->father());
    ui->issued->setText(pass->issued());
    ui->date->setText(pass->date().toString());
    ui->valid->setText(pass->valid().toString());

    QString hiId, loId;
    hiId = QString::number(rand() % 8999 + 1000);
    loId = QString::number(rand() % 899 + 100);

    QString id = hiId + loId;

    QDate date = QDate();
    date.setDate(rand() % 50 + 1950, rand() % 12 + 1, rand() % 30 + 1);

    m_ehistForm->getUi()->id->setText(id);
    m_ehistForm->getUi()->date->setText(date.toString());

    hiId = QString::number(rand() % 899999 + 100000);
    loId = QString::number(rand() % 899999 + 100000);

    id = hiId + loId;

    m_einfoForm->getUi()->inn_id->setText(id);


    hiId = QString::number(rand() % 899999 + 100000);
    loId = QString::number(rand() % 89999 + 10000);

    id = hiId + loId;

    m_einfoForm->getUi()->pens_id->setText(id);

    date.setDate(rand() % 50 + 1950, rand() % 12 + 1, rand() % 30 + 1);
    m_einfoForm->getUi()->date->setText(date.toString());


}

void Employee::secondStep() {
    m_ehistForm->show();

    connect(m_ehistForm, SIGNAL(completed()), this, SLOT(lastStep()));
}

void Employee::lastStep() {
    m_ehistForm->hide();

    m_einfoForm->show();

    connect(m_einfoForm, SIGNAL(completed()), this, SLOT(addEmployee()));
}

void Employee::addEmployee()
{
    m_einfoForm->hide();

    EmployeeInfo info;

    Passport *pass = new Passport();
    pass->setId(ui->id->text());
    pass->setDate(QDate::fromString(ui->date->text()));
    pass->setFirst(ui->name->text());
    pass->setLast(ui->last->text());
    pass->setFather(ui->father->text());
    pass->setIssued(ui->issued->text());
    pass->setValid(QDate::fromString(ui->valid->text()));

    info.pass = pass;

    Ui::Ehist* ehist = m_ehistForm->getUi();
    info.ehistId = ehist->id->text();
    info.ehistDate = QDate::fromString(ehist->date->text());
    info.vacation_start = ehist->vacation_start->date();
    info.vacation_end = ehist->vacation_end->date();

    Ui::EInfo* einfo = m_einfoForm->getUi();
    info.degree = einfo->degree->text();
    info.department = einfo->department->text();
    info.employDate = QDate::fromString(einfo->date->text());
    info.inn = einfo->inn_id->text();
    info.pensId = einfo->pens_id->text();
    info.position = einfo->position->text();
    info.rank = einfo->rank->text();

    submitEmployee(info);
}

void Employee::on_nextButton_clicked()
{
    this->hide();

    secondStep();
}

void Employee::submitEmployee(Employee::EmployeeInfo info)
{
    // Passport insert
    QSqlQuery query;
    query.prepare("INSERT INTO passports (id, name, last_name, father_name, issued, date, valid_thru)"
                  " VALUES (:id, :name, :last, :father, :issued, :date, :valid);");
    query.bindValue(":id",      info.pass->id());
    query.bindValue(":name",    info.pass->first());
    query.bindValue(":last",    info.pass->last());
    query.bindValue(":father",  info.pass->father());
    query.bindValue(":issued",  info.pass->issued());
    query.bindValue(":date",    info.pass->date());
    query.bindValue(":valid",   info.pass->valid());

    if ( !query.exec() ) {
        cerr << "Error inserting passport data: " << query.lastError().text().toStdString() << endl;
    } else {
        cout << "Passport " << info.pass->id().toStdString() << " inserted to database!" << endl;
    }

    // Ehist insert
    query.prepare("INSERT INTO employees_history (id, date)"
                  " VALUES (:id, :date);");
    query.bindValue(":id", info.ehistId);
    query.bindValue(":date", info.ehistDate);

    cout << "Query: " << query.executedQuery().toStdString() << endl;

    if ( !query.exec() ) {
        cerr << "Error inserting ehist data: " << query.lastError().text().toStdString() << endl;
    } else {
        cout << "Ehist " << info.ehistId.toStdString() << " inserted to database!" << endl;
    }

    // Info insert
    query.prepare("INSERT INTO employees (pass_id, ehist_id, inn, pens_id, department, employ_date, position, degree, rank)"
                  " VALUES (:pass, :ehist, :inn, :pens, :department, :date, :position, :degree, :rank);");
    query.bindValue(":pass",        info.pass->id());
    query.bindValue(":ehist",       info.ehistId);
    query.bindValue(":inn",         info.inn);
    query.bindValue(":pens",        info.pensId);
    query.bindValue(":department",  info.department);
    query.bindValue(":date",        info.employDate);
    query.bindValue(":position",    info.position);
    query.bindValue(":degree",      info.degree);
    query.bindValue(":rank",        info.rank);

    cout << "Query: " << query.executedQuery().toStdString() << endl;

    if ( !query.exec() ) {
        cerr << "Error inserting employee data: " << query.lastError().text().toStdString() << endl;
    } else {
        cout << "Employee " << info.pass->first().toStdString()
             << " inserted to database!" << endl;
    }

    emit submitted();
}
