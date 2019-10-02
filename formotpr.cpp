#include "formotpr.h"
#include "ui_formotpr.h"

FormOtpr::FormOtpr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormOtpr)
{
    ui->setupUi(this);

    modelOtpr = new DbTableModel("hoz",this);
    QStringList cols;
    cols<<"id"<<"nam"<<"lit"<<"fnam"<<"adr"<<"adrdop"<<"rs"<<"ks"<<"bank"<<"city"<<"bik"<<"innkpp"<<"boss"<<"telboss"<<"buh"<<"telbuh"<<"empl"<<"telfax"<<"okpo";
    modelOtpr->addColumn(QString("id"),QString("id"),true,TYPE_INT);
    modelOtpr->addColumn(QString("nam"),QString("Крат. наименование"),false,TYPE_STRING);
    for (int i=2; i<cols.size(); i++){
        modelOtpr->addColumn(cols.at(i),cols.at(i),false,TYPE_STRING);
    }
    modelOtpr->setSort("hoz.nam");
    modelOtpr->select();

    ui->tableView->setModel(modelOtpr);
    for (int i=0; i<ui->tableView->model()->columnCount(); i++){
        if (i!=1){
            ui->tableView->setColumnHidden(i,true);
        }
    }
    ui->tableView->setColumnWidth(1,150);

    mapper = new DbMapper(ui->tableView,this);
    mapper->addMapping(ui->lineEditNam,1);
    mapper->addMapping(ui->lineEditLit,2);
    mapper->addMapping(ui->lineEditFnam,3);
    mapper->addMapping(ui->lineEditAdr,4);
    mapper->addMapping(ui->lineEditAdrdop,5);
    mapper->addMapping(ui->lineEditRs,6);
    mapper->addMapping(ui->lineEditKs,7);
    mapper->addMapping(ui->lineEditBank,8);
    mapper->addMapping(ui->lineEditCity,9);
    mapper->addMapping(ui->lineEditBik,10);
    mapper->addMapping(ui->lineEditInnkpp,11);
    mapper->addMapping(ui->lineEditBoss,12);
    mapper->addMapping(ui->lineEditTelboss,13);
    mapper->addMapping(ui->lineEditBuh,14);
    mapper->addMapping(ui->lineEditTelbuh,15);
    mapper->addMapping(ui->lineEditEmpl,16);
    mapper->addMapping(ui->lineEditTelfax,17);
    mapper->addMapping(ui->lineEditOkpo,18);

    ui->horizontalLayoutMapper->insertWidget(0,mapper);
}

FormOtpr::~FormOtpr()
{
    delete ui;
}

void FormOtpr::keyPressEvent(QKeyEvent *pe)
{
    if (pe->key()==Qt::Key_Insert) mapper->slotNew();
    //QDialog::keyPressEvent(pe);
}

