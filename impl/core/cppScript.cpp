//
// Created by henry on 12/15/23.
//

#include "../../../include/hagame/core/cppScript.h"

using namespace hg;

void CppScriptManager::registerScript(std::string path) {
    hg::utils::FileParts parts = utils::f_getParts(path);
    ScriptDef def;
    def.name = parts.name;
    def.path = path;
    m_scriptDefs.push_back(def);

    auto script = std::make_shared<Script>();
    script->def = def;
    script->init = loadFunction<Script::script_init>(def.name + "_init");
    script->update = loadFunction<Script::script_update>(def.name + "_update");
    script->close = loadFunction<Script::script_close>(def.name + "_close");

    m_scripts.insert(std::make_pair(def.name, script));
}

CppScriptManager::CppScriptManager(std::string libPath):
        m_path(libPath)
{
    reload();
}

void CppScriptManager::reload() {

    if (m_lib) {
        dlclose(m_lib);
    }

    m_lib = dlopen(m_path.c_str(), RTLD_NOW);
    if (m_lib == nullptr) {
        std::cout << dlerror() << "\n";
        throw std::runtime_error("Failed to open library or dll");
    }

    for (const auto &[name, script]: m_scripts) {
        script->init = loadFunction<Script::script_init>(name + "_init");
        script->update = loadFunction<Script::script_update>(name + "_update");
    }
}

CppScriptManager::Script *CppScriptManager::get(std::string name) {
    return m_scripts[name].get();
}

template<typename T>
T CppScriptManager::loadFunction(std::string name) {
    std::cout << "LOADING FUNCTION: " << name << "\n";
    return reinterpret_cast<T>(dlsym(m_lib, name.c_str()));
}

CppScript::CppScript(CppScriptManager::Script* script):
        m_script(script)
{}

void CppScript::onInit() {
    m_script->init(m_scene);
}

void CppScript::onUpdate(double dt) {
    m_script->update(m_scene, dt);
}

void CppScript::onClose() {
    m_script->close(m_scene);
}