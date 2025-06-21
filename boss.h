#ifndef BOSS_H
#define BOSS_H

#include <vector>
#include "monster.h"
#include "player.h"
#include "level.h"

class Boss : public Monster {
private:
    std::string special_ability;
    std::vector<int> effects;

public:
    void heal(int restored_hp);

    bool ReadSpecialAbility(const std::string& filename, int key);

    bool ReadNegativeEffects(const std::string& filename, int key);

    std::string getSpecialAbility() const;

    void BattleSystemBoss(Player& player, Level& level);
};

#endif
