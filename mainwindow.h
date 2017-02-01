#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "reports.h"
#include "teacher.h"
#include "textreports.h"
#include "vacations.h"

#include <QMainWindow>
#include <QSqlDatabase>
#include <QtSql/QSql>

#include <menu.h>
#include <employee.h>
#include <QSqlTableModel>
#include <ui_teacher.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateTable();

private slots:
    void on_addButton_clicked();

    void on_pushButton_clicked();

    void on_saveButton_clicked();

    void on_radioButton_clicked(bool checked);

    void on_addTeacherButton_clicked();

    void onTeacherAdded();
    void onVacationAdded();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    void initSQL();

private:
    Ui::MainWindow *ui;

    Employee *m_employee;
    Menu *m_menu;
    Teacher *m_teacher;
    Vacations *m_vacations;
    Reports *m_reports;
    TextReports *m_textReports;

    QSqlTableModel *m_employeesModel;
    QSqlTableModel *m_teachersModel;
    QSqlDatabase m_db;
};

#endif // MAINWINDOW_H
