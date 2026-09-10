//
// Created by henry on 1/28/24.
//
#include "../../../include/hagame/utils/enum.h"

using namespace hg::utils;

std::vector<EnumElement> *hg::utils::GetEnum(uuid_t id) {
    static std::unordered_map<uuid_t, std::vector<EnumElement>> Enums;
    if (Enums.find(id) == Enums.end()) {
        Enums.insert(std::make_pair(id, std::vector<EnumElement>()));
    }
    return &Enums.at(id);
}

std::string hg::utils::GetEnumLabel(hg::utils::uuid_t enumId, hg::utils::enum_t value) {
    for (const auto& e : *GetEnum(enumId)) {
        if (e.key == value) return e.label;
    }

    throw std::runtime_error("Invalid Enum value");
}

EnumElement hg::utils::RegisterValue(uuid_t enumId, std::string label, int offset) {
    auto enums = GetEnum(enumId);
    EnumElement element(label);
    hg::utils::enum_t key;
    if (enums->size() > 0) {
        key = (*enums)[enums->size() - 1].key + 1;
    } else {
        key = 0;
    }

    key += offset;

    element.key = key;
    enums->push_back(element);
    return element;
}
