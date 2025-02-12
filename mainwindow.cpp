#include "mainwindow.h"
#include "global_vars.h"
#include "./ui_mainwindow.h"

static MainWindow* instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!m_server.listen(QHostAddress::Any, 3022))
    {
        ui->plainTextEdit->appendPlainText(tr("Unable to start the server: %1.").arg(m_server.errorString()));
    }

    else
    {
        ui->plainTextEdit->appendPlainText(tr("The server has started and is running on port 3022..."));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::getInstance()
{
    if(instance == nullptr)
    {
        instance = new MainWindow(0);
    }

    return instance;
}

void MainWindow::handleProcessStatus(int nStatus, QString strMessage)
{
    if(nStatus == HandleProcessStatus::HPS_PEER_CONNECTED)
    {
        ui->plainTextEdit->appendPlainText(tr("A new peer has successfully connected..."));
    }

    if(nStatus == HandleProcessStatus::HPS_PEER_CLOSED)
    {
        ui->plainTextEdit->appendPlainText(tr("A new peer has successfully disconnected..."));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, APP_NAME, "Do you want to exit application?") == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
