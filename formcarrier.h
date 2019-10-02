#ifndef FORMCARRIER_H
#define FORMCARRIER_H
#include <QDialog>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"

namespace Ui {
class FormCarrier;
}

class FormCarrier : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormCarrier(QWidget *parent = nullptr);
    ~FormCarrier();

protected:
    void keyPressEvent(QKeyEvent *pe);
    
private:
    Ui::FormCarrier *ui;
    DbTableModel *modelCarrier;
    DbMapper *mapper;
private slots:

};

#endif // FORMCARRIER_H
