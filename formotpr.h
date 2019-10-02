#ifndef FORMOTPR_H
#define FORMOTPR_H

#include <QDialog>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include <QKeyEvent>
#include <QStringList>

namespace Ui {
class FormOtpr;
}

class FormOtpr : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormOtpr(QWidget *parent = 0);
    ~FormOtpr();
protected:
    void keyPressEvent(QKeyEvent *pe);
    
private:
    Ui::FormOtpr *ui;
    DbTableModel *modelOtpr;
    DbMapper *mapper;
private slots:

};

#endif // FORMOTPR_H
