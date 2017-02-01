#include "reports.h"
#include "ui_reports.h"
#include "ui_report1.h"
#include "ui_report2.h"
#include "ui_report3.h"
#include "ui_report4.h"
#include "ui_report5.h"
#include "ui_report6.h"
#include "ui_report7.h"
#include "ui_report8.h"

#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>

#include <algorithm>

Reports::Reports(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reports)
{
    ui->setupUi(this);

    initReports();
}

Reports::~Reports()
{
    delete ui;
}

void Reports::initReports()
{
    m_report1Ui = new Ui::Form;

    m_report1 = new QWidget;
    m_report1Ui->setupUi(m_report1);

    m_report1Model = new QSqlQueryModel(this);
    m_report1Ui->tableView->setModel(m_report1Model);

    m_report1Ui->experience_from->setMaximum(1000);
    m_report1Ui->experience_to->setMaximum(1000);
    m_report1Ui->experience_to->setValue(1000);

    connect(m_report1Ui->pushButton, SIGNAL(clicked()), this, SLOT(updateReport1()));

    m_report2Ui = new Ui::Report2;

    m_report2 = new QWidget;
    m_report2Ui->setupUi(m_report2);

    connect(m_report2Ui->pushButton, SIGNAL(clicked()), this, SLOT(updateReport2()));

    m_report3Ui = new Ui::Report3;

    m_report3 = new QWidget;
    m_report3Ui->setupUi(m_report3);

    m_report3Model = new QStandardItemModel(this);
    m_report3Ui->listView->setModel(m_report3Model);

    connect(m_report3Ui->pushButton, SIGNAL(clicked()), this, SLOT(updateReport3()));
    connect(m_report3Ui->updateListButton, SIGNAL(clicked()), this, SLOT(updateReport3List()));

    m_report4Ui = new Ui::Report4;

    m_report4 = new QWidget;
    m_report4Ui->setupUi(m_report4);

    connect(m_report4Ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateReport4()));

    m_report5Ui = new Ui::Report5;

    m_report5 = new QWidget;
    m_report5Ui->setupUi(m_report5);

    m_report5Model = new QStandardItemModel(this);
    m_report5Ui->employees->setModel(m_report5Model);

    connect(m_report5Ui->mounth, SIGNAL(currentIndexChanged(int)), this, SLOT(updateReport5()));

    m_report6Ui = new Ui::Report6;

    m_report6 = new QWidget;
    m_report6Ui->setupUi(m_report6);

    m_report6Model = new QStandardItemModel(this);
    m_report6Ui->listView->setModel(m_report6Model);

    m_report7Ui = new Ui::Report7;

    m_report7 = new QWidget;
    m_report7Ui->setupUi(m_report7);

    m_report7Model = new QStandardItemModel(this);
    m_report7Ui->listView->setModel(m_report7Model);

    m_report8Ui = new Ui::Report8;

    m_report8 = new QWidget;
    m_report8Ui->setupUi(m_report8);

    m_report8Model = new QStandardItemModel(this);
    m_report8Ui->listView->setModel(m_report8Model);

    connect(m_report8Ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateReport8()));

    findBestEmployer();
}

