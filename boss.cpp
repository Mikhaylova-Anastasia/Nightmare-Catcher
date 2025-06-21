#include "boss.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

void Boss::heal(int restored_hp) {
    hp += restored_hp;
}

bool Boss::ReadSpecialAbility(const std::string& filename, int key) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error in opening the file: " << filename << "\n";
        return false;
    }

    std::string line;
    bool foundKey = false;

    while (std::getline(file, line)) {
        if (line == "===") {
            foundKey = false;
            continue;
        }

        std::istringstream iss(line);
        int currentKey;
        std::string field, dash;

        iss >> currentKey >> field >> dash;

        if (currentKey == key) {
            foundKey = true;

            if (field == "special_ability") {
                std::getline(iss, special_ability);
                if (!special_ability.empty() and (special_ability[0] == ' '))
                    special_ability.erase(0, 1);
            }
        }
        else if (foundKey) {
            break;
        }
    }
    file.close();
    return foundKey;
}

bool Boss::ReadNegativeEffects(const std::string& filename, int key) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error in opening the file\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int currentKey;
        std::string field, dash;

        iss >> currentKey >> field >> dash;

        if (currentKey == key and field == "negative_effect") {
            std::string values;
            std::getline(iss, values);

            if (!values.empty() and values[0] == ' ')
                values.erase(0, 1);

            std::istringstream valStream(values);
            std::string number;

            while (std::getline(valStream, number, ',')) {
                number.erase(0, number.find_first_not_of(' '));
                number.erase(number.find_last_not_of(' ') + 1);
                effects.push_back(std::stoi(number));
            }

            return true;
        }
    }
    return false;
};

std::string Boss::getSpecialAbility() const {
    return special_ability;
}

