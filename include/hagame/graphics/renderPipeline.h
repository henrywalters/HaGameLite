//
// Created by henry on 1/8/23.
//

#ifndef HAGAME2_RENDERPIPELINE_H
#define HAGAME2_RENDERPIPELINE_H

#include "rawTexture.h"

namespace hg {
    class Scene;
}

namespace hg::graphics {

    class RenderPipeline {
    public:

        virtual void clear() {}
        virtual void render(Vec2i size) {}
        virtual void cleanup() {}

        virtual RawTexture<GL_RGBA>* outputTexture() {
            return nullptr;
        }

        void scene(Scene* _scene) {
            m_scene = _scene;
        }

        Scene* scene() const {
            return m_scene;
        }

    private:

        Scene* m_scene;

    };
}

#endif //HAGAME2_RENDERPIPELINE_H
