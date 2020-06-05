#include "formch.h"
#include "ui_formch.h"

FormCh::FormCh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormCh)
{
    ui->setupUi(this);
    loadSettings();
    ui->cmdUpd->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    ui->comboBoxFltPlat->setModel(Rels::instance()->relPol->model());
    ui->comboBoxFltPlat->setModelColumn(Rels::instance()->relPol->columnDisplay());
    ui->comboBoxFltPlat->completer()->setCompletionMode(QCompleter::PopupCompletion);

    ui->comboBoxPEl->setModel(Rels::instance()->modelElPriceDate);
    ui->comboBoxPWire->setModel(Rels::instance()->modelWirePriceDate);

    modelBill = new ModelBill(this);
    ui->tableViewBill->setModel(modelBill);
    ui->tableViewBill->setColumnHidden(0,true);
    for (int i=4; i<ui->tableViewBill->model()->columnCount();i++){
        ui->tableViewBill->setColumnHidden(i,true);
    }
    ui->tableViewBill->setColumnWidth(1,60);
    ui->tableViewBill->setColumnWidth(2,70);
    ui->tableViewBill->setColumnWidth(3,180);

    totalBillModel = new TotalBillModel(this);
    totalMapper = new QDataWidgetMapper(this);
    totalMapper->setModel(totalBillModel);
    totalMapper->addMapping(ui->lineEditStoi,0);
    totalMapper->addMapping(ui->lineEditNds,1);
    totalMapper->addMapping(ui->lineEditItogo,2);
    totalMapper->toFirst();

    modelBillEl = new ModelBillEl(this);
    ui->tableViewEl->setModel(modelBillEl);
    ui->tableViewEl->setColumnHidden(0,true);
    ui->tableViewEl->setColumnHidden(1,true);
    ui->tableViewEl->setColumnWidth(2,110);
    ui->tableViewEl->setColumnWidth(3,50);
    ui->tableViewEl->setColumnWidth(4,100);
    ui->tableViewEl->setColumnWidth(5,80);
    ui->tableViewEl->setColumnWidth(6,50);
    ui->tableViewEl->setColumnWidth(7,50);
    ui->tableViewEl->setColumnWidth(8,40);
    ui->tableViewEl->setColumnWidth(9,80);
    ui->tableViewEl->setColumnWidth(10,50);

    modelBillWire = new ModelBillWire(this);
    ui->tableViewWire->setModel(modelBillWire);
    ui->tableViewWire->setColumnHidden(0,true);
    ui->tableViewWire->setColumnHidden(1,true);
    ui->tableViewWire->setColumnWidth(2,130);
    ui->tableViewWire->setColumnWidth(3,60);
    ui->tableViewWire->setColumnWidth(4,100);
    ui->tableViewWire->setColumnWidth(5,100);
    ui->tableViewWire->setColumnWidth(6,80);
    ui->tableViewWire->setColumnWidth(7,50);
    ui->tableViewWire->setColumnWidth(8,50);
    ui->tableViewWire->setColumnWidth(9,40);
    ui->tableViewWire->setColumnWidth(10,80);
    ui->tableViewWire->setColumnWidth(11,50);

    mapper = new DbMapper(ui->tableViewBill,this);
    ui->horizontalLayoutCmd->insertWidget(0,mapper);
    mapper->addMapping(ui->lineEditNum, 1);
    mapper->addMapping(ui->dateEdit,2);
    mapper->addMapping(ui->comboBoxPlatCh,3);
    mapper->addMapping(ui->comboBoxPostCh,4);
    mapper->addMapping(ui->lineEditDisc,5);

    mapper->addLock(ui->cmdUpd);
    mapper->addLock(ui->cmdCen);
    mapper->addLock(ui->comboBoxPEl);
    mapper->addLock(ui->comboBoxPWire);
    mapper->addEmptyLock(ui->cmdBill);
    mapper->addEmptyLock(ui->cmdBillOf);
    mapper->addEmptyLock(ui->tableViewEl);
    mapper->addEmptyLock(ui->tableViewWire);

    mapper->setDefaultFocus(3);

    connect(ui->checkBoxPlat,SIGNAL(clicked(bool)),ui->comboBoxFltPlat,SLOT(setEnabled(bool)));
    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refreshBill()));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updDataBill(int)));
    connect(modelBill,SIGNAL(sigUpd()),this,SLOT(setDatePrice()));
    connect(modelBillEl,SIGNAL(sigUpd()),this,SLOT(updTotal()));
    connect(modelBillWire,SIGNAL(sigUpd()),this,SLOT(updTotal()));
    connect(ui->cmdCen,SIGNAL(clicked(bool)),this,SLOT(insCen()));
    connect(Rels::instance(),SIGNAL(sigRefresh()),mapper,SLOT(refresh()));
    connect(ui->cmdBill,SIGNAL(clicked(bool)),this,SLOT(createBill()));
    connect(ui->cmdBillOf,SIGNAL(clicked(bool)),this,SLOT(createBillOf()));

    refreshBill();
}

