#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "formfact.h"
#include "formch.h"
#include "formsurel.h"
#include "formsurw.h"
#include "formotpr.h"
#include "formpoluch.h"
#include "formcarrier.h"
#include "formwireprice.h"
#include "formelprice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    FormFact *formFact;
    FormCh *formCh;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void loadSettings();
    void saveSettings();

private slots:
    void surEl();
    void surW();
    void edtOtpr();
    void edtPoluch();
    void edtCarrier();
    void edtWirePrice();
    void edtElPrice();
};

#endif // MAINWINDOW_H
