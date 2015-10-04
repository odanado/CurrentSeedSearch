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
    updateStats();

    for(int i=0;i<6;i++) {
        lowerIVs.append(31);
        upperIVs.append(0);
    }

    connect(&satisfiedIV, SIGNAL(notifyDateChanged(const QString&)), this, SLOT(onDateChanged(const QString&)));
    connect(&satisfiedIV, SIGNAL(notifyComplated(const QString&)), this, SLOT(onComplated(const QString&)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BTN_EXEC_clicked()
{
    calcIVs();
    loadDateTime();


    if(isSetParam) {
        QString ivRange;
        ivRange = "個体値の範囲\n";
        for(int i=0;i<6;i++) {
            QString t;
            t.sprintf("%d-%d\n",lowerIVs[i],upperIVs[i]);
            ivRange += t;
        }

        bool ok;
        ui->TE_Result->setText(ivRange);
        satisfiedIV.setParameter(param);
        satisfiedIV.setLowerIVs(lowerIVs);
        satisfiedIV.setUpperIVs(upperIVs);
        satisfiedIV.setDateTimeRange(range);
        satisfiedIV.setFirstFrame(ui->SB_FirstFrame->text().toInt(&ok));
        satisfiedIV.setLastFrame(ui->SB_LastFrame->text().toInt(&ok));
        satisfiedIV.start();
    }
}

void MainWindow::loadDateTime() {
    int secondRange;
    bool ok;
    PokeRNG::DateTime dateTime,beginDateTime,endDateTime;
    dateTime.set_year(ui->SB_Year->text().toInt(&ok));
    dateTime.set_month(ui->SB_Month->text().toInt(&ok));
    dateTime.set_day(ui->SB_Day->text().toInt(&ok));
    dateTime.set_hour(ui->SB_Hour->text().toInt(&ok));
    dateTime.set_minute(ui->SB_Minute->text().toInt(&ok));
    dateTime.set_second(ui->SB_Second->text().toInt(&ok));

    beginDateTime.set_year(0);
    beginDateTime.set_month(1);
    beginDateTime.set_day(1);
    beginDateTime.set_hour(0);
    beginDateTime.set_minute(0);
    beginDateTime.set_second(0);

    endDateTime.set_year(99);
    endDateTime.set_month(12);
    endDateTime.set_day(31);
    endDateTime.set_hour(23);
    endDateTime.set_minute(59);
    endDateTime.set_second(59);

    PokeRNG::DateTimeIterator beginIt(beginDateTime,beginDateTime,endDateTime);
    PokeRNG::DateTimeIterator endIt(endDateTime,beginDateTime,endDateTime);
    PokeRNG::DateTimeIterator it(dateTime,beginDateTime,endDateTime);
    PokeRNG::DateTimeIterator tmp(dateTime,beginDateTime,endDateTime);

    secondRange = ui->SB_SecondRange->text().toInt(&ok);

    for(int i=0;i<secondRange&&it!=beginIt;i++) {
        --it;
    }
    beginIt=it;
    it=tmp;

    for(int i=0;i<secondRange&&it!=endIt;i++) {
        ++it;
    }
    endIt=it;

    range.set_year((*beginIt).get_year(),(*endIt).get_year());
    range.set_month((*beginIt).get_month(),(*endIt).get_month());
    range.set_day((*beginIt).get_day(),(*endIt).get_day());
    range.set_hour((*beginIt).get_hour(),(*endIt).get_hour());
    range.set_minute((*beginIt).get_minute(),(*endIt).get_minute());
    range.set_second((*beginIt).get_second(),(*endIt).get_second());
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

    for(PokeRNG::u32 iv=0;iv<32;iv++) {
        int hp = calcHP(baseStats[0],iv,0,level);
        if(hp == stats[0]) {
            lowerIVs[0]=std::min(lowerIVs[0],iv);
            upperIVs[0]=std::max(upperIVs[0],iv);
        }
    }

    for(int i=1;i<6;i++) {
        for(PokeRNG::u32 iv=0;iv<32;iv++) {
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


void MainWindow::updateStats() {
    bool ok;

    const auto &baseStats = PokeData::pokemons[ui->CB_PokemonName->currentIndex()].getBaseStats();
    const auto &nature = PokeData::natures[ui->CB_NatureName->currentIndex()];
    int level = ui->SB_Level->text().toInt(&ok);

    ui->SB_StatsH->setValue(calcHP(baseStats.getHP(),31,0,level));

    ui->SB_StatsA->setValue(calcStats(baseStats.getAttack(),31,9,level,nature.getAttackMod()));
    ui->SB_StatsB->setValue(calcStats(baseStats.getDefense(),31,9,level,nature.getDefenseMod()));
    ui->SB_StatsC->setValue(calcStats(baseStats.getSpAtk(),31,9,level,nature.getSpAtkMod()));
    ui->SB_StatsD->setValue(calcStats(baseStats.getSpDef(),31,9,level,nature.getSpDefMod()));
    ui->SB_StatsS->setValue(calcStats(baseStats.getSpeed(),31,9,level,nature.getSpeedMod()));



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

void MainWindow::onDateChanged(const QString &title) {
    setWindowTitle(title);
}

void MainWindow::onComplated(const QString &result) {
    ui->TE_Result->append(result);
}

void MainWindow::on_CB_PokemonName_currentIndexChanged(int index)
{
    updateStats();
}

void MainWindow::on_CB_PokemonName_currentTextChanged(const QString &arg1)
{
    updateStats();
}

void MainWindow::on_CB_PokemonName_editTextChanged(const QString &arg1)
{
    std::string name = arg1.toStdString();
    auto it = PokeData::pokemonIndexes.find(name);
    if(it != PokeData::pokemonIndexes.end()) {
        updateStats();
    }
}

void MainWindow::on_SB_Level_valueChanged(int arg1)
{
    updateStats();
}

void MainWindow::on_CB_NatureName_currentTextChanged(const QString &arg1)
{
    updateStats();
}

void MainWindow::on_CB_NatureName_currentIndexChanged(int index)
{
    updateStats();
}


void MainWindow::on_BTN_ParameterSetting_clicked()
{
    ParameterSettingDialog *dialog = new ParameterSettingDialog(this);
    dialog->setParameter(param);
    dialog->show();
}
