#include "monster.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool Monster::ReadDataFromFile(const std::string& filename, int key) {
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

            if (field == "name") {
                std::getline(iss, name);
                if (!name.empty() && name[0] == ' ')
                    name.erase(0, 1);
            }
            else if (field == "hp") {
                iss >> hp;
            }
            else if (field == "atk") {
                iss >> atk;
            }
        }
        else if (foundKey) {
            break;
        }
    }
    file.close();
    return foundKey;
}

bool Monster::is_alive() const {
    return (hp > 0);
}

void Monster::TakeDamage(int damage) {
    hp -= damage;
    std::cout << "The " << name << " took " << damage << " damage! Remaining HP: ";
    if (hp < 0) {
        std::cout << "0";
    }
    else {
        std::cout << hp;
    }
    std::cout << std::endl;
}

std::string Monster::getName() const {
    return name;
}

int Monster::get_hp() const {
    return hp;
}

int Monster::get_atk() const {
    return atk;
}

void Monster::BattleSystemMonster(Player& player, Level& level) {
    std::cout << "\n== BATTLE BEGINS ==" << std::endl;
    std::cout << "Nightmare catcher" << " vs " << name << std::endl;

    int positive_effect = 0;

    int turns = 1;
    int skip_turns = 0;
    int debuff = 0;
    int debuff_turns = 0;
    float drain = 0;

    while (is_alive() and player.is_alive()) {

        std::cout << "\n== YOUR TURN ==" << std::endl;
        if (player.is_alive()) {
            int choice;
            bool cont = false;
            while (cont == false) {
                std::cout << "What are you going to do?" << std::endl;
                std::cout << "(1)-attack (2)-inventory (3)-special ability" << std::endl;

                std::cin >> choice;
                switch (choice) {
                case 1: {
                    TakeDamage(player.get_atk());
                    cont = true;
                    break;
                }
                case 2: {
                    if (player.inventory() == 0) {
                        std::cout << "Looks like your inventory is empty... Good luck!" << std::endl;
                        break;
                    }
                    else {
                        std::cout << "You have " << player.inventory() << " sweet dream(s)";
                        std::cout << "Absorb a sweet dream?\n1 - HELL YEAH!!!\n2 - HELL NO(((\n";
                        int food = 0;
                        std::cin >> food;

                        if (food == 1) {
                            player.heal(player.get_heal());
                            player.eat(-1);
                            std::cout << "You absorbed a sweet dream to heal yourself, you gained 2 hp! Sweet dreams melt like sugar in your mouth~";
                            std::cout << std::endl;
                        }
                        else {
                            break;
                        }
                        cont = true;
                        break;
                    }
                }
                case 3: {
                    if (level.get_current_level() <= 1) {
                        std::cout << "Oops! It looks like you aren't experienced enough to use special abilities!" << std::endl;
                        break;
                    }
                    else {
                        int option;
                        std::cout << "Choose what special ability you would like to use. Choose wisely!" << std::endl;

                        for (size_t i = 0; i < player.get_abilities().size(); ++i) {
                            std::cout << i + 1 << " - " << player.get_abilities()[i] << std::endl;
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
                        if (player.get_abilities()[option - 1] == "Blindness") {
                            skip_turns = positive_effect;
                            std::cout << "Ta-da! The enemy is blind now!" << std::endl;
                        }
                        else if (player.get_abilities()[option - 1] == "Panic") {
                            debuff = positive_effect;
                            skip_turns = 2;
                            std::cout << "The enemy is trembling in panic!" << std::endl;
                        }
                        else if (player.get_abilities()[option - 1] == "Drain") {
                            drain = positive_effect * 0.01;
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

            if (is_alive()) {
                std::cout << "\n== OPPONENT'S TURN ==" << std::endl;
                if (skip_turns > 0) {
                    skip_turns--;
                    std::cout << "=== MONSTER CAN'T MOVE!" << std::endl;
                }
                else {
                    std::cout << "Nightmares are harmless for human body. But not for yours!\n";
                    if (debuff_turns > 0) {
                        player.TakeDamage(atk - debuff);
                        debuff_turns--;
                    }
                    else {
                        player.TakeDamage(atk);
                    }
                }
                turns++;
            }
        }
    }
    if (player.is_alive()) {
        std::cout << std::endl;
        std::cout << "You won the battle!\n" << std::endl;
        player.eat(1);
    }
    else {
        std::cout << std::endl;
        std::cout << "\nYou died!\n" << std::endl;
    }
}