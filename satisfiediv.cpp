#include "satisfiediv.h"

#define CHECK_IV(i) (lowerIVs[i]<=IVs[i]&&IVs[i]<=upperIVs[i])

void SatisfiedIV::run() {
    QString title,result;
    QList<SatisfiedIVResult> results;
    for(auto timer0=param.get_timer0_min();timer0<=param.get_timer0_max();++timer0) {
        param.set_timer0(timer0);
        for(const auto &dateTime : dateTimeRange) {
            title.sprintf("%2d/%2d/%2d",dateTime.get_year(),dateTime.get_month(),dateTime.get_day());
            emit notifyDateChanged(title);
            param.set_date_time(dateTime);

            calc(&results);
        }
    }

    param.set_date_time(*dateTimeRange.end());
    for(auto timer0=param.get_timer0_min();timer0<=param.get_timer0_max();++timer0) {
        param.set_timer0(timer0);
        calc(&results);
    }

    int cnt=0;
    for(const auto &ret : results) {
        cnt+=ret.getSatisfiedFrames().size();
    }
    result.sprintf("候補件数%d \n",cnt);

    for(const auto &ret : results) {
        QString t;

        auto dateTime=ret.getDateTime();
        t += QString::asprintf("%2d年%2d月%2d日 %2d時%2d分%2d秒",
                               dateTime.get_year(),dateTime.get_month(),dateTime.get_day(),
                               dateTime.get_hour(),dateTime.get_minute(),dateTime.get_second());
        t += QString::asprintf("timer0: %x\n",ret.getTimer0());
        t += QString::asprintf("seed1: %016llX\n",ret.getSeed1());
        t += "消費数: ";
        for(int i=0;i<ret.getSatisfiedFrames().size();++i) {
            t += QString::number(ret.getSatisfiedFrames()[i]);
            if(i+1 != ret.getSatisfiedFrames().size()) {
                t += ", ";
            }
        }

        result += t+"\n";
    }

    emit notifyComplated(result);
}

void SatisfiedIV::calc(QList<SatisfiedIVResult> *results) {
    PokeRNG::u32 IVs[6]={};
    PokeRNG::u64 cur;
    QList<PokeRNG::u32> satisfiedFrames;

    auto seed1 = lcg.next(calc5GenSeed(param));
    mt.seed(seed1>>32);

    for(cur=0;cur<firstFrame;++cur) mt();
    for(int i=0;i<6;++i) IVs[i] = mt()>>27;

    for(cur=firstFrame;cur<=lastFrame;++cur) {
        if(CHECK_IV(0) && CHECK_IV(1) && CHECK_IV(2) && CHECK_IV(3) && CHECK_IV(4) && CHECK_IV(5)) {
            satisfiedFrames.append(cur);
        }
        IVs[0]=IVs[1];
        IVs[1]=IVs[2];
        IVs[2]=IVs[3];
        IVs[3]=IVs[4];
        IVs[4]=IVs[5];
        IVs[5]=mt()>>27;
    }

    if(satisfiedFrames.size()) {
        results->append(SatisfiedIVResult(seed1,param.get_date_time(),satisfiedFrames,param.get_timer0(),param.get_key()));
    }
}

void SatisfiedIV::setDateTimeRange(const PokeRNG::DateTimeRange &range) {
    this->dateTimeRange = range;
}

void SatisfiedIV::setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param) {
    this->param = param;
}

void SatisfiedIV::setLowerIVs(const QList<PokeRNG::u32> &lowerIVs) {
    this->lowerIVs = lowerIVs;
}

void SatisfiedIV::setUpperIVs(const QList<PokeRNG::u32> &upperIVs) {
    this->upperIVs = upperIVs;
}

void SatisfiedIV::setFirstFrame(PokeRNG::u64 firstFrame) {
    this->firstFrame = firstFrame;
}

void SatisfiedIV::setLastFrame(PokeRNG::u64 lastFrame) {
    this->lastFrame = lastFrame;
}



