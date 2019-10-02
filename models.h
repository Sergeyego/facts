#ifndef MODELS_H
#define MODELS_H
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtSql>


class TotalModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    TotalModel(QObject *parent = 0);
    virtual void refresh(int id_fact);
    QVariant data(const QModelIndex &index, int role) const;
};

class TotalFactModel : public TotalModel
{
    Q_OBJECT
public:
    TotalFactModel(QObject *parent = 0);
    void refresh(int id_fact);
};

class TotalBillModel : public TotalModel
{
    Q_OBJECT
public:
    TotalBillModel(QObject *parent = 0);
    void refresh(int id_fact);
};

class CalcFactModel: public QSqlQueryModel
{
public:
    CalcFactModel(int id_fact, double disc, QObject *parent = 0);
};

class CalcBillModel: public QSqlQueryModel
{
public:
    CalcBillModel(int id_bill, double disc, QObject *parent = 0);
};

#endif // MODELS_H
