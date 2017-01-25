#include "passport.h"
#include <QVector>
#include <cstdlib>
#include <fstream>
#include <string>

#include <QStringBuilder>
#include <iostream>

using namespace std;

bool Passport::isInitStatic = false;

QList<QString>* Passport::firstNames = nullptr;
QList<QString>* Passport::lastNames = nullptr;
QList<QString>* Passport::fatherNames = nullptr;
QList<QString>* Passport::issuedNames = nullptr;

Passport* Passport::randomPassport()
{
    if ( !Passport::isInitStatic ) {
        Passport::initStatic();
    }

    QString hiId, loId;
    hiId = QString::number(rand() % 89999 + 10000);
    loId = QString::number(rand() % 89999 + 10000);

    QString id = hiId + loId;

    bool male = (bool) (rand() % 2);
    QString name = Passport::generateFirst(male);
    QString last = Passport::generateLast(male);
    QString father = Passport::generateFather(male);
    QString issued = Passport::generateIssued();

    QDate date = QDate();
    date.setDate(rand() % 50 + 1950, rand() % 12 + 1, rand() % 30 + 1);

    QDate valid = QDate(date);
    valid = valid.addYears(20);

    Passport* pass = new Passport(
                id, name, last, father, issued,
                date, valid);

    return pass;
}

void Passport::initStatic()
{
    cout << "Loading passport name files..." << endl;

    Passport::firstNames = new QList<QString>();
    Passport::lastNames = new QList<QString>();
    Passport::fatherNames = new QList<QString>();
    Passport::issuedNames = new QList<QString>();

    string line;

    cout << "Loading first names.." << endl;
    ifstream in("passports/first.txt");
    while ( in >> line ) {
        firstNames->append(QString::fromStdString(line));
    }

    cout << "Loading last names..." << endl;
    in.close();
    in.open("passports/last.txt");
    while ( in >> line ) {
        lastNames->append(QString::fromStdString(line));
    }

    cout << "Loading father names..." << endl;
    in.close();
    in.open("passports/father.txt");
    while ( in >> line ) {
        fatherNames->append(QString::fromStdString(line));
    }

    cout << "Loading issued names..." << endl;
    in.close();
    in.open("passports/issued.txt");

    string builder;
    while ( (in >> line) ) {
        if ( line == ";" ) {
            issuedNames->append(QString::fromStdString(builder));
        }
        builder += " " + line;

    }
    in.close();

    if ( firstNames->size()   == 0 ||
         lastNames->size()    == 0 ||
         fatherNames->size()  == 0 ||
         issuedNames->size()  == 0 ) {
        cerr << "Name files is empty!" << endl;
        exit(1);
    }

    Passport::isInitStatic = true;
}

Passport::Passport()
{

}

Passport::Passport(QString id, QString first, QString last, QString father,
                   QString issued, QDate date, QDate valid)
{
    m_id = id;
    m_first = first;
    m_last = last;
    m_father = father;
    m_issued = issued;
    m_date = date;
    m_valid = valid;
}

QString Passport::id() const
{
    return m_id;
}

void Passport::setId(const QString &id)
{
    m_id = id;
}

QString Passport::first() const
{
    return m_first;
}

void Passport::setFirst(const QString &first)
{
    m_first = first;
}

QString Passport::last() const
{
    return m_last;
}

void Passport::setLast(const QString &last)
{
    m_last = last;
}

QString Passport::father() const
{
    return m_father;
}

void Passport::setFather(const QString &father)
{
    m_father = father;
}

QString Passport::issued() const
{
    return m_issued;
}

void Passport::setIssued(const QString &issued)
{
    m_issued = issued;
}

QDate Passport::date() const
{
    return m_date;
}

void Passport::setDate(const QDate &date)
{
    m_date = date;
}

QDate Passport::valid() const
{
    return m_valid;
}

void Passport::setValid(const QDate &valid)
{
    m_valid = valid;
}

QString Passport::generateFirst(bool male)
{
    int num = 0;
    while ( true ) {
        num = rand() % Passport::firstNames->size();
        if (!male) {
            if ( num % 2 != 0 ) {
                break;
            } else {
                continue;
            }
        } else {
            if ( num % 2 == 0) {
                break;
            } else {
                continue;
            }
        }
    }

    return Passport::firstNames
            ->at(rand() % Passport::firstNames->size());
}

QString Passport::generateLast(bool male)
{
    int num = 0;
    while ( true ) {
        num = rand() % Passport::lastNames->size();
        if (!male) {
            if ( num % 2 != 0 ) {
                break;
            } else {
                continue;
            }
        } else {
            if ( num % 2 == 0) {
                break;
            } else {
                continue;
            }
        }
    }

    return Passport::lastNames
            ->at(num);
}

QString Passport::generateFather(bool male)
{
    int num = 0;
    while ( true ) {
        num = rand() % Passport::fatherNames->size();
        if (!male) {
            if ( num % 2 != 0 ) {
                break;
            } else {
                continue;
            }
        } else {
            if ( num % 2 == 0) {
                break;
            } else {
                continue;
            }
        }
    }

    return Passport::fatherNames
            ->at(num);
}

QString Passport::generateIssued()
{
    return Passport::issuedNames
            ->at(rand() % Passport::issuedNames->size());
}

