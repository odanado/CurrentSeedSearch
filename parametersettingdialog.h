#ifndef PARAMETERSETTINGDIALOG_H
#define PARAMETERSETTINGDIALOG_H

#include <QDialog>
#include <PokeRNG/PokeRNG.hpp>
#include <QMessageBox>

#include "mainwindow.h"

namespace Ui {
class ParameterSettingDialog;
}

class ParameterSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParameterSettingDialog(QWidget *parent = 0);
    ~ParameterSettingDialog();
    void setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param);
    void setHasSetUpParam(bool hasSetUpParam);

protected:
    virtual void showEvent(QShowEvent *event);

private slots:
    virtual void done(int r);

    void on_BTN_OK_clicked();

private:
    Ui::ParameterSettingDialog *ui;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    bool hasSetUpParam;
    void loadParam();
    bool verifyInput();
};

#endif // PARAMETERSETTINGDIALOG_H
