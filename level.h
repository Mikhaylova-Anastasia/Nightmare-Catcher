#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include "player.h"

class Level {
private:
    int current_level;

public:
    Level(int x);
    int get_current_level() const;

    void GetText(const std::string& filename, const std::string& key);
    void NextLevel();
    void Autosave(const std::string& filename, Player& player);
};
#endif