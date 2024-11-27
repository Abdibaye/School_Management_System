#include "time_table.h"
#include "ui_timetable.h"

Timetable::Timetable(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Timetable)
{
    ui->setupUi(this);
}

Timetable::~Timetable()
{
    delete ui;
}

void Timetable::on_generatePushButton_clicked()
{
    generate = new Generate(this);
    generate->show();
}

