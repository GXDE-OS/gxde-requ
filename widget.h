#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QApplication>
#include <requ.h>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QScreen>
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QScreen *screen, QWidget *parent = nullptr);
    ~Widget();
private:
    QSettings *qaq;
    requ *TopLeft;
    requ *TopRight;
    requ *LowerLeft;
    requ *LowerRight;

signals:

public slots:
};

#endif // WIDGET_H
