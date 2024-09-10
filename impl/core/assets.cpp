#include "../../include/hagame/core/assets.h"
#include "../../include/hagame/utils/string.h"

std::shared_ptr<hg::graphics::ShaderProgram> hg::loadShader(std::string name, std::string vertPath, std::string fragPath) {
    auto vertSrc = hg::utils::f_read(vertPath);
    auto fragSrc = hg::utils::f_read(fragPath);

    auto shader = std::make_shared<hg::graphics::ShaderProgram>(
            name,
            hg::graphics::Shader::LoadVertex(vertSrc),
            hg::graphics::Shader::LoadFragment(fragSrc)
    );

    assets::SHADER_PATHS.set(name, assets::ShaderPaths{vertPath, fragPath});
    assets::SHADERS.set(name, shader);

    return shader;
}

std::shared_ptr<hg::graphics::ShaderProgram> hg::loadShader(std::string path) {
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

std::shared_ptr<hg::graphics::ShaderProgram> hg::loadShader(hg::graphics::ShaderSource source) {
    auto shader = std::make_shared<hg::graphics::ShaderProgram>(
            source.name,
            hg::graphics::Shader::LoadVertex(source.vertexSource),
            hg::graphics::Shader::LoadFragment(source.fragmentSource)
    );

    assets::SHADERS.set(source.name, shader);

    return shader;
}

std::vector<std::shared_ptr<hg::graphics::ShaderProgram>> hg::loadShaders(std::vector<std::string> paths) {
    std::vector<std::shared_ptr<hg::graphics::ShaderProgram>> shaders;

    for (const auto& path : paths) {
        shaders.push_back(loadShader(path));
    }

    return shaders;
}

hg::graphics::ShaderProgram* hg::getShader(std::string name) {
    return assets::SHADERS.get(name).get();
}

std::shared_ptr<hg::graphics::Texture> hg::loadTexture(std::string name, std::string path) {
    auto texture = std::make_shared<hg::graphics::Texture>(path);
    assets::TEXTURES.set(name, texture);
    return texture;
}

std::shared_ptr<hg::graphics::Texture> hg::loadTexture(std::string path) {
    auto parts = hg::utils::s_split(path, '/');
    return loadTexture(hg::utils::f_getParts(parts[parts.size() - 1]).name, path);
}

std::vector<std::shared_ptr<hg::graphics::Texture>> hg::loadTextures(std::vector<std::string> paths) {
    std::vector<std::shared_ptr<hg::graphics::Texture>> textures;
    for (const auto& path : paths) {
        textures.push_back(loadTexture(path));
    }
    return textures;
}

void hg::setMissingTexture(std::string path) {
    auto parts = hg::utils::s_split(path, '/');
    hg::assets::MISSING_TEXTURE = loadTexture(hg::utils::f_getParts(parts[parts.size() - 1]).name, path);
}

hg::graphics::Texture* hg::getTexture(std::string name) {
    if (!assets::TEXTURES.has(name)) {
        return assets::MISSING_TEXTURE.get();
    }
    return assets::TEXTURES.get(name).get();
}

std::shared_ptr<hg::graphics::Font> hg::loadFont(std::string name, std::string path) {
    auto font = std::make_shared<hg::graphics::Font>(path);
    assets::FONTS.set(name, font);
    return font;
}

hg::graphics::Font* hg::getFont(std::string name) {
    return assets::FONTS.get(name).get();
}

std::shared_ptr<hg::graphics::SpriteSheetV2> hg::loadSpriteSheet(std::string name, std::string path) {
    //auto config = hg::utils::MultiConfig::Parse(path);
    //auto spriteSheet = std::make_shared<hg::graphics::SpriteSheet>(config);

    auto config = hg::utils::Config::Parse(hg::utils::f_readLines(path));
    auto spriteSheet = std::make_shared<hg::graphics::SpriteSheetV2>(config);

    assets::SPRITE_SHEETS.set(name, spriteSheet);
    return spriteSheet;
}

hg::graphics::SpriteSheetV2* hg::getSpriteSheet(std::string name) {
    return assets::SPRITE_SHEETS.get(name).get();
}

std::shared_ptr<hg::audio::Stream> hg::loadAudioStream(std::string name, std::string path) {
    auto stream = std::make_shared<hg::audio::Stream>(path);
    assets::AUDIO_STREAMS.set(name, stream);
    return stream;
}

hg::audio::Stream* hg::getAudioStream(std::string name) {
    return assets::AUDIO_STREAMS.get(name).get();
}

bool hg::hasAudioSource(std::string name) {
    return assets::AUDIO_STREAMS.has(name);
}
