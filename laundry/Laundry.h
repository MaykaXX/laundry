#pragma once
#include <string>
#include "GameClock.h"
#include "Assignment.h" 
#include "QuestFlags.h" 
#include <iostream>
#include <istream>
#include <windows.h>
#include <map>
#include <fstream>
#include "termcolor.hpp"
#include <time.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <functional>
#include <algorithm>
#include <random>
#include <limits>
#include <unordered_map>
#undef max

// forward declarations
class Village;
class GameClock;
class QuestFlags;
class Assignment;
class WatchingBirds;

class Laundry {
private:
    Village* village;
    std::string us_name;
    int max_number_of_customers = 0, number_of_customers = 0, amount_clothing_dirty = 0, amount_clothing_clean = 0, level_serviceability = 3;
    float price_washing = 2.0f, price_drying = 1.0f, fine = 0, wallet = 0; // $
    bool is_village = false;

    GameClock& clock;
    QuestFlags& questFlags;
    std::map<int, Assignment> assignments;
    int nextAssigId = 1;
protected:
    bool iscamera;

public:
    Laundry(const std::string& name, GameClock& c, QuestFlags& q);

    void setVillage(Village* v);
    void message_laundry();
    std::string user_text(std::string text) const;
    void check_day();
    void list_activity();
    void add_assigm(const std::string& desc);
    void add_pro_assig(const std::string& desc, const std::vector<std::string>& subs);
    void print_Assigm();
    void completeSubTask(int id, const std::string& item);
    void customers();
    void give_clothes();
    void space_enter(std::string text);
    void process_washing();
    void process_drying();
    void DIY();
    
    std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> getAvailableActions();
    void special_events();
    void code_for_washing();
};