#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include "player.h"
#include "level.h"

class Monster {
protected:
    std::string name;
    int hp;
    int atk;

public:
    bool ReadDataFromFile(const std::string& filename, int key);
    bool is_alive() const;

    void TakeDamage(int damage);

    std::string getName() const;
    int get_hp() const;
    int get_atk() const;

    void BattleSystemMonster(Player& player, Level& level);
};
#endif
