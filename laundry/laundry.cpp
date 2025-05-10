#include <iostream>
#include <windows.h>
#include <string>
#include <map>
#include <fstream>

void list_activity();
void which_one();

int main() {
    SetConsoleOutputCP(65001); // Поддержка Юникода для вывода emoji
    std::cout << "Привет, это прачечная!🫧\n\n";
    list_activity();
}

void list_activity() {
    std::map<int, std::string> activities;
    activities[1] = "Обучение";
    activities[2] = "Стирать одежду 👕";
    activities[3] = "Ждать посетителей 👀";
    activities[4] = "Сушить одежду 🍃";
    activities[5] = "Сложить всё красиво 🌻";
    activities[6] = "Ничего";

    for (const auto& pair : activities) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    which_one();  
}
void which_one() {
    int choose;
    std::cout << "Что хочешь сделать? ";
    std::cin >> choose;
    std::cout << std::endl;

    if (choose == 1) {
        std::ifstream file("instruction.txt");
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
            file.close();
        }
        std::cout << std::endl;
        which_one();
    }
    else if (choose == 2) {
        std::cout << "Одеждда стирается!" << std::endl;
        which_one();
    }
    else if (choose == 3) {
        std::cout << "Порой время словно стоит на месте!" << std::endl;
        which_one();
    }
    else if (choose == 4) {
        std::cout << "Если бы не сушильная машинка,\nодежда бы дружила с ветром!" << std::endl;
        which_one();
    }
    else if (choose == 5) {
        std::cout << "Все сложено идеально!" << std::endl;
        which_one();
    }
    else if (choose == 6) {
        std::cout << "Хорошо, до встречи!🌞\n";
    }
    else {
        std::cout << "ОЙ, ты нажал что-то другое!" << std::endl;
        which_one();
    }


}