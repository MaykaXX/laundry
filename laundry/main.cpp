#include <iostream>
#include <windows.h>
#include "GameClock.h"
#include "Laundry.h"
#include "Village.h"
#include "Utils.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(65001);

    srand(static_cast<unsigned int>(time(NULL)));

    std::string us_name;
    GameClock clock;

    showMaykaRetroBanner();
    clock.random_qual_custom();

    std::cout << "Как тебя зовут: ";
    std::cin >> us_name;
    std::cout << "Добро пожаловать, " << us_name << "!" << std::endl;

    QuestFlags questFlags;
    Laundry laundry(us_name, clock, questFlags);
    Village village(laundry, clock);
    laundry.setVillage(&village);

    laundry.list_activity();
    village.list_activity_village();

    return 0;
}
