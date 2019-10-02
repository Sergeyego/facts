#ifndef FORMSUREL_H
#define FORMSUREL_H

#include <QDialog>
#include "db/dbtablemodel.h"
#include "rels.h"

namespace Ui {
class FormSurEl;
}

class FormSurEl : public QDialog
{
    Q_OBJECT

public:
    explicit FormSurEl(QWidget *parent = 0);
    ~FormSurEl();

private:
    Ui::FormSurEl *ui;
    DbTableModel *modelSurEl;
};

#endif // FORMSUREL_H
