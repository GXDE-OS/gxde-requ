#include "widget.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    // 在 Wayland 下需要使用 xwayland 运行
    qputenv("QT_QPA_PLATFORM", "xcb");

    QApplication a(argc, argv);
    Widget w;

    return a.exec();
}