void Reports::updateReport1()
{
    QString degree = m_report1Ui->degree->currentText();
    degree = (degree == tr("Любая")) ? "e.degree = e.degree" : ("e.degree = '" + degree + "'");

    QString department = m_report1Ui->department->text();
    department = (department.isEmpty())
            ? "e.department = e.department" : "e.department = '" + department + "'";

    int from = m_report1Ui->experience_from->value();
    int to = m_report1Ui->experience_to->value();
    to = std::max(from, to);

    qDebug() << tr("SELECT p.last_name, p.name, p.father_name, t.teacher_load, e.degree, e.department "
               "FROM passports as p "
               "INNER JOIN employees as e "
               "ON p.id = e.pass_id "
               "INNER JOIN teachers as t "
               "ON t.employer_id = e.id "
               "WHERE (%1) AND "
                  "(e.department='%2') AND "
                  "(t.teacher_load BETWEEN %3 AND %4);")
               .arg(degree).arg(department).arg(from).arg(to) << endl;

    m_report1Model->setQuery(tr("SELECT p.last_name, p.name, p.father_name, t.experience, e.degree, e.department "
                             "FROM passports as p "
                             "INNER JOIN employees as e "
                             "ON p.id = e.pass_id "
                             "INNER JOIN teachers as t "
                             "ON t.employer_id = e.id "
                             "WHERE (%1) AND "
                                "   (%2) AND "
                                "(t.experience BETWEEN %3 AND %4);")
                             .arg(degree).arg(department).arg(from).arg(to));

    m_report1Model->setHeaderData(0, Qt::Horizontal, tr("Фамилия"));
    m_report1Model->setHeaderData(1, Qt::Horizontal, tr("Имя"));
    m_report1Model->setHeaderData(2, Qt::Horizontal, tr("Отчество"));
    m_report1Model->setHeaderData(3, Qt::Horizontal, tr("Стаж"));
    m_report1Model->setHeaderData(4, Qt::Horizontal, tr("Категория"));
    m_report1Model->setHeaderData(5, Qt::Horizontal, tr("Кафедра"));

    if ( m_report1Model->lastError().isValid() ) {
        qDebug() << m_report1Model->lastError();
    }

}

void Reports::updateReport2()
{
    qDebug() << "Starting report2 query..." << endl;

    QString department = m_report2Ui->department->text();

    QString queryString = tr(
              "SELECT AVG(t.teacher_load) FROM teachers as t "
              "INNER JOIN employees as e "
              "ON t.employer_id = e.id "
              "WHERE e.department = '%1' AND "
              "e.degree = 'Ассистент';").arg(department);

    qDebug() << "Query string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        qDebug() << "Query has no results..." << endl;
    }

    while ( query.next() ) {
        m_report2Ui->avgLoad->setText( query.value(0).toString() );
    }

    qDebug() << "Report2 was successful!" << endl;
}

void Reports::updateReport3()
{
    qDebug() << "Starting report3 query..." << endl;

    QString department = m_report3Ui->department->text();

    QString queryString = tr(
              "SELECT e.id, p.name, p.last_name FROM passports as p "
              "INNER JOIN employees as e ON e.pass_id = p.id "
              "INNER JOIN teachers as t ON t.employer_id = e.id "
              "WHERE e.department = '%1' AND"
              "      e.rank = 'Доцент';").arg(department);

    qDebug() << "Query string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        QMessageBox::information(this,
                                 tr("Нет результатов"),
                                 tr("На выбранной кафедре нет доцентов. "
                                    "Введите другое название кафедры или добавьте доцентов в базу."),
                                 QMessageBox::Ok);

        qDebug() << "Query has no results..." << endl;
        return;
    }

    m_report3Ui->teachersList->clear();
    while ( query.next() ) {
        m_report3Ui->teachersList->addItem( query.value(1).toString() + " " +
                                        query.value(2).toString(),
                                        query.value(0));
    }

    updateReport3List();
    qDebug() << "Report3 was successful!" << endl;
}

void Reports::updateReport3List()
{
    QStringList parts = m_report3Ui->teachersList->currentText().split(" ");
    QString name = parts.at(0);
    QString last = parts.at(1);

    qDebug() << "Querry list for " << name << " " << last << endl;

    if ( name.isEmpty() || last.isEmpty() ) {
        return;
    }

    QString id = m_report3Ui->teachersList->currentData().toString();

    QString queryString = tr(
                "SELECT n.name FROM disciplines_names as n "
                "INNER JOIN disciplines as d ON d.discipline_id = n.id "
                "WHERE d.employer_id = '%1';").arg(id);

    qDebug() << "Query list string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        qDebug() << "Query list has no results..." << endl;
        return;
    }

    m_report3Model->clear();

    while ( query.next() ) {
        m_report3Model->appendRow(new QStandardItem(query.value(0).toString()));
    }
}

