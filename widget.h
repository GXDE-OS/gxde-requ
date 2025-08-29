#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
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
    requ *TopLeft = new requ(requ::TopLeft);
    requ *TopRight = new requ(requ::TopRight);
    requ *LowerLeft = new requ(requ::LowerLeft);
    requ *LowerRight = new requ(requ::LowerRight);

signals:

public slots:
};

#endif // WIDGET_H
