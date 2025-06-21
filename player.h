#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Player {
private:
    int hp;
    int atk;
    std::string name;
    int items;
    int sweet_dream;
    std::vector<std::string> abilities;
    std::vector<int> ability_effects;

public:

    bool ReadDataFromFile(const std::string& filename);
    bool ReadAbilityName(const std::string& filename, int key);
    bool ReadAbilityEffects(const std::string& filename, int key);
    bool ReadHealAmount(const std::string& filename);
    bool is_alive() const;

    std::vector<std::string> get_abilities() const;
    std::vector<int> get_effects() const;
   
    int get_hp() const;
    int get_atk() const;
    int get_heal() const;
    int inventory() const;
    std::string get_name() const;

    void heal(int restored_hp);
    void enhancing(int icreased_atk);
    void eat(int food);
    void TakeDamage(int damage);
    void ClearAbilities();
};
#endif
