#include "factinfo.h"

FactInfo::FactInfo(int id_fact, QObject *parent) : QObject(parent)
{
    eds.insert(QString::fromUtf8("м"),QString::fromUtf8("006"));
    eds.insert(QString::fromUtf8("т"),QString::fromUtf8("168"));
    eds.insert(QString::fromUtf8("кг"),QString::fromUtf8("166"));
    eds.insert(QString::fromUtf8("шт"),QString::fromUtf8("796"));

    queryFact.prepare("select f.nums, f.dat, f.disc, f.id_hoz, f.nom_dov, f.dat_dov, f.otpr, f.lic_dov, "
                      "f.id_g, f.org_dov, f.plt, f.id_p, f.pos_pol, f.nam_pol, f.dop, po.id_kat, f.pos_klad, f.klad, "
                      "f.drv, c.nam, c.adres, c.inn, f.transport, f.transport_num, f.drvd "
                      "from facts as f "
                      "inner join poluch as po on po.id = f.id_p "
                      "left join carrier as c on c.id = f.id_carrier "
                      "where f.id = :id ");
    queryFact.bindValue(":id",id_fact);
    if (queryFact.exec()){
        queryFact.next();
    } else {
        QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),queryFact.lastError().text(),QMessageBox::Ok);
    }

    otprInfo = new OtprInfo(queryFact.value(6).toInt(),this);
    prodInfo = new OtprInfo(queryFact.value(3).toInt(),this);
    polInfo = new PolInfo(queryFact.value(8).toInt(),this);
    platInfo = new PolInfo(queryFact.value(11).toInt(),this);


    QSqlQuery query;
    query.prepare("select naim, diam, ed, kvo, cena, stoi, nalog, nds, itogo, cod from calc_fact(:id_fact, :disc)");
    query.bindValue(":id_fact",id_fact);
    query.bindValue(":disc",disc());
    if (!query.exec()){
        QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        while (query.next()){
            factData f;
            f.naim=query.value(0).toString();
            f.diam=query.value(1).toDouble();
            f.ed=query.value(2).toString();
            f.kvo=query.value(3).toDouble();
            f.cena=query.value(4).toDouble();
            f.stoi=query.value(5).toDouble();
            f.nalog=query.value(6).toDouble();
            f.nds=query.value(7).toDouble();
            f.itogo=query.value(8).toDouble();
            f.cod=query.value(9).toDouble();
            f.edCod=eds.value(query.value(2).toString());
            contents.push_back(f);
        }
    }

    queryTotal.prepare("select sum(stoi), sum(nalog), sum(itogo), sum(kvo) from calc_fact(:id_fact, :disc)");
    queryTotal.bindValue(":id_fact",id_fact);
    queryTotal.bindValue(":disc",disc());
    if (queryTotal.exec()){
        queryTotal.next();
    } else {
        QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),queryTotal.lastError().text(),QMessageBox::Ok);
    }
}

QString FactInfo::nums()
{
    return (queryFact.size()<1) ? QString() :  queryFact.value(0).toString();
}

QDate FactInfo::dat()
{
    return (queryFact.size()<1) ? QDate() : queryFact.value(1).toDate();
}

double FactInfo::disc()
{
    return (queryFact.size()<1) ? 0 : queryFact.value(2).toDouble();
}

QString FactInfo::dop()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(14).toString();
}

QString FactInfo::plt()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(10).toString();
}

QString FactInfo::klad()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(17).toString();
}

QString FactInfo::pos_klad()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(16).toString();
}

QString FactInfo::drv()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(18).toString();
}

QString FactInfo::nom_dov()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(4).toString();
}

QDate FactInfo::dat_dov()
{
    return (queryFact.size()<1) ? QDate() : queryFact.value(5).toDate();
}

QString FactInfo::org_dov()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(9).toString();
}

QString FactInfo::pos_pol()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(12).toString();
}

QString FactInfo::nam_pol()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(13).toString();
}

QString FactInfo::lic_dov()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(7).toString();
}

QString FactInfo::carrier()
{
    QString carrier=carrier_nam();
    if (queryFact.size()>0){
        if (!carrier.isEmpty() && !queryFact.value(21).toString().isEmpty()){
            carrier+=QString::fromUtf8(" ИНН ")+queryFact.value(21).toString();
        }
        if (!carrier.isEmpty()){
            carrier+=QString::fromUtf8(" ")+queryFact.value(20).toString();
        }
    }
    return carrier;
}

QString FactInfo::carrier_nam()
{
    QString nam;
    if (queryFact.size()>=1){
        nam=queryFact.value(19).toString();
    }
    return (nam=="-") ? QString() : nam;
}

QString FactInfo::transport()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(22).toString();
}

QString FactInfo::transport_num()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(23).toString();
}

QString FactInfo::drvd()
{
    return (queryFact.size()<1) ? QString() : queryFact.value(24).toString();
}

QVector<factData> FactInfo::data() const
{
    return contents;
}

double FactInfo::sumStoi()
{
    return (queryTotal.size()<1) ? 0 : queryTotal.value(0).toDouble();
}

double FactInfo::sumNalog()
{
    return (queryTotal.size()<1) ? 0 : queryTotal.value(1).toDouble();
}

double FactInfo::sumItogo()
{
    return (queryTotal.size()<1) ? 0 : queryTotal.value(2).toDouble();
}

double FactInfo::sumKvo()
{
    return (queryTotal.size()<1) ? 0 : queryTotal.value(3).toDouble();
}

bool FactInfo::isSurgut()
{
    return (queryFact.size()<1) ? false : (queryFact.value(15).toInt()==21);
}

