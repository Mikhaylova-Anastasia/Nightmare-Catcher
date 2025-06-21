#include "level.h"
#include <fstream>
#include <iostream>
#include <string>

Level::Level(int x) : current_level(x) {}

int Level::get_current_level() const {
    return current_level;
}

void Level::GetText(const std::string& filename, const std::string& key) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error in opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string key_prefix = key;
    bool found = false;

    while (std::getline(file, line)) {

        if (line.find(key_prefix, 0) == 0) {
            std::string value;
            value += line;
            value = line.erase(0, key.length() + 7);
            if (line.empty()) {
                std::cout << value << std::endl;
                break;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Key \"" << key << "\" isn't found." << std::endl;
    }

    file.close();
    std::cout << line;
    std::cout << std::endl;
}

void Level::NextLevel() {
    current_level++;
}

void Level::Autosave(const std::string& filename, Player& player) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file.clear();

        file << "name - " << player.get_name() << std::endl;
        file << "hp - " << player.get_hp() << std::endl;
        file << "atk - " << player.get_atk() << std::endl;
        file << "items - " << player.inventory() << std::endl;

        file.close();
        if (current_level < 5) {
            std::cout << "\nNew save point set at the " << current_level + 1 << " level!" << std::endl;
        }
    }
    else {
        std::cerr << "Save error: " << filename << std::endl;
    }
}