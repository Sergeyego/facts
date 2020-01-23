#include "dialogselectbill.h"
#include "ui_dialogselectbill.h"

DialogSelectBill::DialogSelectBill(int id_pol, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectBill)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    QSqlQuery query;
    query.prepare("select id, nums || '-' || dat from bills where id_p = :id_pol order by dat desc");
    query.bindValue(":id_pol",id_pol);
    if (query.exec()){
        model->setQuery(query);
        model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Счет"));
    } else {
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnWidth(1,150);
    if (ui->tableView->model()->rowCount()){
        ui->tableView->selectRow(0);
    }
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
}

DialogSelectBill::~DialogSelectBill()
{
    delete ui;
}

int DialogSelectBill::idBill()
{
    int id_bill=ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0)).toInt();
    return id_bill;
}
