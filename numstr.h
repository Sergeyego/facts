#ifndef NUMSTR_H
#define NUMSTR_H
#include <QString>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

QString money(double n);
QString intstr(int n);
void sysCommand(QString fname);

#endif //NUMSTR_H
