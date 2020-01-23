#include "formfact.h"
#include "ui_formfact.h"

FormFact::FormFact(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFact)
{
    ui->setupUi(this);
    loadSettings();
    ui->cmdUpd->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));

    ui->comboBoxKlad->setModel(Rels::instance()->modelKlad);
    ui->comboBoxPEl->setModel(Rels::instance()->modelElPriceDate);
    ui->comboBoxPWire->setModel(Rels::instance()->modelWirePriceDate);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());

    ui->comboBoxPolFlt->setModel(Rels::instance()->relPol->model());
    ui->comboBoxPolFlt->setModelColumn(Rels::instance()->relPol->columnDisplay());
    ui->comboBoxPolFlt->completer()->setCompletionMode(QCompleter::PopupCompletion);

    modelFact = new ModelFact(this);
    ui->tableViewFact->setModel(modelFact);
    ui->tableViewFact->setColumnHidden(0,true);
    for (int i=4; i<ui->tableViewFact->model()->columnCount();i++){
        ui->tableViewFact->setColumnHidden(i,true);
    }
    ui->tableViewFact->setColumnWidth(1,60);
    ui->tableViewFact->setColumnWidth(2,70);
    ui->tableViewFact->setColumnWidth(3,180);

    totalFactModel = new TotalFactModel(this);
    totalMapper = new QDataWidgetMapper(this);
    totalMapper->setModel(totalFactModel);
    totalMapper->addMapping(ui->lineEditStoi,0);
    totalMapper->addMapping(ui->lineEditNds,1);
    totalMapper->addMapping(ui->lineEditItogo,2);
    totalMapper->toFirst();

    modelFactEl = new ModelFactEl(this);
    ui->tableViewEl->setModel(modelFactEl);
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

    modelFactWire = new ModelFactWire(this);
    ui->tableViewWire->setModel(modelFactWire);
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

    mapper = new DbMapper(ui->tableViewFact,this);
    ui->horizontalLayoutCmd->insertWidget(0,mapper);

    mapper->addMapping(ui->lineEditNumF, 1);
    mapper->addMapping(ui->dateEditF,2);
    mapper->addMapping(ui->comboBoxPlat,3);
    mapper->addMapping(ui->comboBoxPol,7);
    mapper->addMapping(ui->comboBoxPost,4);
    mapper->addMapping(ui->comboBoxOtpr,10);
    mapper->addMapping(ui->lineEditSert,5);
    //mapper->addMapping(ui->lineEditCherez,13);
    mapper->addMapping(ui->lineEditDop,8);
    mapper->addMapping(ui->lineEditPosPol,17);
    mapper->addMapping(ui->lineEditNamPol,18);
    mapper->addMapping(ui->lineEditPlt,9);
    mapper->addMapping(ui->lineEditNumDov,14);
    mapper->addMapping(ui->dateEditDov,15);
    mapper->addMapping(ui->lineEditOrgDov,16);
    mapper->addMapping(ui->lineEditLicDov,19);
    mapper->addMapping(ui->lineEditDrv,12);
    mapper->addMapping(ui->comboBoxKlad,11);
    mapper->addMapping(ui->lineEditDisc,6);
    mapper->addMapping(ui->comboBoxCarrier,20);
    mapper->addMapping(ui->lineEditTransport,21);
    mapper->addMapping(ui->lineEditTransportNum,22);
    mapper->addMapping(ui->lineEditPosKlad,23);
    mapper->addMapping(ui->lineEditDrvD,24);
    mapper->addEmptyLock(ui->tableViewEl);
    mapper->addEmptyLock(ui->tableViewWire);
    mapper->addLock(ui->checkBoxPolFlt);
    mapper->addEmptyLock(ui->cmdSert);
    mapper->addEmptyLock(ui->cmdBill);
    mapper->addLock(ui->cmdUpd);
    mapper->addLock(ui->cmdCen);
    mapper->addLock(ui->comboBoxPEl);
    mapper->addLock(ui->comboBoxPWire);
    mapper->addEmptyLock(ui->cmdFact);
    mapper->addEmptyLock(ui->cmdNakl);
    mapper->addEmptyLock(ui->cmdTn);

    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(refreshFact()));
    connect(ui->checkBoxPolFlt,SIGNAL(clicked(bool)),ui->comboBoxPolFlt,SLOT(setEnabled(bool)));
    //connect(ui->checkBoxPolFlt,SIGNAL(clicked(bool)),this,SLOT(refreshFact()));
    //connect(ui->comboBoxPolFlt,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshFact()));
    connect(ui->comboBoxPlat,SIGNAL(currentIndexChanged(int)),this,SLOT(selectPlat(int)));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updContents(int)));
    connect(modelFactEl,SIGNAL(sigUpd()),this,SLOT(updTotal()));
    connect(modelFactWire,SIGNAL(sigUpd()),this,SLOT(updTotal()));
    connect(ui->cmdCen,SIGNAL(clicked(bool)),this,SLOT(insCen()));
    connect(modelFact,SIGNAL(sigUpd()),this,SLOT(setDatePrice()));
    connect(ui->cmdSert,SIGNAL(clicked(bool)),this,SLOT(setSert()));
    connect(ui->cmdBill,SIGNAL(clicked(bool)),this,SLOT(setBill()));
    connect(Rels::instance(),SIGNAL(sigRefresh()),mapper,SLOT(refresh()));
    connect(ui->cmdFact,SIGNAL(clicked(bool)),this,SLOT(createFact()));
    connect(ui->cmdNakl,SIGNAL(clicked(bool)),this,SLOT(createTvr()));
    connect(ui->cmdTn,SIGNAL(clicked(bool)),this,SLOT(createTn()));

    refreshFact();

}

