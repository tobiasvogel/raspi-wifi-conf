#include "mainwindow.h"
#include "infolabel.h"
#include "editheaders.h"
#include "wifilist.h"

#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString m_langPath = ":/i18n/";
    m_langPath.append("lang_" + QLocale().name().split('_').at(0) + ".qm");
    translator.load(m_langPath);
    a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
