//
// Created by henry on 8/7/24.
//

#ifndef HAGAME2_GRAPHICSCONTEXT_H
#define HAGAME2_GRAPHICSCONTEXT_H

#include "../graphics/shaderProgram.h"
#include "../graphics/batchRenderer.h"

namespace hg::ui {
    struct GraphicsContext {
        graphics::ShaderProgram textShader;
        graphics::ShaderProgram textBufferShader;
        graphics::ShaderProgram textureShader;
        graphics::ShaderProgram colorShader;
        graphics::BatchRenderer renderer;

        GraphicsContext();
    };

}

#endif //HAGAME2_GRAPHICSCONTEXT_H
