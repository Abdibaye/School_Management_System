#ifndef TIME_TABLE_H
#define TIME_TABLE_H

#include <QDialog>
#include <generate.h>

namespace Ui {
class Timetable;
}

class Timetable : public QDialog
{
    Q_OBJECT

public:
    explicit Timetable(QWidget *parent = nullptr);
    ~Timetable();

private slots:
    void on_generatePushButton_clicked();

private:
    Ui::Timetable *ui;
    Generate *generate;
};

#endif // TIME_TABLE_H
