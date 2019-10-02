#ifndef MODELFACTWIRE_H
#define MODELFACTWIRE_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelFactWire : public DbTableModel
{
    Q_OBJECT
public:
    ModelFactWire(QObject *parent=nullptr);
public slots:
    void refresh(int id_fact);
    void insCenNds(QDate date);
    void insDataSert(int id_sert);
};

#endif // MODELFACTWIRE_H
