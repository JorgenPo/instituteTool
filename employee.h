#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QWidget>

#include "ehist.h"
#include "einfo.h"
#include "passport.h"

namespace Ui {
class Employee;
}

class Employee : public QWidget
{
    Q_OBJECT

private:
    struct EmployeeInfo {
        Passport* pass;
        QString ehistId;
        QDate ehistDate;
        QDate vacation_start;
        QDate vacation_end;
        QString inn;
        QString pensId;
        QString department;
        QDate employDate;
        QString position;
        QString degree;
        QString rank;

    };

    void submitEmployee(EmployeeInfo info);

public:
    explicit Employee(QWidget *parent = 0);
    ~Employee();

signals:
    void submitted();

private slots:
    void on_randomButton_clicked();

    void on_nextButton_clicked();

public slots:
    void secondStep();
    void lastStep();
    void addEmployee();

private:
    Ui::Employee *ui;

    Ehist *m_ehistForm;
    EInfo *m_einfoForm;
};

#endif // EMPLOYEE_H
