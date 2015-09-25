#ifndef INCLUDE_POKEMON_HPP
#define INCLUDE_POKEMON_HPP


#include <array>
#include <map>
#include <string>

#include "PokemonData.hpp"

namespace PokeData {
    std::array<PokemonData, 663> createPokemons();
    std::map<std::string, int> createPokemonIndexes();
    const std::array<PokemonData,663> pokemons = createPokemons();
    const std::map<std::string,int> pokemonIndexes = createPokemonIndexes();

}

#endif

