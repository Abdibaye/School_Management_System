#ifndef PAYHISTORY_H
#define PAYHISTORY_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QFileDialog>
#include <QPixmap>
#include <QComboBox>


namespace Ui {
class Payhistory;
}

class Payhistory : public QDialog
{
    Q_OBJECT

public:
    explicit Payhistory(QWidget *parent = nullptr);
    ~Payhistory();
    void setParentPhone(const QString &phone); // Method to set the parent phone
    void showPaymentHistory();

private:
    Ui::Payhistory *ui;
     QString parentPhone; // Store the parent phone

};

#endif // PAYHISTORY_H
