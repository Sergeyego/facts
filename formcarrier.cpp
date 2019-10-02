#include "formcarrier.h"
#include "ui_formcarrier.h"

FormCarrier::FormCarrier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormCarrier)
{
    ui->setupUi(this);

    modelCarrier = new DbTableModel("carrier",this);
    modelCarrier->addColumn(QString("id"),QString::fromUtf8("id"),true,TYPE_INT);
    modelCarrier->addColumn(QString("short"),QString::fromUtf8("Крат. наименование"),false,TYPE_STRING);
    modelCarrier->addColumn(QString("nam"),QString::fromUtf8("nam"),false,TYPE_STRING);
    modelCarrier->addColumn(QString("adres"),QString::fromUtf8("adres"),false,TYPE_STRING);
    modelCarrier->addColumn(QString("inn"),QString::fromUtf8("inn"),false,TYPE_STRING);

    modelCarrier->setSort("carrier.short");
    modelCarrier->select();

    ui->listViewShort->setModel(modelCarrier);
    ui->listViewShort->setModelColumn(1);
    ui->listViewShort->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mapper = new DbMapper(ui->listViewShort,this);
    mapper->addMapping(ui->lineEditShortNam,1);
    mapper->addMapping(ui->lineEditNam,2);
    mapper->addMapping(ui->lineEditAdr,3);
    mapper->addMapping(ui->lineEditInn,4);
    mapper->addLock(ui->listViewShort);

    ui->verticalLayout->addWidget(mapper);

}

FormCarrier::~FormCarrier()
{
    delete ui;
}

void FormCarrier::keyPressEvent(QKeyEvent *pe)
{
    if (pe->key()==Qt::Key_Insert) mapper->slotNew();
}
