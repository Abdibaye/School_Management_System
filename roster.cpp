#include "roster.h"
#include "ui_roster.h"

roster::roster(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::roster)
{
    ui->setupUi(this);

    setWindowTitle("Roster");

    // Set the dialog window flags to allow minimize and restore buttons
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

    initializeDatabase();

    // After ui->setupUi(this);
    ui->showtatallabel->setStyleSheet("font-weight: bold; font-size: 16px; color: blue; padding: 5px; border: 1px solid #cccccc; border-radius: 5px; background-color: #f0f8ff;");
    ui->showaveragelabel->setStyleSheet("font-weight: bold; font-size: 16px; color: green; padding: 5px; border: 1px solid #cccccc; border-radius: 5px; background-color: #f0fff0;");

    // Optionally, you can set fonts
    QFont font;
    font.setPointSize(16);
    font.setBold(true);
    ui->showtatallabel->setFont(font);
    ui->showaveragelabel->setFont(font);

    connect(ui->rosterDatatableWidget, &QTableWidget::itemClicked, this, &roster::onTableItemClicked);

    ui->firstnamelineEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; color: #000000; border: 1px solid #cccccc; padding: 5px; }");
    ui->lastnamelineEdit->setStyleSheet("QLineEdit { background-color: #f0f0f0; color: #000000; border: 1px solid #cccccc; padding: 5px; }");
}

roster::~roster()
{
    delete ui;
}



void roster::initializeDatabase() {
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS rosters ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "student_id INTEGER NOT NULL, "
                    "semester INTEGER NOT NULL, "
                    "escience REAL, "
                    "amharic REAL, "
                    "herrega REAL, "
                    "english REAL, "
                    "spoken REAL, "
                    "oromic REAL, "
                    "safu REAL, "
                    "gscience REAL, "
                    "ogarti REAL, "
                    "maths REAL, "
                    "gada REAL, "
                    "ict REAL, "
                    "fjq REAL, "
                    "total REAL, "
                    "average REAL, "
                    "FOREIGN KEY (student_id) REFERENCES students (id))")) {
        qDebug() << "Error creating rosters table: " << query.lastError().text();
    }
}

void roster::loadGradeData() {
    ui->rosterDatatableWidget->setRowCount(0);
    ui->rosterDatatableWidget->setColumnCount(17); // Adjusted to 17 for total and average
    QStringList headers = {
        "Student ID", "Semester", "E_Science", "Amharic", "Herrega", "English",
        "Spoken", "Oromic", "Safu", "G_Science", "OG-Aartii", "Maths",
        "Gada", "ICT", "FJQ", "Total", "Average"
    };
    ui->rosterDatatableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery query("SELECT student_id, semester, escience, amharic, herrega, english, "
                    "spoken, oromic, safu, gscience, ogarti, maths, gada, ict, fjq, "
                    "total, average FROM rosters");

    while (query.next()) {
        int rowCount = ui->rosterDatatableWidget->rowCount();
        ui->rosterDatatableWidget->insertRow(rowCount);

        for (int column = 0; column < 17; ++column) {
            ui->rosterDatatableWidget->setItem(rowCount, column, new QTableWidgetItem(query.value(column).toString()));
        }
    }
}

