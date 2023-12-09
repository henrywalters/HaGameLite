//
// Created by henry on 2/11/23.
//

#ifndef HAGAME2_SCRIPT_H
#define HAGAME2_SCRIPT_H

#include <dlfcn.h>
#include "object.h"

namespace hg {

    class Scene;

    class Script : public Object {
    public:

        Script() {}
        Script(hg::Scene* scene):
            m_scene(scene)
        {}

        void scene(hg::Scene* scene) {
            m_scene = scene;
        }

        hg::Scene* scene() const {
            return m_scene;
        }

        void init() {
            onInit();
        }

        void update(double dt) {
            onUpdate(dt);
        }

        void close() {
            onClose();
        }

    protected:

        OBJECT_NAME(Script)

        virtual void onInit() {}
        virtual void onUpdate(double dt) {}
        virtual void onClose() {}

        hg::Scene* m_scene;

    private:



    };

    class CppScriptWrapper : public Script {
    public:

        CppScriptWrapper(std::string name, std::string path):
            m_name(name),
            m_path(path)
        {
            m_lib = dlopen(path.c_str(), RTLD_NOW);
            if (m_lib == nullptr) {
                throw std::runtime_error("Failed to open library or dll");
            }
            m_construct = reinterpret_cast<script_ctr>(dlsym(m_lib, ("construct_" + name).c_str()));
            m_init = reinterpret_cast<script_fn>(dlsym(m_lib, ("init_" + name).c_str()));
            m_update = reinterpret_cast<script_fn_double>(dlsym(m_lib, ("update_" + name).c_str()));
            m_close = reinterpret_cast<script_fn>(dlsym(m_lib, ("close_" + name).c_str()));
        }

        void construct(hg::Scene* scene) {
            m_instanceId = m_construct(scene);
        }

    protected:

        void onInit() override {
            m_init(m_instanceId);
        }

        void onUpdate(double dt) override {
            m_update(m_instanceId, dt);
        }

        void onClose() override {
            m_close(m_instanceId);
        }

        std::string toString() const override {
            return m_name;
        }

    private:
        typedef utils::uuid_t (*script_ctr)(hg::Scene* scene);
        typedef void (*script_fn)(utils::uuid_t);
        typedef void (*script_fn_double)(utils::uuid_t, double value);

        void* m_lib;

        utils::uuid_t m_instanceId;

        std::string m_name;
        std::string m_path;

        script_ctr m_construct;
        script_fn m_init;
        script_fn m_close;
        script_fn_double m_update;
    };
}

#endif //HAGAME2_SCRIPT_H
