#ifndef SATISFIEDIV_H
#define SATISFIEDIV_H

#include <QThread>
#include <QList>
#include <QString>
#include <PokeRNG/PokeRNG.hpp>

#include "satisfiedivresult.h"

class SatisfiedIV : public QThread
{
    Q_OBJECT

protected:
    virtual void run();
public:
    SatisfiedIV() = default;

    void setDateTimeRange(const PokeRNG::DateTimeRange &range);
    void setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param);

    void setLowerIVs(const QList<PokeRNG::u32> &lowerIVs);
    void setUpperIVs(const QList<PokeRNG::u32> &upperIVs);
    void setFirstFrame(PokeRNG::u64 firstFrame);
    void setLastFrame(PokeRNG::u64 lastFrame);

private:
    PokeRNG::DateTimeRange dateTimeRange;
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    QList<PokeRNG::u32> lowerIVs,upperIVs;
    PokeRNG::u64 firstFrame,lastFrame;

    PokeRNG::Calc5GenSeed calc5GenSeed;
    PokeRNG::LCG5Gen lcg;
    PokeRNG::MT mt;

    void calc(QList<SatisfiedIVResult> *results);


signals:
    void notifyDateChanged(const QString&);
    void notifyComplated(const QString&);

};

#endif // SATISFIEDIV_H
