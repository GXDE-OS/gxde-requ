#include "widget.h"
#include <DApplication>
#include <DApplicationHelper>
#include <QList>

#ifdef USE_LAYER_SHELL
#include <LayerShellQt/Shell>
#endif

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
#ifdef USE_LAYER_SHELL
    LayerShellQt::Shell::useLayerShell();
#endif

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
