#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSystemTrayIcon>

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
    void initServer();

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void on_actionRegister_triggered();

private:
    Ui::MainWindow *ui;

    TcpServer m_server;
    QSystemTrayIcon *trayIcon;
};
#endif // MAINWINDOW_H
