#include "GameClock.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

GameClock::GameClock() : hour(8), minutes(0), day(1) {}

void GameClock::advanceTime(int hourToAdvance, int minutesToAdvance) {
    hour += hourToAdvance;
    minutes += minutesToAdvance;

    while (minutes >= 60) {
        hour += 1;
        minutes -= 60;
    }

    if (isNight())
        go_to_bed();


    while (hour >= 24) {
        hour -= 24;
        //minutes -= 60;
        day++;
        quality_customers_day = 0;
        std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
        random_qual_custom();
    }
}

void GameClock::go_to_bed() {
    std::cout << "Тебе пора спать!" << std::endl << "Пойдешь?(да/нет) ";
    std::string message;
    std::cin >> message;
    std::cout << std::endl;
    if (message == "да") {
        hour = 8;
        minutes = 00;
        day++;
        std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
        random_qual_custom();
    }
    else if (message == "нет") {
        while (hour >= 24) {
            std::cout << "Ты слишком устал. Спокойной ночи🌙!";
            hour = 8;
            minutes = 00;
            day++;
            std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
            random_qual_custom();
        }
    }
}

void GameClock::random_qual_custom() {
    int random_num = 1 + rand() % 5;
    quality_customers_day = random_num;
    //std::cout << quality_customers_day << std::endl;      
    //std::cout << "Адрес объекта clock: " << this << std::endl;

}

void GameClock::decrementCustomerCount() {
    if (quality_customers_day > 0)
        --quality_customers_day;
}

void GameClock::showTime() const {
    if (minutes > 9)
        std::cout << "День " << day << ", " << hour << ":" << minutes << std::endl << std::endl;
    else
        std::cout << "День " << day << ", " << hour << ":" << "0" << minutes << std::endl << std::endl;
}

bool GameClock::isDaytime() const {
    return hour >= 6 && hour < 20;
}

bool GameClock::isNight() const{
    return !isDaytime();
}

