#ifndef MODELWIREPRICE_H
#define MODELWIREPRICE_H
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelWirePrice : public DbTableModel
{
    Q_OBJECT
public:
    explicit ModelWirePrice(QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
private:
    
public slots:
    void refresh(QDate date);
    void newprice(QDate date, double nds);  
};

#endif // MODELWIREPRICE_H
