#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm> 

class Assignment {
public:
    std::string description;
    std::vector<std::string> subTasks;

    Assignment();
    Assignment(const std::string& desc);
    Assignment(const std::string& desc, const std::vector<std::string>& subs);

    void print() const;
    void completeSubTask(const std::string& item);
};
