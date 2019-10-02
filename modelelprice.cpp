#include "modelelprice.h"
#include <QMessageBox>


ModelElPrice::ModelElPrice(QObject *parent) :
    DbTableModel("cena",parent)
{
    addColumn(QString("id_el"),QString::fromUtf8("Марка"),true,TYPE_STRING,NULL,Rels::instance()->relElMark);
    addColumn(QString("id_diam"),QString::fromUtf8("Ф"),true,TYPE_STRING,NULL,Rels::instance()->relDiam);
    addColumn(QString("ed"),QString::fromUtf8("Ед.изм"),false,TYPE_STRING);
    addColumn(QString("val"),QString::fromUtf8("Цена без НДС"),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("nds"),QString::fromUtf8("НДС,%"),false,TYPE_DOUBLE,new QDoubleValidator(0,200,1,this));
    addColumn(QString("dat"),QString::fromUtf8("Дата"),true,TYPE_DATE);

    setSuffix("inner join elrtr on elrtr.id=cena.id_el inner join diam on cena.id_diam=diam.id ");
    setSort("elrtr.marka, diam.diam");

    setDefaultValue(2,QString::fromUtf8("кг"));
    setDefaultValue(4,20.0);
}

Qt::ItemFlags ModelElPrice::flags(const QModelIndex &index) const
{
    if (index.column()==5) return Qt::ItemIsSelectable |Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    return Qt::ItemIsEditable | Qt::ItemIsSelectable |Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

void ModelElPrice::refresh(QDate date)
{
    setFilter("dat='"+date.toString("yyyy-MM-dd")+"'");
    setDefaultValue(5,date);
    select();
}

void ModelElPrice::newprice(QDate date, double nds)
{
    if (Rels::instance()->modelElPriceDate->rowCount()>0){
        QSqlQuery query;
        query.prepare("insert into cena (id_el, id_diam, dat, val, nds, ed) "
                      "select c.id_el, c.id_diam, :dt as dat, c.val, :nd as nds, c.ed "
                      "from cena as c where c.dat= :dta");
        query.bindValue(":dt",date);
        query.bindValue(":nd",nds);
        query.bindValue(":dta",Rels::instance()->modelElPriceDate->data(Rels::instance()->modelElPriceDate->index(0,0),Qt::EditRole).toDate());
        if (!query.exec()){
            QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
        } else {
            Rels::instance()->modelElPriceDate->refresh();
        }
    }
}


