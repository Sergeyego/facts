#include "formelprice.h"
#include "ui_formelprice.h"

FormElPrice::FormElPrice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormElPrice)
{
    ui->setupUi(this);
    dbXlsx = new DbXlsx(ui->tableView,QString::fromUtf8("Прайс электродов"),this);

    ui->cmdNew->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton)));
    ui->cmdSave->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton)));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->doubleSpinBoxNDS->setValue(20.0);

    modelPrice = new ModelElPrice(this);

    ui->comboBoxDate->setModel(Rels::instance()->modelElPriceDate);
    refresh();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->setModel(modelPrice);

    ui->tableView->setColumnWidth(0,160);
    ui->tableView->setColumnWidth(1,70);
    ui->tableView->setColumnWidth(2,70);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->setColumnWidth(4,70);

    connect(ui->cmdNew,SIGNAL(clicked()),this,SLOT(newPrice()));
    connect(ui->comboBoxDate,SIGNAL(currentIndexChanged(int)),this,SLOT(refresh()));
    connect(ui->cmdSave,SIGNAL(clicked(bool)),dbXlsx,SLOT(saveToFile()));
}

FormElPrice::~FormElPrice()
{
    delete ui;
}

void FormElPrice::keyPressEvent(QKeyEvent *pe)
{
    pe->ignore();
}

void FormElPrice::newPrice()
{
    QDate newDate=ui->dateEdit->date();
    int res=QMessageBox::question(this,QString::fromUtf8("Подтвердите действие"),QString::fromUtf8("Создать новый прайс-лист от ")+newDate.toString("dd.MM.yy")+QString::fromUtf8("?"));
    if (res==QMessageBox::Yes){
        ui->comboBoxDate->blockSignals(true);
        modelPrice->newprice(newDate,ui->doubleSpinBoxNDS->value());
        if (ui->comboBoxDate->count()>=1){
            ui->comboBoxDate->setCurrentIndex(ui->comboBoxDate->findData(newDate,Qt::EditRole));
        }
        refresh();
        ui->comboBoxDate->blockSignals(false);
    }
}

void FormElPrice::refresh()
{
    if (ui->comboBoxDate->currentIndex()>=0 && ui->comboBoxDate->count()>=1){
        modelPrice->refresh(ui->comboBoxDate->currentData(Qt::EditRole).toDate());
    }
}
