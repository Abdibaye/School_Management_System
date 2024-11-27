#include "login_screen.h"
#include "ui_loginscreen.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

LoginScreen::LoginScreen(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginScreen)
{
    ui->setupUi(this);

    //connect(ui->loginpushButton, &QPushButton::clicked, this, &LoginScreen::on_loginpushButton_clicked);

}

LoginScreen::~LoginScreen()
{
    delete ui;
}


void LoginScreen::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "Database error occurred: " << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS user ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT NULL, "
               "password TEXT NULL)");

}

bool LoginScreen::initialAutho(){

    QString username = "admin";
    QString password = "admin";


    QSqlQuery query;
    query.prepare("INSERT INTO user (firstName, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Insert failed: " << query.lastError().text();
        return false;
    } else {
        return true;
    }
}


void LoginScreen::on_loginpushButton_clicked()
{

    //QString username = ui->usernameLineEdit->text();
    //QString password = ui->passwordLineEdit->text();

    registerscreen = new RegisterScreen(this);
    Payment = new payment(this);
    pay = new Pay(this);
    history = new Payhistory(this);
    generate = new Generate(this);
    manuscreen = new ManuScreen(this);
    hide();
    Roster = new roster(this);
    manuscreen->show();


/*    initializeDatabase();
    initialAutho();
    QSqlQuery query;
    query.prepare("SELECT * FROM user WHERE username AND password");
    query.addBindValue(username);
    query.addBindValue(password);
    // query.bindValue(":username", username);
    // query.bindValue(":password", password)*/;



    // query.exec();
    // if (query.next()) {
    //     manuscreen = new ManuScreen(this);
    //     this->close();
    //     manuscreen->show();
    // } else {
    //     QMessageBox::warning(this, "Error", "Invalid username or password.");
    // }

    // if(username == "admin" && password == "admin"){
    //         manuscreen = new ManuScreen(this);
    //         this->close();
    //         manuscreen->show();
    // }else {
    //         QMessageBox::warning(this, "Error", "Invalid username or password.");
    //     }

}

