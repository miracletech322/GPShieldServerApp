#include "mainwindow.h"
#include "global_vars.h"
#include "dlglicense.h"
#include "./ui_mainwindow.h"
#include "encryptdecrypt.h"

static MainWindow* instance = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings(APP_NAME);
    QString strLicense = settings.value("License").toString();

    if(strLicense == "")
    {
        QByteArray byMachine = EncryptDecrypt::getMachineID().toLocal8Bit();
        QByteArray byDate = QDate::currentDate().addDays(14).toString("yyyy-MM-dd").toLocal8Bit();
        QByteArray byType = "Trial";
        QByteArray byMachineInfo = byMachine + "|" + byDate + "|" + byType, fullCiphertext;
        QByteArray byLicenseKey = EncryptDecrypt::encryptAES(byMachineInfo, fullCiphertext);
        settings.setValue("License", byLicenseKey);
        settings.setValue("fullCiphertext", fullCiphertext);

        initServer();
    }
    else
    {
        QByteArray byLicense = settings.value("fullCiphertext").toByteArray();
        QString strLicense = QString::fromLocal8Bit(EncryptDecrypt::decryptAES(byLicense));
        if(strLicense == "")
        {
            ui->plainTextEdit->appendPlainText("Your license key is invalid. Please enter a valid key to continue using the app");
            return;
        }
        QStringList lst = strLicense.split("|");
        if(lst[0] == EncryptDecrypt::getMachineID())
        {
            QDate registeredDate = QDate::fromString(lst[1], "yyyy-MM-dd");
            QDate currentDate = QDate::currentDate();
            if (registeredDate >= currentDate)
            {
                ui->plainTextEdit->appendPlainText(
                    tr("License Key: %1\tType: %2\tExpire: %3\tRemaining: %4 day(s)")
                        .arg(settings.value("License").toString().insert(4, "-").insert(9, "-").insert(14, "-").insert(19, "-"))
                        .arg(lst[2])
                        .arg(lst[1])
                        .arg(currentDate.daysTo(registeredDate))
                );

                initServer();
            }
            else
            {
                ui->plainTextEdit->appendPlainText("Your license key has expired. Please enter a valid key to continue using the app");
            }
        }
        else
        {
            ui->plainTextEdit->appendPlainText("Your license key is invalid. Please enter a valid key to continue using the app");
        }
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
        ui->plainTextEdit->appendPlainText(tr("A new peer has successfully disconnected. %1").arg(strMessage));
    }

    if(nStatus == HandleProcessStatus::HPS_PEER_SET_INFO)
    {
        ui->plainTextEdit->appendPlainText(tr("A new peer has updated its information: %1").arg(strMessage));
    }

    if(nStatus == HandleProcessStatus::HPS_PEER_SEND)
    {
        ui->plainTextEdit->appendPlainText(tr("A new peer has sent alert: %1").arg(strMessage));
    }

    if(nStatus == HandleProcessStatus::HPS_PEER_RECEIVED)
    {
        ui->plainTextEdit->appendPlainText(strMessage);
    }
}

void MainWindow::initServer()
{
    if (!m_server.listen(QHostAddress::Any, 3022))
    {
        ui->plainTextEdit->appendPlainText(tr("Unable to start the server: %1.").arg(m_server.errorString()));
    }

    else
    {
        ui->plainTextEdit->appendPlainText(tr("The server has started and is running on port 3022..."));
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

void MainWindow::on_actionRegister_triggered()
{
    DlgLicense *dlg = new DlgLicense(this);
    dlg->show();
}
