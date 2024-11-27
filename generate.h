#ifndef GENERATE_H
#define GENERATE_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QTableWidget>

namespace Ui {
class Generate;
}

class Generate : public QDialog
{
    Q_OBJECT

public:
    explicit Generate(QWidget *parent = nullptr);
    ~Generate();

private:
    // UI Components
    Ui::Generate *ui;

    // Input fields
    QLineEdit *subjectInput;
    QLineEdit *roomInput;
    QComboBox *teacherComboBox;
    QTableWidget *availabilityGrid;
    QPushButton *generateButton;
    QPushButton *resetButton;

    // New table to display the generated timetable
    QTableWidget *timetableDisplay;

    // Data structures to hold the timetable data
    struct TimetableEntry {
        QString teacher;
        QString subject;
        QString room;
        QString day;      // Day of the week
        QString timeSlot; // E.g., "8-9", "9-10"
    };

    QVector<TimetableEntry> timetable;

    // Setup UI
    void setupUi();

    // Generate timetable
    void generateTimetable();
    void displayTimetable();
    bool detectConflicts(const QString &teacher, const QString &subject, const QString &room);

private slots:
    void onGenerateButtonClicked();
    void onResetButtonClicked();
};

#endif // GENERATE_H
