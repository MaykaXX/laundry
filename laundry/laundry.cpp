#include "Laundry.h"
#include "Village.h"
#include "GameClock.h"
#include "Assignment.h"
#include "Utils.h"
#include "QuestFlags.h"
#include "WatchingBirds.h"
#include <iostream>
#undef max

Laundry::Laundry(const std::string& name, GameClock& c, QuestFlags& q) : us_name(name), clock(c), questFlags(q) {}

void Laundry::setVillage(Village* v) {
    village = v;
}

void Laundry::message_laundry() {
    std::cout << "Ты вернулся в прачечную!🧺" << std::endl;
}
void Laundry::check_day() {
    if (clock.day < 5) {
        iscamera = false;
    }
}
std::string Laundry::user_text(std::string text) const {
    std::string user_name = "👤 " + us_name + ": ";
    return user_name + text;
}

void Laundry::add_assigm(const std::string& desc) {
    assignments[nextAssigId++] = Assignment(desc);
}

void Laundry::removeAssignmentByText(const std::string& text) {
    for (auto it = assignments.begin(); it != assignments.end(); ) {
        if (it->second.getDescription() == text) {
            std::cout << "Задание \"" << it->second.getDescription() << "\" удалено!\n";
            it = assignments.erase(it);
            return; // удалили и вышли
        }
        else {
            ++it;
        }
    }
    std::cout << "Задание \"" << text << "\" не найдено.\n";
}


void Laundry::add_pro_assig(const std::string& desc, const std::vector<std::string>& subs) {
    assignments[nextAssigId++] = Assignment(desc, subs);
}

void Laundry::print_Assigm() {
    for (const auto& pair : assignments) {
        std::cout << pair.first << ": ";
        pair.second.print();
        std::cout << std::endl;
    }
}

// выполненное задание
void Laundry::completeSubTask(int id, const std::string& item) {
    auto it = assignments.find(id);
    if (it != assignments.end()) {
        it->second.completeSubTask(item);
    }
    else {
        std::cout << "Задание с таким ID не найдено.\n";
    }
}

