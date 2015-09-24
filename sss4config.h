#ifndef SSS4CONFIG_H
#define SSS4CONFIG_H

#include <PokeRNG/PokeRNG.hpp>

#include <QFile>
#include <QTextStream>
#include <QList>

class SSS4Config
{
public:
    SSS4Config();
    bool parse();
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> getParameter();

private:
    PokeRNG::Parameters5Gen<PokeRNG::ROMType::None> param;
};

#endif // SSS4CONFIG_H
