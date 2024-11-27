#ifndef ROSTER_H
#define ROSTER_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QWidget>
#include <QTableWidgetItem> // Include this line

namespace Ui {
class roster;
}

class roster : public QDialog
{
    Q_OBJECT

public:
    explicit roster(QWidget *parent = nullptr);
    ~roster();

private slots:
    //void on_savepushButton_clicked();
    void on_savepushButton_clicked();
    void on_refreshPushButton_clicked(int studentID);

    void on_searchpushButton_2_clicked();

    void on_clearpushButton_clicked();

    void on_updatepushButton_clicked();

    void on_clearpushButton_2_clicked();

    void on_deletepushButton_clicked();

    void on_searchpushButton_3_clicked();

private:
    Ui::roster *ui;
    void initializeDatabase();
    void loadGradeData();
    void clear();


    bool insertGrades(const QVariantMap &grades);

    void onTableItemClicked(QTableWidgetItem *item);

};

#endif // ROSTER_H