void Laundry::customers() {
    struct Customers {
        std::string name;
        std::string text;
        std::function<void()> interaction; // функция взаимодействия
    };


    std::vector<Customers> customer = {
        {"👩‍🦰 Ольга", "Здравствуйте! У меня здесь бельё с отпуска песок - повсюду!",
            [this]() {
                print_name("💬Ольга: ");
                slowPrint("Может у вас есть режим 'пляжная стирка'? ");

                std::cout << termcolor::bright_grey << "К сожалению, нету...\n" << termcolor::reset;
                space_enter("К сожалению, нету...");
            }
        },
        {"🧔 Владимир", "",
            [this]() {
                if (!questFlags.get("has_found_sock_vladimir")) {
                    print_name("💬Владимир: ");
                    slowPrint("Надеюсь, сегодня машинка не съест ещё один носок...");

                    std::cout << termcolor::on_bright_magenta << "НоВыЙ кВеСт ПоЛуЧеН!" << termcolor::reset << std::endl;

                    std::string assigment = "💡 Найди второй носок владимира";
                    std::cout << assigment << std::endl;
                    add_assigm(assigment);
                    std::cout << termcolor::bright_grey << "Кажется пора все хорошо сложить.." << termcolor::reset << std::endl;

                    questFlags.set("sock_hunt");
                }
                else {
                    print_name("🧔 Владимир: ");
                    slowPrint("Сегодня без сюрпризов. Одежда чистая, настроение — тоже.");
                    std::cout << termcolor::bright_grey << "Будет сделано!" << termcolor::reset;
                    space_enter("Будет сделано!");
                }
            }
        },
        {"👧 София", "Мам, а можно стирать игрушки?🧸", [this]() {
            print_name("👧 София : ");
            slowPrint("У моего мишки грязная лапка. Ты сможешь его постирать? Он не боится!");

            std::string question;

            while (true) {
                std::cout << "да/нет: ";
                std::cin >> question;
                std::cout << std::endl;

                if (question == "нет") {
                    print_name("👧 София : ");
                    slowPrint("А он и так уже весь пыльный... Ну ладно. Я просто спрячу его под подушку.");
                    std::cout << "😢" << std::endl;
                    break;
                }
                else if (question == "да") {
                    questFlags.set("sofia_bear_given");
                    if (questFlags.get("sofia_bear_given")) {

                        questFlags.set("sofia_bear");

                        std::cout << termcolor::blue << "Получен медведь!" << termcolor::reset << std::endl;
                        break;
                    }
                }
                else {
                    std::cout << termcolor::red << "Неверный ввод!" << termcolor::reset << std::endl;
                }
            }

        }},
        {"🧑‍🔧 Игорь", "Привет! Кто-то опять забыл ключи от сушилки...", [this]() {

            print_name("🧑‍🔧 Игорь: ");
            slowPrint("Прачечная — как мини - галактика.Всё крутится, но ничего не понятно.\nЕсли что сломается — зови.Только не ночью, ладно? \nИ вот еще мои рабочие тряпки");

            questFlags.set("meet_igor");
        }},
        {"👵 Бабушка Зина", "Сначала всё кипятком, потом полоскать - как в старые добрые времена!", [this]() {
            if (questFlags.get("item_for_zina")) {
                print_name("👵 Бабушка Зина: ");
                slowPrint("А вы точно добавили уксус? Без него полотенца как наждак!");

                questFlags.set("item_for_zina");
                std::cout << termcolor::on_bright_magenta << "Новое задание" << termcolor::reset << " «Секретный рецепт бабушки Зины»" << std::endl;

                add_pro_assig("Собрать ингредиенты для бабушки Зины", { "Уксус", "Хозяйственное мыло", "Горячая вода" });
            }
            else {

            }

        }},
        {"", "", [this]() {
            if (!questFlags.get("degree_mark")) {
                print_name("📚 Марк : ");
                slowPrint("Если сдача диплома завтра, значит, сегодня — день большой стирки и большой паники!");
                questFlags.set("degree_mark");
            }
            else {
                print_name("📚 Марк: ");
                slowPrint("Говорят, если три раза постираешь правильно — получаешь степень бакалавра домашнего быта. Я на втором заходе.");

                std::cout << termcolor::bright_grey << "Как сдача диплома?" << termcolor::reset;
                space_enter("Как сдача диплома ? ");

                print_name("📚 Марк: ");
                slowPrint("Тема: «Влияние цикла отжима на концентрацию философской мысли в студенческой среде». Комиссия оценила.");

            }
        }},
        {"🎨 Анна", "Кто-то случайно не находил розовый платок в горошек? 🎀", [this]() {
            space_enter("Кажется, я видел его на полке с другими тканями. Ты проверила там?");
            print_name("🎨 Анна");
            Sleep(100);
            slowPrint("Ой, правда?! Спасибо! 😊✨ Я, наверное, его там пропустила… Пойду посмотрю! 🏃‍♀️💨");
        }},
        {"🐱 Кот Борис", "Мяу! Не трогайте мой плед, он пахнет мною! (≧◡≦)", [this]() {
            space_enter("Может, я помогу его постирать? Только аккуратно, обещаю!");
        }},
        {"🧘‍♂️ Алексей", "Стирать - это как медитация. Тепло, белый шум, аромат свежести...(－‿‿－)", [this]() {
            space_enter("Похоже, прачечная — новое место для медитации!");
        }},
        {"💃 Ника", "Пока бельё крутится, я пойду попрактикую бачату! (^-^)ノ", [this]() {
            space_enter("Звучит весело! Не забудь вернуться к стирке после танца.");
        }},
        {"👨‍💼 Артём", "Я пришёл строго по расписанию. Надеюсь, никто не занял мою машинку!", [this]() {
            space_enter("Не волнуйся, твоя машинка ждёт тебя. (o_o)");
            print_name("👨‍💼 Артём");
            slowPrint("Отлично, пунктуальность должна быть вознаграждена.(^_^)");
        }},
        {"🧕 Амина", "Вода пахнет жасмином? Или это мой кондиционер снова пролился...", [this]() {
            space_enter("Ароматы делают процесс приятнее — полезно замечать детали.");
            print_name("🧕 Амина");
            slowPrint("Точно! Даже в рутинных делах можно найти что-то красивое. (⌒‿⌒)");
        }},
        {"👨‍🍳 Павел", "Шеф-повар не может позволить себе грязный фартук!", [this]() {
            space_enter("Чистый фартук — чистые мысли. (¬‿¬)");
            print_name("👨‍🍳 Павел");
            slowPrint("Абсолютно! Внешний порядок помогает держать мысли ясными.");
        }},
        {"👩‍🎤 Лана", "Бельё постираю - и сразу на репетицию. Барабаны не ждут! (^-^)ノ🥁", [this]() {
            space_enter("Звучит, как будто музыка зовёт всех на праздник!");
            print_name("👩‍🎤 Лана");
            slowPrint("Именно! Хочешь присоединиться и почувствовать ритм?");
            std::string go_concert;
            std::cout << termcolor::green << "(да/нет)" << termcolor::reset;
            std::cin >> go_concert;
            while (true) {
                if (go_concert == "да") {
                    questFlags.set("concert");
                    print_name("👩‍🎤 Лана");
                    slowPrint("Отлично! Тогда приходи 3-го числа (^-^)ノ");
                    break;
                }
                else if (go_concert == "нет") {
                    print_name("👩‍🎤 Лана");
                    slowPrint("Наверное не все любят музыку как я...(._.`)");
                    break;
                }
                else {
                    std::cout << "ОЙ! Введи да или нет.." << std::endl;
                }
            }
        }}, 
        {"👨‍🌾 Николай", "Сначала навоз, потом стирка... Хорошо, что в этом порядке!", [this]() {
            space_enter("Звучит как идеальный баланс между трудом и чистотой.");
            print_name("👨‍🌾 Николай");
            slowPrint("Ага! После работы — порядок, и самому легче дышать.");
        }},
        {"👩‍💻 Вера", "Стираю ночью, чтобы не отвлекаться от кода. Надеюсь, соседи поймут...", [this]() {
            space_enter("Может, стоит написать скрипт, чтобы и стирка по расписанию запускалась?");
            print_name("👩‍💻 Вера");
            slowPrint("Хм, идея! Cron job на прачку… жаль, машинка не понимает bash. (￣▽￣)");
        }},
        {"🧙‍♂️ Станислав", "Я зачаровал носки от исчезновения. Теперь проверим, работает ли! (∩｀-´)⊃━☆ﾟ.*･｡", [this]() {
            space_enter("А если заклинание не сработает, будем искать по старинке. (－‿－)");
            std::cout << termcolor::magenta << "Квест: 🧦 «Заклинание против пропажи»" << termcolor::reset << std::endl;
            add_assigm("🧦 «Заклинание против пропажи»: Найти носка-беглеца");
            questFlags.set("Stanislav_socks");

            print_name("🧙‍♂️ Станислав");
            slowPrint("Мудро сказано. Даже магии нужна надёжная подстраховка. (¬‿¬)");
        }},
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
        //std::cout << clock.quality_customers_day << std::endl;

        std::cout << "Кто-то входит в прачечную...\n";

        std::cout << termcolor::yellow << c.name << ": " << termcolor::reset;
        if (!c.text.empty()) {
            slowPrint(c.text);
        }
        std::cout << std::endl;

        c.interaction();
    }

}

