
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

int enemy_skip_turns = 0;
int enemy_debuff = 0;
int enemy_drain = 0;

int skip_turns = 0;
int debuff = 0;
int drain = 0;


void Blindness() {
    int skip_turns = 1;
}

void Panic() {
    int skip_turns = 1;
    int debuff = 6;
}

void hp_drain() {
    float drain = 0.3;
}

void Enemy_blindness() {
    int skip_turns = 1;
}

void Enemy_panic() {
    int skip_turns = 1;
    int debuff = 6;
}

void Enemy_hp_drain() {
    float drain = 0.3;
}

class Player {
private:
    int hp;
    int atk;
    std::string name;
    int items;

public:
    void readDataFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: could not open file " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string ignore, key, dash;
            iss >> ignore >> key >> dash;

            if (key == "name") {
                iss >> name;
            }
            else if (key == "hp") {
                iss >> hp;
            }
            else if (key == "atk") {
                iss >> atk;
            }
            else if (key == "items") {
                iss >> items;
            }
        }
        file.close();
    }

    bool isAlive() const {
        return (hp > 0);
    }

    int getHp() const {
        return hp;
    }

    int getAtk() const {
        return atk;
    }

    int inventory() const {
        return items;
    }

    std::string getName() const {
        return name;
    }

    void heal(int restored_hp) {
        hp += restored_hp;
    }

    void eat(int food) {
        items += food;
    }
    void takeDamage(int damage) {
        hp -= damage;
        std::cout << "You recieved " << damage << " damage! Remaining HP: " << hp << std::endl;
        }
    };

class Level {
private:
    int currentLevel;

public:
    Level(int x) : currentLevel(x) {}
    int get_current_level() const {
        return currentLevel;
    }

    void getText(const std::string& filename, const std::string& key) {
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
    }

    void nextLevel() {
        currentLevel++;
    }

    void autosave(const std::string& filename, Player& player) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file.clear();
            file << "name - " << player.getName() << std::endl;
            file << "hp - " << player.getHp() << std::endl;
            file << "atk - " << player.getAtk() << std::endl;
            file << "items - " << player.inventory() << std::endl;
            file.close();
            std::cout << "New save point set." << filename << std::endl;
        }
        else {
            std::cerr << "Save error: " << filename << std::endl;
        }
    }

};


