//
// Created by henry on 12/15/23.
//
#include "../../../include/hagame/core/script.h"
#include "../../../include/hagame/core/cppScript.h"

using namespace hg;

std::shared_ptr<Script> ScriptFactory::Create(hg::ScriptDef def) {
    if (def.extension == ".cpp") {
        if (!CppLibraryManager::Has(def.libPath)) {
            throw std::runtime_error("Library path does not exist: " + def.libPath + ". Have you Registered it?");
        }
        return std::make_shared<CppScript>(CppLibraryManager::Get(def.libPath)->get(def.name));
    }

    throw std::runtime_error("Unsupported extension: " + def.extension);
}
