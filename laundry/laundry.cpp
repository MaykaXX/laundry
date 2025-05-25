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
        std::cout << "Хочешь сделать фото?" << termcolor::blink << "(да / нет)" << termcolor::reset;
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
            int age;
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
public:
    void list_activity() {
        std::map<int, std::string> activities = {
            {1, "Обучение"},
            {2, "Стирать одежду 👕"},
            {3, "Ждать посетителей 👀"},
            {4, "Сушить одежду 🍃"},
            {5, "Сложить всё красиво 🌻"},
            {6, "Ничего"}
        };
        
        bool running = true;
        while (running) {
            for (const auto& pair : activities) {
                std::cout << pair.first << " " << pair.second << std::endl;
            }
            
            running = which_one();
        }
    }
    bool which_one() {
        int choose;
        std::cout << "Что хочешь сделать? ";
        std::cin >> choose;
        std::cout << std::endl;

        switch (choose) {
        case 1: {
            std::ifstream file("instruction.txt");
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    std::cout << line << std::endl;
                }
                file.close();
            }
            std::cout << std::endl;
            break;
        }
        case 2:
            code_for_washing();
            break;
        case 3: {
            std::cout << "Порой время словно стоит на месте!" << std::endl;
            WatchingBirds birds;
            birds.random_meet();
            break;
        }
        case 4:
            std::cout << "Если бы не сушильная машинка,\nодежда бы дружила с ветром!" << std::endl;
            break;
        case 5:
            std::cout << "Все сложено идеально!" << std::endl;
            break;
        case 6:
            std::cout << "Хорошо, до встречи!🌞\n";
            return false; // завершить цикл
        default:
            std::cout << "ОЙ, ты нажал что-то другое!" << std::endl;
            break;
        }

        return true; // продолжать цикл

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
    
    Laundry laundry;
    laundry.list_activity();

    return 0;
}