bool roster::insertGrades(const QVariantMap &grades) {
    QSqlQuery query;
    query.prepare("INSERT INTO rosters (student_id, semester, escience, amharic, herrega, english, "
                  "spoken, oromic, safu, gscience, ogarti, maths, gada, ict, fjq, total, average) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(grades.value("student_id"));
    query.addBindValue(grades.value("semester"));
    query.addBindValue(grades.value("escience"));
    query.addBindValue(grades.value("amharic"));
    query.addBindValue(grades.value("herrega"));
    query.addBindValue(grades.value("english"));
    query.addBindValue(grades.value("spoken"));
    query.addBindValue(grades.value("oromic"));
    query.addBindValue(grades.value("safu"));
    query.addBindValue(grades.value("gscience"));
    query.addBindValue(grades.value("ogarti"));
    query.addBindValue(grades.value("maths"));
    query.addBindValue(grades.value("gada"));
    query.addBindValue(grades.value("ict"));
    query.addBindValue(grades.value("fjq"));
    query.addBindValue(grades.value("total"));  // Include total
    query.addBindValue(grades.value("average")); // Include average

    if (!query.exec()) {
        qDebug() << "Insert failed: " << query.lastError().text();
        return false;
    }
    return true;
}

// void roster::displayInsertedGrades(int studentId) {
//     loadGradeData(); // Refresh the table after inserting
// }

void roster::clear() {
    // Clear all input fields in the UI (assuming you have appropriate input fields)
    ui->showStudenIDlineEdit_1->clear();
    ui->gradelineEdit_1->clear();
    // Add similar lines for all other grade fields
}

// void roster::on_addButton_clicked() {

// }


void roster::on_refreshPushButton_clicked(int studentID)
{
    loadGradeData();
}


void roster::on_savepushButton_clicked()
{
    QVariantMap grades;
    grades["student_id"] = ui->showStudenIDlineEdit_2->text().toInt(); // Assuming you have an input for student ID
    grades["semester"] = ui->semesterlineEdit->text().toInt(); // Assuming you have an input for semester

    // Check if semester is valid (only 1 or 2 allowed)
    int semester = grades["semester"].toInt();
    if (semester < 1 || semester > 2) {
        QMessageBox::warning(this, "Invalid Semester", "Please enter a valid semester (1 or 2).");
        return; // Exit the function if the semester is invalid
    }

    grades["escience"] = ui->esciencelineEdit->text().toDouble();
    grades["amharic"] = ui->amhariclineEdit->text().toDouble();
    grades["herrega"] = ui->herregalineEdit->text().toDouble();
    grades["english"] = ui->engilishlineEdit->text().toDouble();
    grades["spoken"] = ui->spokenlineEdit->text().toDouble();
    grades["oromic"] = ui->oromiclineEdit->text().toDouble();
    grades["safu"] = ui->safulineEdit->text().toDouble();
    grades["gscience"] = ui->gsciencelineEdit->text().toDouble();
    grades["ogarti"] = ui->ogartilineEdit->text().toDouble();
    grades["maths"] = ui->mathslineEdit->text().toDouble();
    grades["gada"] = ui->gadalineEdit->text().toDouble();
    grades["ict"] = ui->ictlineEdit->text().toDouble();
    grades["fjq"] = ui->fjqlineEdit->text().toDouble();

    // Calculate total
    double total = grades["escience"].toDouble() + grades["amharic"].toDouble() +
                   grades["herrega"].toDouble() + grades["english"].toDouble() +
                   grades["spoken"].toDouble() + grades["oromic"].toDouble() +
                   grades["safu"].toDouble() + grades["gscience"].toDouble() +
                   grades["ogarti"].toDouble() + grades["maths"].toDouble() +
                   grades["gada"].toDouble() + grades["ict"].toDouble() +
                   grades["fjq"].toDouble();

    // Calculate average
    double average = total / 13; // Assuming there are 13 subjects
    QString formattedAverage = QString::number(average, 'f', 2);

    // Set average to the corresponding line edit
    grades["total"] = total;           // Keep total as is
    grades["average"] = formattedAverage.toDouble();

    // Check if the information already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM rosters WHERE student_id = :studentId AND semester = :semester");
    checkQuery.bindValue(":studentId", grades["student_id"]);
    checkQuery.bindValue(":semester", grades["semester"]);

    if (checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "Duplicate Entry", "The grades for this student and semester already exist.");
            return; // Exit the function if a duplicate entry is found
        }
    }

    // Insert the new grades into the database
    if (insertGrades(grades)) {
        QMessageBox::information(this, "Success", "Grades saved successfully!");
    } else {
        QMessageBox::warning(this, "Failure", "Failed to save grades.");
    }
}

