#ifndef VACATIONS_H
#define VACATIONS_H

#include <QWidget>

namespace Ui {
class Vacations;
}

class Vacations : public QWidget
{
    Q_OBJECT

public:
    explicit Vacations(QWidget *parent = 0);
    ~Vacations();

    void setEmployeeId(int id);

signals:
    void submitted();

private slots:
    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void on_id_valueChanged(int arg1);

private:
    Ui::Vacations *ui;
};

#endif // VACATIONS_H
