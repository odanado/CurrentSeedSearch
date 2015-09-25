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

    initCB_PokemonName();


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

void MainWindow::initCB_PokemonName() {
    QStringList list;
    for(const auto &pokemon : PokeData::Pokemon::pokemons) {
        QString name(QString::fromStdString(pokemon.getName()));
        list.append(name);
    }

    ui->CB_PokemonName->addItems(list);
    ui->CB_PokemonName->setEditText(*list.begin());
}

void MainWindow::on_CB_PokemonName_currentIndexChanged(int index)
{
    changePokemonIndex(index);
}

void MainWindow::on_CB_PokemonName_editTextChanged(const QString &arg1)
{
    std::string name = arg1.toStdString();
    auto it = PokeData::Pokemon::pokemonIndexes.find(name);
    if(it != PokeData::Pokemon::pokemonIndexes.end()) {
        changePokemonIndex(it->second);
    }
}


void MainWindow::changePokemonIndex(int index) {

    QString text;
    const auto &baseStats = PokeData::Pokemon::pokemons[index].getBaseStats();
    text.sprintf("%d-%d-%d-%d-%d-%d",
                 baseStats.getHP(),baseStats.getAttack(),baseStats.getDefence(),
                 baseStats.getSpAtk(),baseStats.getSpDef(),baseStats.getSpeed());

    ui->Label_BaseStats->setText(text);
}
