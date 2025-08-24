#pragma once
#include <string>
#include "GameClock.h"
#include <vector>
#include <map>
#include <functional>


class Laundry; // forward declaration

class Village {
private:
    Laundry& laundry;
    GameClock& clock;
    struct Building {
        std::string name;
        std::string icon;
    };

    std::vector<Building> village;

    bool unlock_new_buildings = false;
public:
    // Конструктор
    Village(Laundry& l, GameClock& c);

    void message_village();

    void drawVillageMap(const std::vector<Building>& buildings);
    void where_go();

    struct house {
        std::string name;
        std::vector<std::string> members;
        std::string specialization;
    };
    
    std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> getActionVillage();

    void print_Silkbrew();
    void print_Gvozdev();
    void print_Rumyanov();
    void print_Slovesnikov();
    void list_activity_village();
};