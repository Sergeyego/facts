#include "modelbill.h"

ModelBill::ModelBill(QObject *parent) : DbTableModel("bills",parent)
{
    addColumn(QString("id"),QString::fromUtf8("id"),true,TYPE_INT);
    addColumn(QString("nums"),QString::fromUtf8("Номер"),false,TYPE_STRING);
    addColumn(QString("dat"),QString::fromUtf8("Дата"),false,TYPE_DATE);
    addColumn(QString("id_p"),QString::fromUtf8("Плательщик"),false,TYPE_STRING,NULL,Rels::instance()->relPol);
    addColumn(QString("id_hoz"),QString::fromUtf8("Поставщик"),false,TYPE_STRING,NULL,Rels::instance()->relPost);
    addColumn(QString("disc"),QString::fromUtf8("Скидка"),false,TYPE_DOUBLE, new QDoubleValidator(0,200,2,this));

    setSort("bills.dat, bills.nums");
    setDefaultValue(5,0);
}

bool ModelBill::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),3,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    setDefaultValue(4,Rels::instance()->current_hoz);
    return DbTableModel::insertRow(row,parent);
}

void ModelBill::refresh(QDate begDate, QDate endDate, int id_pol)
{
    QString flt;
    flt=QString("bills.dat between '")+begDate.toString("yyyy-MM-dd")+QString("' and '")+endDate.toString("yyyy-MM-dd")+QString("'");
    if (id_pol!=-1){
        flt+=" and bills.id_p = "+QString::number(id_pol);
    }
    setFilter(flt);
    select();
}
