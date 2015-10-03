#include "sss4config.h"

SSS4Config::SSS4Config()
{

}

bool SSS4Config::parse() {
    QFile file("config.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // ファイルがない
        return false;
    }

    QTextStream in(&file);
    QStringList configLines = in.readAll().split('\n');
    QList<PokeRNG::u32> macAddrs;
    QList<PokeRNG::u32> nazoValues;
    PokeRNG::u32 vcount, gxstat, frame, timer0Min, timer0Max;
    bool ok;

    if(configLines.count() == 7) {
        // 古いバージョン
        vcount = configLines.at(0).toInt(&ok,16);
        if(!ok) return false;

        timer0Min = timer0Max = configLines.at(1).toInt(&ok,16);
        --timer0Min;
        ++timer0Max;
        if(!ok) return false;

        gxstat = configLines.at(2).toInt(&ok,16);
        if(!ok) return false;

        frame = configLines.at(3).toInt(&ok,16);
        if(!ok) return false;

        for(const auto &macAddr : configLines.at(4).split('-')){
            macAddrs.append(macAddr.toInt(&ok,16));
            if(!ok) return false;
        }

        PokeRNG::u32 nazo = configLines.at(5).toInt(&ok, 16);
        if(!ok) return false;
        nazoValues.append(nazo);
        nazoValues.append(nazo+0xfc);
        nazoValues.append(nazo+0xfc);
        nazoValues.append(nazo+0x148);
        nazoValues.append(nazo+0x148);

    }
    else if(configLines.count() == 9) {
        // 新しいバージョン
        vcount = configLines.at(0).toInt(&ok,16);
        if(!ok) return false;

        timer0Min = configLines.at(1).toInt(&ok,16);
        if(!ok) return false;

        timer0Max = configLines.at(2).toInt(&ok,16);
        if(!ok) return false;

        gxstat = configLines.at(4).toInt(&ok,16);
        if(!ok) return false;

        frame = configLines.at(5).toInt(&ok,16);
        if(!ok) return false;


        for(const auto &macAddr : configLines.at(6).split('-')){
            macAddrs.append(macAddr.toInt(&ok,16));
            if(!ok) return false;
        }

        // TODO 最後の一行でnazo値の値を分岐する
    }
    else {
        // それ以外
        return false;
    }

    param.set_vcount(vcount);
    param.set_gxstat(gxstat);
    param.set_frame(frame);
    param.set_timer0_min(timer0Min);
    param.set_timer0_max(timer0Max);
    param.set_timer0(timer0Min);
    param.set_mac_addr(macAddrs[0],macAddrs[1],macAddrs[2],macAddrs[3],macAddrs[4],macAddrs[5]);
    param.set_nazo1(nazoValues[0]);
    param.set_nazo2(nazoValues[1]);
    param.set_nazo3(nazoValues[2]);
    param.set_nazo4(nazoValues[3]);
    param.set_nazo5(nazoValues[4]);

    return true;
}

PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> SSS4Config::getParameter() {
    return param;
}
