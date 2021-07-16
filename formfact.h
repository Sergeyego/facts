#ifndef FORMFACT_H
#define FORMFACT_H

#include <QWidget>
#include "db/dbmapper.h"
#include "modelfact.h"
#include "modelfactel.h"
#include "modelfactwire.h"
#include "models.h"
#include "numstr.h"
#include "dialogselectsert.h"
#include "dialogselectbill.h"
#include "xlsx/xlsxdocument.h"
#include "factinfo.h"
#include <QSettings>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

using namespace QXlsx;

namespace Ui {
class FormFact;
}

class FormFact : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormFact(QWidget *parent = 0);
    ~FormFact();
    void keyPressEvent(QKeyEvent *p);
    
private:
    Ui::FormFact *ui;
    DbMapper *mapper;
    ModelFact *modelFact;
    ModelFactEl *modelFactEl;
    ModelFactWire *modelFactWire;
    TotalFactModel *totalFactModel;
    QDataWidgetMapper *totalMapper;
    void loadSettings();
    void saveSettings();
    void createTvrSurgut(FactInfo *info);
public slots:
    void setDatePrice();

private slots:
    void refreshFact();
    void selectPlat(int ind);
    void updContents(int ind);
    void updTotal();
    void insCen();
    void setSert();
    void setBill();
    void createFact();
    void createFact17();
    void createTvr();
    void createTn();
};

#endif // FORMFACT_H
