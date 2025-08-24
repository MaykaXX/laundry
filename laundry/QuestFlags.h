#pragma once
#include <unordered_map>
#include <string>


class QuestFlags {
private:
    std::unordered_map<std::string, bool> flags;
public:
    // установка флага
    void set(const std::string& name, bool value = true);
    bool get(const std::string& name) const;
    void vlear(const std::string& name);
    void reset();
};