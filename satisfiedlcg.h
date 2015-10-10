#ifndef SATISFIEDLCG_H
#define SATISFIEDLCG_H

#include <QThread>
#include <QList>
#include <QString>
#include <PokeRNG/PokeRNG.hpp>
#include <algorithm>

#include "satisfiedresult.h"

class SatisfiedLCG : public QThread
{
    Q_OBJECT
protected:
    virtual void run();
public:
    SatisfiedLCG();

    void setFirstDateTimeIt(const PokeRNG::DateTimeIterator &firstDateTimeIt);
    void setLastDateTimeIt(const PokeRNG::DateTimeIterator &lastDateTimeIt);
    void setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param);

    void setLCGSequence(const QList<PokeRNG::u64> &lcgSequence);
    void setFirstFrame(PokeRNG::u64 firstFrame);
    void setLastFrame(PokeRNG::u64 lastFrame);
    void setKeyInputs(const QList<PokeRNG::u32> &keyInputs);

    void shouldCalcOffset(bool shouldCalcOffset_);
    void isBW2(bool BW2_);
    void existMemoryLink(bool existMemoryLink_);

private:
    PokeRNG::DateTimeIterator firstDateTimeIt,lastDateTimeIt;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    QList<PokeRNG::u64> lcgSequence;
    PokeRNG::u64 firstFrame,lastFrame;
    QList<PokeRNG::u32> keyInputs;

    PokeRNG::Calc5GenSeed calc5GenSeed;
    PokeRNG::LCG5Gen lcg;
    PokeRNG::CalcOffset calcOffset;

    bool shouldCalcOffset_;
    bool isBW2_;
    bool existMemoryLink_;


    void calc(QList<SatisfiedResult> *results);
signals:
    void notifyComplated(const QString&);

};

#endif // SATISFIEDLCG_H
