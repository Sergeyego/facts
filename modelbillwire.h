#ifndef MODELBILLWIRE_H
#define MODELBILLWIRE_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelBillWire : public DbTableModel
{
    Q_OBJECT
public:
    ModelBillWire(QObject *parent=nullptr);
public slots:
    void refresh(int id_bill);
    void insCenNds(QDate date);
};

#endif // MODELBILLWIRE_H
