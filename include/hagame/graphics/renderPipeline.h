//
// Created by henry on 1/8/23.
//

#ifndef HAGAME2_RENDERPIPELINE_H
#define HAGAME2_RENDERPIPELINE_H

namespace hg::graphics {

    class Scene;

    class RenderPipeline {
    public:

        virtual void onBeforeRender() {}
        virtual void onRender() {}
        virtual void onAfterRender() {}

        HG_GET(Scene*, scene);

    private:

        Scene* m_scene;

    };
}

#endif //HAGAME2_RENDERPIPELINE_H
