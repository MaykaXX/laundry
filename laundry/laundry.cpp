#include <iostream>
#include <windows.h>
#include <string>
#include <map>

using namespace std;

void list_activity();

int main() {
    SetConsoleOutputCP(65001); // Поддержка Юникода для вывода emoji
    cout << "Привет, это прачечная!🫧\n\n";
    list_activity();
}

void list_activity() {
    map<int, string> activities;
    activities[1] = "Обучение";
    activities[2] = "Стирать одежду 👕";
    activities[3] = "Ждать посетителей 👀";
    activities[4] = "Сушить одежду 🍃";
    activities[5] = "Сложить всё красиво 🌻";

    for (const auto& pair : activities) {
        cout << pair.first << " " << pair.second << endl;
    }

    int choose;

    cout << "Что хочешь сделать? ";
    cin >> choose;
    if (choose == 1) {
        cout << "\033[32mХорошо делай свою работу!\033[0m\n" << endl;
    }
    else if (choose == 2) {

        cout << "Одеждда стирается!" << endl;
    }
    else if (choose == 3) {
        cout << "Порой время словно стоит на месте!" << endl;
    }
    else if (choose == 4) {
        cout << "Если бы не сушильная машинка,\nодежда бы дружила с ветром!" << endl;
    }
    else if (choose == 5) {
        cout << "Все сложено идеально!" << endl;
    }

    cout << "\nТы выбрал" << choose << endl;
}



    