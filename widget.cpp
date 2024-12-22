#include "widget.h"
#include <QDebug>
#include <QScreen>

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    //QDesktopWidget* pDesktopWidget = QApplication::desktop();
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->geometry();
    qaq = new QSettings(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/GXDE/gxde-requ/gxde-requ-setting.qaq",
                        QSettings::IniFormat);
    QString kwinrules;
    QFileInfo file1(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/ukui-kwinrulesrc");
    qDebug()<<file1.exists();
    if(file1.exists() == false)
    {
        kwinrules="deepin-kwinrulesrc";
    } else {
        kwinrules="ukui-kwinrulesrc";
    }

    QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/" + kwinrules);
    file.open(QFile::ReadOnly);
    QString r = QLatin1String(file.readAll());
    file.close();

    QSettings tmp(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/" + kwinrules, QSettings::IniFormat);
    qDebug() << r;
    if(!r.contains("gxde-requ",Qt::CaseSensitive))
    {
        QSettings *tmp = new QSettings(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/" + kwinrules,
                                       QSettings::IniFormat);
        tmp->setValue(QString::number(tmp->value("count").toInt()+1) + "/Description", "gxde-requ");
        tmp->setValue(QString::number(tmp->value("count").toInt()+1) + "/desktop", -1);
        tmp->setValue(QString::number(tmp->value("count").toInt()+1) + "/desktoprule", 2);
        tmp->setValue(QString::number(tmp->value("count").toInt()+1) + "/wmclass", "gxde-requ gxde-requ");
        tmp->setValue(QString::number(tmp->value("count").toInt()+1) + "/wmclasscomplete", true);
        tmp->setValue(QString::number(tmp->value("count").toInt()+1) + "/wmclassmatch", 1);
        tmp->setValue("count", tmp->value("count").toInt()+1);
    }


    TopLeft->setShell(qaq->value("TopLeftShell").toString());
    TopRight->setShell(qaq->value("TopRightShell").toString());
    LowerLeft->setShell(qaq->value("LowerLeftShell").toString());
    LowerRight->setShell(qaq->value("LowerRightShell").toString());

    qDebug() << qaq->value("TopLeftShell").toString();
    qDebug() << qaq->value("TopRightShell").toString();
    qDebug() << qaq->value("LowerLeftShell").toString();
    qDebug() << qaq->value("LowerRightShell").toString();

    //TopLeft->setGeometry(0, 0, 1, 1);
    //TopRight->setGeometry(screenRect.width() - WIDGET_WIDTH, 0, 1, 1);
    //LowerLeft->setGeometry(0, screenRect.height() - WIDGET_WIDTH, 1, 1);
    /*LowerRight->setGeometry(screenRect.width() - WIDGET_WIDTH,
                            screenRect.height() - WIDGET_WIDTH,
                            1,
                            1);*/
    TopLeft->show();
    TopRight->show();
    LowerLeft->show();
    LowerRight->show();
}
