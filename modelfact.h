#ifndef MODELFACT_H
#define MODELFACT_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"
#include <QItemDelegate>
#include <QLineEdit>

class ModelFact : public DbTableModel
{
    Q_OBJECT
public:
    ModelFact(QObject *parent=nullptr);
    bool insertRow(int row, const QModelIndex &parent=QModelIndex());
public slots:
    void refresh(QDate begDate, QDate endDate, int id_pol=-1);
};

#endif // MODELFACT_H
