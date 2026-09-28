#include "requ.h"
#include <QPainter>
#include <QDebug>
#include <QProcess>
#include <QHoverEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QWindow>

#ifdef Q_OS_LINUX
#  include <QDBusInterface>
#endif

requ::requ(QScreen *screen, Place whereIsShow, QWidget *parent) : QWidget(parent) {
    m_screen = screen;
    showPlace = whereIsShow;
    connect(Timer, SIGNAL(timeout()), this, SLOT(runShell()));
    this->setAttribute(Qt::WA_Hover, true);
    this->installEventFilter(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_X11NetWmWindowTypeDock);
    this->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    // 设置 Qt::X11BypassWindowManagerHint 以不被 gxde-top-panel 和 dde-dock 干扰
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool
                         | Qt::WindowDoesNotAcceptFocus | Qt::X11BypassWindowManagerHint);
#ifdef USE_LAYER_SHELL
    // LayerShell must setup before program window sets up.
    if (QGuiApplication::platformName() == "wayland") {
        setupLayerShell();
    }
#endif
    this->raise();
    resizeWindow(whereIsShow);
    setMaximumSize(WIDGET_WIDTH, WIDGET_WIDTH);

#ifdef Q_OS_LINUX // 此部分只支持 Linux
    setTransportFlat();
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, [this]() { setTransportFlat(); });
    timer->start();
#endif
}

void requ::setTransportFlat() {
    // 使用 dbus 判断是否支持透明背景
    QDBusInterface dbus("com.deepin.wm", "/com/deepin/wm", "com.deepin.wm");
    // 因为非 GXDE 未必有该dbus,所以必须保证这个接口存在才修改
    if (dbus.property("compositingEnabled").isValid()) {
        m_supportTransport = dbus.property("compositingEnabled").toBool();
        return;
    }
    m_supportTransport = true;
}

void requ::resizeWindow(Place where) {
    QScreen *screen = m_screen;
    QRect screenRect = screen->geometry();

    int searchWidth = WIDGET_SEARCH_WIDTH;
    if (mouseOnHotPlace && m_supportTransport) {
        searchWidth = WIDGET_WIDTH;
    }
    if (shell.isEmpty()) {
        hide(); // If corner unset then hide window.
        return;
    }
    setFixedSize(searchWidth, searchWidth);

    if (m_useLayerShell) {
        // If under Wayland, then the window shall be already in the correct corner.
        // We only need to update the size.
        show();
        update();
        return;
    }

    // 计算目标位置
    QPoint targetPos;
    switch (where) {
    case Place::LowerLeft:
        targetPos = QPoint(screenRect.x(), screenRect.y() + screenRect.height() - searchWidth);
        break;
    case Place::LowerRight:
        targetPos = QPoint(screenRect.x() + screenRect.width() - searchWidth,
                           screenRect.y() + screenRect.height() - searchWidth);
        break;
    case Place::TopLeft:
        targetPos = QPoint(screenRect.x(), screenRect.y());
        break;
    case Place::TopRight:
        targetPos = QPoint(screenRect.x() + screenRect.width() - searchWidth, screenRect.y());
        break;
    }

    setGeometry(targetPos.x(), targetPos.y(), searchWidth, searchWidth);
    show();
    update();
}

void requ::setShell(QString t) {
    if (t.trimmed().isEmpty()) {
        // 如果没有设置触发命令则不显示窗口
        t.clear();
    }

    shell = t;
    resizeWindow(showPlace);
}

void requ::runShell() {
    qDebug() << shell;
    QProcess::startDetached("bash", QStringList() << "-c" << shell);
    Timer->stop(); // 停止定时器，避免重复触发
}

void requ::paintEvent(QPaintEvent *) {
    if (!mouseOnHotPlace || shell.isEmpty() || !m_supportTransport) {
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

bool requ::eventFilter(QObject *obj, QEvent *event) {
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
                hotZone = QRect(width() - hotZoneSize, height() - hotZoneSize, hotZoneSize,
                                hotZoneSize);
                break;
            }

            if (hotZone.contains(cursorPos)) {
                if (!mouseOnHotPlace) {
                    mouseOnHotPlace = true;
                    resizeWindow(showPlace);
                    qDebug() << "Hot zone triggered!";
                    Timer->stop();
                    Timer->start(200);
                    Timer->setSingleShot(true);
                }
            } else {
                mouseOnHotPlace = false;
                resizeWindow(showPlace);
                Timer->stop();
            }

            update(); // 触发绘制更新动画
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

#ifdef USE_LAYER_SHELL
void requ::setupLayerShell() {
    setScreen(m_screen);
    create();
    QWindow *win = windowHandle();
    if (!win)
        return;

    LayerShellQt::Window *lsWin = LayerShellQt::Window::get(win);
    if (!lsWin)
        return;

    LayerShellQt::Window::Anchors anchors;
    switch (showPlace) {
    case Place::TopLeft:
        anchors = LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop);
        anchors |= LayerShellQt::Window::AnchorLeft;
        break;
    case Place::TopRight:
        anchors = LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorTop);
        anchors |= LayerShellQt::Window::AnchorRight;
        break;
    case Place::LowerLeft:
        anchors = LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorBottom);
        anchors |= LayerShellQt::Window::AnchorLeft;
        break;
    case Place::LowerRight:
        anchors = LayerShellQt::Window::Anchors(LayerShellQt::Window::AnchorBottom);
        anchors |= LayerShellQt::Window::AnchorRight;
        break;
    }

    lsWin->setScreenConfiguration(LayerShellQt::Window::ScreenFromQWindow);
    lsWin->setAnchors(anchors);
    lsWin->setMargins(QMargins());
    lsWin->setExclusiveZone(-1);
    lsWin->setLayer(LayerShellQt::Window::LayerOverlay);
    lsWin->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityNone);
    lsWin->setScope("gxde-requ");

    m_useLayerShell = true;
}
#endif
