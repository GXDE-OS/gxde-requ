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
    //this->setWindowOpacity(0.5);
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
    QProcess *run = new QProcess;
    run->startDetached(shell);
    Timer->stop();  // 运行程序后停止 QTimer 以避免重复打开应用
}

void requ::paintEvent(QPaintEvent*)
{
    if (!mouseOnHotPlace) {
        // 如果鼠标不在热区上则不进行绘制
        return;
    }
    QPainter paint;
    paint.begin(this);
    // 计算圆心坐标
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
    // 绘制渐变的同心圆
    double lessColor = 255.0 / WIDGET_WIDTH;
    for (int i = 0; i <= WIDGET_WIDTH; ++i) {
        // 计算该同心圆的透明度
        int trans = 255 - lessColor * i;
        // 设置绘制的透明度
        paint.setPen(QPen(QColor(0, 255, 255, trans)));
        paint.setBrush(QBrush(QColor(0, 255, 255, trans)));
        // 因为 Qt 中，用于定位圆的坐标并不是圆心，而是与圆相切的矩形左上角点的坐标
        // 因此需要动态计算圆的 x, y 坐标以便正确绘制同心圆
        int drawX = x + (WIDGET_WIDTH - i);
        int drawY = y + (WIDGET_WIDTH - i);
        paint.drawEllipse(drawX, drawY, i * 2, i * 2);
    }
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
        } else if(event->type() == QEvent::HoverLeave){
            mouseOnHotPlace = false;
            Timer->stop();

        }
    }
    update();  // 重新绘制窗口以显示/关闭动画
    return QWidget::eventFilter(obj, event);
}
