#include "register_screen.h"
#include "ui_registerscreen.h"

RegisterScreen::RegisterScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterScreen)
{
    ui->setupUi(this);


    initializeDatabase(); // Initialize the database when the main window is created
    setWindowTitle("Register");

    // Set the dialog window flags to allow minimize and restore buttons
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    connect(ui->datatableWidget, &QTableWidget::itemClicked, this, &RegisterScreen::onTableItemClicked);



}

RegisterScreen::~RegisterScreen()
{
    delete ui;
}

void RegisterScreen::onTableItemClicked(QTableWidgetItem *item) {
    if (!item) {
        return; // Guard against null item
    }

    int row = item->row(); // Get the index of the clicked row
    if (row < 0 || row >= ui->datatableWidget->rowCount()) {
        return; // Guard against invalid row index
    }

    // Safely set values from the table while checking for validity
    if (ui->datatableWidget->item(row, 0)) {
        ui->FirstnameLIneEdit->setText(ui->datatableWidget->item(row, 0)->text()); // First Name
    }
    if (ui->datatableWidget->item(row, 1)) {
        ui->LastNameLineEdit->setText(ui->datatableWidget->item(row, 1)->text()); // Last Name
    }
    if (ui->datatableWidget->item(row, 2)) {
        ui->LastNameLineEdit->setText(ui->datatableWidget->item(row, 2)->text()); // Semester
    }
    if (ui->datatableWidget->item(row, 3)) {
        ui->parentLineEdit->setText(ui->datatableWidget->item(row, 3)->text()); // E_Science
    }
    if (ui->datatableWidget->item(row, 3)) {
        ui->studentLineEdit->setText(ui->datatableWidget->item(row, 3)->text()); // E_Science
    }
}




void RegisterScreen::on_uploadButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif)");
    if (!filePath.isEmpty()) {
        currentImagePath = filePath; // Store the image path
        QPixmap pixmap(filePath);
        ui->Imagelabel->setPixmap(pixmap.scaled(ui->Imagelabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->Imagelabel->setText(""); // Clear the text if any
    }
}
void RegisterScreen::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("updatestudent.db");

    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << db.lastError().text();
        return; // Exit if database can't be opened
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS students ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "firstName TEXT NOT NULL, "
                    "lastName TEXT NOT NULL, "
                    "studentNum TEXT NOT NULL, "
                    "birthDate TEXT NOT NULL, "
                    "address TEXT NOT NULL, "
                    "parentNum TEXT NOT NULL, "
                    "gender TEXT NOT NULL, "
                    "grade TEXT NOT NULL, "
                    "image BLOB)")) {
        qDebug() << "Error creating students table: " << query.lastError().text();
    }
}


void RegisterScreen::loadStudentData() {
    // Clear the existing data in the table
    ui->datatableWidget->setRowCount(0); // Clear all rows
    ui->datatableWidget->setColumnCount(0); // Clear all columns

    // Set the number of columns (you can adjust this based on your table structure)
    ui->datatableWidget->setColumnCount(9); // Adjust according to your table schema
    QStringList headers = {"First Name", "Last Name", "Gender", "Student Phone", "Birth Date", "Parent Phone", "Address", "Grade", "Photo"};
    ui->datatableWidget->setHorizontalHeaderLabels(headers);

    // Retrieve data from the database
    QSqlQuery query("SELECT firstName, lastName, gender, studentNum, birthDate, parentNum, address, grade, image FROM students");

    while (query.next()) {
        int rowCount = ui->datatableWidget->rowCount();
        ui->datatableWidget->insertRow(rowCount); // Add new row

        // Populate the table with data
        for (int column = 0; column < 9; ++column) {
            if (column < 8) { // For text fields
                ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem(query.value(column).toString()));
            } else { // For image, you might want to handle it differently
                // You may want to display the image in a QLabel or use a placeholder for the image
                // For now, we'll just set a placeholder text
                ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem("Image"));
            }
        }
    }
}

