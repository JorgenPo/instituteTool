#ifndef PASSPORT_H
#define PASSPORT_H

#include <QDate>
#include <QString>


class Passport
{
public:
    static Passport* randomPassport();

private:
    static QList<QString>* firstNames;
    static QList<QString>* lastNames;
    static QList<QString>* fatherNames;
    static QList<QString>* issuedNames;

    static void initStatic();

    static bool isInitStatic;

    static QString generateFirst(bool male);
    static QString generateLast(bool male);
    static QString generateFather(bool male);
    static QString generateIssued();

public:
    Passport();
    Passport(QString id, QString first, QString last, QString father,
             QString issued, QDate date, QDate valid);
    QString id() const;
    void setId(const QString &id);

    QString first() const;
    void setFirst(const QString &first);

    QString last() const;
    void setLast(const QString &last);

    QString father() const;
    void setFather(const QString &father);

    QString issued() const;
    void setIssued(const QString &issued);

    QDate date() const;
    void setDate(const QDate &date);

    QDate valid() const;
    void setValid(const QDate &valid);

private:
    QString m_id;
    QString m_first;
    QString m_last;
    QString m_father;
    QString m_issued;
    QDate m_date;
    QDate m_valid;
};

#endif // PASSPORT_H
