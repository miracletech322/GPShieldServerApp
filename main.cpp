#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include "global_vars.h"

QSharedMemory shared("GPShieldServerApp");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icon(":/Resource/assets/logo.png");
    a.setWindowIcon(icon);

    if(!shared.create(512)) {
        QMessageBox::warning(nullptr, APP_NAME, "Another instance of this application is already running.");
        return -1;
    }

    MainWindow::getInstance()->show();

    return a.exec();
}
