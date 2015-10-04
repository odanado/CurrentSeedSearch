#ifndef PARAMETERSETTINGDIALOG_H
#define PARAMETERSETTINGDIALOG_H

#include <QDialog>
#include <PokeRNG/PokeRNG.hpp>

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

protected:
    virtual void showEvent(QShowEvent *event);

private slots:

private:
    Ui::ParameterSettingDialog *ui;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
};

#endif // PARAMETERSETTINGDIALOG_H
