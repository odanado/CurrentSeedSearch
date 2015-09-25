#ifndef INCLUDE_POKEMON_HPP
#define INCLUDE_POKEMON_HPP


#include <array>
#include <map>
#include <string>

#include "PokemonData.hpp"

namespace PokeData {

class Pokemon {
private:
    static std::array<PokemonData, 663> createPokemons();
    static std::map<std::string, int> createPokemonIndexes();
public:
    static const std::array<PokemonData,663> pokemons;
    static const std::map<std::string,int> pokemonIndexes;
};

}

#endif

