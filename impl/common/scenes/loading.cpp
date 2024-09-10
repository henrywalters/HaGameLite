//
// Created by henry on 6/22/24.
//
#include "../../../../include/hagame/common/scenes/loading.h"
#include "../../../include/hagame/core/assets.h"
#include "../../../include/hagame/graphics/shaders/color.h"
#include "../../../include/hagame/graphics/shaders/texture.h"
#include "../../../include/hagame/graphics/shaders/text.h"
#include "../../../include/hagame/graphics/shaders/particle.h"
#include "../../../include/hagame/graphics/windows.h"

using namespace hg;
using namespace hg::graphics;
using namespace hg::utils;

Loading::Loading(graphics::Window *window, Loading::Settings settings):
    m_settings(settings),
    m_window(window),
    m_quad(window->size().cast<float>(), Vec2(0, 0), true),
    m_mesh(&m_quad),
    m_logoQuad(Vec2(window->size()[0] / 2, window->size()[0] / 8), Vec2(window->size()[0] / 2, window->size()[1] / 2)),
    m_logo(&m_logoQuad)
{
    m_quad.centered(false);
    m_mesh.update(&m_quad);

    m_logoQuad.centered();
    m_logo.update(&m_logoQuad);
}

void Loading::onInit() {
    m_renderPasses.create(RenderMode::Color, m_window->size());

    loadDefaultShaders();
    readFilesForLoading();

    auto defaultFont = loadFont(m_settings.font, ASSET_DIR + m_settings.folders.fonts + "/" + m_settings.font + ".ttf");
    defaultFont->fontSize(16);

    loadTexture(m_settings.logo, ASSET_DIR + m_settings.folders.textures + "/" + m_settings.logo + ".png");

    m_messageBuffer = TextBuffer(defaultFont.get(), m_message, Vec3(0, 0, 0), TextHAlignment::Left);
    m_versionBuffer = TextBuffer(defaultFont.get(), m_settings.version, Vec3(0, m_window->size().y() - 16, 0), TextHAlignment::Left);

    resize();

    Windows::Events.subscribe(WindowEvents::Resize, [&](Window* window) {
        resize();
    });
}

void Loading::onUpdate(double dt) {

    m_elapsedTime += dt;

    m_window->color(Color::black());
    m_renderPasses.bind(RenderMode::Color);
    m_renderPasses.clear(RenderMode::Color, Color::black());

    if (m_shaderIdx < m_shaders.size()) {
        auto &[name, paths] = m_shaders[m_shaderIdx++];
        m_message = "Loading shader: " + name + " from " + paths.vertex + " & " + paths.fragment;
        std::cout << m_message << "\n";
        hg::loadShader(name, paths.vertex, paths.fragment);
    } else if (m_spriteSheetIdx < m_spriteSheets.size()) {
        auto &[name, path] = m_spriteSheets[m_spriteSheetIdx++];
        m_message = "Loading sprite sheet: " + name + " from " + path;
        std::cout << m_message << "\n";
        hg::loadSpriteSheet(name, path);
    } else if (m_textureIdx < m_textures.size()) {
        auto &[name, path] = m_textures[m_textureIdx++];
        m_message = "Loading texture: " + name + " from " + path;
        std::cout << m_message << "\n";
        hg::loadTexture(name, path);
    } else if (m_soundIdx < m_sounds.size()) {
        auto &[name, path] = m_sounds[m_soundIdx++];
        m_message = "Loading sound: " + name + " from " + path;
        std::cout << m_message << "\n";
        hg::loadAudioStream(name, path);
    } else if (m_fontIdx < m_fonts.size()) {
        auto &[name, path] = m_fonts[m_fontIdx++];
        m_message = "Loading font: " + name + " from " + path;
        std::cout << m_message << "\n";
        hg::loadFont(name, path);
    } else if (m_materialIdx < m_materials.size()) {
        auto &[name, path] = m_materials[m_materialIdx++];
        m_message = "Loading material: " + name + " from " + path;
        std::cout << m_message << "\n";
        hg::loadMaterials(name, path);
    } else if (m_meshIdx < m_meshes.size()) {
        auto &[name, path] = m_meshes[m_meshIdx++];
        m_message = "Loading mesh: " + name + " from " + path;
        std::cout << m_message << "\n";
        hg::loadMesh(name, path);
    } else {
        m_message = "Initializing";
        if (m_settings.minLoadingTime <= m_elapsedTime) {
            onFinish();
        }
    }

    auto shader = getShader(TEXT_BUFFER_SHADER.name);

    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", Mat4::Orthographic(0, m_window->size().x(), 0, m_window->size().y(), -100, 100));
    shader->setMat4("model", Mat4::Identity());
    shader->setVec4("textColor", Color::white());

    m_messageBuffer.text(m_message);
    m_messageBuffer.render();

    m_versionBuffer.render();

    shader = getShader(TEXTURE_SHADER.name);
    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", Mat4::Orthographic(0, m_window->size().x(), 0, m_window->size().y(), -100, 100));
    shader->setMat4("model", Mat4::Identity());
    shader->setVec4("textColor", Color::white());

    getTexture(m_settings.logo)->bind();
    m_logo.render();

    glDisable(GL_DEPTH_TEST);

    m_renderPasses.render(RenderMode::Color, 1);

    shader->use();
    shader->setMat4("view", Mat4::Identity());
    shader->setMat4("projection", Mat4::Orthographic(0, m_window->size().x(), 0, m_window->size().y(), -100, 100));
    shader->setMat4("model", Mat4::Identity());

    m_renderPasses.get(RenderMode::Color)->textures[0]->bind();
    m_mesh.render();
}