FormCh::~FormCh()
{
    saveSettings();
    delete ui;
}

void FormCh::keyPressEvent(QKeyEvent *p)
{
    if(p->key()==Qt::Key_Escape) mapper->slotEsc();
    if (p->key()==Qt::Key_Insert) mapper->slotNew();
    QWidget::keyPressEvent(p);
}

void FormCh::loadSettings()
{
    QSettings settings("szsm", "facts");
    ui->splitter->restoreState(settings.value("bill_splitter").toByteArray());
}

void FormCh::saveSettings()
{
    QSettings settings("szsm", "facts");
    settings.setValue("bill_splitter",ui->splitter->saveState());
}

void FormCh::updDataBill(int index)
{
    int id_bill=modelBill->data(modelBill->index(index,0),Qt::EditRole).toInt();
    modelBillEl->refresh(id_bill);
    modelBillWire->refresh(id_bill);
    setDatePrice();
    updTotal();
}

void FormCh::refreshBill()
{
    int id_pol=-1;
    if (ui->checkBoxPlat->isChecked()){
        QModelIndex ind=ui->comboBoxFltPlat->model()->index(ui->comboBoxFltPlat->currentIndex(),0);
        id_pol=ui->comboBoxFltPlat->model()->data(ind,Qt::EditRole).toInt();
    }
    modelBill->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_pol);
}

void FormCh::setDatePrice()
{
    const QDate date=modelBill->data(modelBill->index(mapper->currentIndex(),2),Qt::EditRole).toDate();
    if (ui->comboBoxPEl->count()>0){
        QDate tmpDate=ui->comboBoxPEl->model()->data(ui->comboBoxPEl->model()->index(0,0)).toDate();
        int i=0;
        while(date<tmpDate && i<ui->comboBoxPEl->count()-1){
            i++;
            tmpDate=ui->comboBoxPEl->model()->data(ui->comboBoxPEl->model()->index(i,0)).toDate();
        }
        ui->comboBoxPEl->setCurrentIndex(i);
    }
    if (ui->comboBoxPWire->count()>0){
        int i=0;
        QDate tmpDate=ui->comboBoxPWire->model()->data(ui->comboBoxPWire->model()->index(0,0)).toDate();
        while(date<tmpDate && i<ui->comboBoxPWire->count()-1){
            i++;
            tmpDate=ui->comboBoxPWire->model()->data(ui->comboBoxPWire->model()->index(i,0)).toDate();
        }
        ui->comboBoxPWire->setCurrentIndex(i);
    }
}

void FormCh::updTotal()
{
    int id_bill=modelBill->data(modelBill->index(mapper->currentIndex(),0),Qt::EditRole).toInt();
    totalBillModel->refresh(id_bill);
    totalMapper->toFirst();
}

void FormCh::insCen()
{
    QDate dateEl=ui->comboBoxPEl->currentData(Qt::EditRole).toDate();
    QDate dateWire=ui->comboBoxPWire->currentData(Qt::EditRole).toDate();
    modelBillEl->insCenNds(dateEl);
    modelBillWire->insCenNds(dateWire);
    updTotal();
}

