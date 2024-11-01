//
// Created by henry on 4/12/23.
//

#ifndef HAGAME2_ASSETS_H
#define HAGAME2_ASSETS_H

#include "../utils/store.h"
#include "../graphics/shaderProgram.h"
#include "../graphics/texture.h"
#include "../graphics/font.h"
#include "../graphics/spriteSheetV2.h"
#include "../audio/stream.h"
#include "../graphics/mesh.h"
#include "../graphics/material.h"
#include "../graphics/model.h"

namespace hg {
#ifdef __EMSCRIPTEN__
    inline static std::string ASSET_DIR = "./assets/";
#else
    inline static std::string ASSET_DIR = "../assets/";
#endif

    namespace assets {

        struct ShaderPaths {
            std::string vertPath;
            std::string fragPath;
        };

        inline static std::shared_ptr<hg::graphics::Texture> MISSING_TEXTURE;
        inline static hg::utils::Store<std::string, ShaderPaths> SHADER_PATHS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::ShaderProgram>> SHADERS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Texture>> TEXTURES;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Font>> FONTS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::SpriteSheetV2>> SPRITE_SHEETS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::audio::Stream>> AUDIO_STREAMS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Mesh>> MESHES;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Materials>> MATERIALS;
        inline static hg::utils::Store<std::string, std::shared_ptr<hg::graphics::Model>> MODELS;
    }

    // Load a shader directly by name with path
    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(std::string name, std::string vertPath, std::string fragPath);
    // Load a shader from there base name. Ex. "shaders/color" => vert = "shaders/color.vert"; frag = "shaders/color.frag" identified by "color";
    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(std::string path);
    // Recompile all loaded shaders
    void recompileShaders();
    // Recompile a shader by name
    void recompileShader(std::string name);
    // Load a standard HaGame shader
    std::shared_ptr<hg::graphics::ShaderProgram> loadShader(graphics::ShaderSource shader);
    // Load a set of shaders from there base name. Ex. shaders/color => vert = 'shaders/color.vert'; frag = 'shaders/color.frag';
    std::vector<std::shared_ptr<hg::graphics::ShaderProgram>> loadShaders(std::vector<std::string> paths);
    hg::graphics::ShaderProgram* getShader(std::string name);

    void setMissingTexture(std::string path);
    std::shared_ptr<hg::graphics::Texture> loadTexture(std::string name, std::string path);
    std::shared_ptr<hg::graphics::Texture> loadTexture(std::string path);
    std::vector<std::shared_ptr<hg::graphics::Texture>> loadTextures(std::vector<std::string> paths);
    hg::graphics::Texture* getTexture(std::string name);

    std::shared_ptr<hg::graphics::SpriteSheetV2> loadSpriteSheet(std::string name, std::string path);
    hg::graphics::SpriteSheetV2* getSpriteSheet(std::string name);

    std::shared_ptr<hg::graphics::Font> loadFont(std::string name, std::string path);
    hg::graphics::Font* getFont(std::string name);

    std::shared_ptr<hg::audio::Stream> loadAudioStream(std::string name, std::string path);
    bool hasAudioSource(std::string name);
    hg::audio::Stream* getAudioStream(std::string name);

    std::shared_ptr<hg::graphics::Mesh> loadMesh(std::string name, std::string path);
    std::shared_ptr<hg::graphics::Mesh> loadCubeMesh(std::string name, hg::Vec3 size = hg::Vec3::Identity());
    std::shared_ptr<hg::graphics::Mesh> loadGridMesh(std::string name, hg::Vec2 size = hg::Vec2::Identity(), hg::Vec2i cells = hg::Vec2i::Identity());
    std::shared_ptr<hg::graphics::Mesh> loadPlaneMesh(std::string name, hg::Vec2 size = hg::Vec2::Identity(), hg::Vec2i cells = hg::Vec2i::Identity());
    bool hasMesh(std::string name);
    hg::graphics::Mesh* getMesh(std::string name);

    std::shared_ptr<hg::graphics::Materials> loadMaterials(std::string name, std::string path);
    hg::graphics::Materials* getMaterials(std::string name);

    std::shared_ptr<hg::graphics::Model> loadModel(std::string name, std::string path);
    bool hasModel(std::string name);
    hg::graphics::Model* getModel(std::string name);
}


#endif //HAGAME2_ASSETS_H
