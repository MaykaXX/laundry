#include "WatchingBirds.h"
#include "termcolor.hpp"
#include <vector>
#include <map>

void WatchingBirds::random_meet() {
    int num = 1 + std::rand() % 100;
    if (num <= 10) {
        std::cout << "Тихо: Птичка на горизонте!" << std::endl;
        take_photo();
    }
    else {
        std::cout << "Небо чистое... Птиц не видно." << std::endl;
    }
}

void WatchingBirds::take_photo() {
    std::string message;
    std::cout << "Хочешь сделать фото?" << termcolor::cyan << "(да / нет) " << termcolor::reset;
    std::cin >> message;
    std::cout << std::endl;
    if (message == "да") {
        random_bird();
    }
}

void WatchingBirds::random_bird() {
    struct Bird {
        std::string name;
        std::string rarity;
        //int age;
    };

    std::vector<Bird> birds = {
        {"Воробей", "обычная"},
        {"Синица", "обычная"},
        {"Дятел", "редкая"},
        {"Филин", "редкая"},
        {"Феникс", "супер-редкая"},
        {"Трёхглазый ворон", "супер-редкая"},
        {"Жар-птица", "легендарная"},
        {"Гарпия", "легендарная"}
    };

    // Вероятности выпадения (сумма должна быть ≤ 100)
    std::map<std::string, int> rarity_weight = {
        {"обычная", 65},
        {"редкая", 20},
        {"супер-редкая", 10},
        {"легендарная", 5},
    };

    int roll = std::rand() % 100;

    std::string selected_rarity;
    int threshold = 0;

    for (const auto& pair : rarity_weight) {
        const std::string& rarity = pair.first;
        int weight = pair.second;

        threshold += weight;
        if (roll < threshold) {
            selected_rarity = rarity;
            break;
        }
    }

    // Фильтрация птиц по редкости
    std::vector<Bird> filtered;
    for (const auto& bird : birds) {
        if (bird.rarity == selected_rarity)
            filtered.push_back(bird);
    }


    // Выбор случайной птицы данной редкости
    int index = std::rand() % filtered.size();
    Bird chosen = filtered[index];

    std::cout << "Вам попалась птица: " << chosen.name << " [";
    colored_rarity(chosen.rarity);
    std::cout << "]" << std::endl;
    quality_birds += 1;


}
void WatchingBirds::colored_rarity(const std::string& rarity) {
    if (rarity == "обычная")
        std::cout << termcolor::green;
    else if (rarity == "редкая")
        std::cout << termcolor::yellow;
    else if (rarity == "супер-редкая")
        std::cout << termcolor::blue;
    else if (rarity == "легендарная")
        std::cout << termcolor::on_red;

    std::cout << rarity << termcolor::reset;
}
