#include "../../include/hagame/core/assets.h"

std::shared_ptr<hg::graphics::ShaderProgram> hg::loadShader(std::string name, std::string vertPath, std::string fragPath) {
    auto vertSrc = hg::utils::f_read(ASSET_DIR + vertPath);
    auto fragSrc = hg::utils::f_read(ASSET_DIR + fragPath);

    auto shader = std::make_shared<hg::graphics::ShaderProgram>(
            name,
            hg::graphics::Shader::LoadVertex(vertSrc),
            hg::graphics::Shader::LoadFragment(fragSrc)
    );

    assets::SHADERS.set(name, shader);

    return shader;
}

hg::graphics::ShaderProgram* hg::getShader(std::string name) {
    return assets::SHADERS.get(name).get();
}

std::shared_ptr<hg::graphics::Texture> hg::loadTexture(std::string name, std::string path) {
    auto texture = std::make_shared<hg::graphics::Texture>(ASSET_DIR + path);
    assets::TEXTURES.set(name, texture);
    return texture;
}

hg::graphics::Texture* hg::getTexture(std::string name) {
    return assets::TEXTURES.get(name).get();
}

std::shared_ptr<hg::graphics::Font> hg::loadFont(std::string name, std::string path) {
    auto font = std::make_shared<hg::graphics::Font>( ASSET_DIR + path);
    assets::FONTS.set(name, font);
    return font;
}

hg::graphics::Font* hg::getFont(std::string name) {
    return assets::FONTS.get(name).get();
}