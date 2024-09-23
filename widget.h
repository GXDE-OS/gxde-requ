#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QDesktopWidget>
#include <QApplication>
#include <requ.h>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
private:
    QSettings *qaq;
    requ *TopLeft=new requ;
    requ *TopRight=new requ;
    requ *LowerLeft=new requ;
    requ *LowerRight=new requ;

signals:

public slots:
};

#endif // WIDGET_H