void RegisterScreen::displayInsertedStudent(const QString &studentNum) {
    // Clear the existing data in the table
    ui->datatableWidget->setRowCount(0); // Clear all rows

    // Set the number of columns (adjust according to your table schema)
    ui->datatableWidget->setColumnCount(9); // Adjust according to your table schema
    QStringList headers = {"First Name", "Last Name", "Gender", "Student Phone", "Birth Date", "Parent Phone", "Address", "Grade", "Photo"};
    ui->datatableWidget->setHorizontalHeaderLabels(headers);

    // Retrieve data for the specific student
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName, gender, studentNum, birthDate, parentNum, address, grade, image FROM students WHERE studentNum = ?");
    query.addBindValue(studentNum);

    if (query.exec() && query.next()) {
        int rowCount = ui->datatableWidget->rowCount();
        ui->datatableWidget->insertRow(rowCount); // Add new row

        // Populate the table with the retrieved data
        for (int column = 0; column < 9; ++column) {
            if (column < 8) { // For text fields
                ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem(query.value(column).toString()));
            } else { // For image, you might want to handle it differently
                // You may want to display the image in a QLabel or use a placeholder for the image
                ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem("Image"));
            }
        }
    } else {
        qDebug() << "Failed to retrieve student data: " << query.lastError().text();
    }
}




// Insert student data
bool RegisterScreen::insertStudent(const QString &firstName, const QString &lastName, const QString &studentNum,
                                   const QString &birthDate, const QString &address, const QString &gender,
                                   const QString &grade, const QString &parentNum, const QByteArray &imageData) {
    QSqlQuery query;
    query.prepare("INSERT INTO students (firstName, lastName, studentNum, birthDate, address, gender, grade, parentNum, image) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

    // Bind values
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(studentNum);
    query.addBindValue(birthDate);
    query.addBindValue(address);
    query.addBindValue(gender);
    query.addBindValue(grade);
    query.addBindValue(parentNum);
    query.addBindValue(imageData); // Bind the image data as BLOB

    if (!query.exec()) {
        qDebug() << "Insert failed: " << query.lastError().text();
        return false;
    }
    return true;
}

void RegisterScreen::clear(){
    ui->FirstnameLIneEdit->clear();
    ui->LastNameLineEdit->clear();
    ui->studentLineEdit->clear();
    ui->dateEdit->clear();
    ui->parentLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->gradeLineEdit->clear();

    // Reset radio buttons for gender
    ui->maleradioButton->setChecked(false);
    ui->femaleradioButton->setChecked(false);

    // Clear the table widget
    //ui->datatableWidget->setRowCount(0);  // Remove all rows

    // Optionally, you can also reset the image label
    ui->Imagelabel->setText("No image selected");
}


void RegisterScreen::on_clear_btn_clicked(){
    clear();
}

void RegisterScreen::on_add_btn_clicked() {
    QString firstName = ui->FirstnameLIneEdit->text();
    QString lastName = ui->LastNameLineEdit->text();
    QString studentNum = ui->studentLineEdit->text();
    QString birthDate = ui->dateEdit->text();
    QString parentNum = ui->parentLineEdit->text();
    QString address = ui->addressLineEdit->text();
    QString grade = ui->gradeLineEdit->text();

    QString gender = ui->maleradioButton->isChecked() ? "Male" : (ui->femaleradioButton->isChecked() ? "Female" : "");

    // Basic input validation
    if (firstName.isEmpty() || lastName.isEmpty() || studentNum.isEmpty() || gender.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled out.");
        return;
    }

    // Read the image file and convert it to QByteArray
    QByteArray imageData;
    if (!currentImagePath.isEmpty()) {
        QFile imageFile(currentImagePath);
        if (imageFile.open(QIODevice::ReadOnly)) {
            imageData = imageFile.readAll();
            imageFile.close();
        } else {
            QMessageBox::warning(this, "Error", "Could not open image file.");
            return;
        }
    }

    if (insertStudent(firstName, lastName, studentNum, birthDate, address, gender, grade, parentNum, imageData)) {
        QMessageBox::information(this, "Success", "Student information saved successfully!");
        displayInsertedStudent(studentNum);
    } else {
        QMessageBox::warning(this, "Failure", "Failed to save student information.");
    }

    ui->add_btn->setEnabled(true);
}


void RegisterScreen::on_select_btn_clicked() {
    QString firstName = ui->FirstnameLIneEdit->text();
    QString lastName = ui->LastNameLineEdit->text();

    // Input validation
    if (firstName.isEmpty() || lastName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both First Name and Last Name to select.");
        return;
    }

    // Prepare SQL query to select the student by first and last name
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName, gender, studentNum, birthDate, parentNum, address, grade, image FROM students WHERE firstName = ? AND lastName = ?");
    query.addBindValue(firstName);
    query.addBindValue(lastName);

    if (query.exec() && query.next()) {
        // Clear the existing data in the table
        ui->datatableWidget->setRowCount(0); // Clear all rows

        // Set the number of columns and headers for the table widget
        ui->datatableWidget->setColumnCount(9);
        QStringList headers = {"First Name", "Last Name", "Gender", "Student Phone", "Birth Date", "Parent Phone", "Address", "Grade", "Photo"};
        ui->datatableWidget->setHorizontalHeaderLabels(headers);

        // Add the retrieved data to the table widget
        int rowCount = ui->datatableWidget->rowCount();
        ui->datatableWidget->insertRow(rowCount); // Add new row

        // Populate the table with the student data
        for (int column = 0; column < 9; ++column) {
            if (column < 8) { // For text fields
                ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem(query.value(column).toString()));
            } else { // Placeholder for the image
                ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem("Image")); // Handle image display as needed
            }
        }
    } else {
        QMessageBox::warning(this, "Not Found", "No student found with the provided First Name and Last Name.");
    }
}

