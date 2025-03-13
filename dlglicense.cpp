#include "dlglicense.h"
#include "ui_dlglicense.h"
#include "global_vars.h"
#include "encryptdecrypt.h"
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

DlgLicense::DlgLicense(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgLicense)
{
    ui->setupUi(this);

    QString strMachine = EncryptDecrypt::getMachineID();
    ui->edtMachine->setText(strMachine);
    QSettings settings(APP_NAME);
    QString strLicense = settings.value("License").toString();

    ui->edtLicense->setText(strLicense.insert(4, "-").insert(9, "-").insert(14, "-").insert(19, "-"));

    QByteArray byLicense = settings.value("fullCiphertext").toByteArray();
    strLicense = QString::fromLocal8Bit(EncryptDecrypt::decryptAES(byLicense));
    if(strLicense == "")
    {
        ui->lblStatus->setText("Invalid License Key");
        return;
    }

    QStringList lst = strLicense.split("|");
    if(lst[0] == EncryptDecrypt::getMachineID())
    {
        QDate registeredDate = QDate::fromString(lst[1], "yyyy-MM-dd");
        QDate currentDate = QDate::currentDate();
        if (registeredDate >= currentDate)
        {
            ui->lblStatus->setText(
                tr("Type: %2\tExpire: %3\tRemaining: %4 day(s)")
                    .arg(lst[2])
                    .arg(lst[1])
                    .arg(currentDate.daysTo(registeredDate))
            );
        }
        else
        {
            ui->lblStatus->setText("License Key Expired");
        }
    }
    else
    {
        ui->lblStatus->setText("Invalid License Key");
    }
}

DlgLicense::~DlgLicense()
{
    delete ui;
}

void DlgLicense::on_btnOpen_clicked()
{
    QString strPath = QFileDialog::getOpenFileName(this, "Open Key File", "", "Key File(*.key)");
    if(strPath == "")
        return;

    QSettings settings(strPath, QSettings::IniFormat);
    QString strLicense = settings.value("License").toString();
    if(strLicense == "")
        return;

    ui->edtLicense->setText(strLicense.insert(4, "-").insert(9, "-").insert(14, "-").insert(19, "-"));

    QByteArray byLicense = settings.value("fullCiphertext").toByteArray();
    strLicense = QString::fromLocal8Bit(EncryptDecrypt::decryptAES(byLicense));
    if(strLicense == "")
    {
        ui->lblStatus->setText("Invalid License Key");
        return;
    }

    QStringList lst = strLicense.split("|");
    if(lst[0] == EncryptDecrypt::getMachineID())
    {
        QDate registeredDate = QDate::fromString(lst[1], "yyyy-MM-dd");
        QDate currentDate = QDate::currentDate();
        if (registeredDate >= currentDate)
        {
            ui->lblStatus->setText(
                tr("Type: %2\tExpire: %3\tRemaining: %4 day(s)")
                    .arg(lst[2])
                    .arg(lst[1])
                    .arg(currentDate.daysTo(registeredDate))
                );
            QSettings saveSettings(APP_NAME);
            saveSettings.setValue("License", settings.value("License").toByteArray());
            saveSettings.setValue("fullCiphertext", settings.value("fullCiphertext").toByteArray());
            QMessageBox::information(this, "Success", "License key file has been imported successfully.");
        }
        else
        {
            ui->lblStatus->setText("License Key Expired");
        }
    }
    else
    {
        ui->lblStatus->setText("Invalid License Key");
    }
}
