#include "dash_board.h"
#include "ui_dashboard.h"
#include <register_screen.h>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>




Dashboard::Dashboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    //initializeDatabase();
    setWindowTitle("Register");

    // Set the dialog window flags to allow minimize and restore buttons
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
    loadStudents();


    //connect(ui->totalStud_btn, &QPushButton::clicked, this, &Dashboard::totalStudent);

}

Dashboard::~Dashboard()
{
    delete ui;
}

// void Dashboard::initializeDatabase() {
//     QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     db.setDatabaseName("updatestudent.db");

//     if (!db.open()) {
//         qDebug() << "Error: Unable to open database" << db.lastError().text();
//         return; // Exit if database can't be opened
//     }

//     QSqlQuery query;
//     if (!query.exec("CREATE TABLE IF NOT EXISTS students ("
//                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                     "firstName TEXT NOT NULL, "
//                     "lastName TEXT NOT NULL, "
//                     "studentNum TEXT NOT NULL, "
//                     "birthDate TEXT NOT NULL, "
//                     "address TEXT NOT NULL, "
//                     "parentNum TEXT NOT NULL, "
//                     "gender TEXT NOT NULL, "
//                     "grade TEXT NOT NULL, "
//                     "image BLOB)")) {
//         qDebug() << "Error creating students table: " << query.lastError().text();
//     }
// }


void Dashboard::loadStudents() {
    ui->tableWidget->setRowCount(0); // Clear existing rows

    QSqlQuery query("SELECT firstName, lastName, gender,studentNum, birthDate, address, grade,parentNum FROM students");
    while (query.next()) {
        int row = ui->tableWidget->rowCount(); // Get the current number of rows
        ui->tableWidget->insertRow(row); // Insert a new row

        // Populate each cell with data
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // firstName
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // lastName
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // gender
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // studentNum
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // birthDate
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // address
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value(6).toString())); // grade
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(query.value(7).toString())); // parentNum
    }
}


int Dashboard::countTotalStudents() {
    QSqlQuery query("SELECT COUNT(*) FROM students");

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return 0; // Return 0 if the query fails
    }

    query.next();
    return query.value(0).toInt(); // Return the count
}




void Dashboard::on_totalStudent_btn_clicked()
{
    int totalStudents = countTotalStudents(); // Count total students

    // Update the QLabel with the total number of students

    ui->totalLabel->setText(QString("%1").arg(totalStudents));
}


// Function to export data to a CSV file
void Dashboard::exportDataToCSV(const QString &filePath) {
    QSqlQuery query("SELECT firstName, lastName, birthDate, address, studentNum, gender, grade, image FROM students");

    // Check if the query executed successfully
    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        QMessageBox::warning(this, "Error", "Failed to retrieve data from database.");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "File open failed: " << file.errorString();
        QMessageBox::warning(this, "Error", "Failed to open file for writing.");
        return;
    }

    QTextStream out(&file);
    // Write the header
    out << "First Name,Last Name,Birth Date,Address,Phone Number,Gender,Grade,Image Path\n";

    // Prepare a directory to save images
    QString imageDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/ExportedImages/";
    QDir().mkdir(imageDir); // Create directory if it doesn't exist

    // Write the data
    while (query.next()) {
        QString firstName = query.value(0).toString();
        QString lastName = query.value(1).toString();
        QString birthDate = query.value(2).toString();
        QString address = query.value(3).toString();
        QString studentNum = query.value(4).toString();
        QString gender = query.value(5).toString();
        QString grade = query.value(6).toString();

        // Handle image export
        QByteArray imageData = query.value(7).toByteArray();
        QString imagePath = "";

        if (!imageData.isEmpty()) {
            QString imageFileName = QString("%1_%2.png").arg(firstName, lastName);
            QFile imageFile(imageDir + imageFileName);
            if (imageFile.open(QIODevice::WriteOnly)) {
                imageFile.write(imageData);
                imageFile.close();
                imagePath = imageFile.fileName(); // Store the path for CSV
            } else {
                qDebug() << "Failed to save image: " << imageFile.errorString();
            }
        }

        // Write to CSV file
        out << firstName << "," << lastName << "," << birthDate << ","
            << address << "," << studentNum << "," << gender << ","
            << grade << "," << imagePath << "\n"; // Image path
    }

    file.close();
    QMessageBox::information(this, "Success", "Data exported successfully to " + filePath);
}

void Dashboard::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "CSV Files (*.csv)");

    if (!filePath.isEmpty()) {
        exportDataToCSV(filePath);
    }
}


void Dashboard::on_load_btn_clicked()
{
    QString gender = ui->maleradioButton->isChecked() ? "Male" : "Female";
    int grade = ui->spinBox->value();

    // Clear existing data
    ui->tableWidget->setRowCount(0);

    // Prepare SQL query
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName, gender, studentNum, birthDate, address FROM students WHERE gender = :gender AND grade = :grade");
    query.bindValue(":gender", gender);
    query.bindValue(":grade", grade);

    // Execute query
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    // Populate the table with data
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // First Name
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Last Name
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // Gender
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // Phone
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString())); // Birth Date
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // Grade
    }

}

