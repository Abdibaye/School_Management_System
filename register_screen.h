#ifndef REGISTER_SCREEN_H
#define REGISTER_SCREEN_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QPixmap>
#include <QWidget>
#include <QTableWidgetItem> // Include this line


namespace Ui {
class RegisterScreen;
}

class RegisterScreen : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterScreen(QWidget *parent = nullptr);
    ~RegisterScreen();



private slots:
    void on_add_btn_clicked();
    //void on_upd_btn_clicked();
   // void on_delete_btn_clicked();



    void on_uploadButton_clicked();


    void on_clear_btn_clicked();

    void on_upd_btn_clicked();

    void on_delete_btn_clicked();

    void on_select_btn_clicked();

    void on_search_btn_clicked();

private:
    Ui::RegisterScreen *ui;
    QSqlTableModel *model;


    QString currentImagePath;// Add this line in the RegisterScreen class    // This should be declared here

    void initializeDatabase();

    void onTableItemClicked(QTableWidgetItem *item);
    void loadStudentData();
    void clear();
    void displayInsertedStudent(const QString &studentNum);
    bool insertStudent(const QString &firstName, const QString &lastName, const QString &studentNum,
                       const QString &birthDate, const QString &address, const QString &gender,
                       const QString &grade, const QString &parentNum, const QByteArray &imageData);
    // bool updateStudent(const QString &firstName, const QString &lastName, const QString &phoneNumber);
    // bool deleteStudent(const QString &firstName, const QString &lastName);

    // bool studentExists(const QString &firstName, const QString &lastName);
};

#endif // REGISTER_SCREEN_H
