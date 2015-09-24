#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <PokeRNG/PokeRNG.hpp>
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

private:
    Ui::MainWindow *ui;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    bool isSetParam;
};

#endif // MAINWINDOW_H
