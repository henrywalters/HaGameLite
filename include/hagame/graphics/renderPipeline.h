//
// Created by henry on 1/8/23.
//

#ifndef HAGAME2_RENDERPIPELINE_H
#define HAGAME2_RENDERPIPELINE_H

namespace hg::graphics {
    class RenderPipeline {
    public:

        virtual void onBeforeRender() {}
        virtual void onRender() {}
        virtual void onAfterRender() {}
    };
}

#endif //HAGAME2_RENDERPIPELINE_H
