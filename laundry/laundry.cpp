#include <iostream>
#include <istream>
#include <windows.h>
#include <string>
#include <map>
#include <fstream>
#include "termcolor.hpp"
#include <time.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <functional>


void slowPrint(const std::string& text, int delay = 10) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

class GameClock {
private:
    int hour;
    int minutes;
    int day;
public:
    GameClock() : hour(8), minutes(00), day(1) {} // Игра начинается в 6 утра первого дня

    void advanceTime(int hourToAdvance, int minutesToAdvance) {
        hour += hourToAdvance;
        minutes += minutesToAdvance;
        
        while (minutes >= 60) {
            hour += 1;
            minutes -= 60;
        }
        
        while (hour >= 24) {
            hour -= 24;
            //minutes -= 60;
            day++;
            std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
        }
    }

    void go_to_bed() {
        if (isNight()) {
            std::cout << "Тебе пора спать!" << std::endl << "Пойдешь?(да/нет) ";
            std::string message;
            std::cin >> message;
            std::cout << std::endl;
            if (message == "да") {
                hour = 8;
                minutes = 00;
                day++;
                std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
            }
            else if (message == "нет") {
                while (hour >= 24) {
                    std::cout << "Ты слишком устал. Спокойной ночи🌙!";
                    hour = 8;
                    minutes = 00;
                    day++;
                    std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
                }
            }
        }
    }

    void showTime() const {
        if (minutes > 9)
            std::cout << "День " << day << ", " << hour << ":" << minutes << std::endl;
        else
            std::cout << "День " << day << ", " << hour << ":" << "0" << minutes << std::endl;
    }

    bool isDaytime() const {
        return hour >= 6 && hour < 20;
    }

    bool isNight() const {
        return !isDaytime();
    }
    
    friend class Laundry;

};


class WatchingBirds {
protected:
    int quality_birds;
    
    
public:
    void random_meet() {
        int num = 1 + std::rand() % 100;
        if (num <= 10) {
            std::cout << "Тихо: Птичка на горизонте!" << std::endl;
            take_photo();
        }
        else {
            std::cout << "Небо чистое... Птиц не видно." << std::endl;
        }
     }
    void take_photo() {
        std::string message;
        std::cout << "Хочешь сделать фото?" << termcolor::cyan << "(да / нет) " << termcolor::reset;
        std::cin >> message;
        std::cout << std::endl;
        if (message == "да") {
            random_bird();
            
        }
    }
    void random_bird() {
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
    void colored_rarity(const std::string& rarity) {
        if (rarity == "обычная")
            std::cout << termcolor::green;
        else if(rarity == "редкая")
            std::cout << termcolor::yellow;
        else if (rarity == "супер-редкая")
            std::cout << termcolor::blue;
        else if (rarity == "легендарная")
            std::cout << termcolor::on_red;

        std::cout << rarity << termcolor::reset;
    }


       
};

class Laundry {
private:
    std::string us_name;
protected:
    bool iscamera;
public:
    GameClock clock;
    Laundry(const std::string& name) : us_name(name), iscamera(false), clock() {}
    

    void check_day() {
        if (clock.day < 5) {
            iscamera = false;
        }
    }
    
    std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> getAvailableActions() {
        std::map<int, std::string> activities;
        std::map<int, std::function<void()>> actions;

        activities[1] = "Обучение";
        actions[1] = [this]() {
            std::ifstream file("instruction.txt");
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    std::cout << line << std::endl;
                }
                file.close();
            }
            std::cout << std::endl;
            };

        activities[2] = "Стирать одежду 👕";
        actions[2] = [this]() {
            code_for_washing();
            clock.advanceTime(2, 0);
            };

        activities[3] = "Ждать посетителей 👀";
        actions[3] = [this]() {
            std::cout << "Порой время словно стоит на месте!" << std::endl;
            if (iscamera) {
                WatchingBirds birds;
                birds.random_meet();
            }
            clock.advanceTime(1, 0);
            };

        activities[4] = "Сложить всё красиво 🌻";
        actions[4] = [this]() {
            std::cout << "Все сложено идеально!" << std::endl;
            clock.advanceTime(0, 30);
            };

        if (clock.day >= 3) {
            activities[5] = "Сушить одежду 🍃";
            actions[5] = [this]() {
                std::cout << "Если бы не сушильная машинка,\nодежда бы дружила с ветром!" << std::endl;
                clock.advanceTime(2, 0);
                };
        }

        int exitIndex = static_cast<int>(activities.size()) + 1;
        activities[exitIndex] = "Ничего";
        actions[exitIndex] = []() {
            std::cout << "Хорошо, до встречи!🌞\n";
            };

        if (clock.isNight()) {
            
            int sleepIndex = static_cast<int>(activities.size()) + 1;
            activities[sleepIndex] = "Пойти спать 🌙";
            actions[sleepIndex] = [this]() {
                clock.go_to_bed();
                };
        }

        return {activities, actions};

    }


    void list_activity() {
        bool running = true;
        while (running) {
            special_events();
            clock.go_to_bed();
            clock.showTime();

            auto actionsPair = getAvailableActions();
            auto activities = actionsPair.first;
            auto actions = actionsPair.second;
            int exitIndex = static_cast<int>(activities.size());



            for (const auto& pair : activities) {
                std::cout << pair.first << " " << pair.second << std::endl;
            }
            
            int choice;
            std::cout << "Что хочешь сделать? ";
            std::cin >> choice;
            std::cout << std::endl;

            if (actions.count(choice)) {
                actions[choice]();
                if (choice == exitIndex) {
                    running = false;
                }
            }
            else {
                std::cout << "ОЙ, ты нажал что-то другое!" << std::endl;
            }
           
        }
    }
    
    void special_events() {
        switch (clock.day) {
        case 5: {
            std::cout << termcolor::bright_blue << "👵 Ванесса: " << termcolor::reset;
            std::string message = "Привет, " + us_name + " я тут убиралась на чердаке и нашла старый фотоапарат.\nМожет пригодится!";
            std::cout << "Получен " << termcolor::bright_cyan << '[' << "фотоапарат" << ']' << termcolor::reset << std::endl;
            iscamera = true;

            slowPrint(message , 40);
            slowPrint("Теперь тебе доступен мир орнитологии");
        }
        }
    }

    void code_for_washing() {
        std::cout << "Введи код для стиральной машины: ";
        int code;
        std::cin >> code;
        if (code != 1010) {
            std::cout << "Код не верный!";
        }
        else {
            std::cout << "Одежда стирается!" << std::endl;
        }
    }

    void good_day() {
        std::cout << "Пожелать хорошего дня" << termcolor::blue << "(space + enter)" << termcolor::reset << std::endl;
    }

    /*Статус для стиралной машины и сушильной
    [пустая], [загружена], [стирается/сушится], [неисправна(шанс)]

    добавить уровень износа и возможность улучшить машины

    */
};


int main() {
    SetConsoleOutputCP(65001); // Поддержка Юникода для вывода emoji
    srand(time(NULL));
    
    std::cout << termcolor::bright_magenta << "Добро пожаловать в прачечную!🧺\nЗдесь всё крутится вокруг чистоты и уюта\nПрочти краткую инструкцию и помоги первым клиентам освежить свой день!🌞🫧\n\n" << termcolor::reset;
    
    std::string us_name;
    std::cout << "Как тебя зовут: ";
    std::cin >> us_name;
    std::cout << "Добро пожаловать, "<< us_name << std::endl;


    Laundry laundry(us_name);
    laundry.list_activity();
    

    return 0;
}
