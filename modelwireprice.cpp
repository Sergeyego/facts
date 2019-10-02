#include "modelwireprice.h"
#include <QMessageBox>


ModelWirePrice::ModelWirePrice(QObject *parent) :
    DbTableModel("wire_cena",parent)
{
    addColumn(QString("id_provol"),QString::fromUtf8("Марка"),true,TYPE_STRING,NULL,Rels::instance()->relProvol);
    addColumn(QString("id_diam"),QString::fromUtf8("Ф"),true,TYPE_STRING,NULL,Rels::instance()->relDiam);
    addColumn(QString("id_pack"),QString::fromUtf8("Намотка"),true,TYPE_STRING,NULL,Rels::instance()->relPack);
    addColumn(QString("ed"),QString::fromUtf8("Ед.изм"),false,TYPE_STRING);
    addColumn(QString("val"),QString::fromUtf8("Цена без НДС"),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("nds"),QString::fromUtf8("НДС,%"),false,TYPE_DOUBLE,new QDoubleValidator(0,200,1,this));
    addColumn(QString("dat"),QString::fromUtf8("Дата"),true,TYPE_DATE);

    setSuffix("inner join provol on provol.id=wire_cena.id_provol "
              "inner join diam on diam.id=wire_cena.id_diam "
              "inner join wire_pack_kind on wire_pack_kind.id=wire_cena.id_pack");
    setSort("provol.nam, diam.sdim, wire_pack_kind.nam");

    setDefaultValue(3,QString::fromUtf8("кг"));
    setDefaultValue(5,20.0);
}

Qt::ItemFlags ModelWirePrice::flags(const QModelIndex &index) const
{
    if (index.column()==6) return Qt::ItemIsSelectable |Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    return Qt::ItemIsEditable | Qt::ItemIsSelectable |Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

void ModelWirePrice::refresh(QDate date)
{
    setFilter("dat='"+date.toString("yyyy-MM-dd")+"'");
    setDefaultValue(6,date);
    select();
}

void ModelWirePrice::newprice(QDate date, double nds)
{
    if (Rels::instance()->modelWirePriceDate->rowCount()>0){
        QSqlQuery query;
        query.prepare("insert into wire_cena (id_provol, id_diam, id_pack, dat, val, nds, ed) "
                      "select c.id_provol, c.id_diam, c.id_pack, :dt as dat, c.val, :nd as nds, c.ed "
                      "from wire_cena as c where c.dat= :dta");
        query.bindValue(":dt",date);
        query.bindValue(":nd",nds);
        query.bindValue(":dta",Rels::instance()->modelWirePriceDate->data(Rels::instance()->modelWirePriceDate->index(0,0),Qt::EditRole).toDate());
        if (!query.exec()){
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        } else {
            Rels::instance()->modelWirePriceDate->refresh();
        }
    }
}