void Reports::updateReport4()
{
    qDebug() << "Starting report4 query..." << endl;

    QString degree = m_report4Ui->comboBox->currentText();

    QString queryString = tr(
              "SELECT COUNT(e.id) FROM employees as e "
              "WHERE e.degree = '%1';").arg(degree);

    qDebug() << "Query string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        QMessageBox::information(this,
                                 tr("Нет результатов"),
                                 tr("Преподавателей с выбранной категорией не существует, "
                                    "выберите другую категорию из списка."),
                                 QMessageBox::Ok);

        qDebug() << "Query has no results..." << endl;
        return;
    }

    while ( query.next() ) {
        m_report4Ui->spinBox->setValue(query.value(0).toInt());
    }

    qDebug() << "Report3 was successful!" << endl;
}

void Reports::updateReport5()
{
    qDebug() << "Starting report5 query..." << endl;

    int month = m_report5Ui->mounth->currentIndex() + 1;

    QString queryString = tr(
              "SELECT p.name, p.last_name, v.begins, v.ends FROM passports as p "
              "INNER JOIN employees as e ON e.pass_id = p.id "
              "INNER JOIN vacations as v ON v.employer_id = e.id "
              "WHERE MONTH(v.begins) = '%1' OR MONTH(v.ends) = '%1';").arg(month);

    qDebug() << "Query string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        QMessageBox::information(m_report5,
                                 tr("Нет результатов"),
                                 tr("Никто не отдыхает в выбранном месяце."),
                                 QMessageBox::Ok);
        m_report5Model->clear();
        qDebug() << "Query has no results..." << endl;
        return;
    }

    m_report5Model->clear();
    QString string;
    while ( query.next() ) {
        string = query.value(0).toString() + " " +
                 query.value(1).toString() + " отдыхает с " +
                 query.value(2).toDate().toString() + " по " +
                 query.value(3).toDate().toString();
        m_report5Model->appendRow(new QStandardItem(string));
    }

    qDebug() << "Report3 was successful!" << endl;
}

void Reports::updateReport6()
{
    qDebug() << "Starting report6 query..." << endl;

    QDate currentDate = QDate::currentDate();

    QString queryString = tr(
              "SELECT p.name, p.last_name, t.contract_from, t.contract_to FROM teachers as t "
              "INNER JOIN employees as e ON e.id = t.employer_id "
              "INNER JOIN passports as p ON p.id = e.pass_id "
              "WHERE t.contract_to < '%1';").arg(currentDate.toString(Qt::DateFormat::ISODate));


    QSqlQuery query;

    qDebug() << "Query string: " << queryString << endl;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        QMessageBox::information(m_report5,
                                 tr("Нет результатов"),
                                 tr("Все контракты актуальны!"),
                                 QMessageBox::Ok);
        m_report5Model->clear();
        qDebug() << "Query has no results..." << endl;
        return;
    }

    m_report6Model->clear();
    QString string;
    while ( query.next() ) {
        string = query.value(0).toString() + " " +
                 query.value(1).toString() + " контракт с " +
                 query.value(2).toDate().toString(Qt::DateFormat::ISODate) + " по " +
                 query.value(3).toDate().toString(Qt::DateFormat::ISODate);
        m_report6Model->appendRow(new QStandardItem(string));
    }

    qDebug() << "Report6 was successful!" << endl;
}

void Reports::updateReport7()
{
    qDebug() << "Starting report7 query..." << endl;

    QString queryString = tr(
              "SELECT p.name, p.last_name, r.date FROM passports as p "
              "INNER JOIN employees as e ON e.pass_id = p.id "
              "INNER JOIN rewards as r ON r.owner_id = e.id "
              "INNER JOIN rewards_names as n ON r.reward_id = n.id "
              "WHERE n.name = 'За оборону Ленинграда';");


    QSqlQuery query;

    qDebug() << "Query string: " << queryString << endl;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        QMessageBox::information(m_report7,
                                 tr("Нет результатов"),
                                 tr("Нет работников с медалью 'За оборону Ленинграда'!"),
                                 QMessageBox::Ok);
        m_report7Model->clear();
        qDebug() << "Query has no results..." << endl;
        return;
    }

    m_report7Model->clear();
    QString string;
    while ( query.next() ) {
        string = query.value(0).toString() + " " +
                 query.value(1).toString() + " вручена " +
                 query.value(2).toDate().toString(Qt::DateFormat::ISODate);
        m_report7Model->appendRow(new QStandardItem(string));
    }

    qDebug() << "Report7 was successful!" << endl;
}

