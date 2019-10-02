#ifndef FORMELPRICE_H
#define FORMELPRICE_H

#include <QDialog>
#include "modelelprice.h"
#include "rels.h"
#include "db/dbxlsx.h"

namespace Ui {
class FormElPrice;
}

class FormElPrice : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormElPrice(QWidget *parent = nullptr);
    ~FormElPrice();

protected:
    void keyPressEvent(QKeyEvent *pe);
    
private:
    Ui::FormElPrice *ui;
    ModelElPrice *modelPrice;
    DbXlsx *dbXlsx;

private slots:
    void newPrice();
    void refresh();

};

#endif // FORMELPRICE_H
