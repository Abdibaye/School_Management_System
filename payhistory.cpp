#include "payhistory.h"
#include "ui_payhistory.h"

Payhistory::Payhistory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Payhistory)
{
    ui->setupUi(this);
    showPaymentHistory();
}

Payhistory::~Payhistory()
{
    delete ui;
}
void Payhistory::setParentPhone(const QString &phone)
{
    parentPhone = phone;
}

void Payhistory::showPaymentHistory() {
    // Clear the existing data in the payTableWidget
    ui->payTableWidget->setRowCount(0); // Clear all rows

    // Prepare SQL query to fetch payment history for the specific parent
    QSqlQuery query;
    query.prepare("SELECT parentPhone, paymentData, paymentDate FROM payments WHERE parentPhone = ? ORDER BY paymentDate DESC");
    query.addBindValue(parentPhone); // Bind the parent phone number

    if (query.exec()) {
        // Set the number of columns and headers for the payTableWidget
        ui->payTableWidget->setColumnCount(3);
        QStringList headers = {"Parent Phone", "Payment Data", "Payment Date"};
        ui->payTableWidget->setHorizontalHeaderLabels(headers);

        // Populate the table with the payment data
        while (query.next()) {
            int rowCount = ui->payTableWidget->rowCount();
            ui->payTableWidget->insertRow(rowCount); // Add a new row

            // Set the values for each column
            ui->payTableWidget->setItem(rowCount, 0, new QTableWidgetItem(query.value(0).toString())); // Parent Phone
            ui->payTableWidget->setItem(rowCount, 1, new QTableWidgetItem(query.value(1).toString())); // Payment Data
            ui->payTableWidget->setItem(rowCount, 2, new QTableWidgetItem(query.value(2).toString())); // Payment Date
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to retrieve payment history.");
    }
}
