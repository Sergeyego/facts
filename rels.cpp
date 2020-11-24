#include "rels.h"

Rels* Rels::rels_instance=nullptr;

Rels *Rels::instance()
{
    if (rels_instance==nullptr)
        rels_instance = new Rels();
    return rels_instance;
}

Rels::Rels(QObject *parent) : QObject(parent)
{
    relProvol = new DbRelation(QString("select id, nam from provol order by nam"),0,1,this);
    relDiam = new DbRelation(QString("select id, sdim from diam order by sdim"),0,1,this);
    relPack = new DbRelation(QString("select id, nam, short, id_pack_def from wire_pack_kind order by nam"),0,2,this);
    relPol = new DbRelation(QString("select id, short, naim from poluch order by short"),0,1,this);
    relPost = new DbRelation(QString("select id, nam from hoz order by nam"),0,1,this);
    relCarrier = new DbRelation(QString("select id, short from carrier order by short"),0,1,this);
    relElMark = new DbRelation(QString("select id, marka from elrtr order by marka"),0,1,this);
    relSert = new DbRelation(QString("select id, nom_s||'-'||dat_vid, id_pol from sertifikat order by dat_vid desc"),0,1,this);
    relKat = new DbRelation(QString("select id, nam from pol_kat order by nam"),0,1,this);

    modelKlad = new DbRelationalModel(QString("select distinct klad from facts where dat>='"+QDate::currentDate().addYears(-1).toString("yyyy-MM-dd")+"'"),this);
    modelElPriceDate = new DbRelationalModel(QString("select distinct dat from cena order by dat desc"),this);
    modelWirePriceDate = new DbRelationalModel(QString("select distinct dat from wire_cena order by dat desc"),this);
    refreshCurrentHoz();
}

void Rels::refreshCurrentHoz()
{
    QSqlQuery query;
    query.prepare("select max(id) from hoz where current=true");
    if (query.exec()){
        while (query.next()){
            current_hoz=query.value(0).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
}

void Rels::refresh()
{
    relProvol->model()->refresh();
    relDiam->model()->refresh();
    relPack->model()->refresh();
    relPol->model()->refresh();
    relPost->model()->refresh();
    relCarrier->model()->refresh();
    relElMark->model()->refresh();
    relSert->model()->refresh();
    relKat->model()->refresh();

    modelKlad->refresh();
    modelElPriceDate->refresh();
    modelWirePriceDate->refresh();
    refreshCurrentHoz();

    emit sigRefresh();
}
