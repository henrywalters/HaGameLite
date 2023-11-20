//
// Created by henry on 11/12/23.
//

#ifndef HAGAME2_PARTICLE_SHADER_H
#define HAGAME2_PARTICLE_SHADER_H
#include "../shader.h"

namespace hg::graphics {
    const ShaderSource PARTICLE_SHADER {
        "particle",
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "layout (location = 0) in vec3 a_vertex;\n"
        "layout (location = 1) in vec3 a_normal;\n"
        "layout (location = 2) in vec2 a_texture;\n"
        "layout (location = 3) in vec3 a_pos;\n"
        "layout (location = 4) in vec3 a_velocity;\n"
        "layout (location = 5) in vec4 a_startColor;\n"
        "layout (location = 6) in vec4 a_endColor;\n"
        "layout (location = 7) in float a_startTime;\n"
        "layout (location = 8) in float a_aliveFor;\n"
        "layout (location = 9) in vec3 a_gravity;\n"
        "layout (location = 10) in float a_scale;\n"
        "layout (location = 11) in int a_initialized;\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "uniform float currentTime;\n"
        "\n"
        "uniform vec3 position;\n"
        "uniform bool positionRelative;\n"
        "\n"
        "out float AliveFor;\n"
        "out float ElapsedTime;\n"
        "out vec2 TexCoord;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out vec4 StartColor;\n"
        "out vec4 EndColor;\n"
        "\n"
        "void main() {\n"
        "\n"
        "    ElapsedTime = currentTime - a_startTime;\n"
        "    AliveFor = a_aliveFor;\n"
        "\n"
        "    vec3 Velocity = a_velocity + a_gravity * ElapsedTime;\n"
        "    vec3 particlePosition = a_pos + Velocity * ElapsedTime;\n"
        "\n"
        "    if (positionRelative) {\n"
        "        particlePosition += position;\n"
        "    }\n"
        "\n"
        "    gl_Position = projection * view * model * vec4(particlePosition + vec3(a_vertex.x * a_scale, a_vertex.y * a_scale, a_vertex.z * a_scale), 1.0);\n"
        "    Normal = a_normal;\n"
        "    FragPos = vec3(gl_Position);\n"
        "    StartColor = a_startColor;\n"
        "    EndColor = a_endColor;\n"
        "    TexCoord = a_texture;\n"
        "\n"
        "    if (a_initialized == 0) {\n"
        "        StartColor = vec4(0, 0, 0, 0);\n"
        "    }\n"
        "}",
        "#version 300 es\n"
        "\n"
        "precision highp float;\n"
        "\n"
        "in float AliveFor;\n"
        "in float ElapsedTime;\n"
        "in vec2 TexCoord;\n"
        "in vec3 FragPos;\n"
        "in vec4 StartColor;\n"
        "in vec4 EndColor;\n"
        "\n"
        "uniform sampler2D image;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "vec3 rgb2hsv(vec3 c)\n"
        "{\n"
        "    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n"
        "    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));\n"
        "    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));\n"
        "\n"
        "    float d = q.x - min(q.w, q.y);\n"
        "    float e = 0.0000000001;\n"
        "    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);\n"
        "}\n"
        "\n"
        "vec3 hsv2rgb(vec3 c)\n"
        "{\n"
        "    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
        "    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n"
        "    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n"
        "}\n"
        "\n"
        "void main() {\n"
        "\n"
        "    if (ElapsedTime >= AliveFor) {\n"
        "        FragColor = vec4(0, 0, 0, 0);\n"
        "    } else {\n"
        "\n"
        "        float t = ElapsedTime / AliveFor;\n"
        "\n"
        "        float a0 = StartColor.a;\n"
        "        float a1 = EndColor.a;\n"
        "        float a = a0 + (a1 - a0) * t;\n"
        "\n"
        "        vec3 hsv0 = rgb2hsv(StartColor.rgb);\n"
        "        vec3 hsv1 = rgb2hsv(EndColor.rgb);\n"
        "\n"
        "        vec3 rgb = hsv2rgb(hsv0 + (hsv1 - hsv0) * t);\n"
        "\n"
        "        FragColor = vec4(rgb, a);\n"
        "    }\n"
        "\n"
        "}"
    };
}
#endif //HAGAME2_PARTICLE_H