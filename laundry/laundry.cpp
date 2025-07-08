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
#include <unordered_map>
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
    int quality_customers_day;

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
            quality_customers_day = 0;
            std::cout << termcolor::magenta << "Наступил новый день! День " << day << termcolor::reset << std::endl;
            random_qual_custom();
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

    void random_qual_custom() {
        int random_num = 1 + rand() % 3;
        quality_customers_day = random_num;
        std::cout << quality_customers_day << std::endl;      
        //std::cout << "Адрес объекта clock: " << this << std::endl;

    }

    void getCustomerCount() const {
        std::cout << quality_customers_day << std::endl;
        
    }

    void decrementCustomerCount() {
        if (quality_customers_day > 0)
            --quality_customers_day;
    }

    void showTime() const { 
        getCustomerCount();
        //std::cout << "Адрес объекта clock: " << this << std::endl;

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

class QuestFlags {
private: 
    std::unordered_map<std::string, bool> flags;

public:
    // установка флага
    void set(const std::string& name, bool value = true) {
        flags[name] = value;
    }

    // получаем значение флага 
    bool get(const std::string& name) const {
        auto it = flags.find(name);
        return it != flags.end() ? it->second : false;
    }

    // удалить флаг
    void vlear(const std::string& name) {
        flags.erase(name);
    }

    // очистка всех флагов
    void reset() {
        flags.clear();
    }
};

class Village {
private:
    struct Building {
        std::string name;
        std::string icon;
    };

    std::vector<Building> village;

    bool unlock_new_buildings = false;

public:
    GameClock clock;
    
    
    void message_village() {
        std::cout << termcolor::bright_blue << "Ты в деревне" << termcolor::reset << std::endl;
        clock.advanceTime(0, 10);
    }

    void drawVillageMap(const std::vector<Building>& buildings){
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
        
    void where_go() {
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
        else if(choose == "Словесниковы") {
            print_Slovesnikov();
            
        }
        else {
            std::cout << termcolor::red << "Неверный ввод!" << termcolor::reset << std::endl;
        }

        std::cout << "\n[Нажми Enter, чтобы вернуться в меню деревни]\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        list_activity_village();

    }

    std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> getActionVillage() {
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

        return { activities, action };
    }

    struct house {
        std::string name; //например: дом Казановых
        std::vector<std::string> members;
        std::string specialization;
    };
    
    void print_Silkbrew() {
        house family_Silkbrew;
        family_Silkbrew.name = "Тиховары";
        family_Silkbrew.members = { "👴 Мустафа", "👵 Ванесса" };
        family_Silkbrew.specialization = "Silkbrew Delight (русс. Пирог и Пар)";

        std::cout << family_Silkbrew.name << " (" << family_Silkbrew.specialization << "):\n";

        for (const auto& resident : family_Silkbrew.members) {
            std::cout << " - " << resident << std::endl << std::endl;
        }
    }

    void print_Gvozdev() {
        house family_Gvozdev;
        family_Gvozdev.name = "Гвоздевы";
        family_Gvozdev.members = { "🔨 Пётр Гвоздев", " 📐 Алёна Гвоздева", "🐦 Тимоша"};
        family_Gvozdev.specialization = "Ironroot Craftworks (русс. Крепкое Дело)";                       

        std::cout << family_Gvozdev.name << " (" << family_Gvozdev.specialization << "):\n";

        for (const auto& resident : family_Gvozdev.members) {
            std::cout << " - " << resident << std::endl;
        }

    }
    /*  Пётр Гвоздев — строгий, работает с рассвета

        Алёна Гвоздева — чертит планы и любит порядок

        Тимоша — сын, собирает гвозди и делает скворечники */


    void print_Rumyanov() {
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

    void print_Slovesnikov() {
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

    void list_activity_village() {
        bool running = true;
        while (running) {
            clock.showTime();

            auto actionsPair = getActionVillage();
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
    bool sock_hunt = false, has_found_sock_vladimir = false;
    std::string us_name;
    int max_number_of_customers = 0, number_of_customers = 0, amount_clothing_dirty = 0, amount_clothing_clean = 0, level_serviceability = 5;
    float price_washing = 2.0f, price_drying = 1.0f, fine = 0, wallet = 0; // $
    bool is_village = false, has_met_mustafa = false, has_met_vanessa = false, has_met_ayzuk = false;
    // рандомное максимальное к-во покупателей за день (max_number_of_customers)
    // сколько ты сам обслужил покупателей (number_of_customers)
    GameClock& clock;
    QuestFlags questFlags;

protected:
    bool iscamera;
public:

    Laundry(const std::string& name, GameClock& clock_ref) : us_name(name), iscamera(false), clock(clock_ref) {}

    void check_day() {
        if (clock.day < 5) {
            iscamera = false;
        }
    }

    std::string user_text(std::string text) {
        std::string user_name = "👤 " + us_name + ": ";
        return user_name + text;
    }

    void wait_for_enter() {
        std::cout << termcolor::blue << "(нажмите Enter)" << termcolor::reset << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << std::string(3, '\n'); //очищаем экран
    }

    std::map<int, std::string> assigments;
    int nextAssigId = 1;

    void add_assigm(const std::string& description) {
        assigments[nextAssigId] = description;
        nextAssigId++;
    }

    void printAssigm() {
        for (const auto& pair : assigments) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }

    void customers() {
        struct Customers {
            std::string name;
            std::string text;
            std::function<void()> interaction; // функция взаимодействия
        };
        

        std::vector<Customers> customer = {
            {"👩‍🦰 Ольга", "Здравствуйте! У меня здесь бельё с отпуска песок - повсюду!",
                [this]() {
                    slowPrint("💬 Ольга : Может у вас есть режим 'пляжная стирка'? ");
                    
                    std::cout << termcolor::bright_grey << "К сожалению, нету..." << termcolor::reset;
                    wait_for_enter();
                                            
                    this->user_text("К сожалению, нету...");
                }
            },
            {"🧔 Владимир", "",
                [this]() {
                    if (!has_found_sock_vladimir) {
                        slowPrint("💬 Владимир: Надеюсь, сегодня машинка не съест ещё один носок...");

                        std::cout << termcolor::blue << "НоВыЙ кВеСт ПоЛуЧеН!" << termcolor::reset << std::endl;
                        
                        std::string assigment = "💡 Найди второй носок владимира";
                        std::cout << assigment << std::endl;
                        add_assigm(assigment);
                        std::cout << termcolor::bright_grey << "Кажется пора все хорошо сложить.." << termcolor::reset << std::endl;

                        this->sock_hunt = true;
                    }
                    else {
                        slowPrint("🧔 Владимир: Сегодня без сюрпризов. Одежда чистая, настроение — тоже.");
                        std::cout << termcolor::bright_grey << "Будет сделано!" << termcolor::reset;
                        wait_for_enter();

                        this->user_text("Будет сделано!");
                    }
                }
            },
            {"👧 София", "Мам, а можно стирать игрушки?🧸", [this]() {
                std::string name_sofia = "👧 София: ";
                std::cout << name_sofia;
                slowPrint("У моего мишки грязная лапка. Ты сможешь его постирать? Он не боится!");
                
                std::string question;
                std::cout << "да/нет: ";
                std::cin >> question;
                std::cout << std::endl;

                if (question == "нет") {
                    std::cout << name_sofia;
                    slowPrint("А он и так уже весь пыльный... Ну ладно. Я просто спрячу его под подушку.");
                    std::cout << "😢" << std::endl;
                }
                else if (question == "да") {
                    questFlags.set("sofia_bear_given");
                    if (questFlags.get("sofia_bear_given")) {
                        
                        questFlags.set("sofia_bear");

                        std::cout << termcolor::blue << "Получен медведь!" << termcolor::reset << std::endl;
                    }
                }

            }},
            {"🧑‍🔧 Игорь", "Привет! Кто-то опять забыл ключи от сушилки...", [this]() {}},
            {"👵 Бабушка Зина", "Сначала всё кипятком, потом полоскать - как в старые добрые времена!", []() {}},
            {"📚 Марк", "Я пока постираю, заодно диплом напишу...", [this]() {}},
            {"🎨 Анна", "Кто-то случайно не находил розовый платок в горошек? 🎀", [this]() {}},
            {"🐱 Кот Борис", "Мяу! Не трогайте мой плед, он пахнет мною!", [this]() {}},
            {"🧘‍♂️ Алексей", "Стирать - это как медитация. Тепло, белый шум, аромат свежести...", [this]() {}},
            {"💃 Ника", "Пока бельё крутится, я пойду попрактикую бачату!", [this]() {}},
            {"👨‍💼 Артём", "Я пришёл строго по расписанию. Надеюсь, никто не занял мою машинку!", [this]() {}},
            {"🧕 Амина", "Вода пахнет жасмином? Или это мой кондиционер снова пролился...", [this]() {}},
            {"👨‍🍳 Павел", "Шеф-повар не может позволить себе грязный фартук!", [this]() {}},
            {"👩‍🎤 Лана", "Бельё постираю - и сразу на репетицию. Барабаны не ждут!", [this]() {}},
            {"👨‍🌾 Николай", "Сначала навоз, потом стирка... Хорошо, что в этом порядке!", [this]() {}},
            {"👩‍💻 Вера", "Стираю ночью, чтобы не отвлекаться от кода. Надеюсь, соседи поймут...", [this]() {}},
            {"🧙‍♂️ Станислав", "Я зачаровал носки от исчезновения. Теперь проверим, работает ли!", [this]() {}},
            {"🦸‍♀️ Кира", "Мой костюм супергероя нельзя сушить горячим воздухом. Он... сжимается.", [this]() {}},
            {"🎭 Роман", "Костюмы для спектакля должны сиять. Как и актёры.", [this]() {}},
            {"👨‍🚀 Юрий", "После тренировки в невесомости даже форма пахнет звёздами.", [this]() {}},
            {"🧒 Тимур", "Мама велела постирать. Я сам не знаю, что тут делать...", [this]() {}},
            {"👩‍🌾 Дарья", "С грядки — в барабан. Всё как по плану.", [this]() {}},
            {"🐶 Боня", "Гав! Принёс одеяло. Оно само пришло в грязь, честно!", [this]() {}},
            {"👨‍🏫 Евгений", "Прежде чем стирать, проведём небольшой инструктаж по ткани.", [this]() {}},
            {"👩‍🚒 Катя", "Форма пожарная, но теперь пусть будет просто горячая... от стирки!", [this]() {}}

        };          

        if (clock.quality_customers_day == 0) {
            std::cout << "Иногда даже прачечной нужно одиночество." << std::endl;
        }
        else {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(0, customer.size() - 1);

            int index = distrib(gen);

            const Customers& c = customer[index];

            clock.decrementCustomerCount();
            std::cout << clock.quality_customers_day << std::endl;

            std::cout << "Кто-то входит в прачечную...\n";

            std::cout << termcolor::yellow << c.name << ": " << termcolor::reset;
            if (!c.text.empty()) { 
                slowPrint(c.text);
            }
            std::cout << std::endl;

            c.interaction();
        }

    }

    void give_clothes() {
        float salary;
        std::string give;
        while (true) {
            std::cout << termcolor::green << "[отдать одежду(да/нет)]: " << termcolor::reset;
            std::cin >> give;
            std::cout << std::endl;
            if (give == "нет") {
                std::cout << "Клиенты не любят долго ждать! Они заплатят меньше" << std::endl;
                fine += 0.5f;
                std::cout << (price_washing - fine);
                give_clothes();

            }
            else if (give == "да") {
                good_day();
             
                if (fine == 0.0f) {
                    salary = price_washing * amount_clothing_clean;
                    questFlags.get("sofia_bear") ? salary += 1 : salary;
                }
                else {
                    salary = (price_washing - fine) * amount_clothing_clean;
                }
                wallet += salary;
                amount_clothing_clean = 0;
                std::cout << "Ты заработал " << termcolor::yellow << salary << "$" << termcolor::reset << std::endl;
                list_activity();
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
            std::cout << "█ ";
        }
        if (questFlags.get("sofia_bear"))
            std::cout << termcolor::green << "Бонус от стирки медведя + 1$" << termcolor::reset << std::endl;
            
        else 
            std::cout << "\nОдежда постирана!" << std::endl;
        give_clothes();
    }
    
    void process_drying() {
        std::cout << "Процесс сушки..." << std::endl;
        for (int i = 0; i < 10; ++i) {
            Sleep(1500);
            std::cout << "▊ ";
        }
        std::cout << "\nОдежда высушена!" << std::endl;
    }
    
    void showMaykaRetroBanner() {
        std::string banner = R"(
                                                                  .---.        ____      ___    _ ,---.   .--. ______     .-------. 
                      | ,_|      .'  __ `. .'   |  | ||    \  |  ||    _ `''. |  _ _   \      \   \   /  / 
                    ,-./  )     /   '  \  \|   .'  | ||  ,  \ |  || _ | ) _  \| ( ' )  |       \  _. /  '  
                    \  '_ '`)   |___|  /  |.'  '_  | ||  |\_ \|  ||( ''_'  ) ||(_ o _) /        _( )_ .'   
                     > (_)  )      _.-`   |'   ( \.-.||  _( )_\  || . (_) `. || (_,_).' __  ___(_ o _)'    
                    (  .  .-'   .'   _    |' (`. _` /|| (_ o _)  ||(_    ._) '|  |\ \  |  ||   |(_,_)'     
                     `-'`-'|___ |  _( )_  || (_ (_) _)|  (_,_)\  ||  (_.\.' / |  | \ `'   /|   `-'  /      
                      |        \\ (_ o _) / \ /  . \ /|  |    |  ||       .'  |  |  \    /  \      /       
                      `--------` '.(_,_).'   ``-'`-'' '--'    '--''-----'`    ''-'   `'-'    `-..-'        
                                                                                       
        )";
        std::cout << "\033[1;36m"; // Голубой стиль
        slowPrint(banner, 1); 
        std::cout << "\033[0m\n";
    }

    bool baner = true;
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
            
            if (sock_hunt == true) {
                std::cout << termcolor::bright_cyan << "Носок найден!" << termcolor::reset << std::endl;
                has_found_sock_vladimir = true;

                std::cout << termcolor::bright_grey << "Отнести носок Владимиру" << termcolor::reset;
                wait_for_enter();
         
                std::this_thread::sleep_for(std::chrono::seconds(1));
                slowPrint("🧔 Владимир: Я уже приготовился устраивать прощание. А тут — такой поворот!");
            }
            else {
                std::cout << "Все сложено идеально!" << std::endl;
                clock.advanceTime(0, 30);
            }            
        };

        activities[nextIndex] = "Посмотреть задания 💡";
        actions[nextIndex++] = [this]() {

            if (assigments.empty()) {
                std::cout << "🧺 — Нет заданий в корзине" << std::endl;
            }
            else {
                printAssigm();
            }
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
                village.list_activity_village();
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
            clock.getCustomerCount();

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
        int number_errors = 0;
        
        while (number_errors < 2) {
            std::cout << "Введи код для стиральной машины: ";
            int code;
            std::cin >> code;
            
            if (code == 1010) {
                if (questFlags.get("sofia_bear")) {
                    std::cout << termcolor::green << "Мишка в стирке 🧸" << termcolor::reset << std::endl;

                }
                else {
                    std::cout << termcolor::green << "Одежда стирается!" << termcolor::reset << std::endl;
                }
                  break;
              }

            else {
                number_errors++;
                std::cout << termcolor::bright_red << "Код не верный!" << termcolor::reset << std::endl;

                std::string try_again;
                    while(true){
                    std::cout << "Попробуешь еще?(да/нет) ";
                    std::cin >> try_again;
    
                    if (try_again == "да")
                        break; 
                    else if (try_again == "нет") {
                        list_activity();
                        return;
                    }
                    else {
                        std::cout << "Напиши да или нет!";
                    }
                    }
                  }
                }
        if (number_errors == 2) {
            std::cout << termcolor::yellow << "Ты исчерпал все попытки. Загляни в обучение 🌼" << termcolor::reset << std::endl;
            list_activity();
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
    
    std::string us_name;

    GameClock clock;
    
    clock.random_qual_custom();
    clock.getCustomerCount();
    
    
    std::cout << termcolor::bright_magenta << "Добро пожаловать в прачечную!🧺\nЗдесь всё крутится вокруг чистоты и уюта\nПрочти краткую инструкцию и помоги первым клиентам освежить свой день!🌞🫧\n\n" << termcolor::reset;
    
    std::cout << "Как тебя зовут: ";
    std::cin >> us_name;
    std::cout << "Добро пожаловать, "<< us_name << std::endl;
    

    Laundry laundry(us_name, clock);
    laundry.list_activity();
       
    return 0;
}
