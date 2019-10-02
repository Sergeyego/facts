#ifndef MODELBILL_H
#define MODELBILL_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelBill : public DbTableModel
{
    Q_OBJECT
public:
    ModelBill(QObject *parent=nullptr);
    bool insertRow(int row, const QModelIndex &parent=QModelIndex());
public slots:
    void refresh(QDate begDate, QDate endDate, int id_pol=-1);
};

#endif // MODELBILL_H
