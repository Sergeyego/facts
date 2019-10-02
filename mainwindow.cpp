#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadSettings();
    formFact = new FormFact(this);
    ui->tab_1->setLayout(new QVBoxLayout());
    ui->tab_1->layout()->addWidget(formFact);
    formCh = new FormCh(this);
    ui->tab_2->setLayout(new QVBoxLayout());
    ui->tab_2->layout()->addWidget(formCh);

    connect(ui->actionSurEl,SIGNAL(triggered(bool)),this,SLOT(surEl()));
    connect(ui->actionSurW,SIGNAL(triggered(bool)),this,SLOT(surW()));
    connect(ui->actionRefresh,SIGNAL(triggered(bool)),Rels::instance(),SLOT(refresh()));
    connect(ui->actionOtpr,SIGNAL(triggered(bool)),this,SLOT(edtOtpr()));
    connect(ui->actionPoluch,SIGNAL(triggered(bool)),this,SLOT(edtPoluch()));
    connect(ui->actionCarrier,SIGNAL(triggered(bool)),this,SLOT(edtCarrier()));
    connect(ui->actionPWire,SIGNAL(triggered(bool)),this,SLOT(edtWirePrice()));
    connect(ui->actionPEl,SIGNAL(triggered(bool)),this,SLOT(edtElPrice()));
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete Rels::instance();
    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", "facts");
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", "facts");
    settings.setValue("main_geometry",this->saveGeometry());
}

void MainWindow::surEl()
{
    FormSurEl f;
    f.exec();
}

void MainWindow::surW()
{
    FormSurW f;
    f.exec();
}

void MainWindow::edtOtpr()
{
    FormOtpr f;
    f.exec();
    Rels::instance()->relPost->model()->refresh();
}

void MainWindow::edtPoluch()
{
    FormPoluch f;
    f.exec();
    Rels::instance()->relPol->model()->refresh();
}

void MainWindow::edtCarrier()
{
    FormCarrier f;
    f.exec();
    Rels::instance()->relCarrier->model()->refresh();
}

void MainWindow::edtWirePrice()
{
    FormWirePrice f;
    f.exec();
    formFact->setDatePrice();
    formCh->setDatePrice();
}

void MainWindow::edtElPrice()
{
    FormElPrice f;
    f.exec();
    formFact->setDatePrice();
    formCh->setDatePrice();
}
