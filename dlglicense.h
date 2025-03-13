#ifndef DLGLICENSE_H
#define DLGLICENSE_H

#include <QDialog>
#include <QDate>

namespace Ui {
class DlgLicense;
}

class DlgLicense : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLicense(QWidget *parent = nullptr);
    ~DlgLicense();

public slots:
    void on_btnOpen_clicked();

private:
    Ui::DlgLicense *ui;
};

#endif // DLGLICENSE_H
