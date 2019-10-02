#ifndef FORMCH_H
#define FORMCH_H

#include <QWidget>
#include <QDataWidgetMapper>
#include "rels.h"
#include "modelbill.h"
#include "db/dbmapper.h"
#include "models.h"
#include "modelbillel.h"
#include "modelbillwire.h"
#include "xlsx/xlsxdocument.h"
#include "numstr.h"
#include <QDebug>

using namespace QXlsx;

namespace Ui {
class FormCh;
}

class FormCh : public QWidget
{
    Q_OBJECT    
public:
    explicit FormCh(QWidget *parent = nullptr);
    ~FormCh();
    void keyPressEvent(QKeyEvent *p);
    
private:
    Ui::FormCh *ui;
    ModelBill *modelBill;
    DbMapper *mapper;
    ModelBillEl *modelBillEl;
    ModelBillWire *modelBillWire;

    TotalBillModel *totalBillModel;
    QDataWidgetMapper *totalMapper;
    void loadSettings();
    void saveSettings();

public slots:
    void setDatePrice();

private slots:
    void updDataBill(int index);
    void refreshBill();
    void updTotal();
    void insCen();
    void createBill();
    void createBillOf();

};

#endif // FORMCH_H
