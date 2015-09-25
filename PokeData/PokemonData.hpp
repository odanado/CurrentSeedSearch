#ifndef INCLUDE_POKEMON_DATA_HPP
#define INCLUDE_POKEMON_DATA_HPP

#include <string>
#include "BaseStats.hpp"

namespace PokeData {

class PokemonData {
    const std::string name;
    const BaseStats baseStats;
public:
    PokemonData(const std::string &name="",const BaseStats &baseStats=BaseStats());
    std::string getName() const;
    BaseStats getBaseStats() const;
};

}

#endif