FormFact::~FormFact()
{
    saveSettings();
    delete ui;
}

void FormFact::keyPressEvent(QKeyEvent *p)
{
    if (p->key()==Qt::Key_Escape) mapper->slotEsc();
    if (p->key()==Qt::Key_Insert) mapper->slotNew();
    QWidget::keyPressEvent(p);
}

void FormFact::loadSettings()
{
    QSettings settings("szsm", "facts");
    ui->splitter->restoreState(settings.value("facts_splitter").toByteArray());
}

void FormFact::saveSettings()
{
    QSettings settings("szsm", "facts");
    settings.setValue("facts_splitter",ui->splitter->saveState());
}

void FormFact::refreshFact()
{
    int id_pol=-1;
    if (ui->checkBoxPolFlt->isChecked()){
        QModelIndex ind=ui->comboBoxPolFlt->model()->index(ui->comboBoxPolFlt->currentIndex(),0);
        id_pol=ui->comboBoxPolFlt->model()->data(ind,Qt::EditRole).toInt();
    }
    modelFact->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_pol);
}

void FormFact::selectPlat(int ind)
{
    ui->comboBoxPol->setCurrentIndex(ind);
    ui->lineEditOrgDov->setText(ui->comboBoxPlat->model()->data(ui->comboBoxPlat->model()->index(ind,2)).toString());
}

void FormFact::updContents(int ind)
{
    int id_fact=modelFact->data(modelFact->index(ind,0),Qt::EditRole).toInt();
    modelFactEl->refresh(id_fact);
    modelFactWire->refresh(id_fact);
    updTotal();
    setDatePrice();
}

void FormFact::updTotal()
{
    int id_fact=modelFact->data(modelFact->index(mapper->currentIndex(),0),Qt::EditRole).toInt();
    totalFactModel->refresh(id_fact);
    totalMapper->toFirst();
}

void FormFact::insCen()
{
    QDate dateEl=ui->comboBoxPEl->currentData(Qt::EditRole).toDate();
    QDate dateWire=ui->comboBoxPWire->currentData(Qt::EditRole).toDate();
    modelFactEl->insCenNds(dateEl);
    modelFactWire->insCenNds(dateWire);
    updTotal();
}

