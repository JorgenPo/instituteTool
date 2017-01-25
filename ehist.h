#ifndef EHIST_H
#define EHIST_H

#include <QWidget>

namespace Ui {
class Ehist;
}

class Ehist : public QWidget
{
    Q_OBJECT

public:
    explicit Ehist(QWidget *parent = 0);
    ~Ehist();

    Ui::Ehist* getUi() { return ui; }

signals:
    void completed();

private slots:
    void on_nextButton_clicked();

private:
    Ui::Ehist *ui;
};

#endif // EHIST_H
