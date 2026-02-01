#include "widget.h"
#include <QApplication>
#include <QList>

QList <Widget*> list;

void loadWidget() {
    for (int i = 0; i < list.count(); ++i) {
        delete list[i];
        list.remove(i);
    }
    for (QScreen *screen: QGuiApplication::screens()) {
        list << new Widget(screen);
    }
}

int main(int argc, char *argv[])
{
    // 在 Wayland 下需要使用 xwayland 运行
    qputenv("QT_QPA_PLATFORM", "xcb");

    QApplication a(argc, argv);

    loadWidget();

    QAction::connect(&a, &QApplication::screenAdded, NULL, &loadWidget);
    QAction::connect(&a, &QApplication::screenRemoved, NULL, &loadWidget);
    QAction::connect(&a, &QApplication::primaryScreenChanged, NULL, &loadWidget);

    return a.exec();
}
