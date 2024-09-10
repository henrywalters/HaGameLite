//
// Created by henry on 6/22/24.
//

#ifndef HAGAME2_LOADING_H
#define HAGAME2_LOADING_H

#include "../../core/scene.h"
#include "../../graphics/window.h"
#include "../../graphics/renderPasses.h"
#include "../../graphics/textBuffer.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"
#include "../../graphics/shader.h"
#include "../../utils/enum.h"

namespace hg {

    class Loading : public Scene {
    public:

        std::function<void()> onFinish = []() {
            std::cout << "WARNING: onFinish is undefined on loading scene. Please define this to move forward\n";
        };

        enum class RenderMode {
            Color,
        };

        struct AssetFolders {
            std::string audio = "audio";
            std::string textures = "textures";
            std::string shaders = "shaders";
            std::string fonts = "fonts";
        };

        struct Settings {
            double minLoadingTime = 1.0;
            std::string logo;
            std::string version;
            std::string font;
            AssetFolders folders;
        };

        Loading(graphics::Window* window, Settings settings);

    protected:

        void onInit() override;
        void onUpdate(double dt) override;

        virtual void loadDefaultShaders();
        virtual void readFilesForLoading();

    private:

        struct ShaderPaths {
            std::string vertex;
            std::string fragment;
        };

        Settings m_settings;

        std::string m_message = "";

        double m_elapsedTime = 0;

        int m_spriteSheetIdx = 0;
        int m_textureIdx = 0;
        int m_shaderIdx = 0;
        int m_soundIdx = 0;

        graphics::RenderPasses<RenderMode> m_renderPasses;

        graphics::primitives::Quad m_quad;
        graphics::MeshInstance m_mesh;

        graphics::TextBuffer m_messageBuffer;
        graphics::TextBuffer m_versionBuffer;

        graphics::primitives::Quad m_logoQuad;
        graphics::MeshInstance m_logo;

        std::vector<std::tuple<std::string, ShaderPaths>> m_shaders;
        std::vector<std::tuple<std::string, std::string>> m_spriteSheets;
        std::vector<std::tuple<std::string, std::string>> m_textures;
        std::vector<std::tuple<std::string, std::string>> m_sounds;

        hg::graphics::Window* m_window;

        void resize();
    };
}

#endif //HAGAME2_LOADING_H
