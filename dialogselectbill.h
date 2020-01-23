#ifndef DIALOGSELECTBILL_H
#define DIALOGSELECTBILL_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class DialogSelectBill;
}

class DialogSelectBill : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSelectBill(int id_pol, QWidget *parent = 0);
    ~DialogSelectBill();
    int idBill();

private:
    Ui::DialogSelectBill *ui;
    QSqlQueryModel *model;
};

#endif // DIALOGSELECTBILL_H
