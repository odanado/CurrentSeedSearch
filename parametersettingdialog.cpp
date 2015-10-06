#include "parametersettingdialog.h"
#include "ui_parametersettingdialog.h"
#include <QDebug>

ParameterSettingDialog::ParameterSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSettingDialog)
{
    ui->setupUi(this);
    hasSetUpParam = false;

}

ParameterSettingDialog::~ParameterSettingDialog()
{
    delete ui;
}

void ParameterSettingDialog::setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param) {
    this->param = param;
}

void ParameterSettingDialog::done(int r) {

    if(!verifyInput()) {
        return;
    }
    if(!hasSetUpParam) {
        loadParam();
    }
    QDialog::done(r);
}

void ParameterSettingDialog::loadParam() {
    if(!verifyInput()) {
        return;
    }
    bool ok;

    param.set_nazo1(ui->LE_Nazo1->text().toInt(&ok,16));
    param.set_nazo2(ui->LE_Nazo2->text().toInt(&ok,16));
    param.set_nazo3(ui->LE_Nazo3->text().toInt(&ok,16));
    param.set_nazo4(ui->LE_Nazo4->text().toInt(&ok,16));
    param.set_nazo5(ui->LE_Nazo5->text().toInt(&ok,16));
    param.set_mac_addr(ui->LE_MacAddr1->text().toInt(&ok,16),ui->LE_MacAddr2->text().toInt(&ok,16),ui->LE_MacAddr3->text().toInt(&ok,16),
                       ui->LE_MacAddr4->text().toInt(&ok,16),ui->LE_MacAddr5->text().toInt(&ok,16),ui->LE_MacAddr6->text().toInt(&ok,16));

    param.set_timer0_min(ui->LE_FirstTimer0->text().toInt(&ok,16));
    param.set_timer0_max(ui->LE_LastTimer0->text().toInt(&ok,16));

    param.set_vcount(ui->LE_VCount->text().toInt(&ok,16));
    param.set_gxstat(ui->LE_GxStat->text().toInt(&ok,16));
    param.set_frame(ui->LE_Frame->text().toInt(&ok,16));

    hasSetUpParam = true;
    MainWindow *parent = (MainWindow*) parentWidget();
    parent->setParameter(param);

}

bool ParameterSettingDialog::verifyInput() {
    bool ok,result=true;
    ui->LE_Nazo1->text().toInt(&ok,16);
    result &= ok;
    ui->LE_Nazo2->text().toInt(&ok,16);
    result &= ok;
    ui->LE_Nazo3->text().toInt(&ok,16);
    result &= ok;
    ui->LE_Nazo4->text().toInt(&ok,16);
    result &= ok;
    ui->LE_Nazo5->text().toInt(&ok,16);
    result &= ok;

    ui->LE_MacAddr1->text().toInt(&ok,16);
    result &= ok;
    ui->LE_MacAddr2->text().toInt(&ok,16);
    result &= ok;
    ui->LE_MacAddr3->text().toInt(&ok,16);
    result &= ok;
    ui->LE_MacAddr4->text().toInt(&ok,16);
    result &= ok;
    ui->LE_MacAddr5->text().toInt(&ok,16);
    result &= ok;
    ui->LE_MacAddr6->text().toInt(&ok,16);
    result &= ok;

    ui->LE_FirstTimer0->text().toInt(&ok,16);
    result &= ok;
    ui->LE_LastTimer0->text().toInt(&ok,16);
    result &= ok;

    ui->LE_Frame->text().toInt(&ok,16);
    result &= ok;
    ui->LE_GxStat->text().toInt(&ok,16);
    result &= ok;
    ui->LE_VCount->text().toInt(&ok,16);
    result &= ok;

    if(!result) {
        QMessageBox msgBox;
        msgBox.setText("入力値が不正です");
        msgBox.exec();
        return false;
    }

    int first = ui->LE_FirstTimer0->text().toInt(&ok,16);
    int last = ui->LE_LastTimer0->text().toInt(&ok,16);

    if(last < first) {
        QMessageBox msgBox;
        msgBox.setText("Timer0の範囲の前後関係が不正です");
        msgBox.exec();
        return false;
    }

    return result;
}

void ParameterSettingDialog::showEvent(QShowEvent *event) {
    ui->LE_Nazo1->setText(QString::number(param.get_nazo1(),16));
    ui->LE_Nazo2->setText(QString::number(param.get_nazo2(),16));
    ui->LE_Nazo3->setText(QString::number(param.get_nazo3(),16));
    ui->LE_Nazo4->setText(QString::number(param.get_nazo4(),16));
    ui->LE_Nazo5->setText(QString::number(param.get_nazo5(),16));

    ui->LE_MacAddr1->setText(QString::number(param.get_mac_addr1(),16));
    ui->LE_MacAddr2->setText(QString::number(param.get_mac_addr2(),16));
    ui->LE_MacAddr3->setText(QString::number(param.get_mac_addr3(),16));
    ui->LE_MacAddr4->setText(QString::number(param.get_mac_addr4(),16));
    ui->LE_MacAddr5->setText(QString::number(param.get_mac_addr5(),16));
    ui->LE_MacAddr6->setText(QString::number(param.get_mac_addr6(),16));

    ui->LE_FirstTimer0->setText(QString::number(param.get_timer0_min(),16));
    ui->LE_LastTimer0->setText(QString::number(param.get_timer0_max(),16));

    ui->LE_Frame->setText(QString::number(param.get_frame(),16));
    ui->LE_GxStat->setText(QString::number(param.get_gxstat(),16));
    ui->LE_VCount->setText(QString::number(param.get_vcount(),16));

}

void ParameterSettingDialog::setHasSetUpParam(bool hasSetUpParam) {
    this->hasSetUpParam = hasSetUpParam;
}

void ParameterSettingDialog::on_BTN_OK_clicked()
{
    loadParam();
    accept();
}
