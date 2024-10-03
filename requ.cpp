#include "requ.h"
#include <QPainter>
#include <QDebug>
#include <QProcess>

requ::requ(Place whereIsShow, QWidget *parent) : QWidget(parent)
{
    showPlace = whereIsShow;
    connect(Timer, SIGNAL(timeout()), this, SLOT(runShell()));
    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    this->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    this->setWindowFlags(Qt::WindowDoesNotAcceptFocus);
    this->setWindowOpacity(0.5);
    this->raise();
    setFixedSize(WIDGET_WIDTH, WIDGET_WIDTH);
    setMaximumSize(WIDGET_WIDTH, WIDGET_WIDTH);
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

void requ::paintEvent(QPaintEvent*)
{
    if (!mouseOnHotPlace) {
        // 如果鼠标不在热区上则不进行绘制
        return;
    }
    QPainter paint;
    paint.begin(this);
    paint.setPen(QPen(Qt::cyan));
    paint.setBrush(QBrush(Qt::cyan));//设置画刷形式
    //paint.drawRect()
    int x = 0, y = 0;
    switch (showPlace) {
    case Place::TopLeft:
        x = 0;
        y = 0;
        break;
    case Place::TopRight:
        x = WIDGET_WIDTH;
        y = 0;
        break;
    case Place::LowerLeft:
        x = 0;
        y = WIDGET_WIDTH;
        break;
    case Place::LowerRight:
        x = WIDGET_WIDTH;
        y = WIDGET_WIDTH;
        break;
    }
    x -= WIDGET_WIDTH;
    y -= WIDGET_WIDTH;
    paint.drawEllipse(x, y, WIDGET_WIDTH * 2, WIDGET_WIDTH * 2);
    paint.end();
}

bool requ::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this) {
        if(event->type() == QEvent::HoverEnter) {
            mouseOnHotPlace = true;
            qDebug() << "safsdf";
            Timer->stop();
            Timer->start(600);
            Timer->setSingleShot(true);
            return true;
        } else if(event->type()== QEvent::HoverLeave){
            mouseOnHotPlace = false;
            Timer->stop();

        }
    }
    update();  // 重新绘制窗口以显示/关闭动画
    return QWidget::eventFilter(obj,event);
}
