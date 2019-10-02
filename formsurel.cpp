#include "formsurel.h"
#include "ui_formsurel.h"

FormSurEl::FormSurEl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSurEl)
{
    ui->setupUi(this);
    modelSurEl = new DbTableModel("sur_el",this);
    modelSurEl->addColumn("id_el",tr("Марка"),true,TYPE_STRING, NULL, Rels::instance()->relElMark);
    modelSurEl->addColumn("id_diam",tr("Диаметр"),true,TYPE_STRING,NULL,Rels::instance()->relDiam);
    modelSurEl->addColumn("str",tr("Название"),false,TYPE_STRING);
    modelSurEl->setSuffix("inner join elrtr on sur_el.id_el = elrtr.id");
    modelSurEl->setSort("elrtr.marka, sur_el.id_diam");
    modelSurEl->select();
    ui->tableView->setModel(modelSurEl);
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setColumnWidth(2,400);
}

FormSurEl::~FormSurEl()
{
    delete ui;
}
