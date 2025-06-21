#include "player.h"
#include "level.h"
#include "manager.h"
#include <fstream>
#include <iostream>
#include <string>

int main() {
    Player nightmare_catcher;
    nightmare_catcher.ReadDataFromFile("main character.txt");
    nightmare_catcher.ReadHealAmount("heal.txt");

    Level progress(1);

    for (int q = 0; q < 4; q++) {
        std::string n = std::to_string(q);
        progress.GetText("script.txt", "0." + n);
        std::cin.get();
    }

    Manager(nightmare_catcher, progress);

    for (int p = 0; p < 4; p++) {
        std::string e = std::to_string(p);
        progress.GetText("script.txt", "6." + e);
        std::cin.get();
    }

    std::cout << "~ THE END ~";
    return 0;
}