#ifndef PAY_H
#define PAY_H

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

namespace Ui {
class Pay;
}

class Pay : public QDialog
{
    Q_OBJECT

public:
    explicit Pay(QWidget *parent = nullptr);
    ~Pay();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Pay *ui;

    void createPaymentsTable();
};

#endif // PAY_H
