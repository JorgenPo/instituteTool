#ifndef TEACHER_H
#define TEACHER_H

#include <QWidget>
#include <ui_teacher.h>

namespace Ui {
class Teacher;
}

class Teacher : public QWidget
{
    Q_OBJECT

public:
    explicit Teacher(QWidget *parent = 0);
    ~Teacher();

    Ui::Teacher* getUi() { return ui; }

signals:
    void submitted();

private slots:
    void on_addButton_clicked();

    void on_cancelButton_clicked();

private:
    void submitTeacher();

private:
    Ui::Teacher *ui;
};

#endif // TEACHER_H
