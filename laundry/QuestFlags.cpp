#include "QuestFlags.h"

// ��������� �����
void QuestFlags::set(const std::string& name, bool value) {
    flags[name] = value;
}

// ��������� �������� �����
bool QuestFlags::get(const std::string& name) const {
    auto it = flags.find(name);
    return it != flags.end() ? it->second : false;
}

// �������� �����
void QuestFlags::vlear(const std::string& name) {
    flags.erase(name);
}

// ������� ���� ������
void QuestFlags::reset() {
    flags.clear();
}
