#ifndef MANU_SCREEN_H
#define MANU_SCREEN_H

#include <QDialog>
#include <register_screen.h>
#include <dash_board.h>
#include <calender.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <payment.h>
#include <pay.h>
#include <payhistory.h>
#include <generate.h>
#include <time_table.h>
#include <roster.h>


namespace Ui {
class ManuScreen;
}

class ManuScreen : public QDialog
{
    Q_OBJECT

public:
    explicit ManuScreen(QWidget *parent = nullptr);
    ~ManuScreen();

private slots:
    void on_registeration_btn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_payment_btn_clicked();

    void on_timeTablepushButton_clicked();


    void on_rosterpushButton_clicked();

private:
    Ui::ManuScreen *ui;
    RegisterScreen *registerscreen;
    Dashboard *dashboard;
    Calender *calender;
    Timetable *timetable;
    payment *dpayment;
    Generate *generate;
    roster *droster;


    //Generate *generate;



};

#endif // MANU_SCREEN_H
