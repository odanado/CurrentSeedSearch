#include "BaseStats.hpp"

namespace PokeData {

BaseStats::BaseStats(int hp,int attack,int defense,int spAtk,int spDef,int speed) : hp(hp),attack(attack),defense(defense),spAtk(spAtk),spDef(spDef),speed(speed) {
}

int BaseStats::getHP() const {
    return hp;
}

int BaseStats::getAttack() const {
    return attack;
}

int BaseStats::getDefence() const {
    return defense;
}

int BaseStats::getSpAtk() const {
    return spAtk;
}

int BaseStats::getSpDef() const {
    return spDef;
}

int BaseStats::getSpeed() const {
    return speed;
}

}