void FormCh::createBill()
{
    QString templ=QString::fromUtf8("templates/bill.xlsx");
    if (!QFile::exists(templ)){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Ошибка открытия шаблона ")+templ,QMessageBox::Ok);
        return;
    }
    int id_bill=modelBill->data(modelBill->index(mapper->currentIndex(),0),Qt::EditRole).toInt();
    double disc=modelBill->data(modelBill->index(mapper->currentIndex(),5),Qt::EditRole).toDouble();
    Document xlsx(templ);
    Worksheet *ws=xlsx.currentWorksheet();
    QString head=QString::fromUtf8("bill");

    QSqlQuery query;
    query.prepare("select h.innkpp, h.bik, h.rs, h.ks, h.nam, h.bank, h.city, b.nums, b.dat, h.fnam, COALESCE(NULLIF(h.adr_egrul,''), h.adr), h.telboss, h.telbuh, h.telfax, "
                  "p.innkpp, p.naim, COALESCE(NULLIF(p.adres_egrul,''), p.adres), p.telef "
                  "from bills as b "
                  "inner join hoz as h on h.id = b.id_hoz "
                  "inner join poluch as p on p.id = b.id_p "
                  "where b.id = :id ");
    query.bindValue(":id",id_bill);
    if (!query.exec()){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        while (query.next()){
            QStringList innkpp=query.value(0).toString().split(QChar('/'));
            if (innkpp.size()>1){
                ws->writeString(CellReference("D4"),innkpp.at(0));
                ws->writeString(CellReference("M4"),innkpp.at(1));
            }
            ws->writeString(CellReference("W4"),query.value(1).toString());
            ws->writeString(CellReference("W5"),query.value(2).toString());
            ws->writeString(CellReference("W7"),query.value(3).toString());
            ws->writeString(CellReference("B6"),query.value(4).toString());
            ws->writeString(CellReference("B8"),query.value(5).toString()+QString(" ")+query.value(6).toString());
            head=QString::fromUtf8("Счет № ");
            head+=query.value(7).toString();
            head+=QString::fromUtf8(" от ")+query.value(8).toDate().toString("dd.MM.yyyy");
            ws->writeString(CellReference("B11"),head);
            QString post=query.value(9).toString();
            post+=QString::fromUtf8(" Почтовый адрес: ");
            post+=query.value(10).toString();
            post+=QString::fromUtf8(" Телефон/факс: ");
            post+=query.value(11).toString()+QString::fromUtf8("(дир.) ");
            post+=query.value(12).toString()+QString::fromUtf8("(бух.) ");
            post+=query.value(13).toString()+QString::fromUtf8("(отд. сбыта)");
            ws->writeString(CellReference("H13"),post);
            QString pok=QString::fromUtf8("ИНН/КПП: ");
            pok+=query.value(14).toString()+QString::fromUtf8(" ");
            pok+=query.value(15).toString();
            pok+=QString::fromUtf8(" Адрес: ")+query.value(16).toString();
            if (!query.value(17).toString().isEmpty())
                pok+=QString::fromUtf8(" Телефон: ")+query.value(17).toString();
            ws->writeString(CellReference("H15"),pok);
        }
    }

    int i=0;
    double nds=18;
    query.clear();
    query.prepare("select naim, diam, ed, kvo, cena, stoi, nalog, nds, itogo from calc_bill(:id,:disc)");
    query.bindValue(":id",id_bill);
    query.bindValue(":disc",disc);
    if (!query.exec()){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        ws->insertRows(18,query.size());
        while (query.next()){
            ws->writeNumeric(18+i,2,i+1);
            QString nam=query.value(0).toString();
            if (query.value(1).toDouble()!=0.0){
                nam+= QString::fromUtf8(" ф ")+QString::number(query.value(1).toDouble());
            }
            ws->writeString(18+i,4,nam);
            ws->writeString(18+i,16,query.value(2).toString());
            ws->writeNumeric(18+i,20,query.value(4).toDouble());
            ws->writeNumeric(18+i,24,query.value(3).toDouble());
            ws->writeNumeric(18+i,28,query.value(5).toDouble());
            ws->writeNumeric(18+i,32,query.value(6).toDouble());
            ws->writeNumeric(18+i,35,query.value(8).toDouble());
            nds=query.value(7).toDouble();
            i++;
        }
    }
    query.clear();
    query.prepare("Select sum(stoi), sum(nalog), sum(itogo), sum(kvo) from calc_bill(:id, :disc)");
    query.bindValue(":id",id_bill);
    query.bindValue(":disc",disc);
    if (!query.exec()){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        while (query.next()){
            ws->writeString(18+i,4,QString::fromUtf8("ИТОГО"));
            ws->writeNumeric(18+i,24,query.value(3).toDouble());
            ws->writeNumeric(18+i,28,query.value(0).toDouble());
            ws->writeNumeric(18+i,32,query.value(1).toDouble());
            ws->writeNumeric(18+i,35,query.value(2).toDouble());
            QString tot=QString::fromUtf8("Всего наименований ");
            tot+=intstr(i);
            tot+=QString::fromUtf8(", на сумму ")+QLocale().toString(query.value(2).toDouble(),'f',2)+QString::fromUtf8(" руб.");
            tot+=QString::fromUtf8("  в том числе НДС (")+QLocale().toString(nds,'f',1)+QString::fromUtf8("%) ");
            tot+=QLocale().toString(query.value(1).toDouble(),'f',2)+QString::fromUtf8(" руб.");
            ws->writeString(20+i,2,tot);
            QString prop=money(query.value(2).toDouble());
            if (prop.size()>0){
                prop[0]=prop[0].toUpper();
            }
            ws->writeString(21+i,2,prop);
        }
    }
    QDir dir(QDir::homePath()+QString::fromUtf8("/BILL"));
    if (!dir.exists()) dir.mkdir(dir.path());
    QString fname=head.replace(QRegExp("[^\\w]"), "_")+QString::fromUtf8(".xlsx");
    while (fname.contains("__")){
        fname=fname.replace("__","_");
    }
    QString totalName=dir.path()+QChar('/')+fname;
    xlsx.saveAs(totalName);

    sysCommand(totalName);
    return;
}

