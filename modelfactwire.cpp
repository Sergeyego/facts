#include "modelfactwire.h"

ModelFactWire::ModelFactWire(QObject *parent) : DbTableModel("wire_it_fact",parent)
{
    addColumn(QString("id"),QString::fromUtf8("id"),true,TYPE_INT);
    addColumn(QString("id_f"),QString::fromUtf8("id_f"),false,TYPE_INT);
    addColumn(QString("id_prov"),QString::fromUtf8("Марка"),false,TYPE_STRING,NULL,Rels::instance()->relProvol);
    addColumn(QString("id_diam"),QString::fromUtf8("Диам."),false,TYPE_STRING,NULL,Rels::instance()->relDiam);
    addColumn(QString("id_pack"),QString::fromUtf8("Намотка"),false,TYPE_STRING,NULL,Rels::instance()->relPack);
    addColumn(QString("nazv"),QString::fromUtf8("Назв."),false,TYPE_STRING);
    addColumn(QString("kvo"),QString::fromUtf8("К-во"),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,3,this));
    addColumn(QString("up"),QString::fromUtf8("Пред."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("dn"),QString::fromUtf8("Скид."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("ed"),QString::fromUtf8("Ед."),false,TYPE_STRING);
    addColumn(QString("cen_nds"),QString::fromUtf8("Цена б/НДС."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("nds"),QString::fromUtf8("НДС,%"),false,TYPE_DOUBLE,new QDoubleValidator(0,100,1,this));

    setSort("wire_it_fact.id");
    setDefaultValue(5,QString::fromUtf8("проволока"));
    setDefaultValue(9,QString::fromUtf8("кг"));

}

void ModelFactWire::refresh(int id_fact)
{
    setFilter("wire_it_fact.id_f = "+QString::number(id_fact));
    setDefaultValue(1,id_fact);
    select();
}

void ModelFactWire::insCenNds(QDate date)
{
    for (int i=0; i<rowCount(); i++){
        double cen=data(index(i,10),Qt::EditRole).toDouble();
        if (cen==0){
            int id_fact=data(index(i,0),Qt::EditRole).toInt();
            QSqlQuery query;
            query.prepare("update wire_it_fact set cen_nds= "
                          "(select val from wire_cena where "
                          "id_provol=(select id_prov from wire_it_fact where id=:id1) and "
                          "id_diam=(select id_diam from wire_it_fact where id=:id2) and "
                          "id_pack=(select id_pack from wire_it_fact where id=:id3) and dat=:dat1 "
                          "), "
                          "nds= (select nds from wire_cena where "
                          "id_provol=(select id_prov from wire_it_fact where id=:id4) and "
                          "id_diam=(select id_diam from wire_it_fact where id=:id5) and "
                          "id_pack=(select id_pack from wire_it_fact where id=:id6) and dat=:dat2 "
                          ") "
                          "where id=:id7");
            for (int i=1; i<=7; i++) query.bindValue(":id"+QString::number(i),id_fact);
            query.bindValue(":dat1",date);
            query.bindValue(":dat2",date);
            if (!query.exec()){
                QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
            }
        }
    }
    select();
}

void ModelFactWire::insDataSert(int id_sert)
{
    if (this->rowCount()>0){
        int id_fact=data(index(0,1),Qt::EditRole).toInt();
        QSqlQuery query;
        query.prepare("insert into wire_it_fact (id_f, id_prov, id_diam, id_pack, kvo, ed, nazv)"
                      "(select :id_f, m.id_provol, m.id_diam, p.id_pack, o.m_netto, "+QString::fromUtf8("'кг', 'проволока' ")+
                      "from wire_shipment_consist o "
                      "inner join wire_parti p on o.id_wparti=p.id "
                      "inner join wire_parti_m m on p.id_m=m.id "
                      "where o.id_ship=:id_sert)");
        query.bindValue(":id_f",id_fact);
        query.bindValue(":id_sert",id_sert);
        if (query.exec()){
            select();
        } else {
            QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
        }
    }
}
