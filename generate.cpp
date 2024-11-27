#include "generate.h"
#include "ui_generate.h"

Generate::Generate(QWidget *parent)
    : QDialog(parent), ui(new Ui::Generate)
{
    setupUi();
}

Generate::~Generate()
{
    delete ui;
}

void Generate::setupUi() {
    setWindowTitle("Generate TimeTable Setup");
    setFixedSize(800, 600);
    setStyleSheet("QDialog { background-color: #2a6496; }"
                  "QLabel { color: white; }"
                  "QLineEdit, QComboBox { background-color: white; }"
                  "QPushButton { background-color: #4CAF50; color: white; }"
                  "QPushButton:hover { background-color: #45a049; }");

    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(20, 20, 20, 20);

    // Teacher Info Section
    QLabel *teacherHeader = new QLabel("Teacher Information");
    teacherHeader->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(teacherHeader);

    teacherComboBox = new QComboBox();
    teacherComboBox->addItems({"Teacher A", "Teacher B", "Teacher C"}); // Add teacher names
    layout->addWidget(new QLabel("Select Teacher:"));
    layout->addWidget(teacherComboBox);

    // Subject Info Section
    QLabel *subjectHeader = new QLabel("Subject Information");
    subjectHeader->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(subjectHeader);

    subjectInput = new QLineEdit();
    subjectInput->setPlaceholderText("Enter Subject");
    layout->addWidget(new QLabel("Subject:"));
    layout->addWidget(subjectInput);

    roomInput = new QLineEdit();
    roomInput->setPlaceholderText("Enter Room Number");
    layout->addWidget(new QLabel("Room:"));
    layout->addWidget(roomInput);

    // Weekly Availability Grid with Checkboxes
    availabilityGrid = new QTableWidget(7, 5); // 7 days, 5 time slots
    availabilityGrid->setHorizontalHeaderLabels({"8-9", "9-10", "10-11", "11-12", "1-2"});
    availabilityGrid->setVerticalHeaderLabels({"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"});

    for (int row = 0; row < availabilityGrid->rowCount(); ++row) {
        for (int col = 0; col < availabilityGrid->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked); // Set initial state to unchecked
            availabilityGrid->setItem(row, col, item);
        }
    }

    layout->addWidget(new QLabel("Teacher Availability:"));
    layout->addWidget(availabilityGrid);

    // Action Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    generateButton = new QPushButton("Generate Timetable");
    connect(generateButton, &QPushButton::clicked, this, &Generate::onGenerateButtonClicked);
    buttonLayout->addWidget(generateButton);

    resetButton = new QPushButton("Reset");
    connect(resetButton, &QPushButton::clicked, this, &Generate::onResetButtonClicked);
    buttonLayout->addWidget(resetButton);

    layout->addLayout(buttonLayout);

    // New Table to Display Generated Timetable
    timetableDisplay = new QTableWidget();
    timetableDisplay->setColumnCount(5);
    timetableDisplay->setHorizontalHeaderLabels({"Teacher", "Subject", "Room", "Day", "Time"});
    layout->addWidget(new QLabel("Generated Timetable:"));
    layout->addWidget(timetableDisplay);
}

void Generate::onGenerateButtonClicked() {
    generateTimetable();
}

void Generate::onResetButtonClicked() {
    teacherComboBox->setCurrentIndex(0);
    subjectInput->clear();
    roomInput->clear();
    for (int row = 0; row < availabilityGrid->rowCount(); ++row) {
        for (int col = 0; col < availabilityGrid->columnCount(); ++col) {
            availabilityGrid->item(row, col)->setCheckState(Qt::Unchecked);
        }
    }
    timetableDisplay->setRowCount(0); // Clear the displayed timetable
}

void Generate::generateTimetable() {
    QString teacher = teacherComboBox->currentText();
    QString subject = subjectInput->text();
    QString room = roomInput->text();

    // Validate inputs
    if (subject.isEmpty() || room.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    // Check availability and detect conflicts
    if (detectConflicts(teacher, subject, room)) {
        QMessageBox::warning(this, "Conflict Detected", "There are conflicts in the timetable.");
        return;
    }

    // Clear previous timetable display
    timetable.clear();
    timetableDisplay->setRowCount(0);

    // Create a new timetable entry for each checked time slot
    for (int row = 0; row < availabilityGrid->rowCount(); ++row) {
        for (int col = 0; col < availabilityGrid->columnCount(); ++col) {
            if (availabilityGrid->item(row, col)->checkState() == Qt::Checked) {
                QString day = availabilityGrid->verticalHeaderItem(row)->text();
                QString timeSlot = availabilityGrid->horizontalHeaderItem(col)->text();
                TimetableEntry entry = {teacher, subject, room, day, timeSlot};
                timetable.append(entry);
            }
        }
    }

    // Display the generated timetable
    displayTimetable();
}

bool Generate::detectConflicts(const QString &teacher, const QString &subject, const QString &room) {
    for (const auto &entry : timetable) {
        // Check for double bookings for the same teacher and time slot
        if (entry.teacher == teacher && entry.day == entry.day && entry.timeSlot == entry.timeSlot) {
            return true; // Conflict detected
        }

        // Check for room conflicts
        if (entry.room == room && entry.day == entry.day && entry.timeSlot == entry.timeSlot) {
            return true; // Conflict detected
        }
    }
    return false; // No conflict detected
}

void Generate::displayTimetable() {
    for (const auto &entry : timetable) {
        int row = timetableDisplay->rowCount();
        timetableDisplay->insertRow(row);
        timetableDisplay->setItem(row, 0, new QTableWidgetItem(entry.teacher));
        timetableDisplay->setItem(row, 1, new QTableWidgetItem(entry.subject));
        timetableDisplay->setItem(row, 2, new QTableWidgetItem(entry.room));
        timetableDisplay->setItem(row, 3, new QTableWidgetItem(entry.day));
        timetableDisplay->setItem(row, 4, new QTableWidgetItem(entry.timeSlot));
    }
}
