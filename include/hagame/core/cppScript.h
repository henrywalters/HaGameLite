//
// Created by henry on 12/15/23.
//

#ifndef HAGAME2_CPPSCRIPT_H
#define HAGAME2_CPPSCRIPT_H

#include "script.h"

namespace hg {
    class CppScript;

    /*
     * The ScriptWrapper is a wrapper of a shared library that contains one or more Script Files which contain an init and update function
    */
    class CppLibraryWrapper {
    public:

        struct Script {
            typedef void (*script_init)(hg::Scene*);
            typedef void (*script_update)(hg::Scene*, double dt);
            typedef void (*script_close)(hg::Scene*);

            ScriptDef def;

            script_init init;
            script_update update;
            script_close close;
        };

        CppLibraryWrapper(std::string libPath);

        HG_GET(std::string, libPath)

        void reload();

        // register the script from the file path
        void registerScript(std::string path);

        std::vector<ScriptDef> scriptDefs() const { return m_scriptDefs; }

        Script* get(std::string name);

    private:

        void* m_lib = nullptr;
        std::string m_libPath;
        std::string m_path;

        std::vector<ScriptDef> m_scriptDefs;
        std::unordered_map<std::string, std::shared_ptr<Script>> m_scripts;

        template <typename T>
        T loadFunction(std::string name);
    };

    // The CppLibraryManager statically manages a set of CppLibraryWrappers so it may be be used in the Script Factory
    class CppLibraryManager {
    public:

        using lib_map_t = std::unordered_map<std::string, std::unique_ptr<CppLibraryWrapper>>;

        static CppLibraryWrapper* Register(std::string libraryPath);

        static bool Has(std::string libraryPath);

        static CppLibraryWrapper* Get(std::string libraryPath);

        static std::vector<CppLibraryWrapper*> All();

    private:

        static lib_map_t& LibMap();

        static std::unique_ptr<lib_map_t> s_libraries;

    };

    class CppScript : public Script {
    public:

        CppScript(CppLibraryWrapper::Script* script);

        ScriptDef getDef() const override;

    protected:

        void onInit() override;
        void onUpdate(double dt) override;
        void onClose() override;

    private:

        CppLibraryWrapper::Script* m_script;

    };
}

#endif //HAGAME2_CPPSCRIPT_H
