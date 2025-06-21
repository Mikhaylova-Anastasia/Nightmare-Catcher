#include "player.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool Player::ReadDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error in opening the file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field, dash;

        iss >> field >> dash;

        if (field == "name") {
            std::getline(iss, name);
            if (!name.empty() and name[0] == ' ')
                name.erase(0, 1);
        }
        else if (field == "hp") {
            iss >> hp;
        }
        else if (field == "atk") {
            iss >> atk;
        }
        else if (field == "items") {
            iss >> items;
        }
    }
    file.close();
    return true;
}

bool Player::ReadAbilityName(const std::string& filename, int key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error in opening the file!\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int current_key;
        iss >> current_key;

        if (current_key != key)
            continue;

        std::string before_dash;
        std::getline(iss, before_dash, '-');  

        size_t firstSpace = before_dash.find(' ');
        if (firstSpace != std::string::npos)
            before_dash = before_dash.substr(firstSpace + 1);

        std::istringstream nameStream(before_dash);
        std::string ability_name;

        while (std::getline(nameStream, ability_name, ',')) {

            ability_name.erase(0, ability_name.find_first_not_of(" \t"));
            ability_name.erase(ability_name.find_last_not_of(" \t") + 1);

            if (!ability_name.empty())
                abilities.push_back(ability_name);
        }

        return true; 
    }
    return false; 
}

bool Player::ReadAbilityEffects(const std::string& filename, int key) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error in opening the file\n";
        return false;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int current_key;
        iss >> current_key;

        if (current_key != key)
            continue;

        size_t dash_pos = line.find('-');
        if (dash_pos == std::string::npos)
            return false;

        std::string values = line.substr(dash_pos + 1);

        values.erase(0, values.find_first_not_of(" \t"));
        values.erase(values.find_last_not_of(" \t") + 1);

        std::istringstream valStream(values);
        std::string number;

        while (std::getline(valStream, number, ',')) {
            number.erase(0, number.find_first_not_of(" \t"));
            number.erase(number.find_last_not_of(" \t") + 1);

            if (!number.empty())
                ability_effects.push_back(std::stoi(number));
        }

        return true; 
    }

    return false;
}

bool Player::ReadHealAmount(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error in opening the file: " << filename << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field, dash;

        iss >> field >> dash;

        if (field == "sweet-dream") {
            iss >> sweet_dream;
        }
    }
    file.close();
    return true;
}

bool Player::is_alive() const {
    return (hp > 0);
}

int Player::get_hp() const {
    return hp;
}

int Player::get_atk() const {
    return atk;
}

int Player::get_heal() const {
    return sweet_dream;
}

std::vector<std::string>Player::get_abilities() const {
    return abilities; 
}

std::vector<int>Player::get_effects() const {
    return ability_effects;
}

int Player::inventory() const {
    return items;
}

std::string Player::get_name() const {
    return name;
}

void Player::heal(int restored_hp) {
    hp += restored_hp;
}

void Player::enhancing(int icreased_atk) {
    atk += icreased_atk;
}

void Player::eat(int food) {
    items += food;
}

void Player::TakeDamage(int damage) {
    hp -= damage;
    std::cout << "You recieved " << damage << " damage! Remaining HP: ";
    if (hp < 0) {
        std::cout << "0";
    }
    else {
        std::cout << hp;
    }
    std::cout << std::endl;
}

void Player::ClearAbilities() {
    abilities.clear();
    ability_effects.clear();
}