#include "pay.h"
#include "ui_pay.h"

Pay::Pay(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Pay)
{
    ui->setupUi(this);

    createPaymentsTable();
}

Pay::~Pay()
{
    delete ui;
}


void Pay::createPaymentsTable() {
    QSqlQuery query;

    // Create the payments table if it doesn't exist
    if (!query.exec("CREATE TABLE IF NOT EXISTS payments ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "parentPhone TEXT NOT NULL, "
                    "paymentData TEXT NOT NULL, "
                    "paymentDate TEXT NOT NULL, "
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
                    "FOREIGN KEY(parentPhone) REFERENCES students(parentNum))")) {
        qDebug() << "Error creating payments table: " << query.lastError().text();
    }
}



void Pay::on_pushButton_clicked() {
    QString parentPhone = ui->parentphonelineEdit->text();
    QString paymentData = ui->paylineEdit->text();;
    QDate paymentDate = ui->payydateEdit->date();

    // Input validation
    if (parentPhone.isEmpty() || paymentData.isEmpty() ) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    // Prepare SQL query to insert payment data
    QSqlQuery query;
    query.prepare("INSERT INTO payments (parentPhone, paymentData, paymentDate) VALUES (?, ?, ?)");
    query.addBindValue(parentPhone);
    query.addBindValue(paymentData);
    query.addBindValue(paymentDate);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Payment data stored successfully!");
        // Optionally, clear the input fields after successful storage
        ui->paylineEdit->clear();
        ui->payydateEdit->clear();
        ui->parentphonelineEdit->clear();
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to store payment data.");
    }
}