class Monster {
private:
    std::string name;
    int hp;
    int atk;

public:
        void readDataFromFile(const std::string & filename, const std::string& type) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Error: " << filename << std::endl;
                return;
                }

                std::string line;

                while (std::getline(file, line)) {
                    std::istringstream iss(line);
                    std::string ignore, key, dash;
                    iss >> ignore >> key >> dash;

                    if (line.find(type, 0)) {
                        if (key == "name") {
                            std::getline(iss, name);
                            if (!name.empty() && name[0] == ' ') name.erase(0, 1);
                        }
                        else if (key == "hp") {
                            iss >> hp;
                        }
                        else if (key == "atk") {
                            iss >> atk;
                        }
                        else if (key == "===") {
                            break;
                        }
                    }
                    else {
                        std::cerr << "This monster doesn't exists" << std::endl;
                    }
                }
                file.close();
        }

        bool isAlive() const {
            return (hp > 0);
        }

        void takeDamage(int damage) {
            hp -= damage;
            std::cout << "The" << name << " took " << damage << " damage! Remaining HP: " << hp << std::endl;
            }

        std::string getName() const { return name; }
        int getHp() const { return hp; }
        int getAtk() const { return atk; }

        void battle_system_monster(Player& player, Level& level) {
            std::cout << "== BATTLE BEGINS ==" << std::endl;
            std::cout << player.getName() << " vs " << name << std::endl;
            while (isAlive() and player.isAlive()) {
                int turns = 1;
                std::cout << "== YOUR TURN" << std::endl;
                if (player.isAlive()) {
                    int choice;
                    std::cout << "What are you going to do?" << std::endl;
                    std::cout << "(1)-attack (2)-inventory (3)-special ability" << std::endl;
                    std::cin >> choice;
                    switch (choice) {
                    case 1: {
                        if (enemy_debuff > 0) {
                            takeDamage(player.getAtk() - debuff);
                            enemy_debuff = 0;
                            std::cout << "The enemy has recieved " << player.getAtk() - debuff << " damage!" << std::endl;
                        }
                        else {
                            takeDamage(player.getAtk());
                            std::cout << "The enemy has recieved " << player.getAtk() << " damage!" << std::endl;
                        }
                        break;
                    }
                    case 2: {
                        if (player.inventory() == 0) {
                            std::cout << "Looks like your inventory is empty... Good luck!" << std::endl;
                        }
                        else {
                            std::cout << "You have " << player.inventory() << "sweet dream(s)";
                            std::cout << "Absorb a sweet dream?\n1 - HELL YEAH!!!\n2 - HELL NO(((";
                            int food = 0;
                            std::cin >> food;
                            if (food == 1) {
                                player.heal(2);
                                player.eat(-1);
                                std::cout << "You absorbed a sweet dream to heal yourself, you gained 2 hp! Sweet dreams melt like sugar in your mouth~";
                            }
                            break;
                        }
                    case 3: {
                        if (level.get_current_level() == 2) {
                            skip_turns = 1;
                            std::cout << "Ta-da! The enemy is blind now!" << std::endl;
                        }
                        if (level.get_current_level() >= 3 and level.get_current_level() < 5) {
                            int option;
                            std::cout << "Choose what special ability you would like to use. Choose wisely!" << std::endl;
                            if (level.get_current_level() <= 1) {
                                std::cout << "Oops! It looks like you aren't experienced enough to use special abilities!" << std::endl;
                            }
                            if (level.get_current_level() <= 2) {
                                std::cout << "(1)-Blindness" << std::endl;
                            }
                            if (level.get_current_level() <= 3) {
                                std::cout << "(1)-Blindness  (2)-Panic" << std::endl;
                            }
                            if (level.get_current_level() == 5) {
                                std::cout << "(1)-Blindness  (2)-Panic  (3)-Drain" << std::endl;
                            }
                            std::cin >> option;
                            switch (option) {
                            case 1: {
                                skip_turns = 1;
                                std::cout << "The enemy is blind now!" << std::endl;
                                break;
                            }
                            case 2: {
                                debuff = 3;
                                std::cout << "The enemy is trembling in panic!" << std::endl;
                                break;
                            }
                            case 3: {
                                drain = 0.2;
                                std::cout << "You are draining the enemy's lifepower like a vampire!" << std::endl;
                                int current_hp = player.getHp();
                                player.heal(current_hp * drain);
                                takeDamage(hp * drain);
                                drain = 0;
                                break;
                            }
                            }
                        }
                        }
                        }
                        std::cout << "== OPPONENT'S TURN" << std::endl;
                        if (isAlive()) {
                            if (skip_turns == 1) {
                                skip_turns = 0;
                                std::cout << "=== MONSTER CAN'T MOVE!" << std::endl;
                            }
                            else {
                                if (debuff > 0) {
                                    player.takeDamage(atk - debuff);
                                    debuff = 0;
                                }
                                else {
                                    player.takeDamage(atk);
                                }
                            }
                            turns++;
                        }
                        if (player.isAlive()) {
                            std::cout << "You won the battle!" << std::endl;
                            player.eat(1);
                        }
                        else {
                            std::cout << "You died!" << std::endl;
                        }
                    }
                }
            }
    }
};
 
class Boss : public Monster {
private:
    std::string name;
    int hp;
    int atk;
    std::string specialAbility;

public:
    void heal(int restored_hp) {
        hp += restored_hp;
    }

    std::string getSpecialAbility() const {
        return specialAbility;
    }

