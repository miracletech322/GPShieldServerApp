#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "tcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* getInstance();
    void handleProcessStatus(int nStatus, QString strMessage = "");

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    TcpServer m_server;
};
#endif // MAINWINDOW_H
