#ifndef TEXTREPORTS_H
#define TEXTREPORTS_H

#include <QWidget>

namespace Ui {
class TextReports;
}

class TextReports : public QWidget
{
    Q_OBJECT

public:
    explicit TextReports(QWidget *parent = 0);
    ~TextReports();

private slots:
    void on_report1_clicked();

private:
    Ui::TextReports *ui;
};

#endif // TEXTREPORTS_H
