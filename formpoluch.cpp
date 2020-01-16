#include "formpoluch.h"
#include "ui_formpoluch.h"
#include <QDebug>

FormPoluch::FormPoluch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormPoluch)
{
    ui->setupUi(this);

    modelPoluch = new DbTableModel("poluch",this);
    modelPoluch->addColumn(QString("id"),QString("id"),true,TYPE_INT);
    modelPoluch->addColumn(QString("short"),QString::fromUtf8("Крат. наименование"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("naim"),QString::fromUtf8("naim"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("adres"),QString::fromUtf8("adres"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("telef"),QString::fromUtf8("telef"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("innkpp"),QString::fromUtf8("innkpp"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("id_kat"),QString::fromUtf8("id_kat"),false,TYPE_STRING,NULL,Rels::instance()->relKat);
    modelPoluch->addColumn(QString("bank"),QString::fromUtf8("bank"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("city"),QString::fromUtf8("city"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("rs"),QString::fromUtf8("rs"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("ks"),QString::fromUtf8("ks"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("bik"),QString::fromUtf8("bik"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("okpo"),QString::fromUtf8("okpo"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("adres_egrul"),QString::fromUtf8("adres_egrul"),false,TYPE_STRING);
    modelPoluch->addColumn(QString("naim_en"),QString::fromUtf8("naim"),false,TYPE_STRING);

    modelPoluch->setSort("poluch.short");
    modelPoluch->select();

    ui->tableView->setModel(modelPoluch);
    for (int i=0; i<ui->tableView->model()->columnCount(); i++){
        if (i!=1){
            ui->tableView->setColumnHidden(i,true);
        }
    }
    ui->tableView->setColumnWidth(1,200);

    mapper = new DbMapper(ui->tableView,this);
    mapper->addMapping(ui->lineEditShort,1);
    mapper->addMapping(ui->lineEditNaim,2);
    mapper->addMapping(ui->lineEditAdres,3);
    mapper->addMapping(ui->lineEditTelef,4);
    mapper->addMapping(ui->lineEditInnKpp,5);
    mapper->addMapping(ui->comboBoxKat,6);
    mapper->addMapping(ui->lineEditBank,7);
    mapper->addMapping(ui->lineEditCity,8);
    mapper->addMapping(ui->lineEditRs,9);
    mapper->addMapping(ui->lineEditKs,10);
    mapper->addMapping(ui->lineEditBIK,11);
    mapper->addMapping(ui->lineEditOkpo,12);
    mapper->addMapping(ui->lineEditAdresEgrul,13);
    mapper->addMapping(ui->lineEditNaimEn,14);

    ui->horizontalLayoutMapper->insertWidget(0,mapper);
}

FormPoluch::~FormPoluch()
{
    delete ui;
}

void FormPoluch::keyPressEvent(QKeyEvent *pe)
{
    if (pe->key()==Qt::Key_Insert) mapper->slotNew();
    //QDialog::keyPressEvent(pe);
}

