#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

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
    initCB_NatureName();

    for(int i=0;i<6;i++) {
        lowerIVs.append(32);
        upperIVs.append(-1);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BTN_EXEC_clicked()
{
    calcIVs();
    if(isSetParam) {
        PokeRNG::Calc5GenSeed calcSeed;
        PokeRNG::LCG5Gen lcg;
        PokeRNG::u64 seed1 = lcg.next(calcSeed(param));
        ui->TE_Result->setText(QString::number(seed1,16));
    }
}

void MainWindow::calcIVs() {
    auto nature = PokeData::natures[PokeData::natureIndexes.at(ui->CB_NatureName->currentText().toStdString())];
    auto baseStatsTmp = PokeData::pokemons[PokeData::pokemonIndexes.at(ui->CB_PokemonName->currentText().toStdString())].getBaseStats();

    int level;
    QList<int> stats,natureMod,baseStats;
    bool ok;
    stats.append(ui->SB_StatsH->text().toInt(&ok));
    stats.append(ui->SB_StatsA->text().toInt(&ok));
    stats.append(ui->SB_StatsB->text().toInt(&ok));
    stats.append(ui->SB_StatsC->text().toInt(&ok));
    stats.append(ui->SB_StatsD->text().toInt(&ok));
    stats.append(ui->SB_StatsS->text().toInt(&ok));
    natureMod.append(nature.getAttackMod());
    natureMod.append(nature.getDefenseMod());
    natureMod.append(nature.getSpAtkMod());
    natureMod.append(nature.getSpDefMod());
    natureMod.append(nature.getSpeedMod());
    baseStats.append(baseStatsTmp.getHP());
    baseStats.append(baseStatsTmp.getAttack());
    baseStats.append(baseStatsTmp.getDefense());
    baseStats.append(baseStatsTmp.getSpAtk());
    baseStats.append(baseStatsTmp.getSpDef());
    baseStats.append(baseStatsTmp.getSpeed());
    level = ui->SB_Level->text().toInt(&ok);

    for(int iv=0;iv<32;iv++) {
        int hp = calcHP(baseStats[0],iv,0,level);
        if(hp == stats[0]) {
            lowerIVs[0]=std::min(lowerIVs[0],iv);
            upperIVs[0]=std::max(upperIVs[0],iv);
        }
    }

    for(int i=1;i<6;i++) {
        for(int iv=0;iv<32;iv++) {
            int stat = calcStats(baseStats[i],iv,0,level,natureMod[i-1]);
            if(stat == stats[i]) {
                lowerIVs[i]=std::min(lowerIVs[i],iv);
                upperIVs[i]=std::max(upperIVs[i],iv);
            }
        }
    }

}

void MainWindow::initCB_NatureName() {
    QStringList list;
    for(const auto &nature : PokeData::natures) {
        QString name(QString::fromStdString(nature.getName()));
        list.append(name);
    }

    ui->CB_NatureName->addItems(list);
    ui->CB_NatureName->setEditText(list.front());
}

void MainWindow::initCB_PokemonName() {
    QStringList list;
    for(const auto &pokemon : PokeData::pokemons) {
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
    auto it = PokeData::pokemonIndexes.find(name);
    if(it != PokeData::pokemonIndexes.end()) {
        changePokemonIndex(it->second);
    }
}


void MainWindow::changePokemonIndex(int index) {

    QString text;
    const auto &baseStats = PokeData::pokemons[index].getBaseStats();
    text.sprintf("%d-%d-%d-%d-%d-%d",
                 baseStats.getHP(),baseStats.getAttack(),baseStats.getDefense(),
                 baseStats.getSpAtk(),baseStats.getSpDef(),baseStats.getSpeed());

    ui->Label_BaseStats->setText(text);
}

int MainWindow::calcStats(int baseStats, int iv, int ev, int level, int mod) {
    int stats;
    stats = (baseStats*2 + iv + ev/4) * level/100 + 5;
    stats = stats * mod / 10;
    return stats;
}

int MainWindow::calcHP(int baseStats, int iv, int ev, int level) {
    int stats;
    stats = (baseStats*2 + iv + ev/4) * level/100 + 10 + level;
    return stats;
}
