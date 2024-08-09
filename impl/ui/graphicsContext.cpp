//
// Created by henry on 8/7/24.
//
#include "../../../include/hagame/ui/graphicsContext.h"
#include "../../../include/hagame/graphics/shaders/text.h"
#include "../../../include/hagame/graphics/shaders/color.h"
#include "../../../include/hagame/graphics/shaders/texture.h"

using namespace hg::graphics;

hg::ui::GraphicsContext::GraphicsContext() :
        colorShader(COLOR_SHADER.name, Shader::LoadVertex(COLOR_SHADER.vertexSource), Shader::LoadFragment(COLOR_SHADER.fragmentSource)),
        textShader(TEXT_SHADER.name, Shader::LoadVertex(TEXT_SHADER.vertexSource), Shader::LoadFragment(TEXT_SHADER.fragmentSource)),
        textBufferShader(TEXT_BUFFER_SHADER.name, Shader::LoadVertex(TEXT_BUFFER_SHADER.vertexSource), Shader::LoadFragment(TEXT_BUFFER_SHADER.fragmentSource)),
        textureShader(TEXTURE_SHADER.name, Shader::LoadVertex(TEXTURE_SHADER.vertexSource), Shader::LoadFragment(TEXTURE_SHADER.fragmentSource))
{}