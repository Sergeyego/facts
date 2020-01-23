#ifndef MODELFACTEL_H
#define MODELFACTEL_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelFactEl : public DbTableModel
{
    Q_OBJECT
public:
    ModelFactEl(QObject *parent=nullptr);
public slots:
    void refresh(int id_fact);
    void insCenNds(QDate date);
    void insDataSert(int id_sert);
    void insDataBill(int id_bill);
};

#endif // MODELFACTEL_H
