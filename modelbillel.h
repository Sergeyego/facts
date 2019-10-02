#ifndef MODELBILLEL_H
#define MODELBILLEL_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelBillEl : public DbTableModel
{
    Q_OBJECT
public:
    ModelBillEl(QObject *parent=nullptr);
public slots:
    void refresh(int id_bill);
    void insCenNds(QDate date);
};

#endif // MODELBILLEL_H
