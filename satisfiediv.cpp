#include "satisfiediv.h"

#define CHECK_IV(i) (lowerIVs[i]<=IVs[i]&&IVs[i]<=upperIVs[i])

void SatisfiedIV::run() {
    QString title,result;
    for(auto timer0=param.get_timer0_min();timer0<=param.get_timer0_max();++timer0) {
        param.set_timer0(timer0);
        for(const auto &dateTime : dateTimeRange) {
            title.sprintf("%2d/%2d/%2d",dateTime.get_year(),dateTime.get_month(),dateTime.get_day());
            emit notifyDateChanged(title);

            result+=calc(dateTime, timer0);
        }
    }

    for(auto timer0=param.get_timer0_min();timer0<=param.get_timer0_max();++timer0) {
        param.set_timer0(timer0);
        result += calc(*dateTimeRange.end(),timer0);
    }

    emit notifyComplated(result);
}

QString SatisfiedIV::calc(const PokeRNG::DateTime &dateTime, const PokeRNG::u32 timer0) {
    QString result="消費数: ";
    PokeRNG::u32 IVs[6]={};
    PokeRNG::u64 cur;
    param.set_date_time(dateTime);

    auto seed1 = lcg.next(calc5GenSeed(param));
    mt.seed(seed1>>32);

    for(cur=0;cur<firstFrame;++cur) mt();
    for(int i=0;i<6;++i) IVs[i] = mt()>>27;

    for(cur=firstFrame;cur<=lastFrame;++cur) {
        if(CHECK_IV(0) && CHECK_IV(1) && CHECK_IV(2) && CHECK_IV(3) && CHECK_IV(4) && CHECK_IV(5)) {
            result+=QString::number(cur)+", ";
        }
        IVs[0]=IVs[1];
        IVs[1]=IVs[2];
        IVs[2]=IVs[3];
        IVs[3]=IVs[4];
        IVs[4]=IVs[5];
        IVs[5]=mt()>>27;
    }

    if(result != "消費数: ") {
        QString subject;
        subject.sprintf("seed1 = %llX\n%d年%d月%d日 %d時%d分%d秒 timer0 %X\n",seed1,
                        dateTime.get_year(),dateTime.get_month(),dateTime.get_day(),
                        dateTime.get_hour(),dateTime.get_minute(),dateTime.get_second(),
                        timer0);

        result = subject+result;
    }

    return result;
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