void Loading::resize() {
    auto size = m_window->size();

    m_quad.size(size.cast<float>());
    m_mesh.update(&m_quad);

    m_logoQuad.offset(size.cast<float>() * 0.5);
    auto texSize = getTexture( m_settings.logo)->image->size.cast<float>();
    float targetSize = size[1] * 0.25;
    float ratio = targetSize / texSize[1];
    m_logoQuad.size(Vec2(texSize[0] * ratio, texSize[1] * ratio));
    m_logo.update(&m_logoQuad);

    m_renderPasses.resize(RenderMode::Color, size);

    m_versionBuffer.pos(Vec3(0, size[1] - 16, 0));
}

void Loading::loadDefaultShaders() {
    loadShader(COLOR_SHADER);
    loadShader(BATCH_COLOR_SHADER);
    loadShader(TEXTURE_SHADER);
    loadShader(BATCH_TEXTURE_SHADER);
    loadShader(TEXT_SHADER);
    loadShader(TEXT_BUFFER_SHADER);
    loadShader(PARTICLE_SHADER);
}

void Loading::readFilesForLoading() {

    if (f_exists(ASSET_DIR + m_settings.folders.audio)) {
        for (const auto& file : d_listFiles(ASSET_DIR + m_settings.folders.audio, true)) {
            auto parts = f_getParts(file);
            auto name = s_replace(parts.path, ASSET_DIR + m_settings.folders.audio + "/", "") + parts.name;
            m_sounds.push_back(std::make_tuple(name, file));
        }
    }

    std::cout << "READ SOUNDS\n";

    if (f_exists(ASSET_DIR + m_settings.folders.meshes)) {
        for (const auto& file : d_listFiles(ASSET_DIR + m_settings.folders.meshes, true)) {
            auto parts = f_getParts(file);
            auto name = s_replace(parts.path, ASSET_DIR + m_settings.folders.meshes + "/", "") + parts.name;
            if (parts.extension == "obj") {
                m_meshes.push_back(std::make_tuple(name, file));
            } else if (parts.extension == "mtl") {
                m_materials.push_back(std::make_tuple(name, file));
            }
        }
    }

    std::cout << "READ MESHES\n";

    for (const auto& file : d_listFiles(ASSET_DIR + m_settings.folders.fonts, true)) {
        auto parts = f_getParts(file);
        auto name = s_replace(parts.path, ASSET_DIR + m_settings.folders.fonts + "/", "") + parts.name;
        if (name != m_settings.font && parts.extension == "ttf") {
            m_fonts.push_back(std::make_tuple(name, file));
        }
    }

    std::cout << "READ FONTS\n";

    for (const auto& file : d_listFiles(ASSET_DIR + m_settings.folders.textures, true)) {
        auto parts = f_getParts(file);
        auto name = s_replace(parts.path, ASSET_DIR + m_settings.folders.textures + "/", "") + parts.name;
        if (parts.extension == "png") {
            m_textures.push_back(std::make_tuple(name, file));
        } else if (parts.extension == "hgss") {
            m_spriteSheets.push_back(std::make_tuple(name, file));
        }
    }

    std::cout << "READ TEXTURES\n";

    auto shaderFiles = std::unordered_map<std::string, ShaderPaths>();

    for (const auto& file: d_listFiles(ASSET_DIR + m_settings.folders.shaders, true)) {
        auto parts = f_getParts(file);
        auto name = s_replace(parts.path, ASSET_DIR + m_settings.folders.shaders + "/", "") + parts.name;
        if (shaderFiles.find(name) == shaderFiles.end()) {
            shaderFiles.insert(std::make_pair(name, ShaderPaths{}));
        }
        if (parts.extension == "frag") {
            shaderFiles[name].fragment = file;
        } else if (parts.extension == "vert") {
            shaderFiles[name].vertex = file;
        } else {
            throw std::runtime_error("Invalid file extension in shaders folder");
        }

        if (shaderFiles[name].vertex != "" && shaderFiles[name].fragment != "") {
            m_shaders.push_back(std::make_tuple(name, shaderFiles[name]));
        }
    }

    std::cout << "READ SHADERS\n";
}
