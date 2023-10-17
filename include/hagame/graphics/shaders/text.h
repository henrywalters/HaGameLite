//
// Created by henry on 9/27/23.
//

#ifndef HAGAME2_TEXT_SHADER_H
#define HAGAME2_TEXT_SHADER_H

#include "../shader.h"

namespace hg::graphics {
    const ShaderSource TEXT_SHADER {
        "text",
        // VERTEX
        "#version 300 es\n"

        "precision highp float;\n"

        "layout (location = 0) in vec3 a_vertex;\n"
        "layout (location = 1) in vec3 a_normal;\n"
        "layout (location = 2) in vec2 a_texture;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "uniform vec4 textColor;\n"

        "out vec2 TexCoord;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out vec4 TextColor;\n"

        "void main() {\n"
        "    gl_Position = projection * view * model *  vec4(a_vertex, 1.0);\n"
        "    Normal = a_normal;\n"
        "    FragPos = vec3(gl_Position);\n"
        "    TexCoord = a_texture;\n"
        "    TextColor = textColor;\n"
        "}",
        // FRAGMENT
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "in vec3 FragPos;\n"
        "in vec4 TextColor;\n"
        "\n"
        "uniform sampler2D image;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main() {\n"
        "    FragColor = vec4(TextColor.rgb, texture(image, TexCoord).r);\n"
        "}"
    };

    const ShaderSource TEXT_BUFFER_SHADER {
            "text_buffer",
            // VERTEX
            "#version 300 es\n"

            "precision highp float;\n"

            "layout (location = 0) in vec3 a_vertex;\n"
            "layout (location = 1) in vec3 a_normal;\n"
            "layout (location = 2) in vec2 a_texture;\n"
            "layout (location = 3) in vec3 a_position;\n"

            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"

            "uniform vec4 textColor;\n"

            "out vec2 TexCoord;\n"
            "out vec3 Normal;\n"
            "out vec3 FragPos;\n"
            "out vec4 TextColor;\n"

            "void main() {\n"
            "    gl_Position = projection * view * model *  vec4(a_vertex + a_position, 1.0);\n"
            "    Normal = a_normal;\n"
            "    FragPos = vec3(gl_Position);\n"
            "    TexCoord = a_texture;\n"
            "    TextColor = textColor;\n"
            "}",
            // FRAGMENT
            "#version 300 es\n"
            "\n"
            "precision highp float;\n"
            "\n"
            "in vec2 TexCoord;\n"
            "in vec3 FragPos;\n"
            "in vec4 TextColor;\n"
            "\n"
            "uniform sampler2D image;\n"
            "\n"
            "out vec4 FragColor;\n"
            "\n"
            "void main() {\n"
            "    FragColor = vec4(TextColor.rgb, texture(image, TexCoord).r);\n"
            "}"
    };
}

#endif //HAGAME2_TEXT_H
