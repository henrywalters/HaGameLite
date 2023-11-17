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
#include "../graphics/spriteSheet.h"

namespace hg {
#ifdef __EMSCRIPTEN__
    inline static std::string ASSET_DIR = "./assets/";
#else
    inline static std::string ASSET_DIR = "../assets/";
#endif

    namespace assets {
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::ShaderProgram>> SHADERS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Texture>> TEXTURES;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Font>> FONTS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::SpriteSheet>> SPRITE_SHEETS;
    }

    // Load a shader directly by name with path
    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(std::string name, std::string vertPath, std::string fragPath);

    // Load a shader from there base name. Ex. "shaders/color" => vert = "shaders/color.vert"; frag = "shaders/color.frag" identified by "color";
    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(std::string path);

    // Load a standard HaGame shader
    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(graphics::ShaderSource shader);

    // Load a set of shaders from there base name. Ex. shaders/color => vert = 'shaders/color.vert'; frag = 'shaders/color.frag';
    std::vector<std::shared_ptr<hg::graphics::ShaderProgram>> loadShaders(std::vector<std::string> paths);

    hg::graphics::ShaderProgram* getShader(std::string name);

    std::shared_ptr<hg::graphics::Texture> loadTexture(std::string name, std::string path);
    std::shared_ptr<hg::graphics::Texture> loadTexture(std::string path);
    std::vector<std::shared_ptr<hg::graphics::Texture>> loadTextures(std::vector<std::string> paths);
    hg::graphics::Texture* getTexture(std::string name);

    std::shared_ptr<hg::graphics::SpriteSheet> loadSpriteSheet(std::string name, std::string path);
    hg::graphics::SpriteSheet* getSpriteSheet(std::string name);

    std::shared_ptr<hg::graphics::Font> loadFont(std::string name, std::string path);
    hg::graphics::Font* getFont(std::string name);
}


#endif //HAGAME2_ASSETS_H
