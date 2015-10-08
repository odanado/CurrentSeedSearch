#include "satisfiedivresult.h"

SatisfiedIVResult::SatisfiedIVResult()
{

}

SatisfiedIVResult::SatisfiedIVResult(PokeRNG::u64 seed1, const PokeRNG::DateTime &dateTime, const QList<PokeRNG::u32> &satisfiedFrames, PokeRNG::u32 timer0, PokeRNG::u32 key) :
seed1(seed1), dateTime(dateTime), satisfiedFrames(satisfiedFrames), timer0(timer0), key(key){

}

void SatisfiedIVResult::setSeed1(PokeRNG::u64 seed1) {
    this->seed1 = seed1;
}

void SatisfiedIVResult::setDateTime(const PokeRNG::DateTime &dateTime) {
    this->dateTime = dateTime;
}

void SatisfiedIVResult::setSatisfiedFrames(const QList<PokeRNG::u32> &satisfiedFrames) {
    this->satisfiedFrames = satisfiedFrames;
}

void SatisfiedIVResult::setTimer0(PokeRNG::u32 timer0) {
    this->timer0 = timer0;
}

void SatisfiedIVResult::setKey(PokeRNG::u32 key) {
    this->key = key;
}

PokeRNG::u64 SatisfiedIVResult::getSeed1() const {
    return seed1;
}

PokeRNG::DateTime SatisfiedIVResult::getDateTime() const {
    return dateTime;
}

QList<PokeRNG::u32> SatisfiedIVResult::getSatisfiedFrames() const {
    return satisfiedFrames;
}

PokeRNG::u32 SatisfiedIVResult::getTimer0() const {
    return timer0;
}

PokeRNG::u32 SatisfiedIVResult::getKey() const {
    return key;
}
