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

class Laundry;
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
        
        if (isNight())
            go_to_bed();
            

        while (hour >= 24) {
            hour -= 24;
            //minutes -= 60;
            day++;
            std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
        }
    }

    void go_to_bed() {
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

class Village {
public:
    void message_village() {
        std::cout << termcolor::bright_blue << "Ты в деревне" << termcolor::reset << std::endl;
    }

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
    int max_number_of_customers = 0, number_of_customers = 0, amount_clothing_dirty = 0, amount_clothing_clean = 0, wallet = 2, level_serviceability = 0;
    float price_washing = 2, price_drying = 1, fine = 0; // $
    bool is_village = false, has_met_mustafa = false, has_met_vanessa = false, has_met_ayzuk = false;
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
                give_clothes();

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
                amount_clothing_clean = 0;
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

    void process_washing() {
        std::cout << "Процесс стирки..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            Sleep(1500);
            std::cout << "▊ ";
        }
        std::cout << "\nОдежда постирана!" << std::endl;
    }
    
    void process_drying() {
        std::cout << "Процесс сушки..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            Sleep(1500);
            std::cout << "▊ ";
        }
        std::cout << "\nОдежда высушена!" << std::endl;
    }

    std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> getAvailableActions() {
        std::map<int, std::string> activities;
        std::map<int, std::function<void()>> actions;

        int nextIndex = 1;
        activities[nextIndex] = "Обучение";
        actions[nextIndex++] = [this]() {
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

        activities[nextIndex] = "Стирать одежду 👕";
        actions[nextIndex++] = [this]() {
            while (amount_clothing_dirty > 0 && level_serviceability > 0) {
                code_for_washing();
                clock.advanceTime(2, 0);
                amount_clothing_clean = amount_clothing_dirty;
                amount_clothing_dirty = 0;
                level_serviceability -= 1;
                process_washing();

                give_clothes();
            }
            if(amount_clothing_dirty == 0) 
                std::cout << "У тебя нет что стирать!" << std::endl;
            if (level_serviceability == 0) {
                std::cout << termcolor::italic << "Стиральная машина сломана!\nТребуется для починки 1$" << termcolor::reset << std::endl;
                
                std::string pay;
                
                std::cout << "[заплатить 1$]" << termcolor::blue << "(space + enter)" << termcolor::reset;
                
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::getline(std::cin, pay);

                wallet -= 1;
                level_serviceability = 5;
                std::cout << termcolor::italic << "Заплачено 1$\nУровень исправности стиральной машины: " << level_serviceability << termcolor::reset << std::endl;
            }
        };


        activities[nextIndex] = "Ждать посетителей 👀";
        actions[nextIndex++] = [this]() {
            std::cout << "Порой время словно стоит на месте!" << std::endl;
            if (iscamera) {
                WatchingBirds birds;
                birds.random_meet();
            }
            customers();
            amount_clothing_dirty += 1;
            clock.advanceTime(1, 0);
            };

        activities[nextIndex] = "Сложить всё красиво 🌻";
        actions[nextIndex++] = [this]() {
            std::cout << "Все сложено идеально!" << std::endl;
            clock.advanceTime(0, 30);
            };

        activities[nextIndex] = "Посмотреть в кошелек 💰";
        actions[nextIndex++] = [this]() {
            if (wallet != 0)
                std::cout << "У тебя " << termcolor::yellow << wallet << "$" << termcolor::reset << std::endl;
            else
                std::cout << "Да здесь одни мухи!🪰🕸️" << std::endl; 
            };

        if (clock.day >= 3) {
            activities[nextIndex] = "Сушить одежду 🍃";
            actions[nextIndex++] = [this]() {
                std::cout << "Если бы не сушильная машинка,\nодежда бы дружила с ветром!" << std::endl;
                clock.advanceTime(2, 0);
                };
        }

        if (is_village) {
            activities[nextIndex] = "Пойти в деревню";
            actions[nextIndex++] = [this]() {
                Village village;
                village.message_village();
                clock.advanceTime(0, 10);
                };
        }
       
        int exitIndex = static_cast<int>(activities.size()) + 1;
        activities[exitIndex] = "Ничего";
        actions[exitIndex++] = []() {
            std::cout << "Хорошо, до встречи!🌞\n";
            };

        if (clock.isNight()) {
            int sleepIndex = static_cast<int>(activities.size()) + 1;
            activities[exitIndex] = "Пойти спать 🌙";
            actions[exitIndex++] = [this]() {
                clock.go_to_bed();
                list_activity();
                };
        }

        return {activities, actions};

    }


    void list_activity() {
        bool running = true;
        while (running) {
            special_events();
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
            if (!has_met_vanessa){
                std::cout << termcolor::bright_blue << "👵 Ванесса: " << termcolor::reset;
            std::string message = "Привет, " + us_name + ", я тут убиралась на чердаке и нашла старый фотоапарат.\nМожет пригодится!";
            slowPrint(message, 40);

            std::cout << "\nПолучен " << termcolor::bright_cyan << "[фотоапарат]\n" << termcolor::reset << std::endl;
            iscamera = true;

            Sleep(100);
            std::cout << termcolor::italic << "Теперь тебе доступен мир орнитологии\n" << termcolor::reset;
        }
            break;
        }
        case 2: {
            if (!has_met_mustafa) {
                std::cout << termcolor::bright_blue << "👴 Мустафа: " << termcolor::reset;
                std::string message = "Здравствуй! Вижу, ты у нас впервые. Наша деревня Каталин хоть и мала, да уютом славится.\nЗагляни в гости — поведаю тебе удивительные истории о здешних местах, а моя Ванесса пирог испечёт к чаю.\n"
                    "Живём мы неподалёку, так что ещё обязательно встретимся!\n";
                slowPrint(message);

                Sleep(100);
                is_village = true;
                std::cout << termcolor::italic << "Открыта деревня\n" << termcolor::reset;
                has_met_mustafa = true;
                
            }
            break;
        }
        case 7: {
            if (!has_met_ayzuk) {
                std::cout << termcolor::bright_blue << "👦 Айзик: " << termcolor::reset;
                std::string message = "Эээ... здрасьте! У меня тут, ну... очень деликатная проблема. Только маме ни слова, ладно?\n"
                    "Короче, мне срочно надо постирать... двойку из дневника! В журнале 'Мастер на все руки' написано, что это сработает. Ну я и подумал — а вдруг?";
                slowPrint(message, 50);

                std::string diary;
                std::cout << "\n[Взять дневник]" << termcolor::blue << "(space + enter)" << termcolor::reset << std::endl;

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::getline(std::cin, diary);

                std::cout << termcolor::italic << "\nПолучен дневник Айзика" << termcolor::reset << std::endl;

                process_washing();

                std::cout << termcolor::bright_blue << "\n👦 Айзик: " << termcolor::reset;
                std::string message1 = "\nОго! Получилось?! Ты волшебница, честное слово!\n"
                    "Теперь мама точно не узнает... Наверное. Спасибо тебе огромное!\n"
                    "Если что — я в долгу не останусь. У меня ещё кое-что надо... эээ... починить.";
                slowPrint(message1);
                std::cout << std::endl;
                good_day();
            }
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
