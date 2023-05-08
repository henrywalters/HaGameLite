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

    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(std::string name, std::string vertPath, std::string fragPath);
    hg::graphics::ShaderProgram* getShader(std::string name);

    std::shared_ptr<hg::graphics::Texture> loadTexture(std::string name, std::string path);
    hg::graphics::Texture* getTexture(std::string name);

    std::shared_ptr<hg::graphics::Font> loadFont(std::string name, std::string path);
    hg::graphics::Font* getFont(std::string name);
}


#endif //HAGAME2_ASSETS_H
