#ifndef REPORTS_H
#define REPORTS_H

#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QWidget>
#include <QComboBox>

namespace Ui {
class Reports;
class Form;
class Report2;
class Report3;
class Report4;
class Report5;
class Report6;
class Report7;
class Report8;
}

class Reports : public QWidget
{
    Q_OBJECT

public:
    explicit Reports(QWidget *parent = 0);
    ~Reports();
private:
    void initReports();


private slots:
    void on_query1_clicked();
    void updateReport1();
    void updateReport2();
    void updateReport3();
    void updateReport3List();
    void updateReport4();
    void updateReport5();
    void updateReport6();
    void updateReport7();
    void updateReport8();

    void findBestEmployer();

    void on_query2_clicked();

    void on_query4_clicked();

    void on_teachersList_currentTextChanged(const QString &arg1);

    void on_teachersList_currentIndexChanged(int);

    void on_updateListButton_clicked();

    void on_query3_clicked();

    void on_query5_clicked();

    void on_query6_clicked();

    void on_query7_clicked();

    void on_query8_clicked();

    void on_query9_clicked();

private:
    Ui::Reports *ui;

    QWidget  *m_report1;
    Ui::Form *m_report1Ui;

    QWidget     *m_report2;
    Ui::Report2 *m_report2Ui;

    QWidget     *m_report3;
    Ui::Report3 *m_report3Ui;

    QWidget     *m_report4;
    Ui::Report4 *m_report4Ui;

    QWidget     *m_report5;
    Ui::Report5 *m_report5Ui;

    QWidget     *m_report6;
    Ui::Report6 *m_report6Ui;

    QWidget     *m_report7;
    Ui::Report7 *m_report7Ui;

    QWidget     *m_report8;
    Ui::Report8 *m_report8Ui;

    QSqlQueryModel *m_report1Model;
    QStandardItemModel* m_report3Model;
    QStandardItemModel* m_report5Model;
    QStandardItemModel* m_report6Model;
    QStandardItemModel* m_report7Model;
    QStandardItemModel* m_report8Model;

    QString*    m_bestTeacher;
};

#endif // REPORTS_H
