#ifndef MODELELPRICE_H
#define MODELELPRICE_H
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelElPrice : public DbTableModel
{
    Q_OBJECT
public:
    explicit ModelElPrice(QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
private:
    
public slots:
    void refresh(QDate date);
    void newprice(QDate date, double nds);  
};

#endif // MODELELPRICE_H
