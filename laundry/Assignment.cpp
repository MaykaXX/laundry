#include "Assignment.h"

// Конструкторы
Assignment::Assignment() = default;

Assignment::Assignment(const std::string& desc) : description(desc) {}

Assignment::Assignment(const std::string& desc, const std::vector<std::string>& subs)
    : description(desc), subTasks(subs) {
}


void Assignment::print() const {
    std::cout << description << std::endl;
    if (!subTasks.empty()) {
        for (const auto& sub : subTasks) {
            std::cout << "    - " << sub << std::endl;
        }
    }
}

void Assignment::completeSubTask(const std::string& item) {
    auto it = std::find(subTasks.begin(), subTasks.end(), item);
    if (it != subTasks.end()) {
        subTasks.erase(it);
        std::cout << "Подзадача \"" << item << "\" выполнена и удалена!\n";
    }
    else {
        std::cout << "Такой подзадачи нет.\n";
    }
}
