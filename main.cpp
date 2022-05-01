#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持高分辨屏幕的标准缩放比例
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
