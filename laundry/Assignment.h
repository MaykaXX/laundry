#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm> 

class Assignment {
private:
    std::string description;
    std::vector<std::string> subTasks;
public:

    Assignment();
    Assignment(const std::string& desc);
    Assignment(const std::string& desc, const std::vector<std::string>& subs);

    void print() const;
    void completeSubTask(const std::string& item);

    std::string getDescription() const { return description; }

};
