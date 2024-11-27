#ifndef CALENDER_H
#define CALENDER_H

#include <QDialog>

namespace Ui {
class Calender;
}

class Calender : public QDialog
{
    Q_OBJECT

public:
    explicit Calender(QWidget *parent = nullptr);
    ~Calender();

private:
    Ui::Calender *ui;
};

#endif // CALENDER_H
