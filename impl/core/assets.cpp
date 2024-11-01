#include "../../include/hagame/core/assets.h"
#include "../../include/hagame/utils/string.h"
#include "../../include/hagame/graphics/primitives/cube.h"
#include "../../include/hagame/graphics/primitives/grid.h"
#include "../../include/hagame/graphics/primitives/plane.h"

using namespace hg::graphics;
using namespace hg::audio;

std::shared_ptr<ShaderProgram> hg::loadShader(std::string name, std::string vertPath, std::string fragPath) {
    auto vertSrc = hg::utils::f_read(vertPath);
    auto fragSrc = hg::utils::f_read(fragPath);

    auto shader = std::make_shared<ShaderProgram>(
            name,
            Shader::LoadVertex(vertSrc),
            Shader::LoadFragment(fragSrc)
    );

    assets::SHADER_PATHS.set(name, assets::ShaderPaths{vertPath, fragPath});
    assets::SHADERS.set(name, shader);

    return shader;
}

std::shared_ptr<ShaderProgram> hg::loadShader(std::string path) {
    auto parts = hg::utils::s_split(path, '/');
    auto f_parts = hg::utils::f_getParts(parts[parts.size() - 1]);
    return loadShader(f_parts.name, path + ".vert", path + ".frag");
}

void hg::recompileShaders() {
    assets::SHADERS.forEach([&](std::string key, auto shader) {
        recompileShader(key);
    });
}

void hg::recompileShader(std::string name) {
    auto paths = assets::SHADER_PATHS.get(name);
    loadShader(name, paths.vertPath, paths.fragPath);
}

std::shared_ptr<ShaderProgram> hg::loadShader(ShaderSource source) {
    auto shader = std::make_shared<ShaderProgram>(
            source.name,
            Shader::LoadVertex(source.vertexSource),
            Shader::LoadFragment(source.fragmentSource)
    );

    assets::SHADERS.set(source.name, shader);

    return shader;
}

std::vector<std::shared_ptr<ShaderProgram>> hg::loadShaders(std::vector<std::string> paths) {
    std::vector<std::shared_ptr<ShaderProgram>> shaders;

    for (const auto& path : paths) {
        shaders.push_back(loadShader(path));
    }

    return shaders;
}

ShaderProgram* hg::getShader(std::string name) {
    return assets::SHADERS.get(name).get();
}

std::shared_ptr<Texture> hg::loadTexture(std::string name, std::string path) {
    auto texture = std::make_shared<Texture>(path);
    assets::TEXTURES.set(name, texture);
    return texture;
}

std::shared_ptr<Texture> hg::loadTexture(std::string path) {
    auto parts = hg::utils::s_split(path, '/');
    return loadTexture(hg::utils::f_getParts(parts[parts.size() - 1]).name, path);
}

std::vector<std::shared_ptr<Texture>> hg::loadTextures(std::vector<std::string> paths) {
    std::vector<std::shared_ptr<Texture>> textures;
    for (const auto& path : paths) {
        textures.push_back(loadTexture(path));
    }
    return textures;
}

void hg::setMissingTexture(std::string path) {
    auto parts = hg::utils::s_split(path, '/');
    hg::assets::MISSING_TEXTURE = loadTexture(hg::utils::f_getParts(parts[parts.size() - 1]).name, path);
}

Texture* hg::getTexture(std::string name) {
    if (!assets::TEXTURES.has(name)) {
        return assets::MISSING_TEXTURE.get();
    }
    return assets::TEXTURES.get(name).get();
}

std::shared_ptr<Font> hg::loadFont(std::string name, std::string path) {
    auto font = std::make_shared<Font>(path);
    assets::FONTS.set(name, font);
    return font;
}

Font* hg::getFont(std::string name) {
    return assets::FONTS.get(name).get();
}

std::shared_ptr<SpriteSheetV2> hg::loadSpriteSheet(std::string name, std::string path) {
    //auto config = hg::utils::MultiConfig::Parse(path);
    //auto spriteSheet = std::make_shared<SpriteSheet>(config);

    auto config = hg::utils::Config::Parse(hg::utils::f_readLines(path));
    auto spriteSheet = std::make_shared<SpriteSheetV2>(config);

    assets::SPRITE_SHEETS.set(name, spriteSheet);
    return spriteSheet;
}

SpriteSheetV2* hg::getSpriteSheet(std::string name) {
    return assets::SPRITE_SHEETS.get(name).get();
}

std::shared_ptr<Stream> hg::loadAudioStream(std::string name, std::string path) {
    auto stream = std::make_shared<Stream>(path);
    assets::AUDIO_STREAMS.set(name, stream);
    return stream;
}

Stream* hg::getAudioStream(std::string name) {
    return assets::AUDIO_STREAMS.get(name).get();
}

bool hg::hasAudioSource(std::string name) {
    return assets::AUDIO_STREAMS.has(name);
}

std::shared_ptr<Mesh> hg::loadMesh(std::string name, std::string path) {
    auto mesh = std::make_shared<Mesh>(path);
    assets::MESHES.set(name, mesh);
    return mesh;
}

std::shared_ptr<hg::graphics::Mesh> hg::loadCubeMesh(std::string name, hg::Vec3 size) {
    auto mesh = std::make_shared<primitives::Cube>(size);
    assets::MESHES.set(name, mesh);
    return mesh;
}

std::shared_ptr<hg::graphics::Mesh> hg::loadGridMesh(std::string name, hg::Vec2 size, hg::Vec2i cells) {
    auto mesh = std::make_shared<primitives::Grid>(size, cells);
    assets::MESHES.set(name, mesh);
    return mesh;
}

std::shared_ptr<hg::graphics::Mesh> hg::loadPlaneMesh(std::string name, hg::Vec2 size, hg::Vec2i cells) {
    auto mesh = std::make_shared<primitives::Plane>(size, cells);
    assets::MESHES.set(name, mesh);
    return mesh;
}

bool hg::hasMesh(std::string name) {
    return assets::MESHES.has(name);
}

Mesh* hg::getMesh(std::string name) {
    return assets::MESHES.get(name).get();
}

std::shared_ptr<hg::graphics::Materials> hg::loadMaterials(std::string name, std::string path) {
    auto materials = std::make_shared<Materials>(path);
    assets::MATERIALS.set(name, materials);
    return materials;
}

hg::graphics::Materials *hg::getMaterials(std::string name) {
    return assets::MATERIALS.get(name).get();
}

std::shared_ptr<hg::graphics::Model> hg::loadModel(std::string name, std::string path) {
    auto model = std::make_shared<Model>(path);
    assets::MODELS.set(name, model);
    return model;
}

bool hg::hasModel(std::string name) {
    return assets::MODELS.has(name);
}

hg::graphics::Model *hg::getModel(std::string name) {
    return assets::MODELS.get(name).get();
}