void roster::on_searchpushButton_2_clicked()
{
    QString studentId = ui->showStudenIDlineEdit_1->text(); // Assuming you have a line edit for student ID
    QString grade = ui->gradelineEdit_1->text(); // Assuming you have a line edit for grade

    // Input validation
    if (studentId.isEmpty() || grade.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter both Student ID and Grade.");
        return;
    }

    // Prepare SQL query to retrieve student's grades and personal information
    QSqlQuery query;
    query.prepare(R"(
        SELECT s.firstName, s.lastName, r.semester, r.escience, r.amharic, r.herrega, r.english,
               r.spoken, r.oromic, r.safu, r.gscience, r.ogarti, r.maths, r.gada,
               r.ict, r.fjq, r.total, r.average
        FROM students s
        JOIN rosters r ON s.id = r.student_id
        WHERE r.student_id = :studentId AND s.grade = :grade
    )");

    // Bind values ensuring studentId is converted to int
    query.bindValue(":studentId", studentId.toInt());
    query.bindValue(":grade", grade);

    // Debug: Check the query execution
    if (!query.exec()) {
        qDebug() << "Query execution error:" << query.lastError().text();
        return;
    }

    // Check if any results are returned
    if (query.next()) {
        // Clear the existing data in the table
        ui->rosterDatatableWidget->setRowCount(0);

        // Set the number of columns
        ui->rosterDatatableWidget->setColumnCount(18);
        QStringList headers = {
            "First Name", "Last Name", "Semester", "E_Science", "Amharic", "Herrega", "English",
            "Spoken", "Oromic", "Safu", "G_Science", "OG-Aartii", "Maths",
            "Gada", "ICT", "FJQ", "Total", "Average"
        };
        ui->rosterDatatableWidget->setHorizontalHeaderLabels(headers);

        // Add found data to the table
        int rowCount = ui->rosterDatatableWidget->rowCount();
        ui->rosterDatatableWidget->insertRow(rowCount);

        // Populate the table with the retrieved data
        for (int i = 0; i < headers.size(); ++i) {
            QString value = query.value(i).toString();
            ui->rosterDatatableWidget->setItem(rowCount, i, new QTableWidgetItem(value));
            // Debug: Print each value for verification
            qDebug() << headers[i] << ":" << value;
        }
    } else {
        // No results found
        QMessageBox::warning(this, "Not Found", "No grades found for the provided Student ID.");
    }
}

void roster::on_clearpushButton_clicked()
{
    clear();
}

void roster::onTableItemClicked(QTableWidgetItem *item) {
    if (!item) {
        return; // Guard against null item
    }

    int row = item->row(); // Get the index of the clicked row
    if (row < 0 || row >= ui->rosterDatatableWidget->rowCount()) {
        return; // Guard against invalid row index
    }

    // Safely set values from the table while checking for validity
    if (ui->rosterDatatableWidget->item(row, 0)) {
        ui->firstnamelineEdit->setText(ui->rosterDatatableWidget->item(row, 0)->text()); // First Name
    }
    if (ui->rosterDatatableWidget->item(row, 1)) {
        ui->lastnamelineEdit->setText(ui->rosterDatatableWidget->item(row, 1)->text()); // Last Name
    }
    if (ui->rosterDatatableWidget->item(row, 2)) {
        ui->semesterlineEdit->setText(ui->rosterDatatableWidget->item(row, 2)->text()); // Semester
    }
    if (ui->rosterDatatableWidget->item(row, 3)) {
        ui->esciencelineEdit->setText(ui->rosterDatatableWidget->item(row, 3)->text()); // E_Science
    }
    if (ui->rosterDatatableWidget->item(row, 4)) {
        ui->amhariclineEdit->setText(ui->rosterDatatableWidget->item(row, 4)->text()); // Amharic
    }
    if (ui->rosterDatatableWidget->item(row, 5)) {
        ui->herregalineEdit->setText(ui->rosterDatatableWidget->item(row, 5)->text()); // Herrega
    }
    if (ui->rosterDatatableWidget->item(row, 6)) {
        ui->engilishlineEdit->setText(ui->rosterDatatableWidget->item(row, 6)->text()); // English
    }
    if (ui->rosterDatatableWidget->item(row, 7)) {
        ui->spokenlineEdit->setText(ui->rosterDatatableWidget->item(row, 7)->text()); // Spoken
    }
    if (ui->rosterDatatableWidget->item(row, 8)) {
        ui->oromiclineEdit->setText(ui->rosterDatatableWidget->item(row, 8)->text()); // Oromic
    }
    if (ui->rosterDatatableWidget->item(row, 9)) {
        ui->safulineEdit->setText(ui->rosterDatatableWidget->item(row, 9)->text()); // Safu
    }
    if (ui->rosterDatatableWidget->item(row, 10)) {
        ui->gsciencelineEdit->setText(ui->rosterDatatableWidget->item(row, 10)->text()); // G_Science
    }
    if (ui->rosterDatatableWidget->item(row, 11)) {
        ui->ogartilineEdit->setText(ui->rosterDatatableWidget->item(row, 11)->text()); // OG_Aartii
    }
    if (ui->rosterDatatableWidget->item(row, 12)) {
        ui->mathslineEdit->setText(ui->rosterDatatableWidget->item(row, 12)->text()); // Maths
    }
    if (ui->rosterDatatableWidget->item(row, 13)) {
        ui->gadalineEdit->setText(ui->rosterDatatableWidget->item(row, 13)->text()); // Gada
    }
    if (ui->rosterDatatableWidget->item(row, 14)) {
        ui->ictlineEdit->setText(ui->rosterDatatableWidget->item(row, 14)->text()); // ICT
    }
    if (ui->rosterDatatableWidget->item(row, 15)) {
        ui->fjqlineEdit->setText(ui->rosterDatatableWidget->item(row, 15)->text()); // FJQ
    }
    if (ui->rosterDatatableWidget->item(row, 16)) {
        ui->showtatallabel->setText(ui->rosterDatatableWidget->item(row, 16)->text()); // Total
    }
    if (ui->rosterDatatableWidget->item(row, 17)) {
        ui->showaveragelabel->setText(ui->rosterDatatableWidget->item(row, 17)->text()); // Average
    }
}


