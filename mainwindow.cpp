#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    hasSetUpParam = false;
    SSS4Config sss4Config("config.txt");
    if(sss4Config.exist() && sss4Config.parse()) {
        param = sss4Config.getParameter();
        hasSetUpParam = true;
    }
    else {
        ParameterSettingDialog *dialog = new ParameterSettingDialog(this);
        dialog->setParameter(param);
        dialog->setHasSetUpParam(false);
        dialog->show();
        hasSetUpParam = true;
    }


    initCB_PokemonName();
    initCB_NatureName();
    updateStats();

    for(int i=0;i<6;i++) {
        lowerIVs.append(31);
        upperIVs.append(0);
    }

    connect(&satisfiedIV, SIGNAL(notifyComplated(const QString&)), this, SLOT(onComplated(const QString&)));
    connect(&satisfiedLCG, SIGNAL(notifyComplated(const QString&)), this, SLOT(onComplated(const QString&)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BTN_EXEC_clicked()
{
    loadDateTime();
    initKeyInputs();


    if(hasSetUpParam) {
        bool ok;
        ui->TE_Result->setText("");
        if(ui->tabWidget->currentIndex() == 0) {
            calcIVs();
            QString ivRange;
            ivRange = "個体値の範囲\n";
            for(int i=0;i<6;i++) {
                QString t;
                t.sprintf("%d-%d\n",lowerIVs[i],upperIVs[i]);
                ivRange += t;
            }
            ui->TE_Result->setText(ivRange);

            if(!validIVs()) {
                QMessageBox msgBox;
                msgBox.setText("入力した実数値が個体値0-31に収まっていません");
                msgBox.exec();
                return;
            }

            satisfiedIV.setParameter(param);
            satisfiedIV.setLowerIVs(lowerIVs);
            satisfiedIV.setUpperIVs(upperIVs);
            satisfiedIV.setFirstDateTimeIt(firstDateTimeIt);
            satisfiedIV.setLastDateTimeIt(lastDateTimeIt);
            satisfiedIV.setFirstFrame(ui->SB_FirstFrame->text().toInt(&ok));
            satisfiedIV.setLastFrame(ui->SB_LastFrame->text().toInt(&ok));
            satisfiedIV.setKeyInputs(keyInputs);
            satisfiedIV.start();
        }
        else if(ui->tabWidget->currentIndex() == 1) {
            QList<PokeRNG::u64> lcgSeq;
            QString repohari = ui->LE_Repohari->text();
            for(const auto &val : repohari) {
                lcgSeq.append(val.digitValue());
            }
            satisfiedLCG.setParameter(param);
            satisfiedLCG.setLCGSequence(lcgSeq);
            satisfiedLCG.setFirstDateTimeIt(firstDateTimeIt);
            satisfiedLCG.setLastDateTimeIt(lastDateTimeIt);
            satisfiedLCG.setFirstFrame(ui->SB_FirstLCGFrame->text().toInt(&ok));
            satisfiedLCG.setLastFrame(ui->SB_LastLCGFrame->text().toInt(&ok));
            satisfiedLCG.setKeyInputs(keyInputs);
            satisfiedLCG.shouldCalcOffset(false);
            satisfiedLCG.start();
        }
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

    firstDateTimeIt = PokeRNG::DateTimeIterator(beginDateTime,beginDateTime,endDateTime);
    lastDateTimeIt = PokeRNG::DateTimeIterator(endDateTime,beginDateTime,endDateTime);
    PokeRNG::DateTimeIterator it(dateTime,beginDateTime,endDateTime);
    PokeRNG::DateTimeIterator tmp(dateTime,beginDateTime,endDateTime);

    secondRange = ui->SB_SecondRange->text().toInt(&ok);

    for(int i=0;i<secondRange&&it!=firstDateTimeIt;i++) {
        --it;
    }
    firstDateTimeIt=it;
    it=tmp;

    for(int i=0;i<secondRange&&it!=lastDateTimeIt;i++) {
        ++it;
    }
    lastDateTimeIt=it;
}

void MainWindow::calcIVs() {
    for(int i=0;i<6;i++) {
        lowerIVs[i]=31;
        upperIVs[i]=0;
    }
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

    ui->SB_StatsA->setValue(calcStats(baseStats.getAttack(),31,0,level,nature.getAttackMod()));
    ui->SB_StatsB->setValue(calcStats(baseStats.getDefense(),31,0,level,nature.getDefenseMod()));
    ui->SB_StatsC->setValue(calcStats(baseStats.getSpAtk(),31,0,level,nature.getSpAtkMod()));
    ui->SB_StatsD->setValue(calcStats(baseStats.getSpDef(),31,0,level,nature.getSpDefMod()));
    ui->SB_StatsS->setValue(calcStats(baseStats.getSpeed(),31,0,level,nature.getSpeedMod()));



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
    dialog->setHasSetUpParam(hasSetUpParam);
    dialog->show();
}

void MainWindow::setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param) {
    this->param = param;
}

void MainWindow::initKeyInputs() {
    keyInputs.clear();
    QList<std::pair<int,int>> keySet;
    keySet.append(std::make_pair(ui->CB_KeyInputA->checkState() == Qt::Checked,0));
    keySet.append(std::make_pair(ui->CB_KeyInputB->checkState() == Qt::Checked,1));
    keySet.append(std::make_pair(ui->CB_KeyInputSe->checkState() == Qt::Checked,2));
    keySet.append(std::make_pair(ui->CB_KeyInputSt->checkState() == Qt::Checked,3));
    keySet.append(std::make_pair(ui->CB_KeyInputRight->checkState() == Qt::Checked,4));
    keySet.append(std::make_pair(ui->CB_KeyInputLeft->checkState() == Qt::Checked,5));
    keySet.append(std::make_pair(ui->CB_KeyInputUp->checkState() == Qt::Checked,6));
    keySet.append(std::make_pair(ui->CB_KeyInputDown->checkState() == Qt::Checked,7));
    keySet.append(std::make_pair(ui->CB_KeyInputR->checkState() == Qt::Checked,8));
    keySet.append(std::make_pair(ui->CB_KeyInputL->checkState() == Qt::Checked,9));
    keySet.append(std::make_pair(ui->CB_KeyInputX->checkState() == Qt::Checked,10));
    keySet.append(std::make_pair(ui->CB_KeyInputY->checkState() == Qt::Checked,11));

    keyInputs.reserve(4096);

    for(int bit=0;bit<4096;++bit) {
        if(std::all_of(keySet.begin(),keySet.end(),
                       [bit](std::pair<int,int> elem) { return (elem.first || !(bit>>elem.second&1));})) {
            keyInputs.append(bit ^ 0x2fff);
        }
    }
}

/**
 * @fn
 * 個体値候補が有効か調べる
 * @brief MainWindow::validIVs
 * @return 有効ならtrue
 */
bool MainWindow::validIVs() {
    bool ret=true;
    for(int i=0;i<6;i++) {
        ret &= (lowerIVs[i] <= upperIVs[i]);
    }

    return ret;
}
