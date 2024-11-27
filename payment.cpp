#include "payment.h"
#include "ui_payment.h"


payment::payment(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::payment)
{
    ui->setupUi(this);

     // Initialize the database when the main window is created
    setWindowTitle("Register");

    // Set the dialog window flags to allow minimize and restore buttons
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
}

payment::~payment()
{
    delete ui;
}




void payment::on_paysearch_btn_clicked()
{
        QString firstName = ui->payfirstnameLineEdit->text();
        QString lastName = ui->paylastnameLinedEdit->text();
        QString parentPhone = ui->payparentNumLineEdit->text();

        // Input validation
        if (firstName.isEmpty() || lastName.isEmpty() || parentPhone.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
            return;
        }

        // Prepare the SQL query to search for the student
        QSqlQuery query;
        query.prepare("SELECT firstName, lastName, gender, studentNum, birthDate, parentNum, address, grade FROM students WHERE firstName = ? AND lastName = ? AND parentNum = ?");
        query.addBindValue(firstName);
        query.addBindValue(lastName);
        query.addBindValue(parentPhone);

        if (query.exec() && query.next()) {
            // Clear existing data in the payment table
            ui->paymentTableWidget->setRowCount(0); // Clear all rows

            // Set the number of columns and headers for the payment table widget
            ui->paymentTableWidget->setColumnCount(9); // One additional column for options
            QStringList headers = {"First Name", "Last Name", "Gender", "Student Phone", "Birth Date", "Parent Phone", "Address", "Grade", "Option"};
            ui->paymentTableWidget->setHorizontalHeaderLabels(headers);

            // Insert a new row and populate it with the student data
            int rowCount = ui->paymentTableWidget->rowCount();
            ui->paymentTableWidget->insertRow(rowCount); // Add new row

            for (int column = 0; column < 8; ++column) {
                ui->paymentTableWidget->setItem(rowCount, column, new QTableWidgetItem(query.value(column).toString()));
            }

            // Create a combo box and a button for the "Option" column
            QComboBox *comboBox = new QComboBox();
            comboBox->addItem("Select Option");
            comboBox->addItem("PaidHistory");
            comboBox->addItem("PayNow");

            //QPushButton *payButton = new QPushButton("Pay");

            // Create a layout for the combo box and button
            QWidget *widget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(widget);
            layout->addWidget(comboBox);
           // layout->addWidget(payButton);
            layout->setSpacing(0);
            layout->setContentsMargins(0, 0, 0, 0);
            widget->setLayout(layout);

            // Add the widget to the table
            ui->paymentTableWidget->setCellWidget(rowCount, 8, widget);

            // Connect combo box selection signal
            connect(comboBox, &QComboBox::currentIndexChanged, this, [this, parentPhone](int index) {
                if (index == 1) { // "PayHistory" selected
                    Payhistory *payhistory = new Payhistory(this);
                    payhistory->setParentPhone(parentPhone); // Pass the parent phone number
                    payhistory->showPaymentHistory(); // Show the payment history
                    payhistory->exec(); // Show history dialog as modal
                } else if (index == 2) { // "Pay" selected
                    Pay *pay = new Pay(this);
                    pay->exec(); // Show pay dialog as modal
                }
            });
        }

}
void payment::on_paidpushButton_clicked()
{
    // Get the selected date from the QDateEdit widget
    QDate selectedDate = ui->paiddateEdit->date();
    QString month = QString::number(selectedDate.month()).rightJustified(2, '0'); // Format to two digits
    QString year = QString::number(selectedDate.year());

    // Clear the existing data in the table widget
    ui->paymentTableWidget->setRowCount(0);

    // Prepare SQL query to fetch all paid users for the selected month and year
    QSqlQuery query;
    query.prepare("SELECT s.parentNum, s.firstName || ' ' || s.lastName AS studentName, "
                  "p.paymentData, p.paymentDate "
                  "FROM students s "
                  "JOIN payments p ON s.parentNum = p.parentPhone "
                  "WHERE strftime('%Y', p.paymentDate) = ? AND strftime('%m', p.paymentDate) = ? "
                  "ORDER BY p.paymentDate DESC");

    query.addBindValue(year);
    query.addBindValue(month);

    if (query.exec()) {
        // Check if any results were returned
        if (query.size() == 0) {
            QMessageBox::information(this, "No Data", "No paid users found for the selected month and year.");
            return;
        }

        // Set the number of columns and headers for the payment table widget
        ui->paymentTableWidget->setColumnCount(4);
        QStringList headers = {"Parent Phone", "Student Name", "Payment Data", "Payment Date"};
        ui->paymentTableWidget->setHorizontalHeaderLabels(headers);

        // Populate the table with all paid users
        while (query.next()) {
            int rowCount = ui->paymentTableWidget->rowCount();
            ui->paymentTableWidget->insertRow(rowCount);

            ui->paymentTableWidget->setItem(rowCount, 0, new QTableWidgetItem(query.value(0).toString())); // Parent Phone
            ui->paymentTableWidget->setItem(rowCount, 1, new QTableWidgetItem(query.value(1).toString())); // Student Name
            ui->paymentTableWidget->setItem(rowCount, 2, new QTableWidgetItem(query.value(2).toString())); // Payment Data
            ui->paymentTableWidget->setItem(rowCount, 3, new QTableWidgetItem(query.value(3).toString())); // Payment Date
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to retrieve paid users: " + query.lastError().text());
        qDebug() << "SQL Query:" << query.lastQuery();
    }
}

void payment::on_unpaidpushButton_clicked()
{
    // Get the selected date from the QDateEdit widget
    QDate selectedDate = ui->paiddateEdit->date();
    QString month = QString::number(selectedDate.month()).rightJustified(2, '0'); // Format to two digits
    QString year = QString::number(selectedDate.year());

    // Clear the existing data in the table widget
    ui->paymentTableWidget->setRowCount(0);

    // Prepare SQL query to fetch all unpaid students for the selected month and year
    QSqlQuery query;
    query.prepare("SELECT s.parentNum, s.firstName || ' ' || s.lastName AS studentName "
                  "FROM students s "
                  "LEFT JOIN payments p ON s.parentNum = p.parentPhone AND strftime('%Y', p.paymentDate) = ? AND strftime('%m', p.paymentDate) = ? "
                  "WHERE p.id IS NULL");

    query.addBindValue(year);
    query.addBindValue(month);

    if (query.exec()) {
        // Check if any results were returned
        if (query.size() == 0) {
            QMessageBox::information(this, "No Data", "No unpaid users found for the selected month and year.");
            return;
        }

        // Set the number of columns and headers for the payment table widget
        ui->paymentTableWidget->setColumnCount(3);
        QStringList headers = {"Parent Phone", "Student Name", "Status"};
        ui->paymentTableWidget->setHorizontalHeaderLabels(headers);

        // Populate the table with all students marked as unpaid
        while (query.next()) {
            int rowCount = ui->paymentTableWidget->rowCount();
            ui->paymentTableWidget->insertRow(rowCount);

            // Populate student details
            ui->paymentTableWidget->setItem(rowCount, 0, new QTableWidgetItem(query.value(0).toString())); // Parent Phone
            ui->paymentTableWidget->setItem(rowCount, 1, new QTableWidgetItem(query.value(1).toString())); // Student Name

            // Add the status column
            QTableWidgetItem *statusItem = new QTableWidgetItem("Unpaid");
            statusItem->setForeground(QBrush(Qt::red)); // Set text color to red
            ui->paymentTableWidget->setItem(rowCount, 2, statusItem);
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to retrieve students: " + query.lastError().text());
        qDebug() << "SQL Query:" << query.lastQuery();
    }
}

void payment::on_tpaidpushButton_clicked() {
    // Get the selected date from the QDateEdit widget
    QDate selectedDate = ui->paiddateEdit->date();
    QString month = QString::number(selectedDate.month()).rightJustified(2, '0'); // Format to two digits
    QString year = QString::number(selectedDate.year());

    // Prepare SQL query to count the number of paid students for the selected month and year
    QSqlQuery query;
    query.prepare("SELECT COUNT(DISTINCT parentPhone) FROM payments "
                  "WHERE strftime('%Y', paymentDate) = ? AND strftime('%m', paymentDate) = ?");

    query.addBindValue(year);
    query.addBindValue(month);

    if (query.exec()) {
        if (query.next()) {
            int totalPaidStudents = query.value(0).toInt(); // Get the count from the query result
            ui->totalPaidLabel->setText(QString("%1").arg(totalPaidStudents));
        } else {
            ui->totalPaidLabel->setText("0");
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to retrieve the total number of paid students: " + query.lastError().text());
        qDebug() << "SQL Query Error:" << query.lastError().text();
        qDebug() << "SQL Query:" << query.lastQuery();
    }
}

void payment::on_tunpaidpushButton_clicked() {
    // Get the selected date from the QDateEdit widget
    QDate selectedDate = ui->paiddateEdit->date();
    QString month = QString::number(selectedDate.month()).rightJustified(2, '0'); // Format to two digits
    QString year = QString::number(selectedDate.year());

    // Prepare SQL query to count the number of unpaid students for the selected month and year
    QSqlQuery query;
    query.prepare("SELECT COUNT(DISTINCT s.parentNum) FROM students s "
                  "LEFT JOIN payments p ON s.parentNum = p.parentPhone "
                  "AND strftime('%Y', p.paymentDate) = ? AND strftime('%m', p.paymentDate) = ? "
                  "WHERE p.id IS NULL");

    query.addBindValue(year);
    query.addBindValue(month);

    if (query.exec()) {
        if (query.next()) {
            int totalUnpaidStudents = query.value(0).toInt(); // Get the count from the query result
            ui->totalunPaidLabel->setText(QString("%1").arg(totalUnpaidStudents)); // Ensure you have a QLabel for total unpaid
        } else {
            ui->totalunPaidLabel->setText("0");
        }
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to retrieve the total number of unpaid students: " + query.lastError().text());
        qDebug() << "SQL Query Error:" << query.lastError().text();
        qDebug() << "SQL Query:" << query.lastQuery();
    }
}
