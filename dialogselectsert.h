#ifndef DIALOGSELECTSERT_H
#define DIALOGSELECTSERT_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "rels.h"

namespace Ui {
class DialogSelectSert;
}

class DialogSelectSert : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSelectSert(int id_pol, QWidget *parent = 0);
    ~DialogSelectSert();
    int idSert();
    
private:
    Ui::DialogSelectSert *ui;
    QSortFilterProxyModel *model;
    QModelIndex selectIndex;

private slots:
    void select(QModelIndex index);
};

#endif // DIALOGSELECTSERT_H
