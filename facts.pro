#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T10:34:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = facts
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(xlsx/qtxlsx.pri)


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    formfact.cpp \
    modelfact.cpp \
    rels.cpp \
    modelfactel.cpp \
    modelfactwire.cpp \
    models.cpp \
    dialogselectsert.cpp \
    formch.cpp \
    modelbill.cpp \
    modelbillel.cpp \
    modelbillwire.cpp \
    formsurel.cpp \
    formsurw.cpp \
    formotpr.cpp \
    formpoluch.cpp \
    formcarrier.cpp \
    modelwireprice.cpp \
    formwireprice.cpp \
    modelelprice.cpp \
    formelprice.cpp \
    numstr.cpp \
    factinfo.cpp \
    db/dbxlsx.cpp \
    dialogselectbill.cpp

HEADERS += \
        mainwindow.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    formfact.h \
    modelfact.h \
    rels.h \
    modelfactel.h \
    modelfactwire.h \
    models.h \
    dialogselectsert.h \
    formch.h \
    modelbill.h \
    modelbillel.h \
    modelbillwire.h \
    formsurel.h \
    formsurw.h \
    formotpr.h \
    formpoluch.h \
    formcarrier.h \
    modelwireprice.h \
    formwireprice.h \
    modelelprice.h \
    formelprice.h \
    numstr.h \
    factinfo.h \
    db/dbxlsx.h \
    dialogselectbill.h

FORMS += \
        mainwindow.ui \
    db/dblogin.ui \
    formfact.ui \
    dialogselectsert.ui \
    formch.ui \
    formsurel.ui \
    formsurw.ui \
    formotpr.ui \
    formpoluch.ui \
    formcarrier.ui \
    formwireprice.ui \
    formelprice.ui \
    dialogselectbill.ui

RESOURCES += \
    res.qrc

RC_FILE = ico.rc

DISTFILES += \
    templates/bill_of.xlsx \
    templates/bill.xlsx \
    templates/fact17.xlsx \
    templates/ttn17.xlsx \
    templates/tvr_s.xlsx \
    templates/tvr.xlsx \
    ico.rc \
    templates/tn21.xlsx \
    templates/fact21.xlsx
