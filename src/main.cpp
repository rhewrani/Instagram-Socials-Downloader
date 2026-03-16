#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.overlay = new BlockingOverlay(nullptr, "Loading data…");
    w.overlay->show();
    qApp->processEvents();

    return a.exec();
}
