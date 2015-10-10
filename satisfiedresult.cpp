#include "satisfiedresult.h"

SatisfiedResult::SatisfiedResult()
{

}

SatisfiedResult::SatisfiedResult(PokeRNG::u64 seed1, const PokeRNG::DateTime &dateTime, const QList<PokeRNG::u64> &satisfiedFrames, PokeRNG::u32 timer0, PokeRNG::u32 key) :
seed1(seed1), dateTime(dateTime), satisfiedFrames(satisfiedFrames), timer0(timer0), key(key){

}

const QStringList SatisfiedResult::keyTexts = QStringList({"A","B","Se","St","右","左","上","下","R","L","X","Y"});

QString SatisfiedResult::keyDecode(PokeRNG::u32 keyInput) const {
    keyInput ^= 0x2fff;
    QString ret;
    for(int i=0;i<keyTexts.size();i++) {
        if(keyInput>>i&1) ret += keyTexts[i];
    }

    return ret;
}

QString SatisfiedResult::getKeyText() const {
    return keyDecode(key);
}

void SatisfiedResult::setSeed1(PokeRNG::u64 seed1) {
    this->seed1 = seed1;
}

void SatisfiedResult::setDateTime(const PokeRNG::DateTime &dateTime) {
    this->dateTime = dateTime;
}

void SatisfiedResult::setSatisfiedFrames(const QList<PokeRNG::u64> &satisfiedFrames) {
    this->satisfiedFrames = satisfiedFrames;
}

void SatisfiedResult::setTimer0(PokeRNG::u32 timer0) {
    this->timer0 = timer0;
}

void SatisfiedResult::setKey(PokeRNG::u32 key) {
    this->key = key;
}

PokeRNG::u64 SatisfiedResult::getSeed1() const {
    return seed1;
}

PokeRNG::DateTime SatisfiedResult::getDateTime() const {
    return dateTime;
}

QList<PokeRNG::u64> SatisfiedResult::getSatisfiedFrames() const {
    return satisfiedFrames;
}

PokeRNG::u32 SatisfiedResult::getTimer0() const {
    return timer0;
}

PokeRNG::u32 SatisfiedResult::getKey() const {
    return key;
}