    void battle_system_boss(Player& player, Level& level) {

        std::cout << "=== BATTLE BEGINS ===\n";
        std::cout << player.getName() << " vs " << name << "\n";

        while (player.isAlive() and isAlive()) {
            int turns = 1;
            std::cout << "=== YOUR TURN\n";
            if (player.isAlive()) {
                int choice;
                std::cout << "What are you going to do?" << std::endl;
                std::cout << "(1)-attack (2)-inventory (3)-special ability" << std::endl;
                std::cin >> choice;
                switch (choice) {
                case 1: {
                    if (enemy_debuff > 0) {
                        takeDamage(player.getAtk() - debuff);
                        enemy_debuff = 0;
                        std::cout << "The enemy has recieved " << player.getAtk() - debuff << " damage!" << std::endl;
                    }
                    else {
                        takeDamage(player.getAtk());
                        std::cout << "The enemy has recieved " << player.getAtk() << " damage!" << std::endl;
                    }
                    break;
                }
                case 2: {
                    if (player.inventory() == 0) {
                        std::cout << "Looks like your inventory is empty... Good luck!" << std::endl;
                    }
                    else {
                        std::cout << "You have " << player.inventory() << "sweet dream(s)";
                        std::cout << "Absorb a sweet dream?\n1 - HELL YEAH!!!\n2 - HELL NO(((";
                        int food = 0;
                        std::cin >> food;
                        if (food == 1) {
                            player.heal(2);
                            player.eat(-1);
                            std::cout << "You absorbed a sweet dream to heal yourself, you gained 2 hp! Sweet dreams melt like sugar in your mouth~";
                        }
                    }
                    break;
                }
                case 3: {
                    if (level.get_current_level() == 2) {
                        skip_turns = 1;
                        std::cout << "Ta-da! The enemy is blind now!" << std::endl;
                    }
                    if (level.get_current_level() >= 3 and level.get_current_level() < 5) {
                        int option;
                        std::cout << "Choose what special ability you would like to use. Choose wisely!" << std::endl;
                        if (level.get_current_level() <= 1) {
                            std::cout << "Oops! It looks like you aren't experienced enough to use special abilities!" << std::endl;
                        }
                        if (level.get_current_level() <= 2) {
                            std::cout << "(1)-Blindness" << std::endl;
                        }
                        if (level.get_current_level() <= 3) {
                            std::cout << "(1)-Blindness  (2)-Panic" << std::endl;
                        }
                        if (level.get_current_level() == 5) {
                            std::cout << "(1)-Blindness  (2)-Panic  (3)-Drain" << std::endl;
                        }
                        std::cin >> option;
                        switch (option) {
                        case 1: {
                            skip_turns = 1;
                            std::cout << "The enemy is blind now!" << std::endl;
                            break;
                        }
                        case 2: {
                            debuff = 3;
                            std::cout << "The enemy is trembling in panic!" << std::endl;
                            break;
                        }
                        case 3: {
                            drain = 0.2;
                            std::cout << "You are draining the enemy's lifepower like a vampire!" << std::endl;
                            player.heal(hp * drain);
                            takeDamage(hp * drain);
                            drain = 0;
                            break;
                        }
                        }
                    }
                }
                }
                std::cout << "=== OPPONENT'S TURN\n";
                if (isAlive()) {
                    if (turns == 3) {
                        if (level.get_current_level() == 1) {
                            enemy_skip_turns = 1;
                        }
                        if (level.get_current_level() == 2) {
                            enemy_debuff = 3;
                        }
                        if (level.get_current_level()) {
                            enemy_drain = 0.2;
                        }
                    }
                    if (skip_turns == 1) {
                        skip_turns = 0;
                        std::cout << "=== BOSS CAN'T MOVE\n";
                    }
                    else {
                        if (debuff > 0) {
                            player.takeDamage(atk - debuff);
                            debuff = 0;
                            if (enemy_drain > 0) {
                                heal(hp * drain);
                                player.takeDamage(hp * drain);
                                enemy_drain = 0;
                            }
                            else {
                                player.takeDamage(atk);
                            }

                        }
                    }
                }
                turns++;
            }
            if (player.isAlive()) {
                std::cout << "You won the battle!\n";
                std::cout << "Bravo, you improved your skills after crashing a strong nightmare! Your health increased by 15 HP! (Total: " << player.getHp() << " HP)" << std::endl;

            }
            else {
                std::cout << "You died!\n";
            }
        }
    }
 };


 void manager(Player& player, Level& level) {
     while (level.get_current_level() <= 5) {
         if (level.get_current_level() == 3) {
             level.getText("script.txt", "8");
             std::string path;
             std::string a;
             while (path != "3213") {
                 level.getText("script.txt", "9");
                 std::cin >> a;
                 std::string text = "9." + a;
                 level.getText("script.txt", text);
                 if (path == "3") {
                     std::string text = "10." + a;
                 }
                 if (path == "32") {
                     std::string text = "11." + a;
                 }
                 if (path == "321") {
                     std::string text = "12." + a;
                 }
                 path += a;
             }
             level.getText("script.txt", "13");
         }
         else {

             std::string k = std::to_string(level.get_current_level());
             level.getText("script.txt", k);
             std::cin.get();
             level.getText("script.txt", k + ".1");
             std::cin.get();

             Monster enemy;
             enemy.readDataFromFile("monsters.txt", k);
             enemy.battle_system_monster(player, level);

             level.getText("script.txt", k + ".2");
             std::cin.get();
             level.getText("script.txt", k + ".3");
             std::cin.get();
             level.getText("script.txt", k + ".4");
             std::cin.get();

             Boss opponent;
             opponent.readDataFromFile("bosses.txt", k);
             opponent.battle_system_boss(player, level);

             if (player.isAlive()) {

                 player.heal(15);
                 level.getText("script.txt", k + ".2");

                 level.autosave("main character.txt", player);
             };
         }
     }
 };

int main() {
    Player nightmare_catcher;
    nightmare_catcher.readDataFromFile("main character.txt");
    Level progress(1);
    progress.getText("script.txt", "0");
    std::cin.get();
    progress.getText("script.txt", "0.1");
    std::cin.get();
    progress.getText("script.txt", "0.2");
    std::cin.get();
    progress.getText("script.txt", "0.3");
    std::cin.get();
    manager(nightmare_catcher, progress);
    }
