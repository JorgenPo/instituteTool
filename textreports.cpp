#include "textreports.h"
#include "ui_textreports.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QTextBrowser>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

TextReports::TextReports(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextReports)
{
    ui->setupUi(this);
}

TextReports::~TextReports()
{
    delete ui;
}

void TextReports::on_report1_clicked()
{
    QTextBrowser browser;

    QSqlQuery query;

    int month = QDate::currentDate().month();

    qDebug() << "Starting report5 query..." << endl;

    QString queryString = tr(
              "SELECT p.name, p.last_name, v.begins, v.ends FROM passports as p "
              "INNER JOIN employees as e ON e.pass_id = p.id "
              "INNER JOIN vacations as v ON v.employer_id = e.id "
              "WHERE MONTH(v.begins) = '%1' OR MONTH(v.ends) = '%1';").arg(month);

    qDebug() << "Query string: " << queryString << endl;

    if ( !query.exec(queryString) ) {
        qDebug() << "Error with query " << queryString
                 << ";\nERROR TEXT: " << query.lastError() << endl;
        return;
    }


    QString string;
    browser.clear();

    browser.append("<h1>Сотрудники в отпуске в текущем месяце:</h1>");

    while ( query.next() ) {
        string = query.value(0).toString() + " " +
                 query.value(1).toString() + " отдыхает с " +
                 query.value(2).toDate().toString() + " по " +
                 query.value(3).toDate().toString();

        browser.append(string);
        browser.append("\n");
    }

    QString filename = "report" + QDate::currentDate().toString(Qt::DateFormat::ISODate) + ".html";

    QFile file(filename);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
        qDebug() << "Cannot save report file!" << endl;
        return;
    }

    QTextStream out(&file);
    out << browser.toHtml() << endl;

    file.close();

    QMessageBox::information(this,
                             tr("Отчет (сохранен в %1)").arg(filename),
                             browser.toPlainText(),
                             QMessageBox::Ok);
}
