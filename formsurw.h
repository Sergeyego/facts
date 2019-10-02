#ifndef FORMSURW_H
#define FORMSURW_H

#include <QDialog>
#include "rels.h"
#include "db/dbtablemodel.h"

namespace Ui {
class FormSurW;
}

class FormSurW : public QDialog
{
    Q_OBJECT

public:
    explicit FormSurW(QWidget *parent = 0);
    ~FormSurW();

private:
    Ui::FormSurW *ui;
    DbTableModel *modelSurW;
};

#endif // FORMSURW_H
