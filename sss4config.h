#ifndef SSS4CONFIG_H
#define SSS4CONFIG_H

#include <PokeRNG/PokeRNG.hpp>

#include <QFile>
#include <QTextStream>
#include <QList>

class SSS4Config
{
public:
    SSS4Config(const QString &fileName="config.txt");
    bool parse();
    bool exist();
    void save(const PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> &param, bool isNewVersion=true);
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> getParameter();

private:
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
    QString fileName;
};

#endif // SSS4CONFIG_H