void Boss::BattleSystemBoss(Player& player, Level& level) {
    std::cout << "\n== BATTLE BEGINS ==" << std::endl;
    std::cout << "Nightmare catcher" << " vs " << name << std::endl;

    int negative_effect = 0;
    int positive_effect = 0;

    int turns = 1;
    int skip_turns = 0;
    int debuff = 0;
    int debuff_turns = 0;
    float drain = 0;

    int enemy_skip_turns = 0;
    int enemy_debuff = 0;
    int enemy_debuff_turns = 0;
    float enemy_drain = 0;

    while (is_alive() and player.is_alive()) {
        std::cout << "\n== YOUR TURN" << std::endl;
        if (player.is_alive()) {
            int choice;
            bool cont = false;

            while (cont == false) {
                if (enemy_skip_turns > 0) {
                    std::cout << "\nWhoa! It's so dark around, you feel disoriented...\n";
                    enemy_skip_turns--;
                    cont = true;
                    break;
                }
                else {
                    std::cout << "What are you going to do?" << std::endl;
                    std::cout << "(1)-attack (2)-inventory (3)-special ability" << std::endl;
                    std::cin >> choice;
                    switch (choice) {
                    case 1: {
                        if (enemy_debuff_turns > 0) {
                            TakeDamage(player.get_atk() - enemy_debuff);
                            enemy_debuff_turns--;
                        }
                        else {
                            enemy_debuff = 0;
                            TakeDamage(player.get_atk());
                        }
                        cont = true;
                        break;
                    }
                    case 2: {
                        if (player.inventory() == 0) {
                            std::cout << "Looks like your inventory is empty... Good luck!" << std::endl;
                            break;
                        }
                        else {
                            std::cout << "You have " << player.inventory() << " sweet dream(s)\n";
                            std::cout << "Absorb a sweet dream?\n1 - HELL YEAH!!!\n2 - HELL NO(((\n";

                            int food = 0;
                            std::cin >> food;
                            if (food == 1) {
                                player.heal(player.get_heal());
                                player.eat(-1);
                                std::cout << "You absorbed a sweet dream to heal yourself, you gained 2 hp! Sweet dreams melt like sugar in your mouth~\n";
                            }
                            else {
                                break;
                            }
                            cont = true;
                            break;
                        }
                    }
                    case 3: {
                        if (level.get_current_level() == 1) {
                            std::cout << "Oops! It looks like you aren't experienced enough to use special abilities!" << std::endl;
                            break;
                        }
                        else {
                            int option;
                            std::cout << "Choose what special ability you would like to use. Choose wisely!" << std::endl;

                            for (size_t i = 0; i < player.get_abilities().size(); ++i) {
                                std::cout << i+1 << " - " << player.get_abilities()[i] << std::endl;
                            }

                            std::cin >> option;
                            switch (option) {
                            case 1: {
                                positive_effect = player.get_effects()[0];
                                cont = true;
                                break;
                            }
                            case 2: {
                                positive_effect = player.get_effects()[1];
                                cont = true;
                                break;
                            }
                            case 3: {
                                positive_effect = player.get_effects()[2];
                                cont = true;
                                break;
                            }
                            }
                            if (player.get_abilities()[option-1] == "Blindness") {
                                skip_turns = positive_effect;
                                std::cout << "Ta-da! The enemy is blind now!" << std::endl;
                            }
                            else if (player.get_abilities()[option - 1] == "Panic") {
                                debuff = positive_effect;
                                skip_turns = 2;
                                std::cout << "The enemy is trembling in panic!" << std::endl;
                            }
                            else if (player.get_abilities()[option - 1] == "Drain") {
                                drain = positive_effect*0.01;
                                std::cout << "You are draining the enemy's lifepower like a vampire!" << std::endl;
                                player.heal(hp * drain);
                                TakeDamage(hp * drain);
                                std::cout << "\nYou gained " << hp * drain << " hp! (Total: " << player.get_hp() << " hp)";
                                drain = 0;
                            }
                        }
                    }
                    }
                }
            }
            if (is_alive()) {
                std::cout << "\n== OPPONENT'S TURN\n";

                if (skip_turns > 0) {
                    skip_turns--;
                    std::cout << "== BOSS CAN'T MOVE ==\n";
                    if (turns == 3) {
                        turns--;
                    }
                }
                else {
                    if (turns == 3) {
                        int random_index = 0;
                        if (special_ability == "all") {

                            std::vector<std::string> abilities = { "Blindness", "Panic", "Drain" };
                            std::srand(static_cast<unsigned int>(std::time(nullptr)));
                            random_index = std::rand() % abilities.size();
                            std::string selected_ability = abilities[random_index];

                            special_ability = selected_ability;
                        }

                        if (special_ability.find(',') != std::string::npos) {

                            size_t split_pos = special_ability.find(", ");

                            std::string ability1 = special_ability.substr(0, split_pos);
                            std::string ability2 = special_ability.substr(split_pos + 2);

                            std::vector<std::string> abilities = { ability1, ability2 };
                            std::srand(static_cast<unsigned int>(std::time(nullptr)));
                            random_index = std::rand() % abilities.size();
                            std::string selected_ability = abilities[random_index];

                            special_ability = selected_ability;
                        }
                        
                        negative_effect = effects[random_index];

                        std::cout << "\nThe " + name + " gathers it's power and uses the special ability - " << special_ability << std::endl;

                        if (special_ability == "Blindness") {
                            enemy_skip_turns = negative_effect;
                        }
                        if (special_ability == "Panic") {
                            enemy_debuff = negative_effect;
                            enemy_debuff_turns = 2;
                        }
                        if (special_ability == "Drain") {
                            enemy_drain = negative_effect*0.01;
                            player.TakeDamage(player.get_hp() * enemy_drain);
                            heal(player.get_hp() * enemy_drain);
                            std::cout << "\nThe monster restored " << player.get_hp() * enemy_drain << " hp! (Remaining HP: " << hp << ")\n";
                            enemy_drain = 0;
                        }
                        turns = 0;
                    }
                    else {
                        if (debuff_turns > 0) {
                            player.TakeDamage(atk - debuff);
                            debuff_turns--;
                        }
                        else {
                            debuff = 0;
                            player.TakeDamage(atk);
                        }
                    }
                }
            }
        }
        turns++;
    }

    if (player.is_alive()) {
        std::cout << std::endl;
        std::cout << "You won the battle!\n";
    }
    else {
        std::cout << std::endl;
        std::cout << "\nYou died!\n";
    }
}