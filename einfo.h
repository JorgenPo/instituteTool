#ifndef EINFO_H
#define EINFO_H

#include <QWidget>

namespace Ui {
class EInfo;
}

class EInfo : public QWidget
{
    Q_OBJECT

public:
    explicit EInfo(QWidget *parent = 0);
    ~EInfo();

    Ui::EInfo* getUi() { return ui; }
signals:
    void completed();

private slots:
    void on_addButton_clicked();

private:
    Ui::EInfo *ui;
};

#endif // EINFO_H