void Reports::updateReport8()
{
    qDebug() << "Starting report8 query..." << endl;

    QString id = m_report8Ui->comboBox->currentData().toString();

    QString queryString = tr(
              "SELECT p.name, p.last_name FROM passports as p "
              "INNER JOIN employees as e ON e.pass_id = p.id "
              "INNER JOIN disciplines as d ON d.employer_id = e.id "
              "WHERE d.discipline_id = '%1';").arg(id);


    QSqlQuery query;

    qDebug() << "Query string: " << queryString << endl;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        m_report8Model->clear();
        qDebug() << "Query has no results..." << endl;
        return;
    }

    m_report8Model->clear();
    QString string;
    while ( query.next() ) {
        string = query.value(0).toString() + " " +
                 query.value(1).toString();
        m_report8Model->appendRow(new QStandardItem(string));
    }

    qDebug() << "Report8 was successful!" << endl;
}

void Reports::findBestEmployer()
{
    QString queryString = tr(
              "SELECT p.name, p.last_name, e.employ_date FROM passports as p "
              "INNER JOIN employees as e ON e.pass_id = p.id "
              "WHERE e.employ_date IN (SELECT MIN(e.employ_date) FROM employees as e);");

    QSqlQuery query;

    qDebug() << "Query string: " << queryString << endl;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        qDebug() << "Query has no results..." << endl;
        return;
    }

    query.next();

    QDate date = query.value(2).toDate();
    int years = date.daysTo(QDate::currentDate()) / 365;

    QString teacher = QString(query.value(0).toString() + " " + query.value(1).toString() +
            " проработал(а) целых %1 лет!!!").arg(years);

    m_bestTeacher = new QString(teacher);
}

void Reports::on_query1_clicked()
{
    m_report1->show();
    updateReport1();
}

void Reports::on_query2_clicked()
{
    m_report2->show();
}

void Reports::on_query4_clicked()
{
    m_report3->show();
}

void Reports::on_query3_clicked()
{
    qDebug() << "Starting report4 query..." << endl;

    QString queryString = tr(
              "SELECT e.degree FROM employees as e "
              "INNER JOIN teachers as t ON t.employer_id = e.id "
              "GROUP BY e.degree;");

    qDebug() << "Query string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        QMessageBox::information(this,
                                 tr("Нет результатов"),
                                 tr("Произошла ошибка. Возможно, база данных пуста или подключение закрыто."),
                                 QMessageBox::Ok);

        qDebug() << "Query has no results..." << endl;
    }

    m_report4Ui->comboBox->clear();
    while ( query.next() ) {
        m_report4Ui->comboBox->addItem(query.value(0).toString());
    }

    qDebug() << "Report4 list query was successful!" << endl;

    m_report4->show();
}

void Reports::on_query5_clicked()
{
    m_report5->show();
    updateReport5();
}

void Reports::on_query6_clicked()
{
    updateReport6();
    m_report6->show();
}

void Reports::on_query7_clicked()
{
    updateReport7();
    m_report7->show();
}

void Reports::on_query8_clicked()
{
    qDebug() << "Starting report8 query..." << endl;

    QString queryString = tr(
              "SELECT d.id, d.name FROM disciplines_names as d;");

    qDebug() << "Query string: " << queryString << endl;

    QSqlQuery query;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }

    if ( query.size() == 0 ) {
        qDebug() << "Query has no results..." << endl;
    }

    m_report8Ui->comboBox->clear();
    while ( query.next() ) {
        m_report8Ui->comboBox->addItem(query.value(1).toString(), query.value(0));
    }

    qDebug() << "Report4 list query was successful!" << endl;

    m_report8->show();
}

void Reports::on_query9_clicked()
{
    QString name = *m_bestTeacher;
    QMessageBox::information(this,
                             tr("Самый опытный сотрудник"),
                             tr("%1 самый опытный сотрудник!").arg(name),
                             QMessageBox::Ok);
}
