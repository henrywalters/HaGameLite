//
// Created by henry on 11/6/23.
//

#ifndef HAGAME2_TEXTURE_SHADER_H
#define HAGAME2_TEXTURE_SHADER_H

#include "../shader.h"

namespace hg::graphics {
    const ShaderSource TEXTURE_SHADER {
        "texture",
        "#version 420\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "layout (location = 0) in vec3 a_vertex;\n"
        "layout (location = 1) in vec3 a_normal;\n"
        "layout (location = 2) in vec2 a_texture;\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = projection * view * model * vec4(a_vertex, 1.0);\n"
        "    Normal = a_normal;\n"
        "    FragPos = vec3(gl_Position);\n"
        "    TexCoord = a_texture;\n"
        "}",
        "#version 420\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "in vec3 FragPos;\n"
        "\n"
        "uniform sampler2D image;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main() {\n"
        "    FragColor = texture(image, TexCoord);\n"
        "}"
    };
}

#endif //HAGAME2_TEXTURE_H
