#include "requ.h"
#include <QPainter>
#include <QDebug>
#include <QProcess>
#include <QHoverEvent>

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
    QProcess::startDetached(shell);
    Timer->stop(); // 停止定时器，避免重复触发
}


void requ::paintEvent(QPaintEvent *)
{
    if (!mouseOnHotPlace || shell.isEmpty()) {
        return; // 热区未激活或未设置命令时不绘制
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
    double lessColor = 240.0 / WIDGET_WIDTH;
    for (int i = 0; i <= WIDGET_WIDTH; ++i) {
        // 设置绘制的透明度
        paint.setPen(QPen(QColor(0, 191, 255, lessColor)));
        paint.setBrush(QBrush(QColor(0, 191, 255, lessColor)));
        // 因为 Qt 中，用于定位圆的坐标并不是圆心，而是四条棱与圆相切的矩形左上角点的坐标
        // 因此需要动态计算圆的 x, y 坐标以便正确绘制同心圆
        int drawX = x + (WIDGET_WIDTH - i);
        int drawY = y + (WIDGET_WIDTH - i);
        paint.drawEllipse(drawX, drawY, i * 2, i * 2);
    }
    paint.end();
}

bool requ::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this) {
        if (event->type() == QEvent::HoverMove || event->type() == QEvent::HoverEnter) {
            auto hoverEvent = static_cast<QHoverEvent *>(event);
            QPoint cursorPos = hoverEvent->pos(); // 获取鼠标位置

            // 定义热区范围
            int hotZoneSize = 2; // 热区大小
            QRect hotZone;
            switch (showPlace) {
            case Place::TopLeft:
                hotZone = QRect(0, 0, hotZoneSize, hotZoneSize);
                break;
            case Place::TopRight:
                hotZone = QRect(width() - hotZoneSize, 0, hotZoneSize, hotZoneSize);
                break;
            case Place::LowerLeft:
                hotZone = QRect(0, height() - hotZoneSize, hotZoneSize, hotZoneSize);
                break;
            case Place::LowerRight:
                hotZone = QRect(width() - hotZoneSize, height() - hotZoneSize, hotZoneSize, hotZoneSize);
                break;
            }

            if (hotZone.contains(cursorPos)) {
                if (!mouseOnHotPlace) {
                    mouseOnHotPlace = true;
                    qDebug() << "Hot zone triggered!";
                    Timer->stop();
                    Timer->start(200);
                    Timer->setSingleShot(true);
                }
            } else {
                mouseOnHotPlace = false;
                Timer->stop();
            }

            update(); // 触发绘制更新动画
            return true;
        } else if (event->type() == QEvent::HoverLeave) {
            mouseOnHotPlace = false;
            Timer->stop();
            update(); // 鼠标离开时更新动画状态
        }
    }
    return QWidget::eventFilter(obj, event);
}
