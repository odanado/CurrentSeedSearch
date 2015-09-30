#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QString>

#include <PokeRNG/PokeRNG.hpp>
#include <PokeData/PokeData.hpp>
#include "sss4config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BTN_EXEC_clicked();

    void on_CB_PokemonName_currentIndexChanged(int index);

    void on_CB_PokemonName_editTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    bool isSetParam;

    void initCB_PokemonName();
    void changePokemonIndex(int);
    void initCB_NatureName();

};

#endif // MAINWINDOW_H