void RegisterScreen::on_delete_btn_clicked() {
    // Get the student number from the input field
    QString studentNum = ui->studentLineEdit->text();

    if (studentNum.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a Student Phone number to delete.");
        return;
    }



    // Prepare SQL query for deletion
    QSqlQuery query;
    query.prepare("DELETE FROM students WHERE studentNum = ?");
    query.addBindValue(studentNum);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Student information deleted successfully!");
        on_clear_btn_clicked(); // Clear the form
    } else {
        QMessageBox::warning(this, "Failure", "Failed to delete student information.");
    }
}

void RegisterScreen::on_upd_btn_clicked() {
    // Get the student number from the input field
    QString studentNum = ui->studentLineEdit->text();

    if (studentNum.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a Student Phone number to update.");
        return;
    }

    // Retrieve updated student information from the input fields
    QString firstName = ui->FirstnameLIneEdit->text();
    QString lastName = ui->LastNameLineEdit->text();
    QString birthDate = ui->dateEdit->text();
    QString parentNum = ui->parentLineEdit->text();
    QString address = ui->addressLineEdit->text();
    QString grade = ui->gradeLineEdit->text();

    QString gender = ui->maleradioButton->isChecked() ? "Male" : (ui->femaleradioButton->isChecked() ? "Female" : "");

    // Prepare SQL query for updating
    QSqlQuery query;
    query.prepare("UPDATE students SET firstName = ?, lastName = ?, gender = ?, birthDate = ?, parentNum = ?, address = ?, grade = ? WHERE studentNum = ?");
    query.addBindValue(firstName);
    query.addBindValue(lastName);
    query.addBindValue(gender);
    query.addBindValue(birthDate);
    query.addBindValue(parentNum);
    query.addBindValue(address);
    query.addBindValue(grade);
    query.addBindValue(studentNum);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Student information updated successfully!");
        displayInsertedStudent(studentNum); // Refresh the table with updated data
    } else {
        QMessageBox::warning(this, "Failure", "Failed to update student information.");
    }
}

void RegisterScreen::on_search_btn_clicked() {
    QString firstName = ui->FirstnameLIneEdit->text();
    QString lastName = ui->LastNameLineEdit->text();

    // Prepare SQL query to search for students
    QSqlQuery query;
    query.prepare("SELECT firstName, lastName, gender, studentNum, birthDate, parentNum, address, grade, image FROM students WHERE firstName LIKE ? AND lastName LIKE ?");
    query.addBindValue(firstName.isEmpty() ? "%" : "%" + firstName + "%");
    query.addBindValue(lastName.isEmpty() ? "%" : "%" + lastName + "%");

    if (query.exec()) {
        // Clear the existing data in the table
        ui->datatableWidget->setRowCount(0); // Clear all rows

        // Set the number of columns and headers for the table widget
        ui->datatableWidget->setColumnCount(9);
        QStringList headers = {"First Name", "Last Name", "Gender", "Student Phone", "Birth Date", "Parent Phone", "Address", "Grade", "Photo"};
        ui->datatableWidget->setHorizontalHeaderLabels(headers);

        // Check if any records were found
        int rowCount = 0;
        while (query.next()) {
            ui->datatableWidget->insertRow(rowCount); // Add new row

            // Populate the table with the student data
            for (int column = 0; column < 9; ++column) {
                if (column < 8) { // For text fields
                    ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem(query.value(column).toString()));
                } else { // Placeholder for the image
                    ui->datatableWidget->setItem(rowCount, column, new QTableWidgetItem("Image")); // Handle image display as needed
                }
            }
            rowCount++;
        }

        if (rowCount == 0) {
            QMessageBox::information(this, "No Results", "No students found matching the search criteria.");
        }
    } else {
        QMessageBox::warning(this, "Query Error", "Failed to execute search query.");
    }
}
