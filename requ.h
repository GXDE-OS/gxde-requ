#ifndef REQU_H
#define REQU_H

#include <QWidget>
#include <QEvent>
#include <QTimer>
#include <QProcess>
#include <QCoreApplication>
class requ : public QWidget
{
    Q_OBJECT
public:
    explicit requ(QWidget *parent = nullptr);
    void setShell(QString t);

private:
    QTimer *Timer=new QTimer;
    QString shell;
signals:
protected:
    bool eventFilter(QObject *obj, QEvent *event);
public slots:
    void runShell();
};

#endif // REQU_H
