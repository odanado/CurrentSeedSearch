#ifndef SATISFIEDIVRESULT_H
#define SATISFIEDIVRESULT_H

#include <PokeRNG/PokeRNG.hpp>
#include <QList>

class SatisfiedIVResult
{
public:
    SatisfiedIVResult();
    void setSeed1(PokeRNG::u64 seed1);
    void setDateTime(const PokeRNG::DateTime &dateTime);
    void setSatisfiedFrames(const QList<PokeRNG::u32> &satisfiedFrames);
    void setTimer0(PokeRNG::u32 timer0);
    void setKey(PokeRNG::u32 key);

    PokeRNG::u64 getSeed1() const;
    PokeRNG::DateTime getDateTime() const;
    QList<PokeRNG::u32> getSatisfiedFrames() const;
    PokeRNG::u32 getTimer0() const;
    PokeRNG::u32 getKey() const;

private:
    PokeRNG::u64 seed1;
    PokeRNG::DateTime dateTime;
    QList<PokeRNG::u32> satisfiedFrames;
    PokeRNG::u32 timer0;
    PokeRNG::u32 key;
};

#endif // SATISFIEDIVRESULT_H
