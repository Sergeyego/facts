#include "modelfactel.h"

ModelFactEl::ModelFactEl(QObject *parent) : DbTableModel("it_fact",parent)
{
    addColumn(QString("id"),QString::fromUtf8("id"),true,TYPE_INT);
    addColumn(QString("id_f"),QString::fromUtf8("id_f"),false,TYPE_INT);
    addColumn(QString("id_el"),QString::fromUtf8("Марка"),false,TYPE_STRING,NULL,Rels::instance()->relElMark);
    addColumn(QString("diam"),QString::fromUtf8("Диам."),false,TYPE_DOUBLE,new QDoubleValidator(0,10,1,this));
    addColumn(QString("nazv"),QString::fromUtf8("Назв."),false,TYPE_STRING);
    addColumn(QString("kvo"),QString::fromUtf8("К-во"),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,3,this));
    addColumn(QString("up"),QString::fromUtf8("Пред."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("dn"),QString::fromUtf8("Скид."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("ed"),QString::fromUtf8("Ед."),false,TYPE_STRING);
    addColumn(QString("cen_nds"),QString::fromUtf8("Цена б/НДС."),false,TYPE_DOUBLE,new QDoubleValidator(0,1000000000,2,this));
    addColumn(QString("nds"),QString::fromUtf8("НДС,%"),false,TYPE_DOUBLE,new QDoubleValidator(0,100,1,this));

    setSort("it_fact.id");
    setDefaultValue(4,QString::fromUtf8("электроды"));
    setDefaultValue(8,QString::fromUtf8("кг"));
}

void ModelFactEl::refresh(int id_fact)
{
    setFilter("it_fact.id_f = "+QString::number(id_fact));
    setDefaultValue(1,id_fact);
    select();
}

void ModelFactEl::insCenNds(QDate date)
{
    for (int i=0; i<rowCount(); i++){
        double cen=data(index(i,9),Qt::EditRole).toDouble();
        if (cen==0){
            int id=data(index(i,0),Qt::EditRole).toInt();
            QSqlQuery query;
            query.prepare("update it_fact set cen_nds= "
                          "(select val from cena where "
                          "id_el=(select id_el from it_fact where id=:id1) and "
                          "id_diam=(select d.id from diam as d where d.diam=(select diam from it_fact where id=:id2)) and dat=:dat1 "
                          "), nds= (select nds from cena where "
                          "id_el=(select id_el from it_fact where id=:id3) and "
                          "id_diam=(select d.id from diam as d where d.diam=(select diam from it_fact where id=:id4)) and dat=:dat2 "
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

void ModelFactEl::insDataSert(int id_sert)
{
    if (this->rowCount()>0){
        int id_fact=data(index(0,1),Qt::EditRole).toInt();
        QSqlQuery query;
        query.prepare("insert into it_fact (id_f, id_el, diam, kvo, ed, nazv)"
                      "(select :id_f, p.id_el, p.diam, o.massa, "+QString::fromUtf8("'кг', 'электроды' ")+
                      "from otpusk o "
                      "inner join parti p on o.id_part=p.id "
                      "inner join elrtr e on p.id_el=e.id "
                      "where o.id_sert=:id_sert)");
        query.bindValue(":id_f",id_fact);
        query.bindValue(":id_sert",id_sert);
        if (query.exec()){
            select();
        } else {
            QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
        }
    }
}

void ModelFactEl::insDataBill(int id_bill)
{
    if (this->rowCount()>0){
        int id_fact=data(index(0,1),Qt::EditRole).toInt();
        QSqlQuery query;
        query.prepare("insert into it_fact (id_f, id_el, diam, kvo, ed, nazv, nds, dn, cen_nds, up) "
                      "(select :id_f, b.id_el, b.diam, b.kvo, b.ed, b.nazv, b.nds, b.dn, b.cen_nds, b.up "
                      "from it_bill as b where b.id_b = :id_bill )");
        query.bindValue(":id_f",id_fact);
        query.bindValue(":id_bill",id_bill);
        if (query.exec()){
            select();
            emit sigUpd();
        } else {
            QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
        }
    }
}
