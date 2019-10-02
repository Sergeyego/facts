#include "models.h"
#include <QMessageBox>

TotalModel::TotalModel(QObject *parent)
    :QSqlQueryModel(parent)
{
}

void TotalModel::refresh(int /*id_fact*/)
{
}

QVariant TotalModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if ((index.column()<3)&& (role==Qt::EditRole||role==Qt::DisplayRole)){
        return QLocale().toString(value.toDouble(),'f',2);
    }
    return QSqlQueryModel::data(index,role);
}


TotalFactModel::TotalFactModel(QObject *parent)
    :TotalModel(parent)
{
}

void TotalFactModel::refresh(int id_fact)
{
    QSqlQuery query;
    query.prepare("Select sum(stoi), sum(nalog), sum(itogo), sum(kvo) from calc_fact(:id_fact, (select disc from facts where id = :id_fact2))");
    query.bindValue(":id_fact",id_fact);
    query.bindValue(":id_fact2",id_fact);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    } else {
        setQuery(query);
    }
}


TotalBillModel::TotalBillModel(QObject *parent)
    :TotalModel(parent)
{
}

void TotalBillModel::refresh(int id_bill)
{
    QSqlQuery query;
    query.prepare("Select sum(stoi), sum(nalog), sum(itogo), sum(kvo) from calc_bill(:id_bill, (select disc from bills where id=:id_bill2))");
    query.bindValue(":id_bill",id_bill);
    query.bindValue(":id_bill2",id_bill);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Ok);
    } else {
        setQuery(query);
    }
}


CalcFactModel::CalcFactModel(int id_fact, double disc, QObject *parent): QSqlQueryModel(parent)
{
    QSqlQuery query;
    query.prepare("Select naim, diam, ed, kvo, cena, stoi, nalog, nds, itogo, cod from calc_fact(:id_fact, :disc)");
    query.bindValue(":id_fact",id_fact);
    query.bindValue(":disc",disc);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        setQuery(query);
    }
}


CalcBillModel::CalcBillModel(int id_bill, double disc, QObject *parent): QSqlQueryModel(parent)
{
    QSqlQuery query;
    query.prepare("Select naim, diam, ed, kvo, cena, stoi, nalog, nds, itogo from calc_bill(:id_bill, :disc)");
    query.bindValue(":id_bill",id_bill);
    query.bindValue(":disc",disc);
    if (!query.exec()){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        setQuery(query);
    }
}
