#include "Village.h"
#include "Laundry.h"
#include "Utils.h"
#include <iostream>

Village::Village(Laundry& l, GameClock& c)
    : laundry(l), clock(c) {
}

void Village::message_village() {
    std::cout << "Ты в деревне!" << std::endl;
    clock.advanceTime(0, 10);
}

void Village::drawVillageMap(const std::vector<Building>& buildings) {
    std::cout << termcolor::cyan << "\n╔═══════════ ДЕРЕВНЯ ═══════════╗\n" << termcolor::reset;
    std::cout << "║                               ║\n";

    for (const auto& b : buildings) {
        std::cout << "║   " << b.icon << "  " << b.name;
        // Добить пробелами до конца рамки (30 символов ширина содержимого)
        int padding = 19 - static_cast<int>(b.name.size()) - 4;
        std::cout << std::string(padding, ' ') << "║\n";
    }

    std::cout << "║                               ║\n";
    std::cout << termcolor::cyan << "╚═══════════════════════════════╝\n" << termcolor::reset;
}

void Village::where_go() {
    std::string choose;
    std::cout << "Куда хочешь пойти? ";
    std::cin.ignore();
    std::getline(std::cin, choose);
    std::cout << std::endl;

    if (choose == "Дом Тиховаров") {
        print_Silkbrew();
        std::cout << "👴 Мустафа: ";
        slowPrint("А вот и ты. Заходи, не стой на пороге — Ванесса сейчас пирог принесёт, свежий, с ревенем.\nА пока сядь, расскажу тебе одну историю. Про Каталин\nДавным-давно здесь не было ничего — только поле, ветер да один старик с иглой. Каталь его звали. Шил он людям одежду и никогда не брал плату. Говорил: «Холод прогоняется не только тканью, но и добром».\nЛюди начали селиться рядом. Кто с молотком, кто с тестом, кто с книгами. Так и выросла деревня. Не по приказу, а по доброй воле.\nС тех пор и живём: каждый делает своё дело — и для себя, и для других.\nА теперь вот и ты тут. Уж не просто так, наверное?");

        unlock_new_buildings = true;
        std::cout << termcolor::blue << "[открыты новые здания]" << termcolor::reset << std::endl;

        std::cout << "👵 Ванесса";
        slowPrint("С дороги-то, поди, устал. Вот, милай, пирог. Только достала из печи — ты пока попробуй, остальное само приложится.");

        clock.advanceTime(1, 30);
    }

    else if (choose == "Дом Гвоздевых") {
        print_Gvozdev();

    }
    else if (choose == "Румяновых") {
        print_Rumyanov();

    }
    else if (choose == "Словесниковы") {
        print_Slovesnikov();

    }
    else {
        std::cout << termcolor::red << "Неверный ввод!" << termcolor::reset << std::endl;
    }

    std::cout << "\n[Нажми Enter, чтобы вернуться в меню деревни]\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    list_activity_village();
}

std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> Village::getActionVillage() {
    std::map<int, std::string> activities;
    std::map<int, std::function<void()>> action;

    int nextIndex = 1;
    activities[nextIndex] = "Посмотреть карту деревни";
    action[nextIndex++] = [this]() {
        std::vector<Building> village;

        if (!unlock_new_buildings) {
            village = {
               {"🏡", "Дом Тиховаров"},
               {"🧺", "Прачечная (ты здесь)"},
               {"[?]", "???"},
            };
        }
        else if (unlock_new_buildings) {
            village = {
                {"🏡", "Дом Тиховаров"},
                {"🧺", "Прачечная (ты здесь)"},
                {"🧱", "Дом Гвоздевых"},
                {"🥖", "Румяновых"},
                {"📖", "Словесниковы"}
            };
        }
        drawVillageMap(village);
        };

    activities[nextIndex] = "Перейти к дому";
    action[nextIndex++] = [this]() {
        where_go();
        };

    activities[nextIndex] = "Вернуться в прачечную";
    action[nextIndex++] = [this]() {
        laundry.message_laundry();
        laundry.list_activity();
        };

    return { activities, action };
}
void Village::print_Silkbrew() {
    house family_Silkbrew;
    family_Silkbrew.name = "Тиховары";
    family_Silkbrew.members = { "👴 Мустафа", "👵 Ванесса" };
    family_Silkbrew.specialization = "Silkbrew Delight (русс. Пирог и Пар)";

    std::cout << family_Silkbrew.name << " (" << family_Silkbrew.specialization << "):\n";

    for (const auto& resident : family_Silkbrew.members) {
        std::cout << " - " << resident << std::endl << std::endl;
    }
}

void Village::print_Gvozdev() {
    house family_Gvozdev;
    family_Gvozdev.name = "Гвоздевы";
    family_Gvozdev.members = { "🔨 Пётр Гвоздев", " 📐 Алёна Гвоздева", "🐦 Тимоша" };
    family_Gvozdev.specialization = "Ironroot Craftworks (русс. Крепкое Дело)";

    std::cout << family_Gvozdev.name << " (" << family_Gvozdev.specialization << "):\n";

    for (const auto& resident : family_Gvozdev.members) {
        std::cout << " - " << resident << std::endl;
    }

}
/*  Пётр Гвоздев — строгий, работает с рассвета

    Алёна Гвоздева — чертит планы и любит порядок

    Тимоша — сын, собирает гвозди и делает скворечники */

void Village::print_Rumyanov() {
    house family_Rumyanov;
    family_Rumyanov.name = "Румяновы";
    family_Rumyanov.members = { "🍞 Марфа Румянова", "🚲 Захар Румянов", "🧁 Варя" };
    family_Rumyanov.specialization = "Hearth & Crust (русс. Очаг и Корочка)";

    std::cout << family_Rumyanov.name << " (" << family_Rumyanov.specialization << "):\n";

    for (const auto& resident : family_Rumyanov.members) {
        std::cout << " - " << resident << std::endl;
    }
}
/*  Марфа Румянова — печёт хлеб и разговаривает с тестом

    Захар Румянов — развозит выпечку, шутит со всеми

    Варя — их дочь, делает пирожки с удивительными начинками */

void Village::print_Slovesnikov() {
    house family_Slovesnikov;
    family_Slovesnikov.name = "Словесниковы ";
    family_Slovesnikov.members = { "📖 Семён Словесников", "✏️ Лидия Словесникова", "🌠 Миша" };
    family_Slovesnikov.specialization = "Hearth & Crust (русс. Очаг и Корочка)";

    std::cout << family_Slovesnikov.name << " (" << family_Slovesnikov.specialization << "):\n";

    for (const auto& resident : family_Slovesnikov.members) {
        std::cout << " - " << resident << std::endl;
    }
}
/*  Семён Словесников — ведёт летопись, читает у костра

    Лидия Словесникова — учит грамоте всех желающих

    Миша — сын, сочиняет сказки и мечтает стать писателем */

void Village::list_activity_village() {
    std::cout << "Активности в деревне..." << std::endl;
    laundry.message_laundry(); // вызов метода из Laundry
}