void FormFact::setDatePrice()
{
    const QDate date=modelFact->data(modelFact->index(mapper->currentIndex(),2),Qt::EditRole).toDate();
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

void FormFact::setSert()
{
    int id_pol=modelFact->data(modelFact->index(mapper->currentIndex(),3),Qt::EditRole).toInt();
    DialogSelectSert d(id_pol);
    if (d.exec()==QDialog::Accepted){
        int id_sert=d.idSert();
        modelFactEl->insDataSert(id_sert);
        modelFactWire->insDataSert(id_sert);
        modelFact->setData(modelFact->index(mapper->currentIndex(),5),id_sert,Qt::EditRole);
        modelFact->submitRow();
    }
}

void FormFact::setBill()
{
    int id_pol=modelFact->data(modelFact->index(mapper->currentIndex(),3),Qt::EditRole).toInt();
    DialogSelectBill d(id_pol);
    if (d.exec()==QDialog::Accepted){
        int id_bill=d.idBill();
        modelFactEl->insDataBill(id_bill);
        modelFactWire->insDataBill(id_bill);
    }
}

void FormFact::createFact()
{
    QString templ=QString::fromUtf8("templates/fact17.xlsx");
    if (!QFile::exists(templ)){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Ошибка открытия шаблона ")+templ,QMessageBox::Ok);
        return;
    }

    int id_fact=modelFact->data(modelFact->index(mapper->currentIndex(),0),Qt::EditRole).toInt();

    Document xlsx(templ);
    Worksheet *ws=xlsx.currentWorksheet();
    QString head=QString::fromUtf8("СЧЕТ-ФАКТУРА ");

    FactInfo info(id_fact);
    head+=info.nums()+QString::fromUtf8(" от ")+info.dat().toString("dd.MM.yyyy");
    ws->writeString(CellReference("AS7"),info.nums());
    QDate d=info.dat();
    ws->writeNumeric(CellReference("BF7"),d.day());
    ws->writeString(CellReference("BN7"),QDate::longMonthName(d.month())+QString::fromUtf8(" ")+QString::number(d.year())+QString::fromUtf8(" года"));
    ws->writeString(CellReference("M10"),info.prod()->fnam());
    ws->writeString(CellReference("I11"),info.prod()->adr());
    ws->writeString(CellReference("Y12"),info.prod()->innkpp());
    ws->writeString(CellReference("AI13"),info.otpr()->fnam()+" "+info.otpr()->adr());
    ws->writeString(CellReference("AH14"),info.pol()->fnam()+" "+info.pol()->adr());
    ws->writeString(CellReference("AS15"),info.plt());
    ws->writeString(CellReference("O16"),info.plat()->fnam());
    ws->writeString(CellReference("I17"),info.plat()->adr());
    ws->writeString(CellReference("AA18"),info.plat()->innkpp());
    if (!info.dop().isEmpty()){
        ws->writeString(CellReference("A21"),QString::fromUtf8("Дополнение: ")+info.dop());
    }

    int i=0;
    ws->insertRows(25,info.data().size()-1,true,true);
    for (factData cnt : info.data()){
        QString nam=cnt.naim;
        if (cnt.diam!=0.0){
            nam+= QString::fromUtf8(" ф ")+QString::number(cnt.diam);
        }
        ws->writeString(25+i,1,nam);
        ws->writeString(25+i,29,cnt.edCod);
        ws->writeString(25+i,35,cnt.ed);
        ws->writeNumeric(25+i,46,cnt.kvo);
        ws->writeNumeric(25+i,54,cnt.cena);
        ws->writeNumeric(25+i,65,cnt.stoi);
        ws->writeString(25+i,90,QString::number(cnt.nds)+QString::fromUtf8("%"));
        ws->writeNumeric(25+i,100,cnt.nalog);
        ws->writeNumeric(25+i,112,cnt.itogo);
        i++;
    }

    ws->writeNumeric(25+i,65,info.sumStoi());
    ws->writeNumeric(25+i,100,info.sumNalog());
    ws->writeNumeric(25+i,112,info.sumItogo());

    QDir dir(QDir::homePath()+QString::fromUtf8("/FACT"));
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

void FormFact::createTvr()
{
    int id_fact=modelFact->data(modelFact->index(mapper->currentIndex(),0),Qt::EditRole).toInt();
    FactInfo info(id_fact);
    if (info.isSurgut()){
        createTvrSurgut(&info);
        return;
    }

    QString templ=QString::fromUtf8("templates/tvr.xlsx");
    if (!QFile::exists(templ)){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Ошибка открытия шаблона ")+templ,QMessageBox::Ok);
        return;
    }

    Document xlsx(templ);
    Worksheet *ws=xlsx.currentWorksheet();
    QString head=QString::fromUtf8("Товарная накладная ");

    head+=info.nums()+QString::fromUtf8(" от ")+info.dat().toString("dd.MM.yyyy");
    ws->writeString(CellReference("AX22"),info.nums());
    ws->writeString(CellReference("BI22"),info.dat().toString("dd.MM.yyyy"));

    ws->writeString(CellReference("A3"),info.otpr()->totalInfo());
    ws->writeString(CellReference("CF3"),info.otpr()->okpo());

    ws->writeString(CellReference("I10"),info.prod()->totalInfo());
    ws->writeString(CellReference("CF9"),info.prod()->okpo());

    ws->writeString(CellReference("L8"),info.pol()->totalInfo());
    ws->writeString(CellReference("CF8"),info.pol()->okpo());

    ws->writeString(CellReference("I12"),info.plat()->totalInfo());
    ws->writeString(CellReference("CF11"),info.plat()->okpo());

    if (ui->checkBoxTtn->isChecked()){
        ws->writeString(CellReference("CF17"),info.nums());
        ws->writeString(CellReference("CF18"),info.dat().toString("dd.MM.yyyy"));
    }

    ws->writeString(CellReference("I14"),info.dop());
    /*-------------*/

    ws->writeString(CellReference("K31"),intstr(info.data().size()));
    ws->writeString(CellReference("AN35"),intstr(info.sumKvo())+QString::fromUtf8(" кг"));
    ws->writeNumeric(CellReference("CA34"),info.sumKvo());
    if(!info.nom_dov().isEmpty()){
        ws->writeString(CellReference("BH40"),info.nom_dov());
        ws->writeNumeric(CellReference("BY40"),info.dat_dov().day());
        ws->writeString(CellReference("CC40"),QDate::longMonthName(info.dat_dov().month()));
        ws->writeNumeric(CellReference("CL40"),info.dat_dov().year());
    }
    ws->writeString(CellReference("BD42"),info.lic_dov());
    ws->writeString(CellReference("AX44"),info.org_dov());
    QString s=money(info.sumItogo());
    QString rub=s.left(s.indexOf(QString::fromUtf8("рубл"))-1);
    QString kop=s.mid(s.indexOf(QString::fromUtf8("копе"))-3,2);
    ws->writeString(CellReference("A44"),rub);
    ws->writeString(CellReference("AM44"),kop);

    ws->writeString(CellReference("BE48"),info.pos_pol());
    ws->writeString(CellReference("BY48"),info.nam_pol());

    ws->writeString(CellReference("L50"),info.pos_klad());
    ws->writeString(CellReference("AG50"),info.klad());

    ws->writeNumeric(CellReference("N53"),info.dat().day());
    ws->writeString(CellReference("R53"),QDate::longMonthName(info.dat().month()));
    ws->writeNumeric(CellReference("AA53"),info.dat().year());

    int i=0;
    const int pos=27;
    ws->insertRows(pos,info.data().size()-1,true,true);
    for (factData cnt : info.data()){
        QString nam=cnt.naim;
        if (cnt.diam!=0.0){
            nam+= QString::fromUtf8(" ф ")+QString::number(cnt.diam);
        }
        ws->writeNumeric(pos+i,col_from_name("A"),i+1);
        ws->writeString(pos+i,col_from_name("D"),nam);
        ws->writeString(pos+i,col_from_name("AC"),cnt.edCod);
        ws->writeString(pos+i,col_from_name("X"),cnt.ed);
        ws->writeNumeric(pos+i,col_from_name("BB"),cnt.kvo);
        ws->writeNumeric(pos+i,col_from_name("BH"),cnt.cena);
        ws->writeNumeric(pos+i,col_from_name("BQ"),cnt.stoi);
        ws->writeNumeric(pos+i,col_from_name("BX"),cnt.nds);
        ws->writeNumeric(pos+i,col_from_name("CB"),cnt.nalog);
        ws->writeNumeric(pos+i,col_from_name("CI"),cnt.itogo);
        i++;
    }

    ws->writeNumeric(pos+i,col_from_name("BB"),info.sumKvo());
    ws->writeNumeric(pos+i,col_from_name("BQ"),info.sumStoi());
    ws->writeNumeric(pos+i,col_from_name("CB"),info.sumNalog());
    ws->writeNumeric(pos+i,col_from_name("CI"),info.sumItogo());



    QDir dir(QDir::homePath()+QString::fromUtf8("/TOVR"));
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

void FormFact::createTvrSurgut(FactInfo *info)
{
    QString templ=QString::fromUtf8("templates/tvr_s.xlsx");
    if (!QFile::exists(templ)){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Ошибка открытия шаблона ")+templ,QMessageBox::Ok);
        return;
    }

    Document xlsx(templ);
    Worksheet *ws=xlsx.currentWorksheet();
    QString head=QString::fromUtf8("Товарная накладная ");
    info->dat();

    head+=info->nums()+QString::fromUtf8(" от ")+info->dat().toString("dd.MM.yyyy");
    ws->writeString(CellReference("AX22"),info->nums());
    ws->writeString(CellReference("BI22"),info->dat().toString("dd.MM.yyyy"));

    ws->writeString(CellReference("A3"),info->otpr()->totalInfo());

    ws->writeString(CellReference("I10"),info->prod()->totalInfo());

    ws->writeString(CellReference("L8"),info->pol()->totalInfo());

    ws->writeString(CellReference("I12"),info->plat()->totalInfo());

    if (ui->checkBoxTtn->isChecked()){
        ws->writeString(CellReference("CF17"),info->nums());
        ws->writeString(CellReference("CF18"),info->dat().toString("dd.MM.yyyy"));
    }

    ws->writeString(CellReference("I14"),info->dop());
    QString dop=info->dop();
    QRegExp reg(QString::fromUtf8("договор\\s+(\\S.+)\\s+от\\s+(\\d+\\.\\d+\\.\\d+)"));
    reg.setCaseSensitivity(Qt::CaseInsensitive);
    if (reg.indexIn(dop)!=-1){
        ws->writeString(CellReference("CF13"),reg.cap(1));
        ws->writeString(CellReference("CF15"),reg.cap(2));
    }

    /*-------------*/

    ws->writeString(CellReference("K31"),intstr(info->data().size()));
    ws->writeString(CellReference("AN35"),intstr(info->sumKvo())+QString::fromUtf8(" кг"));
    ws->writeNumeric(CellReference("CA34"),info->sumKvo());
    if(!info->nom_dov().isEmpty()){
        ws->writeString(CellReference("BH40"),info->nom_dov());
        ws->writeNumeric(CellReference("BY40"),info->dat_dov().day());
        ws->writeString(CellReference("CC40"),QDate::longMonthName(info->dat_dov().month()));
        ws->writeNumeric(CellReference("CL40"),info->dat_dov().year());
    }
    ws->writeString(CellReference("BD42"),info->lic_dov());
    ws->writeString(CellReference("AX44"),info->org_dov());
    QString s=money(info->sumItogo());
    QString rub=s.left(s.indexOf(QString::fromUtf8("рубл"))-1);
    QString kop=s.mid(s.indexOf(QString::fromUtf8("копе"))-3,2);
    ws->writeString(CellReference("A44"),rub);
    ws->writeString(CellReference("AM44"),kop);

    ws->writeString(CellReference("BE48"),info->pos_pol());
    ws->writeString(CellReference("BY48"),info->nam_pol());

    ws->writeString(CellReference("L50"),info->pos_klad());
    ws->writeString(CellReference("AG50"),info->klad());

    ws->writeNumeric(CellReference("N53"),info->dat().day());
    ws->writeString(CellReference("R53"),QDate::longMonthName(info->dat().month()));
    ws->writeNumeric(CellReference("AA53"),info->dat().year());

    int i=0;
    const int pos=27;
    ws->insertRows(pos,info->data().size()-1,true,true);
    for (factData cnt : info->data()){
        QString nam=cnt.naim;
        if (cnt.diam!=0.0){
            nam+= QString::fromUtf8(" ф ")+QString::number(cnt.diam);
        }
        ws->writeNumeric(pos+i,col_from_name("A"),i+1);
        ws->writeString(pos+i,col_from_name("D"),nam);
        ws->writeString(pos+i,col_from_name("AC"),cnt.edCod);
        ws->writeString(pos+i,col_from_name("X"),cnt.ed);
        ws->writeNumeric(pos+i,col_from_name("BB"),cnt.kvo);
        ws->writeNumeric(pos+i,col_from_name("BH"),cnt.cena);
        ws->writeNumeric(pos+i,col_from_name("BQ"),cnt.stoi);
        ws->writeNumeric(pos+i,col_from_name("BX"),cnt.nds);
        ws->writeNumeric(pos+i,col_from_name("CB"),cnt.nalog);
        ws->writeNumeric(pos+i,col_from_name("CI"),cnt.itogo);
        i++;
    }

    ws->writeNumeric(pos+i,col_from_name("BB"),info->sumKvo());
    ws->writeNumeric(pos+i,col_from_name("BQ"),info->sumStoi());
    ws->writeNumeric(pos+i,col_from_name("CB"),info->sumNalog());
    ws->writeNumeric(pos+i,col_from_name("CI"),info->sumItogo());

    ws->writeNumeric(pos+i+1,col_from_name("BB"),info->sumKvo());
    ws->writeNumeric(pos+i+1,col_from_name("BQ"),info->sumStoi());
    ws->writeNumeric(pos+i+1,col_from_name("CB"),info->sumNalog());
    ws->writeNumeric(pos+i+1,col_from_name("CI"),info->sumItogo());

    QDir dir(QDir::homePath()+QString::fromUtf8("/TOVR"));
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

void FormFact::createTn()
{
    int id_fact=modelFact->data(modelFact->index(mapper->currentIndex(),0),Qt::EditRole).toInt();
    FactInfo info(id_fact);

    QString templ=QString::fromUtf8("templates/ttn17.xlsx");
    if (!QFile::exists(templ)){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Ошибка открытия шаблона ")+templ,QMessageBox::Ok);
        return;
    }

    Document xlsx(templ);
    QStringList sheets=xlsx.sheetNames();
    if (sheets.size()<2){
        QMessageBox::critical(this,QString::fromUtf8("Ошибка"),QString::fromUtf8("Шаблон содержит меньше двух листов ")+templ,QMessageBox::Ok);
        return;
    }

    xlsx.selectSheet(sheets.at(0));
    Worksheet *ws=xlsx.currentWorksheet();
    QString head=QString::fromUtf8("ТРАНСПОРТНАЯ НАКЛАДНАЯ ");

    head+=info.nums()+QString::fromUtf8(" от ")+info.dat().toString("dd.MM.yyyy");
    ws->writeString(CellReference("CM10"),info.nums());
    ws->writeString(CellReference("BI10"),info.dat().toString("dd.MM.yyyy"));
    ws->writeString(CellReference("B15"),info.otpr()->info());
    ws->writeString(CellReference("BD15"),info.pol()->info());

    QString gruz;
    if (modelFactEl->rowCount()>0 && !modelFactEl->isAdd()){
        gruz+=QString::fromUtf8("сварочные электроды");
    }
    if (modelFactWire->rowCount()>0 && !modelFactWire->isAdd()){
        if (!gruz.isEmpty()) gruz+=QString::fromUtf8(" ,");
        gruz+=QString::fromUtf8("сварочная проволока");
    }
    ws->writeString(CellReference("B18"),gruz);

    QString kg=QLocale().toString(info.sumKvo())+QString::fromUtf8(" (")+intstr(info.sumKvo())+QString::fromUtf8(" килограмм.) кг");
    ws->writeString(CellReference("B22"),kg);

    ws->writeString(CellReference("B40"),info.otpr()->adr());
    ws->writeString(CellReference("BD40"),info.pol()->adr());
    ws->writeString(CellReference("B42"),info.dat().toString("dd.MM.yyyy"));
    ws->writeString(CellReference("B48"),kg);
    ws->writeString(CellReference("BD48"),kg);

    ws->writeString(CellReference("B50"),info.pos_klad()+QString::fromUtf8("                     ")+info.klad());
    QString vod=info.drvd()+QString::fromUtf8("                        ")+info.drv();
    ws->writeString(CellReference("B52"),vod);
    ws->writeString(CellReference("BD52"),vod);

    ws->writeString(CellReference("B67"),info.dat().toString("dd.MM.yyyy"));

    QString prin=info.carrier();
    if (!prin.isEmpty()){
        prin+=", ";
    }
    prin+=info.drvd();
    if (!prin.isEmpty()){
        prin+=" ";
    }
    prin+=info.drv();
    if (!ui->lineEditPasp->text().isEmpty() && !info.drv().isEmpty()){
        prin+=QString::fromUtf8(" паспорт ")+ui->lineEditPasp->text();
    }
    ws->writeString(CellReference("AD67"),prin);

    xlsx.selectSheet(sheets.at(1));
    ws=xlsx.currentWorksheet();

    ws->writeString(CellReference("B7"),info.carrier());
    ws->writeString(CellReference("B9"),info.drv());
    ws->writeString(CellReference("B12"),info.transport());
    ws->writeString(CellReference("BP12"),info.transport_num());

    ws->writeString(CellReference("B41"),info.otpr()->fnam()+", "+info.otpr()->nach());
    ws->writeString(CellReference("AE41"),info.dat().toString("dd.MM.yyyy"));
    QString car=info.carrier_nam();
    if (!car.isEmpty()){
        car+=", ";
    }
    car+=info.drvd()+" "+info.drv();
    ws->writeString(CellReference("BD41"),car);
    ws->writeString(CellReference("CG41"),info.dat().toString("dd.MM.yyyy"));

    QDir dir(QDir::homePath()+QString::fromUtf8("/TTN"));
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
