#ifndef PAYMENT_H
#define PAYMENT_H

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
#include <payhistory.h>
#include <pay.h>

namespace Ui {
class payment;
}

class payment : public QDialog
{
    Q_OBJECT

public:
    explicit payment(QWidget *parent = nullptr);
    ~payment();

private slots:

    void on_paidpushButton_clicked();

    void on_unpaidpushButton_clicked();

    void on_paysearch_btn_clicked();

    void on_tpaidpushButton_clicked();

    void on_tunpaidpushButton_clicked();

private:
    Ui::payment *ui;

    void on_paymentConfirmed();
};

#endif // PAYMENT_H