void roster::on_updatepushButton_clicked()
{
    // Retrieve the student ID and semester
    int studentId = ui->showStudenIDlineEdit_2->text().toInt();
    int semester = ui->semesterlineEdit->text().toInt(); // Assuming you have an input for semester
    if (studentId <= 0 || (semester != 1 && semester != 2)) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid Student ID and Semester (1 or 2).");
        return;
    }

    // Get updated grades from line edits
    double escience = ui->esciencelineEdit->text().toDouble();
    double amharic = ui->amhariclineEdit->text().toDouble();
    double herrega = ui->herregalineEdit->text().toDouble();
    double english = ui->engilishlineEdit->text().toDouble();
    double spoken = ui->spokenlineEdit->text().toDouble();
    double oromic = ui->oromiclineEdit->text().toDouble();
    double safu = ui->safulineEdit->text().toDouble();
    double gscience = ui->gsciencelineEdit->text().toDouble();
    double ogarti = ui->ogartilineEdit->text().toDouble();
    double maths = ui->mathslineEdit->text().toDouble();
    double gada = ui->gadalineEdit->text().toDouble();
    double ict = ui->ictlineEdit->text().toDouble();
    double fjq = ui->fjqlineEdit->text().toDouble();

    // Calculate total and average
    double total = escience + amharic + herrega + english + spoken +
                   oromic + safu + gscience + ogarti + maths + gada + ict + fjq;
    double average = total / 13; // Assuming there are 13 subjects

    // Prepare the update query
    QSqlQuery query;
    query.prepare(R"(
        UPDATE rosters
        SET escience = :escience,
            amharic = :amharic,
            herrega = :herrega,
            english = :english,
            spoken = :spoken,
            oromic = :oromic,
            safu = :safu,
            gscience = :gscience,
            ogarti = :ogarti,
            maths = :maths,
            gada = :gada,
            ict = :ict,
            fjq = :fjq,
            total = :total,
            average = :average
        WHERE student_id = :studentId AND semester = :semester
    )");

    // Bind values
    query.bindValue(":escience", escience);
    query.bindValue(":amharic", amharic);
    query.bindValue(":herrega", herrega);
    query.bindValue(":english", english);
    query.bindValue(":spoken", spoken);
    query.bindValue(":oromic", oromic);
    query.bindValue(":safu", safu);
    query.bindValue(":gscience", gscience);
    query.bindValue(":ogarti", ogarti);
    query.bindValue(":maths", maths);
    query.bindValue(":gada", gada);
    query.bindValue(":ict", ict);
    query.bindValue(":fjq", fjq);
    query.bindValue(":total", total);
    query.bindValue(":average", QString::number(average, 'f', 2)); // Save average with two decimal points
    query.bindValue(":studentId", studentId);
    query.bindValue(":semester", semester); // Bind the semester for the WHERE clause

    // Execute the query
    if (query.exec()) {
        QMessageBox::information(this, "Success", "Grades updated successfully!");
        on_refreshPushButton_clicked(studentId); // Refresh the table or UI if needed
    } else {
        // Show an error message with detailed information
        QMessageBox::warning(this, "Failure", "Failed to update grades: " + query.lastError().text());
    }
}


