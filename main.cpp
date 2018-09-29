#include "efipatcher.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EfiPatcher w;
    w.show();

    return a.exec();
}
