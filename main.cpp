#include "widget.h"
#include <DApplication>
#include <DApplicationHelper>
#include <QList>

QList<Widget *> list;

void loadWidget() {
    qDeleteAll(list);
    list.clear();
    for (QScreen *screen : QGuiApplication::screens()) {
        list << new Widget(screen);
    }
}

int main(int argc, char *argv[]) {
    Dtk::Widget::DApplication a(argc, argv);

    if (!a.setSingleInstance("gxde-requ")) {
        return 0;
    }

    loadWidget();

    QAction::connect(&a, &QApplication::screenAdded, NULL, &loadWidget);
    QAction::connect(&a, &QApplication::screenRemoved, NULL, &loadWidget);
    QAction::connect(&a, &QApplication::primaryScreenChanged, NULL, &loadWidget);

    return a.exec();
}
