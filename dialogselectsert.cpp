#include "dialogselectsert.h"
#include "ui_dialogselectsert.h"
#include <QRegExp>

DialogSelectSert::DialogSelectSert(int id_pol, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectSert)
{
    ui->setupUi(this);
    model = new QSortFilterProxyModel(this);
    model->setSourceModel(Rels::instance()->relSert->model());
    model->setFilterKeyColumn(2);
    model->setFilterFixedString(QString::number(id_pol));
    model->setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Сертификат"));

    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnWidth(1,150);
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(ui->tableView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(select(QModelIndex)));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    ui->tableView->selectRow(0);
}

DialogSelectSert::~DialogSelectSert()
{
    delete ui;
}

int DialogSelectSert::idSert()
{
    return model->data(model->index(selectIndex.row(),0)).toInt();
}

void DialogSelectSert::select(QModelIndex index)
{
    selectIndex=index;
}
