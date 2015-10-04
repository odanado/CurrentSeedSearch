#include "parametersettingdialog.h"
#include "ui_parametersettingdialog.h"

ParameterSettingDialog::ParameterSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParameterSettingDialog)
{
    ui->setupUi(this);

}

ParameterSettingDialog::~ParameterSettingDialog()
{
    delete ui;
}

void ParameterSettingDialog::setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param) {
    this->param = param;
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
