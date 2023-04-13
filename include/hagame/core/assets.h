//
// Created by henry on 4/12/23.
//

#ifndef HAGAME2_ASSETS_H
#define HAGAME2_ASSETS_H

#include "../utils/file.h"
#include "../utils/store.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/texture.h"
#include "../graphics/font.h"

namespace hg {
#ifdef __EMSCRIPTEN__
    inline static std::string ASSET_DIR = "./assets/";
#else
    inline static std::string ASSET_DIR = "../assets/";
#endif

    namespace assets {
        inline static hg::utils::Store<std::shared_ptr<hg::graphics::ShaderProgram>> SHADERS;
        inline static hg::utils::Store<std::shared_ptr<hg::graphics::Texture>> TEXTURES;
        inline static hg::utils::Store<std::shared_ptr<hg::graphics::Font>> FONTS;
    }

    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(std::string name, std::string vertPath, std::string fragPath) {
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

    hg::graphics::ShaderProgram* getShader(std::string name) {
        return assets::SHADERS.get(name).get();
    }

    std::shared_ptr<hg::graphics::Texture> loadTexture(std::string name, std::string path) {
        auto texture = std::make_shared<hg::graphics::Texture>(ASSET_DIR + path);
        assets::TEXTURES.set(name, texture);
        return texture;
    }

    hg::graphics::Texture* getTexture(std::string name) {
        return assets::TEXTURES.get(name).get();
    }

    std::shared_ptr<hg::graphics::Font> loadFont(std::string name, std::string path) {
        auto font = std::make_shared<hg::graphics::Font>( ASSET_DIR + path);
        assets::FONTS.set(name, font);
        return font;
    }

    hg::graphics::Font* getFont(std::string name) {
        return assets::FONTS.get(name).get();
    }
}


#endif //HAGAME2_ASSETS_H
