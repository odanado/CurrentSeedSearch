#ifndef INCLUDE_BASE_STATS_HPP
#define INCLUDE_BASE_STATS_HPP

namespace PokeData {

class BaseStats {
    const int hp;
    const int attack;
    const int defense;
    const int spAtk;
    const int spDef;
    const int speed;

public:
    BaseStats(int hp=0,int attack=0,int defense=0,int spAtk=0,int spDef=0,int speed=0);
    int getHP() const;
    int getAttack() const;
    int getDefence() const;
    int getSpAtk() const;
    int getSpDef() const;
    int getSpeed() const;
};

}

#endif

