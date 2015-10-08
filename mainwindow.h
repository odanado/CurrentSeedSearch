#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QString>
#include <QList>

#include "parametersettingdialog.h"

#include <PokeRNG/PokeRNG.hpp>
#include <PokeData/PokeData.hpp>
#include "sss4config.h"
#include "satisfiediv.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param);

private slots:
    void on_BTN_EXEC_clicked();

    void on_CB_PokemonName_currentIndexChanged(int index);

    void on_CB_PokemonName_editTextChanged(const QString &arg1);

    void onComplated(const QString&);

    void on_SB_Level_valueChanged(int arg1);

    void on_CB_NatureName_currentTextChanged(const QString &arg1);

    void on_CB_NatureName_currentIndexChanged(int index);

    void on_CB_PokemonName_currentTextChanged(const QString &arg1);

    void on_BTN_ParameterSetting_clicked();

private:
    Ui::MainWindow *ui;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    bool hasSetUpParam;

    void initCB_PokemonName();
    void initCB_NatureName();

    void updateStats();

    int calcStats(int baseStats,int iv,int ev,int level,int mod);
    int calcHP(int baseStats,int iv,int ev,int level);
    void calcIVs();
    void loadDateTime();
    QList<PokeRNG::u32> lowerIVs,upperIVs;
    PokeRNG::DateTimeRange range;

    SatisfiedIV satisfiedIV;

};

#endif // MAINWINDOW_H
