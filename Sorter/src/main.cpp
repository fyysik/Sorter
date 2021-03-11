#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[]) {
    // Initialize a Qt application:
    QApplication app(argc, argv);

    // Create main window and mark it as visible:
    MainWindow w;
    w.setMinimumSize(1024,864);
    w.show();

    // Enter message loop:
    return app.exec();
}
