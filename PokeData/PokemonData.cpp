#include "PokemonData.hpp"

namespace PokeData {

PokemonData::PokemonData(const std::string &name,const BaseStats &baseStats) : name(name),baseStats(baseStats) { }

std::string PokemonData::getName() const {
    return name;
}

BaseStats PokemonData::getBaseStats() const {
    return baseStats;
}

}

