#ifndef SATISFIEDIV_H
#define SATISFIEDIV_H

#include <QThread>
#include <QList>
#include <QString>
#include <PokeRNG/PokeRNG.hpp>

#include "satisfiedresult.h"

class SatisfiedIV : public QThread
{
    Q_OBJECT

protected:
    virtual void run();
public:
    SatisfiedIV();

    void setFirstDateTimeIt(const PokeRNG::DateTimeIterator &firstDateTimeIt);
    void setLastDateTimeIt(const PokeRNG::DateTimeIterator &lastDateTimeIt);
    void setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param);

    void setLowerIVs(const QList<PokeRNG::u32> &lowerIVs);
    void setUpperIVs(const QList<PokeRNG::u32> &upperIVs);
    void setFirstFrame(PokeRNG::u64 firstFrame);
    void setLastFrame(PokeRNG::u64 lastFrame);
    void setKeyInputs(const QList<PokeRNG::u32> &keyInputs);

private:
    PokeRNG::DateTimeIterator firstDateTimeIt,lastDateTimeIt;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    QList<PokeRNG::u32> lowerIVs,upperIVs;
    PokeRNG::u64 firstFrame,lastFrame;
    QList<PokeRNG::u32> keyInputs;

    PokeRNG::Calc5GenSeed calc5GenSeed;
    PokeRNG::LCG5Gen lcg;
    PokeRNG::MT mt;

    void calc(QList<SatisfiedResult> *results);

signals:
    void notifyComplated(const QString&);

};

#endif // SATISFIEDIV_H
