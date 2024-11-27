#include "manu_screen.h"
#include "ui_manuscreen.h"



ManuScreen::ManuScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManuScreen)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // QVBoxLayout *layout = new QVBoxLayout(this);
    // QPushButton *button = new QPushButton("Close Dialog", this);

    // layout->addWidget(button);
    // connect(button, &QPushButton::clicked, this, &QDialog::accept);
}

ManuScreen::~ManuScreen()
{
    delete ui;
}

void ManuScreen::on_registeration_btn_clicked()
{
    registerscreen = new RegisterScreen(this);
    registerscreen->show();
}


void ManuScreen::on_pushButton_clicked()
{
    dashboard = new Dashboard(this);
    dashboard->show();
}


void ManuScreen::on_pushButton_3_clicked()
{
    calender = new Calender(this);
    calender->show();
}




void ManuScreen::on_pushButton_2_clicked()
{
    hide();
}



void ManuScreen::on_payment_btn_clicked()
{
    dpayment = new payment(this);
    dpayment->show();

}


void ManuScreen::on_timeTablepushButton_clicked()
{
    generate = new Generate(this);
    generate->show();
}





void ManuScreen::on_rosterpushButton_clicked()
{
    droster = new roster(this);
    droster->show();
}

