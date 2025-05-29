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
#include <algorithm>
#include <random>
#include <limits>
#undef max


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
            std::cout << "День " << day << ", " << hour << ":" << minutes << std::endl << std::endl;
        else
            std::cout << "День " << day << ", " << hour << ":" << "0" << minutes << std::endl << std::endl;
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
    int max_number_of_customers = 0, number_of_customers = 0, amount_clothing_dirty = 0, amount_clothing_clean = 0, wallet = 0;
    float price_washing = 2, price_drying = 1, fine = 0; // $
    // рандомное максимальное к-во покупателей за день (max_number_of_customers)
    // сколько ты сам обслужил покупателей (number_of_customers)

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

    void customers() {
        struct Customers {
            std::string name;
            std::string text;
        };

        std::vector<Customers> customer = {
            {"👩‍🦰 Ольга", "Здравствуйте! У меня здесь бельё с отпуска песок - повсюду!"},
            {"🧔 Владимир", "Надеюсь, сегодня машинка не съест ещё один носок..."},
            {"👧 София", "Мам, а можно стирать игрушки?🧸"},
            {"🧑‍🔧 Игорь", "Привет! Кто-то опять забыл ключи от сушилки..."},
            {"👵 Бабушка Зина", "Сначала всё кипятком, потом полоскать - как в старые добрые времена!"},
            {"📚 Марк", "Я пока постираю, заодно диплом напишу..."},
            {"🎨 Анна", "Кто-то случайно не находил розовый платок в горошек? 🎀"},
            {"🐱 Кот Борис", "Мяу! Не трогайте мой плед, он пахнет мною!"},
            {"🧘‍♂️ Алексей", "Стирать - это как медитация. Тепло, белый шум, аромат свежести..."},
            {"💃 Ника", "Пока бельё крутится, я пойду попрактикую бачату!"},
            {"👨‍💼 Артём", "Я пришёл строго по расписанию. Надеюсь, никто не занял мою машинку!"},
            {"🧕 Амина", "Вода пахнет жасмином? Или это мой кондиционер снова пролился..."},
            {"👨‍🍳 Павел", "Шеф-повар не может позволить себе грязный фартук!"},
            {"👩‍🎤 Лана", "Бельё постираю - и сразу на репетицию. Барабаны не ждут!"},
            {"👨‍🌾 Николай", "Сначала навоз, потом стирка... Хорошо, что в этом порядке!"},
        };

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, customer.size() - 1);
        int index = distrib(gen);

        const Customers& c = customer[index];

        std::cout << termcolor::yellow << c.name << ": " << termcolor::reset;
        slowPrint(c.text);
        std::cout << std::endl;
    }


    void give_clothes() {
        int salary;
        std::string give;
        while (true) {
            std::cout << termcolor::green << "[отдать одежду(да/нет)]: " << termcolor::reset;
            std::cin >> give;
            std::cout << std::endl;
            if (give == "нет") {
                std::cout << "Клиенты не любят долго ждать! Они заплатят меньше" << std::endl;
                fine += 0.5f;

            }
            else if (give == "да") {
                good_day();
             
                if (fine == 0.0f) {
                    salary = price_washing * amount_clothing_clean;
                }
                else {
                    salary = (price_washing - fine) * amount_clothing_clean;
                }
                wallet += salary;
                std::cout << "Ты заработал " << termcolor::yellow << salary << "$" << termcolor::reset << std::endl;
            }
                break;
            }
        }


    void good_day() {
        std::string wish;
        std::cout << "[пожелать хорошего дня🌞]" << termcolor::blue << "(space + enter)" << termcolor::reset << std::endl;
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        std::getline(std::cin, wish); // читаем строку с пробелами
        
        std::cout << std::endl;
        std::string user = "👤" + us_name + ": " + "хорошего дня! 🌞";
        slowPrint(user);

        if (wish != " ") {
            std::cout << "Попробуй еще раз (сначало space, потом enter)" << std::endl;
            good_day();
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
            if (amount_clothing_dirty > 0) {
                code_for_washing();
                clock.advanceTime(2, 0);
                amount_clothing_clean = amount_clothing_dirty;
                amount_clothing_dirty = 0;
                std::cout << "Одежда постирана!" << std::endl;

                give_clothes();
            }
            else {
                std::cout << "У тебя нет что стирать!" << std::endl;
            }
        };


        activities[3] = "Ждать посетителей 👀";
        actions[3] = [this]() {
            std::cout << "Порой время словно стоит на месте!" << std::endl;
            if (iscamera) {
                WatchingBirds birds;
                birds.random_meet();
            }
            customers();
            amount_clothing_dirty += 1;
            clock.advanceTime(1, 0);
            };

        activities[4] = "Сложить всё красиво 🌻";
        actions[4] = [this]() {
            std::cout << "Все сложено идеально!" << std::endl;
            clock.advanceTime(0, 30);
            };

        activities[5] = "Посмотреть в кошелек 💰";
        actions[5] = [this]() {
            if (wallet != 0)
                std::cout << "У тебя " << termcolor::yellow << wallet << "$" << termcolor::reset << std::endl;
            else
                std::cout << "Да здесь одни мухи!🪰🕸️" << std::endl; 
            };

        if (clock.day >= 3) {
            activities[6] = "Сушить одежду 🍃";
            actions[6] = [this]() {
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