void FormCh::createBillOf()
{
    QString templ=QString::fromUtf8("templates/bill_of.xlsx");
    if (!QFile::exists(templ)){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Ошибка открытия шаблона ")+templ,QMessageBox::Ok);
        return;
    }
    int id_bill=modelBill->data(modelBill->index(mapper->currentIndex(),0),Qt::EditRole).toInt();
    double disc=modelBill->data(modelBill->index(mapper->currentIndex(),5),Qt::EditRole).toDouble();
    Document xlsx(templ);
    Worksheet *ws=xlsx.currentWorksheet();
    QString head=QString::fromUtf8("bill");

    QSqlQuery query;
    query.prepare("select h.innkpp, h.bik, h.rs, h.ks, h.nam, h.bank, h.city, b.nums, b.dat, h.fnam, COALESCE(NULLIF(h.adr_egrul,''), h.adr), h.telboss, h.telbuh, h.telfax, "
                  "p.innkpp, p.naim, COALESCE(NULLIF(p.adres_egrul,''), p.adres), p.telef "
                  "from bills as b "
                  "inner join hoz as h on h.id = b.id_hoz "
                  "inner join poluch as p on p.id = b.id_p "
                  "where b.id = :id ");
    query.bindValue(":id",id_bill);
    if (!query.exec()){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        while (query.next()){
            QStringList innkpp=query.value(0).toString().split(QChar('/'));
            if (innkpp.size()>1){
                ws->writeString(CellReference("D4"),innkpp.at(0));
                ws->writeString(CellReference("M4"),innkpp.at(1));
            }
            ws->writeString(CellReference("W4"),query.value(1).toString());
            ws->writeString(CellReference("W5"),query.value(2).toString());
            ws->writeString(CellReference("W7"),query.value(3).toString());
            ws->writeString(CellReference("B6"),query.value(4).toString());
            ws->writeString(CellReference("B8"),query.value(5).toString()+QString(" ")+query.value(6).toString());
            head=QString::fromUtf8("Счет-оферта № ");
            head+=query.value(7).toString();
            head+=QString::fromUtf8(" от ")+query.value(8).toDate().toString("dd.MM.yyyy");
            ws->writeString(CellReference("B11"),head);
            QString post=query.value(9).toString();
            post+=QString::fromUtf8(" Почтовый адрес: ");
            post+=query.value(10).toString();
            post+=QString::fromUtf8(" Телефон/факс: ");
            post+=query.value(11).toString()+QString::fromUtf8("(дир.) ");
            post+=query.value(12).toString()+QString::fromUtf8("(бух.) ");
            post+=query.value(13).toString()+QString::fromUtf8("(отд. сбыта)");
            ws->writeString(CellReference("H13"),post);
            QString pok=QString::fromUtf8("ИНН/КПП: ");
            pok+=query.value(14).toString()+QString::fromUtf8(" ");
            pok+=query.value(15).toString();
            pok+=QString::fromUtf8(" Адрес: ")+query.value(16).toString();
            if (!query.value(17).toString().isEmpty())
                pok+=QString::fromUtf8(" Телефон: ")+query.value(17).toString();
            ws->writeString(CellReference("H15"),pok);
        }
    }

    int i=0;
    double nds=18;
    query.clear();
    query.prepare("select naim, diam, ed, kvo, cena, stoi, nalog, nds, itogo from calc_bill(:id,:disc)");
    query.bindValue(":id",id_bill);
    query.bindValue(":disc",disc);
    if (!query.exec()){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        ws->insertRows(18,query.size());
        while (query.next()){
            ws->writeNumeric(18+i,2,i+1);
            QString nam=query.value(0).toString();
            if (query.value(1).toDouble()!=0.0){
                nam+= QString::fromUtf8(" ф ")+QString::number(query.value(1).toDouble());
            }
            ws->writeString(18+i,4,nam);
            ws->writeString(18+i,16,query.value(2).toString());
            ws->writeNumeric(18+i,20,query.value(4).toDouble());
            ws->writeNumeric(18+i,24,query.value(3).toDouble());
            ws->writeNumeric(18+i,28,query.value(5).toDouble());
            ws->writeNumeric(18+i,32,query.value(6).toDouble());
            ws->writeNumeric(18+i,35,query.value(8).toDouble());
            nds=query.value(7).toDouble();
            i++;
        }
    }
    query.clear();
    query.prepare("Select sum(stoi), sum(nalog), sum(itogo), sum(kvo) from calc_bill(:id, :disc)");
    query.bindValue(":id",id_bill);
    query.bindValue(":disc",disc);
    if (!query.exec()){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    } else {
        while (query.next()){
            ws->writeString(18+i,4,QString::fromUtf8("ИТОГО"));
            ws->writeNumeric(18+i,24,query.value(3).toDouble());
            ws->writeNumeric(18+i,28,query.value(0).toDouble());
            ws->writeNumeric(18+i,32,query.value(1).toDouble());
            ws->writeNumeric(18+i,35,query.value(2).toDouble());
            QString tot=QString::fromUtf8("Всего наименований ");
            tot+=intstr(i);
            tot+=QString::fromUtf8(", на сумму ")+QLocale().toString(query.value(2).toDouble(),'f',2)+QString::fromUtf8(" руб.");
            tot+=QString::fromUtf8("  в том числе НДС (")+QLocale().toString(nds,'f',1)+QString::fromUtf8("%) ");
            tot+=QLocale().toString(query.value(1).toDouble(),'f',2)+QString::fromUtf8(" руб.");
            ws->writeString(20+i,2,tot);
            QString prop=money(query.value(2).toDouble());
            if (prop.size()>0){
                prop[0]=prop[0].toUpper();
            }
            ws->writeString(21+i,2,prop);
        }
    }
    QDir dir(QDir::homePath()+QString::fromUtf8("/BILL"));
    if (!dir.exists()) dir.mkdir(dir.path());
    QString fname=head.replace(QRegExp("[^\\w]"), "_")+QString::fromUtf8(".xlsx");
    while (fname.contains("__")){
        fname=fname.replace("__","_");
    }
    QString totalName=dir.path()+QChar('/')+fname;
    xlsx.saveAs(totalName);

    sysCommand(totalName);
    return;
}
