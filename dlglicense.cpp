#include "dlglicense.h"
#include "ui_dlglicense.h"
#include "global_vars.h"
#include "encryptdecrypt.h"
#include <QFileDialog>
#include <QSettings>

DlgLicense::DlgLicense(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgLicense)
{
    ui->setupUi(this);

    QString strMachine = EncryptDecrypt::getMachineID();
    QSettings settings(APP_NAME);
    QString strLicense = settings.value("License").toString();

    ui->edtMachine->setText(strMachine);
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
    QSettings settings(strPath);

}
