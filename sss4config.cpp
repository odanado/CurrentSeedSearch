#include "sss4config.h"
#include <QDebug>

SSS4Config::SSS4Config(const QString &fileName) :
    fileName(fileName) {

}

bool SSS4Config::exist() {
    QFile file(fileName);
    return file.open(QIODevice::ReadOnly | QIODevice::Text);
}

bool SSS4Config::parse() {

    if(!exist()) {
        // ファイルがない
        return false;
    }

    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
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
    else if(configLines.count() == 9 || configLines.count() == 10 || configLines.count() == 14) {
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

        int nazoCond = configLines.at(7).toInt(&ok);


        if(configLines.count() == 9) {
            if(nazoCond == 0) {
                // B2
                nazoValues.append(PokeRNG::ROMType::B2Ja::nazo1);
                nazoValues.append(PokeRNG::ROMType::B2Ja::nazo2);
                nazoValues.append(PokeRNG::ROMType::B2Ja::nazo3);
                nazoValues.append(PokeRNG::ROMType::B2Ja::nazo4);
                nazoValues.append(PokeRNG::ROMType::B2Ja::nazo5);

            }
            else if(nazoCond == 1) {
                // W2
                nazoValues.append(PokeRNG::ROMType::W2Ja::nazo1);
                nazoValues.append(PokeRNG::ROMType::W2Ja::nazo2);
                nazoValues.append(PokeRNG::ROMType::W2Ja::nazo3);
                nazoValues.append(PokeRNG::ROMType::W2Ja::nazo4);
                nazoValues.append(PokeRNG::ROMType::W2Ja::nazo5);
            }
            else if(nazoCond == 3) {
                // B1
                nazoValues.append(PokeRNG::ROMType::B1Ja::nazo1);
                nazoValues.append(PokeRNG::ROMType::B1Ja::nazo2);
                nazoValues.append(PokeRNG::ROMType::B1Ja::nazo3);
                nazoValues.append(PokeRNG::ROMType::B1Ja::nazo4);
                nazoValues.append(PokeRNG::ROMType::B1Ja::nazo5);
            }
            else if(nazoCond == 4) {
                // W1
                nazoValues.append(PokeRNG::ROMType::W1Ja::nazo1);
                nazoValues.append(PokeRNG::ROMType::W1Ja::nazo2);
                nazoValues.append(PokeRNG::ROMType::W1Ja::nazo3);
                nazoValues.append(PokeRNG::ROMType::W1Ja::nazo4);
                nazoValues.append(PokeRNG::ROMType::W1Ja::nazo5);
            }
            else {
                return false;
            }
        }
        if(configLines.count() == 10) {
            if(nazoCond == 5) {
                // 1つ指定
                PokeRNG::u32 nazo = configLines.at(8).toInt(&ok, 16);
                if(!ok) return false;
                nazoValues.append(nazo);
                nazoValues.append(nazo+0xfc);
                nazoValues.append(nazo+0xfc);
                nazoValues.append(nazo+0x148);
                nazoValues.append(nazo+0x148);
            }
            else {
                return false;
            }
        }

        if(configLines.count() == 14) {
            if(nazoCond == 2) {
                // 5つ指定
                nazoValues.append(configLines.at(8).toInt(&ok, 16));
                if(!ok) return false;
                nazoValues.append(configLines.at(9).toInt(&ok, 16));
                if(!ok) return false;
                nazoValues.append(configLines.at(10).toInt(&ok, 16));
                if(!ok) return false;
                nazoValues.append(configLines.at(11).toInt(&ok, 16));
                if(!ok) return false;
                nazoValues.append(configLines.at(12).toInt(&ok, 16));
                if(!ok) return false;
            }
            else {
                return false;
            }
        }
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

void SSS4Config::save(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param, bool isNewVersion) {

    QFile file(fileName);
    file.open(QFile::WriteOnly|QIODevice::Text);
    QTextStream out(&file);

    if(isNewVersion) {
        out<<QString::number(param.get_vcount(),16)<<endl;
        out<<QString::number(param.get_timer0_min(),16)<<endl;
        out<<QString::number(param.get_timer0_max(),16)<<endl;
        out<<"1"<<endl;
        out<<QString::number(param.get_gxstat(),16)<<endl;
        out<<QString::number(param.get_frame(),16)<<endl;
        QString macAddr;
        macAddr.sprintf("%02X-%02X-%02X-%02X-%02X-%02X",
                        param.get_mac_addr1(),param.get_mac_addr2(),param.get_mac_addr3(),
                        param.get_mac_addr4(),param.get_mac_addr5(),param.get_mac_addr6());
        out<<macAddr<<endl;

        if(param.get_nazo1()+0x148 == param.get_nazo5()) {
            if(param.get_nazo1() == PokeRNG::ROMType::B1Ja::nazo1) {
                out<<"3"<<endl;
            }
            else if(param.get_nazo1() == PokeRNG::ROMType::W1Ja::nazo1) {
                out<<"4"<<endl;
            }
            else {
                out<<"5"<<endl;
                QString nazo;
                nazo.sprintf("%08X",param.get_nazo1());
                out<<nazo<<endl;
            }
        }
        else {
            if(param.get_nazo1() == PokeRNG::ROMType::B2Ja::nazo1 && param.get_nazo2() == PokeRNG::ROMType::B2Ja::nazo2 &&
                    param.get_nazo3() == PokeRNG::ROMType::B2Ja::nazo3 && param.get_nazo4() == PokeRNG::ROMType::B2Ja::nazo4 &&
                    param.get_nazo5() == PokeRNG::ROMType::B2Ja::nazo5) {
                out<<"0"<<endl;
            }
            else if(param.get_nazo1() == PokeRNG::ROMType::W2Ja::nazo1 && param.get_nazo2() == PokeRNG::ROMType::W2Ja::nazo2 &&
                    param.get_nazo3() == PokeRNG::ROMType::W2Ja::nazo3 && param.get_nazo4() == PokeRNG::ROMType::W2Ja::nazo4 &&
                    param.get_nazo5() == PokeRNG::ROMType::W2Ja::nazo5){
                out<<"1"<<endl;
            }
            else {
                out<<"2"<<endl;
                QString nazo1,nazo2,nazo3,nazo4,nazo5;
                nazo1.sprintf("%08X",param.get_nazo1());
                nazo2.sprintf("%08X",param.get_nazo2());
                nazo3.sprintf("%08X",param.get_nazo3());
                nazo4.sprintf("%08X",param.get_nazo4());
                nazo5.sprintf("%08X",param.get_nazo5());
                out<<nazo1<<endl;
                out<<nazo2<<endl;
                out<<nazo3<<endl;
                out<<nazo4<<endl;
                out<<nazo5<<endl;
            }
        }
    }
    else {
        out<<QString::number(param.get_vcount(),16)<<endl;
        // 要出典
        out<<QString::number((param.get_timer0_min()+param.get_timer0_max())/2,16)<<endl;
        out<<QString::number(param.get_gxstat(),16)<<endl;
        out<<QString::number(param.get_frame(),16)<<endl;
        QString macAddr;
        macAddr.sprintf("%02X-%02X-%02X-%02X-%02X-%02X",
                        param.get_mac_addr1(),param.get_mac_addr2(),param.get_mac_addr3(),
                        param.get_mac_addr4(),param.get_mac_addr5(),param.get_mac_addr6());
        out<<macAddr<<endl;
        QString nazo;
        nazo.sprintf("%08X",param.get_nazo1());
        out<<nazo<<endl;
    }


}

PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> SSS4Config::getParameter() {
    return param;
}