OtprInfo *FactInfo::otpr() const
{
    return otprInfo;
}

OtprInfo *FactInfo::prod() const
{
    return prodInfo;
}

PolInfo *FactInfo::pol() const
{
    return polInfo;
}

PolInfo *FactInfo::plat() const
{
    return platInfo;
}

OtprInfo::OtprInfo(int id_otpr, QObject *parent) : QObject(parent)
{
    query.prepare("select nam, fnam, adr, rs, ks, bank, city, bik, innkpp, telboss, telbuh, telfax, okpo, empl from hoz where id= :id");
    query.bindValue(":id",id_otpr);
    if (query.exec()){
        query.next();
    } else {
        QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

QString OtprInfo::nam()
{
    return (query.size()<1) ? QString() : query.value(0).toString();
}

QString OtprInfo::fnam()
{
    return (query.size()<1) ? QString() : query.value(1).toString();
}

QString OtprInfo::adr()
{
    return (query.size()<1) ? QString() : query.value(2).toString();
}

QString OtprInfo::rs()
{
    return (query.size()<1) ? QString() : query.value(3).toString();
}

QString OtprInfo::ks()
{
    return (query.size()<1) ? QString() : query.value(4).toString();
}

QString OtprInfo::bank()
{
    return (query.size()<1) ? QString() : query.value(5).toString();
}

QString OtprInfo::city()
{
    return (query.size()<1) ? QString() : query.value(6).toString();
}

QString OtprInfo::bik()
{
    return (query.size()<1) ? QString() : query.value(7).toString();
}

QString OtprInfo::innkpp()
{
    return (query.size()<1) ? QString() : query.value(8).toString();
}

QString OtprInfo::tels()
{
    return (query.size()<1) ? QString() :
                              (query.value(9).toString()+QString::fromUtf8(", ")+query.value(10).toString()+QString::fromUtf8("(бух.), ")+query.value(11).toString()+QString::fromUtf8("(отд. сбыта)"));
}

QString OtprInfo::nach()
{
    return (query.size()<1) ? QString() : query.value(13).toString();
}

QString OtprInfo::okpo()
{
    return (query.size()<1) ? QString() : query.value(12).toString();
}

QString OtprInfo::info()
{
    QString otpr=fnam();
    if (!adr().isEmpty()) otpr+=QString::fromUtf8(", ")+adr();
    if (!tels().isEmpty()) otpr+=QString::fromUtf8(", тел. ")+tels();
    if (!innkpp().isEmpty()) otpr+=QString::fromUtf8(", ИНН/КПП ")+innkpp();
    return otpr;
}

QString OtprInfo::totalInfo()
{
    QString otpr=info();
    if (!bank().isEmpty()) otpr+=QString::fromUtf8(", в банке ")+bank()+QString::fromUtf8(" ")+city();
    if (!rs().isEmpty()) otpr+=QString::fromUtf8(", р/с ")+rs();
    if (!ks().isEmpty()) otpr+=QString::fromUtf8(", к/с ")+ks();
    if (!bik().isEmpty()) otpr+=QString::fromUtf8(", БИК ")+bik();
    return otpr;
}

PolInfo::PolInfo(int id_pol, QObject *parent) : QObject(parent)
{
    query.prepare("select short, naim, COALESCE(NULLIF(adres_egrul,''), adres), rs, ks, bank, city, bik, innkpp, telef, okpo from poluch where id= :id");
    query.bindValue(":id",id_pol);
    if (query.exec()){
        query.next();
    } else {
        QMessageBox::critical(NULL,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
}

QString PolInfo::nam()
{
    return (query.size()<1) ? QString() : query.value(0).toString();
}

QString PolInfo::fnam()
{
    return (query.size()<1) ? QString() : query.value(1).toString();
}

QString PolInfo::adr()
{
    return (query.size()<1) ? QString() : query.value(2).toString();
}

QString PolInfo::rs()
{
    return (query.size()<1) ? QString() : query.value(3).toString();
}

QString PolInfo::ks()
{
    return (query.size()<1) ? QString() : query.value(4).toString();
}

QString PolInfo::bank()
{
    return (query.size()<1) ? QString() : query.value(5).toString();
}

QString PolInfo::city()
{
    return (query.size()<1) ? QString() : query.value(6).toString();
}

QString PolInfo::bik()
{
    return (query.size()<1) ? QString() : query.value(7).toString();
}

QString PolInfo::innkpp()
{
    return (query.size()<1) ? QString() : query.value(8).toString();
}

QString PolInfo::tels()
{
    return (query.size()<1) ? QString() :query.value(9).toString();
}

QString PolInfo::okpo()
{
    return (query.size()<1) ? QString() : query.value(10).toString();
}

QString PolInfo::info()
{
    QString pol=fnam();
    if(!adr().isEmpty()) pol+=QString(", ")+adr();
    if (!tels().isEmpty()) pol+=QString::fromUtf8(", тел. ")+tels();
    if (!innkpp().isEmpty()) pol+=QString::fromUtf8(", ИНН/КПП ")+innkpp();
    return pol;
}

QString PolInfo::totalInfo()
{
    QString pol=info();
    if (!bank().isEmpty()) pol+=QString::fromUtf8(", в банке ")+bank()+QString(" ")+city();
    if (!rs().isEmpty()) pol+=QString::fromUtf8(", р/с ")+rs();
    if (!ks().isEmpty()) pol+=QString::fromUtf8(", к/с ")+ks();
    if (!bik().isEmpty()) pol+=QString::fromUtf8(", БИК ")+bik();
    return pol;
}
