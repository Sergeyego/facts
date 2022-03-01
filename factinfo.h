#ifndef FACTINFO_H
#define FACTINFO_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

struct factData {
    factData(QString naim=QString(), double diam=0, QString ed=QString(),double kvo=0, double cena=0,
             double stoi=0, double nalog=0, double nds=0, double itogo=0, QString cod=QString(), QString edCod=QString()):
        naim(naim), diam(diam), ed(ed), kvo(kvo), cena(cena), stoi(stoi), nalog(nalog), nds(nds), itogo(itogo), cod(cod), edCod(edCod)
    {
    }
    QString naim;
    double diam;
    QString ed;
    double kvo;
    double cena;
    double stoi;
    double nalog;
    double nds;
    double itogo;
    QString cod;
    QString edCod;
};

class OtprInfo : public QObject
{
    Q_OBJECT
public:
    explicit OtprInfo(int id_otpr, QObject *parent = nullptr);
    QString nam();
    QString fnam();
    QString adr();
    QString rs();
    QString ks();
    QString bank();
    QString city();
    QString bik();
    QString innkpp();
    QString tels();
    QString nach();
    QString okpo();
    QString info();
    QString totalInfo();
private:
    QSqlQuery query;
};

class PolInfo : public QObject
{
    Q_OBJECT
public:
    explicit PolInfo(int id_pol, QObject *parent = nullptr);
    QString nam();
    QString fnam();
    QString adr();
    QString rs();
    QString ks();
    QString bank();
    QString city();
    QString bik();
    QString innkpp();
    QString tels();
    QString okpo();
    QString info();
    QString totalInfo();
private:
    QSqlQuery query;
};

class FactInfo : public QObject
{
    Q_OBJECT
public:
    explicit FactInfo(int id_fact, QObject *parent = nullptr);

public:
    QString nums();
    QDate dat();
    double disc();
    QString dop();
    QString plt();
    QString klad();
    QString pos_klad();
    QString drv();
    QString nom_dov();
    QDate dat_dov();
    QString org_dov();
    QString pos_pol();
    QString nam_pol();
    QString lic_dov();
    QString carrier();
    QString carrier_nam();
    QString transport();
    QString transport_num();
    QString drvd();
    int grm();
    QVector <factData> data() const;
    double sumStoi();
    double sumNalog();
    double sumItogo();
    double sumKvo();
    bool isSurgut();
    OtprInfo *otpr() const;
    OtprInfo *prod() const;
    PolInfo *pol() const;
    PolInfo *plat() const;

private:
    QSqlQuery queryFact;
    QSqlQuery queryTotal;
    QVector<factData> contents;
    QMap<QString,QString> eds;
    OtprInfo *otprInfo;
    OtprInfo *prodInfo;
    PolInfo *polInfo;
    PolInfo *platInfo;

signals:

public slots:
};

#endif // FACTINFO_H
