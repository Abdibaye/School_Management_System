#ifndef LOGIN_SCREEN_H
#define LOGIN_SCREEN_H

#include <QMainWindow>
#include <manu_screen.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class LoginScreen;
}
QT_END_NAMESPACE

class LoginScreen : public QMainWindow
{
    Q_OBJECT

public:
    LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

private slots:
    void on_loginpushButton_clicked();


private:
    Ui::LoginScreen *ui;
    ManuScreen *manuscreen;
    RegisterScreen *registerscreen;
    payment *Payment;
    Pay *pay;
    Payhistory *history;
    Generate *generate;
    roster *Roster;


    bool initialAutho();
    void initializeDatabase();

};
#endif // LOGIN_SCREEN_H
