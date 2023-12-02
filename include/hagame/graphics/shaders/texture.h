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
        "uniform float useLighting;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out float UseLighting;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = projection * view * model * vec4(a_vertex, 1.0);\n"
        "    Normal = a_normal;\n"
        "    FragPos = vec3(gl_Position);\n"
        "    TexCoord = a_texture;\n"
        "    UseLighting = useLighting;\n"
        "}",
        "#version 420\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "in vec2 TexCoord;\n"
        "in vec3 FragPos;\n"
        "in float UseLighting;\n"
        "\n"
        "layout (binding = 0) uniform sampler2D image;\n"
        "layout (binding = 1) uniform sampler2D lighting;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main() {\n"
        "    vec4 albedo = texture(image, TexCoord);\n"
        "    vec4 lighting = texture(lighting, TexCoord);\n"
        "    float min_light = 0.05;\n"
        "    float r = UseLighting == 1.0 ? clamp(lighting.r, min_light, 1.0) : 1.0;\n"
        "    float g = UseLighting == 1.0 ? clamp(lighting.g, min_light, 1.0) : 1.0;\n"
        "    float b = UseLighting == 1.0 ? clamp(lighting.b, min_light, 1.0) : 1.0;\n"
        "    FragColor = vec4(albedo.r * r, albedo.g * g, albedo.b * b, albedo.a);\n"
        "}"
    };
}

#endif //HAGAME2_TEXTURE_H
