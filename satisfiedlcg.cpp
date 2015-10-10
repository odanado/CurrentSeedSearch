#include "satisfiedlcg.h"

#include <QDebug>

SatisfiedLCG::SatisfiedLCG()
{

}

void SatisfiedLCG::run() {
    QList<SatisfiedResult> results;
    for(const auto &keyInput : keyInputs) {
        param.set_key(keyInput);
        for(auto timer0=param.get_timer0_min();timer0<=param.get_timer0_max();++timer0) {
            param.set_timer0(timer0);
            for(auto it=firstDateTimeIt;it!=lastDateTimeIt;++it) {
                param.set_date_time(*it);
                calc(&results);
            }
        }
    }

    param.set_date_time(*lastDateTimeIt);
    for(const auto &keyInput : keyInputs) {
        param.set_key(keyInput);
        for(auto timer0=param.get_timer0_min();timer0<=param.get_timer0_max();++timer0) {
            param.set_timer0(timer0);
            calc(&results);
        }
    }
    int cnt=0;
    for(const auto &ret : results) {
        cnt+=ret.getSatisfiedFrames().size();
    }
    QString result;
    result.sprintf("候補件数%d \n",cnt);
    for(const auto &ret : results) {
        QString t;

        auto dateTime=ret.getDateTime();
        t += QString::asprintf("%2d年%2d月%2d日 %2d時%2d分%2d秒\n",
                               dateTime.get_year(),dateTime.get_month(),dateTime.get_day(),
                               dateTime.get_hour(),dateTime.get_minute(),dateTime.get_second());
        t += QString::asprintf("timer0: %x\n",ret.getTimer0());

        if(keyInputs.size() > 1) {
            t += "キー入力: " + ret.getKeyText() + "\n";
        }
        t += QString::asprintf("seed1: %016llX\n",ret.getSeed1());
        t += "次に使われる乱数の位置: ";
        for(int i=0;i<ret.getSatisfiedFrames().size();++i) {
            t += QString::number(ret.getSatisfiedFrames()[i] + lcgSequence.size());
            if(i+1 != ret.getSatisfiedFrames().size()) {
                t += ", ";
            }
        }

        result += t+"\n\n";
    }

    emit notifyComplated(result);
}

void SatisfiedLCG::calc(QList<SatisfiedResult> *results) {
    PokeRNG::u64 cur = 0;
    QList<PokeRNG::u64> satisfiedFrames;

    auto seed1 = lcg.next(calc5GenSeed(param));
    auto seed = seed1;
    if(shouldCalcOffset_) {
        if(isBW2_) {
            cur = calcOffset.bw2(seed1,true,true,existMemoryLink_);
        }
        else {
            cur = calcOffset.bw1(seed1,true,true);
        }
    }

    cur += firstFrame;
    seed = lcg.next_n(seed, cur);

    for(int i=0;i<=lastFrame-firstFrame;++i) {
        // 更新->判定だから1つ戻しておく
        PokeRNG::u64 r = lcg.prev(seed);
        if(std::all_of(lcgSequence.begin(), lcgSequence.end(),[this,&r,seed1](PokeRNG::u64 x){
                       r=lcg.next(r);
                       return ((r>>61ULL))==x;}))
        {
            satisfiedFrames.append(cur+i);
        }
        seed = lcg.next(seed);
    }

    if(satisfiedFrames.size()) {
        results->append(SatisfiedResult(seed1,param.get_date_time(),satisfiedFrames,param.get_timer0(),param.get_key()));
    }
}

void SatisfiedLCG::setFirstDateTimeIt(const PokeRNG::DateTimeIterator &firstDateTimeIt) {
    this->firstDateTimeIt = firstDateTimeIt;
}

void SatisfiedLCG::setLastDateTimeIt(const PokeRNG::DateTimeIterator &lastDateTimeIt) {
    this->lastDateTimeIt = lastDateTimeIt;
}

void SatisfiedLCG::setParameter(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param) {
    this->param = param;
}

void SatisfiedLCG::setLCGSequence(const QList<PokeRNG::u64> &lcgSequence) {
    this->lcgSequence = lcgSequence;
}

void SatisfiedLCG::setFirstFrame(PokeRNG::u64 firstFrame) {
    this->firstFrame = firstFrame;
}

void SatisfiedLCG::setLastFrame(PokeRNG::u64 lastFrame) {
    this->lastFrame = lastFrame;
}

void SatisfiedLCG::setKeyInputs(const QList<PokeRNG::u32> &keyInputs) {
    this->keyInputs = keyInputs;
}

 void SatisfiedLCG::shouldCalcOffset(bool shouldCalcOffset_) {
     this->shouldCalcOffset_ = shouldCalcOffset_;
 }

 void SatisfiedLCG::isBW2(bool isBW2_) {
     this->isBW2_ = isBW2_;
 }

 void SatisfiedLCG::existMemoryLink(bool existMemoryLink_) {
     this->existMemoryLink_ = existMemoryLink_;
 }
