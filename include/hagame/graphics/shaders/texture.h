//
// Created by henry on 11/6/23.
//

#ifndef HAGAME2_TEXTURE_SHADER_H
#define HAGAME2_TEXTURE_SHADER_H

#include "../shader.h"

namespace hg::graphics {

    inline const std::string _textureAt(int index) {
        return "case " + std::to_string(index) + ":\n" + "FragColor=texture(images[" + std::to_string(index) + "], TexCoord) * Color;\nbreak;\n";
    }

    inline const std::string _texturesAt(int start, int end) {
        std::string out;
        for (int i = start; i < end; i++) {
            out += _textureAt(i);
        }
        return out;
    }

    const int MAX_BATCH_TEXTURES = 16;

    const ShaderSource TEXTURE_SHADER {
        "texture",
        "#version 300 es\n"
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
        "#version 300 es\n"
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

    const ShaderSource BATCH_TEXTURE_SHADER {
        "batch_texture",
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "layout (location = 0) in vec3 a_vertex;\n"
        "layout (location = 1) in vec3 a_normal;\n"
        "layout (location = 2) in vec2 a_texture;\n"
        "layout (location = 3) in vec2 a_size;\n"
        "layout (location = 4) in vec2 a_offset;\n"
        "layout (location = 5) in vec2 a_texOffset;\n"
        "layout (location = 6) in vec2 a_texSize;\n"
        "layout (location = 7) in vec4 a_color;\n"
        "layout (location = 8) in float a_textureIndex;\n"
        "layout (location = 9) in vec4 a_model0;\n"
        "layout (location = 10) in vec4 a_model1;\n"
        "layout (location = 11) in vec4 a_model2;\n"
        "layout (location = 12) in vec4 a_model3;\n"
        "\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out vec4 Color;\n"
        "out float Texture;\n"
        "\n"
        "void main() {\n"
        "\n"
        "    mat4 model = mat4(1.0);\n"
        "    model[0] = a_model0;\n"
        "    model[1] = a_model1;\n"
        "    model[2] = a_model2;\n"
        "    model[3] = a_model3;\n"
        "\n"
        "    gl_Position = projection * view * transpose(model) * vec4(a_vertex.xy * a_size + a_offset, 0.0, 1.0);\n"
        "    Normal = a_normal;\n"
        "    FragPos = vec3(gl_Position);\n"
        "    Color = a_color;\n"
        "    TexCoord = vec2(a_texOffset.x + a_texSize.x * a_texture.x, a_texOffset.y + a_texSize.y * a_texture.y);\n"
        "    Texture = a_textureIndex; \n"
        "}",
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "in vec3 FragPos;\n"
        "in vec4 Color;\n"
        "in float Texture;\n"
        "\n"
        "uniform sampler2D images[" + std::to_string(MAX_BATCH_TEXTURES) + "];\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main() {\n"
        "    int tid = int(Texture);\n"
//        "    for (int i = 0; i < 32; i++) {\n"
//        "       if (i == int(tid)) { \n"
//        "           FragColor=texture(images[i], TexCoord) * Color;\n"
//        "       }\n"
//        "    }\n"
        "    switch (tid) {\n"
        + _texturesAt(0, MAX_BATCH_TEXTURES) +
        "    }\n"
        "}"
    };
}

#endif //HAGAME2_TEXTURE_H
