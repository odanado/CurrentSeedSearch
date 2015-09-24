#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isSetParam = false;
    SSS4Config sss4Config;
    if(sss4Config.parse()) {
        param = sss4Config.getParameter();
        isSetParam = true;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BTN_EXEC_clicked()
{
    if(isSetParam) {
        PokeRNG::Calc5GenSeed calcSeed;
        PokeRNG::LCG5Gen lcg;
        PokeRNG::u64 seed1 = lcg.next(calcSeed(param));
        ui->TE_Result->setText(QString::number(seed1,16));
    }
}
