#ifndef FORMPOLUCH_H
#define FORMPOLUCH_H

#include <QDialog>
#include "rels.h"
#include "db/dbmapper.h"
#include "db/dbtablemodel.h"
#include <QKeyEvent>

namespace Ui {
class FormPoluch;
}

class FormPoluch : public QDialog
{
    Q_OBJECT
    
public:
    explicit FormPoluch(QWidget *parent = 0);
    ~FormPoluch();
protected:
    void keyPressEvent(QKeyEvent *pe);
    
private:
    Ui::FormPoluch *ui;
    DbTableModel *modelPoluch;
    DbMapper *mapper;

};

#endif // FORMPOLUCH_H
