#include "modelfact.h"

ModelFact::ModelFact(QObject *parent) : DbTableModel(QString::fromUtf8("facts"),parent)
{
    addColumn(QString("id"),QString::fromUtf8("id"),true,TYPE_INT);
    addColumn(QString("nums"),QString::fromUtf8("Номер"),false,TYPE_STRING);
    addColumn(QString("dat"),QString::fromUtf8("Дата"),false,TYPE_DATE);
    addColumn(QString("id_p"),QString::fromUtf8("Плательщик"),false,TYPE_STRING,NULL,Rels::instance()->relPol);
    addColumn(QString("id_hoz"),QString::fromUtf8("Поставщик"),false,TYPE_STRING,NULL,Rels::instance()->relPost);
    addColumn(QString("id_s"),QString::fromUtf8("Сертификат"),false,TYPE_STRING,NULL,Rels::instance()->relSert);
    addColumn(QString("disc"),QString::fromUtf8("Скидка"),false,TYPE_DOUBLE, new QDoubleValidator(0,200,2,this));
    addColumn(QString("id_g"),QString::fromUtf8("Получатель"),false,TYPE_STRING,NULL,Rels::instance()->relPol);
    addColumn(QString("dop"),QString::fromUtf8("Дополнит."),false,TYPE_STRING);
    addColumn(QString("plt"),QString::fromUtf8("Пл.рас.док"),false,TYPE_STRING);
    addColumn(QString("otpr"),QString::fromUtf8("Отправитель"),false,TYPE_STRING,NULL,Rels::instance()->relPost);
    addColumn(QString("klad"),QString::fromUtf8("Отп. ФИО"),false,TYPE_STRING);
    addColumn(QString("drv"),QString::fromUtf8("Водитель"),false,TYPE_STRING);
    addColumn(QString("cherez"),QString::fromUtf8("Через"),false,TYPE_STRING);
    addColumn(QString("nom_dov"),QString::fromUtf8("Номер доверенности"),false,TYPE_STRING);
    addColumn(QString("dat_dov"),QString::fromUtf8("Дата доверенности"),false,TYPE_DATE);
    addColumn(QString("org_dov"),QString::fromUtf8("Огранизац. дов."),false,TYPE_STRING);
    addColumn(QString("pos_pol"),QString::fromUtf8("Получ. долж."),false,TYPE_STRING);
    addColumn(QString("nam_pol"),QString::fromUtf8("Получ. ФИО"),false,TYPE_STRING);
    addColumn(QString("lic_dov"),QString::fromUtf8("Лицо дов."),false,TYPE_STRING);
    addColumn(QString("id_carrier"),QString::fromUtf8("Перевозчик"),false,TYPE_STRING,NULL,Rels::instance()->relCarrier);
    addColumn(QString("transport"),QString::fromUtf8("Транспорт"),false,TYPE_STRING);
    addColumn(QString("transport_num"),QString::fromUtf8("Регистрац. номер"),false,TYPE_STRING);
    addColumn(QString("pos_klad"),QString::fromUtf8("Отп. додж."),false,TYPE_STRING);
    addColumn(QString("drvd"),QString::fromUtf8("Тн. додж."),false,TYPE_STRING);
    addColumn(QString("grm"),QString::fromUtf8("Груз.мест"),false,TYPE_INT,new QIntValidator(1,999999,this));

    setSort("facts.dat, facts.nums");
    setDefaultValue(6,0);
    setDefaultValue(23,QString::fromUtf8("Заведующий складом"));
    setDefaultValue(24,QString::fromUtf8("водитель"));
}

bool ModelFact::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    setDefaultValue(4,Rels::instance()->current_hoz);
    setDefaultValue(10,Rels::instance()->current_hoz);
    return DbTableModel::insertRow(row,parent);
}

void ModelFact::refresh(QDate begDate, QDate endDate, int id_pol)
{
    QString flt;
    flt=QString("facts.dat between '")+begDate.toString("yyyy-MM-dd")+QString("' and '")+endDate.toString("yyyy-MM-dd")+QString("'");
    if (id_pol!=-1){
        flt+=" and facts.id_p = "+QString::number(id_pol);
    }
    setFilter(flt);
    select();
}
