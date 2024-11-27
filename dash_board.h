#ifndef DASH_BOARD_H
#define DASH_BOARD_H

#include <QDialog>
#include <register_screen.h>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>


namespace Ui {
class Dashboard;
}

class Dashboard : public QDialog
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    void loadStudents();
    void initializeDatabase();


private slots:
    //void on_pushButton_clicked();

    void on_totalStudent_btn_clicked();

    void on_pushButton_clicked();

    void on_load_btn_clicked();

private:
    Ui::Dashboard *ui;

    void totalStudent();
    int countTotalStudents();
    void exportDataToCSV(const QString &filePath);

};

#endif // DASH_BOARD_H
