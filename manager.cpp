#include "manager.h"
#include "monster.h"
#include "boss.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

void Manager(Player& player, Level& level) {

    int atk_increase = 0;
    int hp_increase = 0;

    std::ifstream file("level.txt");
    if (!file.is_open()) {
        std::cerr << "Error in opening the file\n";
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string field, dash;

        iss >> field >> dash;

        if (field == "hp_increase") {
            iss >> hp_increase;
        }
        else if (field == "atk_increase") {
            iss >> atk_increase;
        }
    }
    file.close();

    while (level.get_current_level() <= 5) {
        if (level.get_current_level() == 3) {
            for (int j = 0; j < 4; j++) {
                std::string g = std::to_string(j);
                std::cin.get();
                level.GetText("script.txt", "3." + g);
            }
            std::string path;
            std::string a;

            while (path != "3213") {
                level.GetText("script.txt", "3.4");
                std::cout << "\n(1) - go right\n";
                std::cout << "(2) - go left\n";
                std::cout << "(3) - go forward\n";
                std::cin >> a;
                path += a;

                if (path == "3" or path == "32" or path == "321" or path == "3213") {
                    level.GetText("script.txt", "3.7");
                }
                else {
                    std::vector<std::string> numbers = { "3.5", "3.6" };
                    std::srand(static_cast<unsigned int>(std::time(nullptr)));
                    int randomIndex = std::rand() % numbers.size();
                    std::string wrong_path = numbers[randomIndex];

                    level.GetText("script.txt", wrong_path);
                    path = "";
                }
            }
            for (int d = 8; d < 12; d++) {
                std::string m = std::to_string(d);
                std::cin.get();
                level.GetText("script.txt", "3." + m);
            }
            player.heal(15);
            player.enhancing(2);
            level.Autosave("main character.txt", player);
            level.NextLevel();
        }
        else {
            std::string k = std::to_string(level.get_current_level());

            for (int h = 0; h < 21; h++) {
                std::string n = std::to_string(h);
                level.GetText("script.txt", k + "." + n);
                std::cin.get();

                if (h == 6) {
                    player.ReadAbilityName("hero abilities.txt", level.get_current_level());
                    player.ReadAbilityEffects("hero abilities.txt", level.get_current_level());

                    Monster enemy;

                    for (int i = 0; i < 3; i++) {
                        if (level.get_current_level() == 5) {
                            std::vector<int> type = { 1, 2, 4 };
                            int randomIndex = std::rand() % type.size();
                            int monster_type = type[randomIndex];
                            enemy.ReadDataFromFile("monsters.txt", monster_type);
                        }
                        else {
                            enemy.ReadDataFromFile("monsters.txt", level.get_current_level());
                        }
                        enemy.BattleSystemMonster(player, level);

                        if (player.is_alive() == false) {
                            std::cout << "Your path is difficult. It seems like you weren't ready for such fight. Try again and learn from your mistakes.\n" << std::endl;
                            h = 0;
                            player.ReadDataFromFile("main character.txt");
                            break;
                        }

                        if (i < 2) { level.GetText("script.txt", k + ".7"); }
                    }
                    std::cin.get();
                    h++;
                }
                if (h == 17) {

                    player.ReadAbilityName("hero abilities.txt", level.get_current_level());
                    player.ReadAbilityEffects("hero abilities.txt", level.get_current_level());

                    Boss opponent;

                    opponent.ReadDataFromFile("bosses.txt", level.get_current_level());
                    opponent.ReadSpecialAbility("bosses.txt", level.get_current_level());
                    opponent.ReadNegativeEffects("bosses.txt", level.get_current_level());
                    opponent.BattleSystemBoss(player, level);

                    if (player.is_alive() == false) {
                        std::cout << "Your path is difficult. It seems like you weren't ready for such fight. Try again and learn from your mistakes.\n" << std::endl;
                        h = 0;
                        player.ReadDataFromFile("main character.txt");
                    }
                    std::cin.get();
                }
                if (h == 20) {

                    int current_items = player.inventory();
                    player.ReadDataFromFile("main character.txt");
                    player.eat(current_items);
                    player.heal(hp_increase);
                    player.enhancing(atk_increase);

                    std::cout << "\nBravo, you improved your skills after crashing a strong nightmare! Your health increased by 15 HP! (Total: " << player.get_hp() << " HP)" << std::endl;
                    level.Autosave("main character.txt", player);
                    level.NextLevel();
                    std::cout << std::endl;
                    std::cin.get();
                }
            }
        }
    }
}