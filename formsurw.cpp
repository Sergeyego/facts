#include "formsurw.h"
#include "ui_formsurw.h"

FormSurW::FormSurW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSurW)
{
    ui->setupUi(this);
    modelSurW = new DbTableModel("sur_w",this);
    modelSurW->addColumn("id_pr",tr("Марка"),true,TYPE_STRING, NULL, Rels::instance()->relProvol);
    modelSurW->addColumn("id_diam",tr("Диаметр"),true,TYPE_STRING, NULL, Rels::instance()->relDiam);
    modelSurW->addColumn("id_pack",tr("Намотка"),true,TYPE_STRING, NULL, Rels::instance()->relPack);
    modelSurW->addColumn("str",tr("Название"),false,TYPE_STRING);
    modelSurW->setSuffix("inner join provol on sur_w.id_pr = provol.id");
    modelSurW->setSort("provol.nam");
    modelSurW->select();
    ui->tableView->setModel(modelSurW);
    ui->tableView->setColumnWidth(0,150);
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,400);
}

FormSurW::~FormSurW()
{
    delete ui;
}
