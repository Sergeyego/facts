#include "modelbillel.h"

ModelBillEl::ModelBillEl(QObject *parent) : DbTableModel("it_bill",parent)
{
    addColumn(QString("id"),QString::fromUtf8("id"),true,TYPE_INT);
    addColumn(QString("id_b"),QString::fromUtf8("id_b"),false,TYPE_INT);
    addColumn(QString("id_el"),QString::fromUtf8("Марка"),false,TYPE_STRING,NULL,Rels::instance()->relElMark);
    addColumn(QString("diam"),QString::fromUtf8("Диам."),false,TYPE_DOUBLE,new QDoubleValidator(0,10,1,this));
    addColumn(QString("nazv"),QString::fromUtf8("Назв."),false,TYPE_STRING);
    addColumn(QString("kvo"),QString::fromUtf8("К-во"),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,3,this));
    addColumn(QString("up"),QString::fromUtf8("Пред."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("dn"),QString::fromUtf8("Скид."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("ed"),QString::fromUtf8("Ед."),false,TYPE_STRING);
    addColumn(QString("cen_nds"),QString::fromUtf8("Цена б/НДС."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("nds"),QString::fromUtf8("НДС,%"),false,TYPE_DOUBLE,new QDoubleValidator(0,100,1,this));

    setSort("it_bill.id");
    setDefaultValue(4,QString::fromUtf8("электроды"));
    setDefaultValue(8,QString::fromUtf8("кг"));
}

void ModelBillEl::refresh(int id_bill)
{
    setFilter("it_bill.id_b = "+QString::number(id_bill));
    setDefaultValue(1,id_bill);
    select();
}

void ModelBillEl::insCenNds(QDate date)
{
    for (int i=0; i<rowCount(); i++){
        double cen=data(index(i,9),Qt::EditRole).toDouble();
        if (cen==0){
            int id=data(index(i,0),Qt::EditRole).toInt();
            QSqlQuery query;
            query.prepare("update it_bill set cen_nds= "
                              "(select val from cena where "
                              "id_el=(select id_el from it_bill where id=:id1) and "
                              "id_diam=(select d.id from diam as d where d.diam=(select diam from it_bill where id=:id2)) and dat=:dat1 "
                              "), nds= (select nds from cena where "
                              "id_el=(select id_el from it_bill where id=:id3) and "
                              "id_diam=(select dn.id from diam as dn where dn.diam=(select diam from it_bill where id=:id4)) and dat=:dat2 "
                              ") "
                              "where id=:id5");
            for (int i=1; i<=5; i++) query.bindValue(":id"+QString::number(i),id);
            query.bindValue(":dat1",date);
            query.bindValue(":dat2",date);
            if (!query.exec()){
                QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
            }
        }
    }
    select();
}
