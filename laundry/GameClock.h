#pragma once
#include <iostream>
#include "termcolor.hpp"

class Laundry; // forward declaration

class GameClock {
private:
    int hour;
    int minutes;
    int day;
    int quality_customers_day;

public:
    GameClock();

    void advanceTime(int hourToAdvance, int minutesToAdvance);
    void go_to_bed();
    void random_qual_custom();
    void decrementCustomerCount();
    void showTime() const;
    bool isDaytime() const;
    bool isNight() const;

    int getDay() const { return day; }

    friend class Laundry;
};