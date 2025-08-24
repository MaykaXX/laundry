#include "QuestFlags.h"

// Установка флага
void QuestFlags::set(const std::string& name, bool value) {
    flags[name] = value;
}

// Получение значения флага
bool QuestFlags::get(const std::string& name) const {
    auto it = flags.find(name);
    return it != flags.end() ? it->second : false;
}

// Удаление флага
void QuestFlags::vlear(const std::string& name) {
    flags.erase(name);
}

// Очистка всех флагов
void QuestFlags::reset() {
    flags.clear();
}
