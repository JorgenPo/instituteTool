#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>

#include <QDesktopWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initSQL();

    m_menu = new Menu;
    m_employee = new Employee;
    m_teacher = new Teacher;
    m_vacations = new Vacations;
    m_reports = new Reports;

    this->move(QApplication::desktop()->availableGeometry().center()
               - this->rect().center());
    m_menu->move(QApplication::desktop()->availableGeometry().center()
               - m_menu->rect().center());
    m_employee->move(QApplication::desktop()->availableGeometry().center()
               - m_employee->rect().center());
    m_teacher->move(QApplication::desktop()->availableGeometry().center()
               - m_teacher->rect().center());
    m_vacations->move(QApplication::desktop()->availableGeometry().center()
               - m_vacations->rect().center());
    m_reports->move(QApplication::desktop()->availableGeometry().center()
               - m_reports->rect().center());

    connect(m_menu, SIGNAL(onEntered()), this, SLOT(show()));

    m_menu->show();

    m_employeesModel = new QSqlTableModel(this);
    m_employeesModel->setTable("employees");
    m_employeesModel->setSort(0, Qt::AscendingOrder);
    m_employeesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_employeesModel->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    m_employeesModel->setHeaderData(1, Qt::Horizontal, tr("Паспорт"));
    m_employeesModel->setHeaderData(2, Qt::Horizontal, tr("Труд. книжка"));
    m_employeesModel->setHeaderData(3, Qt::Horizontal, tr("ИНН"));
    m_employeesModel->setHeaderData(4, Qt::Horizontal, tr("СНИЛС"));
    m_employeesModel->setHeaderData(5, Qt::Horizontal, tr("Отдел/Кафедра"));
    m_employeesModel->setHeaderData(6, Qt::Horizontal, tr("Дата приема"));
    m_employeesModel->setHeaderData(7, Qt::Horizontal, tr("Должность"));
    m_employeesModel->setHeaderData(8, Qt::Horizontal, tr("Степень"));
    m_employeesModel->setHeaderData(9, Qt::Horizontal, tr("Звание"));

    m_teachersModel = new QSqlTableModel(this);
    m_teachersModel->setTable("teachers");
    m_teachersModel->setSort(0, Qt::AscendingOrder);
    m_teachersModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_teachersModel->setHeaderData(0, Qt::Horizontal, tr("Номер"));
    m_teachersModel->setHeaderData(1, Qt::Horizontal, tr("Нагрузка ч."));
    m_teachersModel->setHeaderData(2, Qt::Horizontal, tr("Дата заключения контракта"));
    m_teachersModel->setHeaderData(3, Qt::Horizontal, tr("Дата окончания контракта"));
    m_teachersModel->setHeaderData(4, Qt::Horizontal, tr("Стаж"));

    m_employeesModel->select();
    ui->tableView->setModel(m_employeesModel);

    updateTable();
}

void MainWindow::initSQL() {
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("institute");
    m_db.setUserName("root");
    m_db.setPassword("4s6qymbp");

    if ( !m_db.open() ) {
        cerr << "Error open database: " << m_db.lastError()
                .text().toStdString() << endl;
    }
}

void MainWindow::updateTable()
{
    m_employeesModel->select();
    m_teachersModel->select();

    ((QSqlTableModel) ui->tableView->model()).select();

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    m_employee->show();
    connect(m_employee, SIGNAL(submitted()), this, SLOT(updateTable()));
}

void MainWindow::on_pushButton_clicked()
{
    int row = ui->tableView->selectionModel()
            ->selectedRows().first().row();

    ui->tableView->model()->removeRow(row);
}

void MainWindow::on_saveButton_clicked()
{
    m_employeesModel->submitAll();
    m_teachersModel->submitAll();
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    if ( checked ) {
        ui->label->setText(tr("Список преподавателей"));
        ui->tableView->setModel(m_teachersModel);
        ui->addTeacherButton->setEnabled(false);
    } else {
        ui->label->setText(tr("Список сотрудников"));
        ui->tableView->setModel(m_employeesModel);
        ui->addTeacherButton->setEnabled(true);
    }

    updateTable();
}

void MainWindow::on_addTeacherButton_clicked()
{
    int row = ui->tableView->selectionModel()->selectedRows().first().row();
    int id = m_employeesModel->index(row, 0).data().toInt();

    m_teacher->getUi()->id->setValue(id);

    m_teacher->getUi()->contract_start->setDate(QDate::currentDate());
    m_teacher->getUi()->contract_end->setDate(QDate::currentDate().addYears(5));

    m_teacher->show();

    connect(m_teacher, SIGNAL(submitted()), this, SLOT(onTeacherAdded()));
}

void MainWindow::onTeacherAdded()
{
    m_teacher->hide();
    updateTable();
}

void MainWindow::onVacationAdded()
{
    m_vacations->hide();
    updateTable();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{

}

void MainWindow::on_pushButton_2_clicked()
{
    int row = ui->tableView->selectionModel()->selectedRows().first().row();
    int id = m_employeesModel->index(row, 0).data().toInt();

    m_vacations->setEmployeeId(id);
    m_vacations->show();

    connect(m_vacations, SIGNAL(submitted()), this, SLOT(onVacationAdded()));
}

void MainWindow::on_pushButton_5_clicked()
{
    QString query = ui->searchLine->text();

    if ( query.isEmpty() ) {
        m_employeesModel->setFilter("");
        m_teachersModel->setFilter("");
        m_employeesModel->select();
        return;
    }

    QSqlQuery q;
    q.prepare("SELECT id FROM passports WHERE "
              "(name = :query) OR "
              "(last_name = :query) OR "
              "(father_name = :query);");

    q.bindValue(":query", query);

    cout << q.executedQuery().toStdString() << endl;

    if ( !q.exec() ) {
        cerr << "Error sorting employees: " << q.lastError().text().toStdString() << endl;
    }

    QStringList list;
    QString passId = "";
    while ( q.next() ) {
        passId = q.value(0).toString();
        list << QString(tr("pass_id='%1'")).arg(passId);
    }

    if ( passId.isEmpty() ) {
        QMessageBox::information(
                    this, tr("Запрос не дал результатов"),
                     QString("К сожалению, результаты, удовлетворяющие запросу "
                             "%1 не найдены. Попробуйте изменить запрос.").arg(query));

        return;
    }

    QString filter = list.join(" OR ");
    cout << filter.toStdString() << " filter applied!" << endl;

    m_employeesModel->setFilter(filter);
    m_employeesModel->select();
    m_teachersModel->setFilter(filter);
    m_employeesModel->select();
}

void MainWindow::on_pushButton_4_clicked()
{
    m_reports->show();
}