void roster::on_clearpushButton_2_clicked()
{
    // Clear all QLineEdit widgets
    ui->showStudenIDlineEdit_2->clear();
    ui->esciencelineEdit->clear();
    ui->amhariclineEdit->clear();
    ui->herregalineEdit->clear();
    ui->engilishlineEdit->clear();
    ui->spokenlineEdit->clear();
    ui->oromiclineEdit->clear();
    ui->safulineEdit->clear();
    ui->gsciencelineEdit->clear();
    ui->ogartilineEdit->clear();
    ui->mathslineEdit->clear();
    ui->gadalineEdit->clear();
    ui->ictlineEdit->clear();
    ui->fjqlineEdit->clear();
    ui->showtatallabel->setText("000");
    ui->showaveragelabel->setText("000");

    //If you have other widgets like combo boxes or spin boxes


    // Clear the selection from the QTableWidget
    ui->rosterDatatableWidget->clearSelection();

    // Clear the contents of the QTableWidget
    ui->rosterDatatableWidget->setRowCount(0); // This clears all rows from the table
    ui->rosterDatatableWidget->setColumnCount(0); // Optional: reset column count if needed
}


void roster::on_deletepushButton_clicked()
{
    // Retrieve student ID and semester from UI
    int studentId = ui->showStudenIDlineEdit_2->text().toInt(); // Assuming you have an input for student ID
    int semester = ui->semesterlineEdit->text().toInt(); // Assuming you have an input for semester

    // Validate input
    if (studentId <= 0 || (semester != 1 && semester != 2)) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid Student ID and Semester (1 or 2).");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion",
                                  "Are you sure you want to delete the grades for this student and semester?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return; // Exit if the user does not confirm
    }

    // Prepare SQL delete query
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM rosters WHERE student_id = :studentId AND semester = :semester");
    deleteQuery.bindValue(":studentId", studentId);
    deleteQuery.bindValue(":semester", semester);

    // Execute the delete query
    if (deleteQuery.exec()) {
        QMessageBox::information(this, "Success", "Grades deleted successfully!");
        // Optionally refresh the UI or update the displayed data
        on_refreshPushButton_clicked(studentId); // Refresh data for the specific student
    } else {
        QMessageBox::warning(this, "Failure", "Failed to delete grades: " + deleteQuery.lastError().text());
    }
}

void roster::on_searchpushButton_3_clicked()
{
    QString firstName = ui->searchbyIDlineEdit->text(); // Assuming you have a line edit for the first name

    // Input validation
    if (firstName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a first name to search.");
        return;
    }

    // Prepare SQL query to retrieve student's grades
    QSqlQuery query;
    query.prepare(R"(
        SELECT s.firstName, s.lastName, r.semester, r.escience, r.amharic, r.herrega, r.english,
               r.spoken, r.oromic, r.safu, r.gscience, r.ogarti, r.maths, r.gada,
               r.ict, r.fjq, r.total, r.average
        FROM students s
        JOIN rosters r ON s.id = r.student_id
        WHERE s.firstName = :firstName
    )");
    query.bindValue(":firstName", firstName);

    // Execute the query
    if (query.exec()) {
        // Clear existing data in the table
        ui->rosterDatatableWidget->setRowCount(0);

        // Set the number of columns if not already set
        if (ui->rosterDatatableWidget->columnCount() == 0) {
            ui->rosterDatatableWidget->setColumnCount(18);
            QStringList headers = {
                "First Name", "Last Name", "Semester", "E_Science", "Amharic", "Herrega", "English",
                "Spoken", "Oromic", "Safu", "G_Science", "OG-Aartii", "Maths",
                "Gada", "ICT", "FJQ", "Total", "Average"
            };
            ui->rosterDatatableWidget->setHorizontalHeaderLabels(headers);
        }

        // Check if any results are returned
        while (query.next()) {
            int rowCount = ui->rosterDatatableWidget->rowCount();
            ui->rosterDatatableWidget->insertRow(rowCount);

            // Populate the table with the retrieved data
            for (int i = 0; i < 19; ++i) {
                ui->rosterDatatableWidget->setItem(rowCount, i, new QTableWidgetItem(query.value(i).toString()));
            }
        }

        // If no results found, inform the user
        if (ui->rosterDatatableWidget->rowCount() == 0) {
            QMessageBox::information(this, "Not Found", "No grades found for the provided first name.");
        }
    } else {
        QMessageBox::warning(this, "Query Error", "Failed to execute query: " + query.lastError().text());
    }
}

