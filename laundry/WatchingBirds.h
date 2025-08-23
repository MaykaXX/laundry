#pragma once
#include <iostream>

class WatchingBirds {
protected:
    int quality_birds;
public:
    void random_meet();
    void take_photo();
    void random_bird();
    void colored_rarity(const std::string& rarity);
};