#ifndef FORMWIREPRICE_H
#define FORMWIREPRICE_H

#include <QDialog>
#include "modelwireprice.h"
#include "rels.h"
#include "db/dbxlsx.h"

namespace Ui {
class FormWirePrice;
}

class FormWirePrice : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormWirePrice(QWidget *parent = nullptr);
    ~FormWirePrice();

protected:
    void keyPressEvent(QKeyEvent *pe);
    
private:
    Ui::FormWirePrice *ui;
    ModelWirePrice *modelPrice;
    DbXlsx *dbXlsx;

private slots:
    void newPrice();
    void refresh();

};

#endif // FORMWIREPRICE_H
