#include "calender.h"
#include "ui_calender.h"

Calender::Calender(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Calender)
{
    ui->setupUi(this);
}

Calender::~Calender()
{
    delete ui;
}