void Laundry::give_clothes() {
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
            space_enter("Хорошего дня! 🌞");

            if (fine == 0.0f) {
                salary = price_washing * amount_clothing_clean;
                questFlags.get("sofia_bear") ? salary += 1 : salary;  // тернарный оператор 
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

void Laundry::space_enter(std::string text) {
    std::string wish;
    std::cout << termcolor::blue << "(space + enter)" << termcolor::reset << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(std::cin, wish); // читаем строку с пробелами

    std::cout << std::endl;
    std::string user = "👤" + us_name + ": " + text;
    slowPrint(user);

    if (wish != " ") {
        std::cout << "Попробуй еще раз (сначало space, потом enter)" << std::endl;
        space_enter(text);
    }
}

void Laundry::process_washing() {
    std::cout << "Процесс стирки..." << std::endl;
    for (int i = 0; i < 10; ++i) {
        Sleep(1500);
        std::cout << "█ ";
    }
    if (questFlags.get("sofia_bear")) {
        std::cout << termcolor::green << "\nБонус от стирки медведя + 1$" << termcolor::reset << std::endl;
        questFlags.vlear("sofia_bear");

    }
    else if (questFlags.get("Stanislav_socks")) {
        std::cout << termcolor::blue << "В барабане только один носок.." << termcolor::reset << std::endl;

        print_name("🧙‍♂️ Станислав");
        slowPrint("Что?! Магия подвела?! Нужно найти беглеца!");

        std::string assignment = "Найти беглеца-носка";
        std::cout << termcolor::magenta << "Новый квест: " << assignment << termcolor::reset << std::endl;
        add_assigm(assignment);
        
        std::cout << termcolor::on_bright_grey << "Пора сложить все по полочкам" << termcolor::reset << std::endl;
    }
    else {
        std::cout << "\nОдежда постирана!" << std::endl;
        give_clothes();
    }
}

void Laundry::process_drying() {
    std::cout << "Процесс сушки..." << std::endl;
    for (int i = 0; i < 10; ++i) {
        Sleep(1500);                // !!!!добавить флаг когда появится возможность сушить чтобы доп. спрашивать у клиентов сушить или нет  чтобы они на рандом говорили да или нет
        std::cout << "▊ ";
    }
    std::cout << "\nОдежда высушена!" << std::endl;
}

void Laundry::DIY() {
    std::cout << "Требуется для починки 1$" << std::endl;

    std::string pay;

    std::cout << "[заплатить 1$]" << termcolor::blue << "(space + enter)" << termcolor::reset;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::getline(std::cin, pay);

    wallet -= 1;
    level_serviceability = 3;
    std::cout << termcolor::italic << "Заплачено 1$\nУровень исправности стиральной машины: " << level_serviceability << termcolor::reset << std::endl;
}

std::pair<std::map<int, std::string>, std::map<int, std::function<void()>>> Laundry::getAvailableActions() {
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
        if (amount_clothing_dirty == 0)
            std::cout << "У тебя нет что стирать!" << std::endl;
        if (level_serviceability == 0) {
            std::cout << termcolor::italic << "Стиральная машина сломана!" << termcolor::reset << std::endl;

            if (questFlags.get("meet_igor")) {
                while (true) {
                    int enter;
                    std::cout << "Хочешь починить самостоятельно (1) или позвонить Игорю (2): ";
                    std::cin >> enter;

                    if (enter == 1) {
                        DIY();
                        break;
                    }
                    else if (enter == 2) {
                        if (clock.isDaytime()) {
                            print_name("🧑‍🔧 Игорь: ");
                            slowPrint("Опять перегрузили! Думаете, если три носка — это ещё не стирка?");

                            std::cout << "Процесс ремонта..." << std::endl;
                            for (int i = 0; i < 3; ++i) {
                                Sleep(1000);
                                std::cout << "█ ";

                                print_name("\n🧑‍🔧 Игорь: ");
                                slowPrint("Виноват был трансмодульный перегиб с фрагментацией потока.Шутка.Там просто носок застрял.");
                                level_serviceability = 3;
                            }
                            break;
                        }
                        else {
                            std::cout << termcolor::bright_red << "🧑‍🔧 Игорь уже спит! Придётся как-то самому..." << termcolor::reset << std::endl;
                        }
                    }
                    else {
                        std::cout << termcolor::red << "Введите 1 или 2" << termcolor::reset << std::endl;
                    }
                }
            }
            else {
                DIY();
            }

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
        if (questFlags.get("sock_hunt")) {
            std::cout << termcolor::bright_cyan << "Носок найден!" << termcolor::reset << std::endl;
            //has_found_sock_vladimir = true;
            questFlags.set("has_found_sock_vladimir");
            questFlags.vlear("sock_hunt");

            std::cout << termcolor::bright_grey << "Отнести носок Владимиру" << termcolor::reset;
            space_enter("Вот ваш носок!");

            std::this_thread::sleep_for(std::chrono::seconds(1));
            print_name("🧔 Владимир: ");
            slowPrint("Я уже приготовился устраивать прощание. А тут — такой поворот!");
        }
        else if (questFlags.get("Stanislav_socks")) {
            std::cout << termcolor::bright_cyan << "Носок-беглец найден!" << termcolor::reset << std::endl;

            std::cout << "Отдать носка-беглеца: ";
            space_enter("А вот и носочек! Он прилип к футболке..");

            print_name("🧙‍♂️ Станислав");
            slowPrint("Ура! Значит, магия работает… просто машинка хитрая! (¬‿¬)");

            level_serviceability = 4;
            std::cout << std::endl << termcolor::bright_magenta << "Вы получате бонус для стиральной машинки: 4 исправность!" << termcolor::reset << std::endl ;

        }
        else {
            std::cout << "Все сложено идеально!" << std::endl;
            clock.advanceTime(0, 30);
        }
        };

    activities[nextIndex] = "Скоротать время 😮‍💨";
    actions[nextIndex++] = [this]() {
        clock.advanceTime(3, 0);
        };

    activities[nextIndex] = "Посмотреть задания 💡";
    actions[nextIndex++] = [this]() {

        if (assignments.empty()) {
            std::cout << "🧺 — Нет заданий в корзине" << std::endl;
        }
        else {
            print_Assigm();
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
            village->message_village();
            village->list_activity_village();
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

    return { activities, actions };

}

void Laundry::list_activity() {
    bool running = true;
    while (running) {
        special_events();
        clock.showTime();
        //clock.getCustomerCount();

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

void Laundry::special_events() {
    switch (clock.day) {
    case 5: {
        if (!questFlags.get("has_met_vanessa")) {
            std::cout << termcolor::bright_blue << "👵 Ванесса: " << termcolor::reset;
            std::string message = "Привет, " + us_name + ", я тут убиралась на чердаке и нашла старый фотоапарат.\nМожет пригодится!";
            slowPrint(message, 40);

            std::cout << "\nПолучен " << termcolor::bright_cyan << "[фотоапарат]\n" << termcolor::reset << std::endl;
            iscamera = true;

            Sleep(100);
            questFlags.set("has_met_vanessa");
            std::cout << termcolor::italic << "Теперь тебе доступен мир орнитологии\n" << termcolor::reset;
        }
        break;
    }
    case 2: {
        if (!questFlags.get("has_met_mustafa")) {
            std::cout << termcolor::bright_blue << "👴 Мустафа: " << termcolor::reset;
            std::string message = "Здравствуй! Вижу, ты у нас впервые. Наша деревня Каталин хоть и мала, да уютом славится.\nЗагляни в гости — поведаю тебе удивительные истории о здешних местах, а моя Ванесса пирог испечёт к чаю.\n"
                "Живём мы неподалёку, так что ещё обязательно встретимся!\n";
            slowPrint(message);

            Sleep(100);
            is_village = true;
            std::cout << termcolor::italic << "Открыта деревня\n" << termcolor::reset;
            questFlags.set("has_met_mustafa");

        }
        break;
    }
    case 3: {
        if (questFlags.get("concert")) {
            // сделать концерт
        }
        break;
    }
    case 7: {
        if (!questFlags.get("has_met_ayzuk")) {
            std::cout << termcolor::bright_blue << "👦 Айзик: " << termcolor::reset;
            std::string message = "Эээ... здрасьте! У меня тут, ну... очень деликатная проблема. Только маме ни слова, ладно?\n"
                "Короче, мне срочно надо постирать... двойку из дневника! В журнале 'Мастер на все руки' написано, что это сработает. Ну я и подумал — а вдруг?";
            slowPrint(message, 50);

            std::string diary;
            std::cout << "\n[Взять дневник]" << termcolor::blue << "(space + enter)" << termcolor::reset << std::endl;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::getline(std::cin, diary);

            questFlags.set("has_met_ayzuk");
            std::cout << termcolor::italic << "\nПолучен дневник Айзика" << termcolor::reset << std::endl;

            process_washing();

            std::cout << termcolor::bright_blue << "\n👦 Айзик: " << termcolor::reset;
            std::string message1 = "\nОго! Получилось?! Ты волшебница, честное слово!\n"
                "Теперь мама точно не узнает... Наверное. Спасибо тебе огромное!\n"
                "Если что — я в долгу не останусь. У меня ещё кое-что надо... эээ... починить.";
            slowPrint(message1);
            std::cout << std::endl;
            space_enter("Хорошего дня! 🌞");
        }
    }
    }
}

void Laundry::code_for_washing() {
    int number_errors = 0;

    while (number_errors < 2) {
        std::cout << "Введи код для стиральной машины: ";
        int code;
        std::cin >> code;

        if (code == 1010) {
            if (questFlags.get("sofia_bear")) {
                std::cout << termcolor::green << "Мишка в стирке 🧸" << termcolor::reset << std::endl;
            }
            else if (questFlags.get("Stanislav_socks")) {
                std::cout << termcolor::blue << "Два носка уже в барабане!" << termcolor::reset << std::endl;
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
            while (true) {
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

