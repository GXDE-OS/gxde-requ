#include "requ.h"
#include <QDebug>
#include <QProcess>

requ::requ(QWidget *parent) : QWidget(parent)
{
    connect(Timer, SIGNAL(timeout()), this, SLOT(runShell()));
    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    this->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    this->setWindowFlags(Qt::WindowDoesNotAcceptFocus);
    this->raise();
    setFixedSize(1,1);
    setMaximumSize(1,1);
}
void requ::setShell(QString t)
{
    shell=t;
}
void requ::runShell()
{
    QProcess *run=new QProcess;
    run->startDetached(shell);
}
bool requ::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this) {
        if(event->type() == QEvent::HoverEnter) {
                qDebug()<<"safsdf";
                Timer->stop();
                Timer->start(400);
                Timer->setSingleShot(true);
                return true;
        }else if(event->type()== QEvent::HoverLeave){
            Timer->stop();

        }
    }
    return QWidget::eventFilter(obj,event);
}
