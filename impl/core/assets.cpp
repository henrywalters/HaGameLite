#include "../../include/hagame/core/assets.h"
#include "../../include/hagame/utils/string.h"
#include "../../include/hagame/utils/log.h"
#include "../../include/hagame/graphics/primitives/cube.h"
#include "../../include/hagame/graphics/primitives/grid.h"
#include "../../include/hagame/graphics/primitives/plane.h"

#include <filesystem>
#include <unordered_map>

using namespace hg::graphics;
using namespace hg::audio;

namespace {

    enum class Kind { Shader, Texture, Font, SpriteSheet, RigAtlas, Skeleton, RigAnimations, Audio, Mesh, Materials,
                      Model, Prefab };

    struct Source {
        Kind kind;
        std::string name;
    };

    // The same file however its path was written ("../assets/x.png", "../assets/./x.png", an absolute path)
    std::string fileKey(std::string const& path) {
        std::error_code error;
        auto canonical = std::filesystem::weakly_canonical(path, error);
        return error ? path : canonical.string();
    }

    // What was loaded from each file, so a change to it can be loaded again under the same names
    std::unordered_map<std::string, std::vector<Source>>& sources() {
        static std::unordered_map<std::string, std::vector<Source>> s_sources;
        return s_sources;
    }

    void remember(std::string const& path, Kind kind, std::string const& name) {
        auto& list = sources()[fileKey(path)];
        for (auto const& source : list) {
            if (source.kind == kind && source.name == name) {
                return;
            }
        }
        list.push_back({kind, name});
    }

    // Assets replaced by loading another under the same name. They're kept, as components and renderers hold raw
    // pointers to them: those keep working, showing the old version until they look the asset up again
    std::vector<std::shared_ptr<void>>& retired() {
        static std::vector<std::shared_ptr<void>> s_retired;
        return s_retired;
    }

    template <class Store, class T>
    void put(Store& store, std::string const& name, std::shared_ptr<T> value) {
        if (store.has(name)) {
            retired().push_back(store.get(name));
        }
        store.set(name, value);
    }
}

std::shared_ptr<ShaderProgram> hg::loadShader(std::string name, std::string vertPath, std::string fragPath) {
    auto vertSrc = hg::utils::f_read(vertPath);
    auto fragSrc = hg::utils::f_read(fragPath);

    auto shader = std::make_shared<ShaderProgram>(
            name,
            Shader::LoadVertex(vertSrc),
            Shader::LoadFragment(fragSrc)
    );

    assets::SHADER_PATHS.set(name, assets::ShaderPaths{vertPath, fragPath});
    put(assets::SHADERS, name, shader);
    remember(vertPath, Kind::Shader, name);
    remember(fragPath, Kind::Shader, name);

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

    put(assets::SHADERS, source.name, shader);

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
    put(assets::TEXTURES, name, texture);
    remember(path, Kind::Texture, name);
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
    put(assets::FONTS, name, font);
    remember(path, Kind::Font, name);
    return font;
}

bool hg::hasFont(std::string name) {
    return assets::FONTS.has(name);
}

Font* hg::getFont(std::string name) {
    return assets::FONTS.get(name).get();
}

bool hg::hasSpriteSheet(std::string name) {
    return assets::SPRITE_SHEETS.has(name);
}

std::shared_ptr<SpriteSheetV2> hg::loadSpriteSheet(std::string name, std::string path) {
    //auto config = hg::utils::MultiConfig::Parse(path);
    //auto spriteSheet = std::make_shared<SpriteSheet>(config);

    auto config = nlohmann::json::parse(hg::utils::f_read(path));
    auto spriteSheet = std::make_shared<SpriteSheetV2>(config);
    put(assets::SPRITE_SHEETS, name, spriteSheet);
    remember(path, Kind::SpriteSheet, name);
    return spriteSheet;
}

SpriteSheetV2* hg::getSpriteSheet(std::string name) {
    return assets::SPRITE_SHEETS.get(name).get();
}

bool hg::hasRigAtlas(std::string name) {
    return assets::RIG_ATLASES.has(name);
}

std::shared_ptr<RigAtlas> hg::loadRigAtlas(std::string name, std::string path) {
    auto json = nlohmann::json::parse(hg::utils::f_read(path));
    auto atlas = std::make_shared<RigAtlas>(json);
    put(assets::RIG_ATLASES, name, atlas);
    remember(path, Kind::RigAtlas, name);
    return atlas;
}

RigAtlas* hg::getRigAtlas(std::string name) {
    return assets::RIG_ATLASES.get(name).get();
}

bool hg::hasSkeleton(std::string name) {
    return assets::SKELETONS.has(name);
}

std::shared_ptr<Skeleton> hg::loadSkeleton(std::string name, std::string path) {
    auto json = nlohmann::json::parse(hg::utils::f_read(path));
    auto skeleton = std::make_shared<Skeleton>(json);
    put(assets::SKELETONS, name, skeleton);
    remember(path, Kind::Skeleton, name);
    return skeleton;
}

Skeleton* hg::getSkeleton(std::string name) {
    return assets::SKELETONS.get(name).get();
}

bool hg::hasRigAnimationSet(std::string name) {
    return assets::RIG_ANIMATION_SETS.has(name);
}

std::shared_ptr<RigAnimationSet> hg::loadRigAnimationSet(std::string name, std::string path) {
    auto json = nlohmann::json::parse(hg::utils::f_read(path));
    auto set = std::make_shared<RigAnimationSet>(json);
    set->filePath = path;
    put(assets::RIG_ANIMATION_SETS, name, set);
    remember(path, Kind::RigAnimations, name);
    return set;
}

RigAnimationSet* hg::getRigAnimationSet(std::string name) {
    return assets::RIG_ANIMATION_SETS.get(name).get();
}

std::shared_ptr<Stream> hg::loadAudioStream(std::string name, std::string path) {
    auto stream = std::make_shared<Stream>(path);
    put(assets::AUDIO_STREAMS, name, stream);
    remember(path, Kind::Audio, name);
    return stream;
}

Stream* hg::getAudioStream(std::string name) {
    if (!assets::AUDIO_STREAMS.has(name)) {
        // Sounds are loaded at startup, so one added while the game is running isn't there yet: load it now. Null,
        // rather than throwing like the store does, for a name with no file, e.g. an AudioSource not set up yet
        auto path = ASSET_DIR + name + ".wav";
        if (name.empty() || !utils::f_exists(path)) {
            return nullptr;
        }
        return loadAudioStream(name, path).get();
    }
    return assets::AUDIO_STREAMS.get(name).get();
}

bool hg::hasAudioSource(std::string name) {
    return assets::AUDIO_STREAMS.has(name);
}

std::shared_ptr<Mesh> hg::loadMesh(std::string name, std::string path) {
    auto mesh = std::make_shared<Mesh>(path);
    put(assets::MESHES, name, mesh);
    remember(path, Kind::Mesh, name);
    return mesh;
}

std::shared_ptr<hg::graphics::Mesh> hg::loadCubeMesh(std::string name, hg::Vec3 size) {
    auto mesh = std::make_shared<primitives::Cube>(size);
    put(assets::MESHES, name, mesh);
    return mesh;
}

std::shared_ptr<hg::graphics::Mesh> hg::loadGridMesh(std::string name, hg::Vec2 size, hg::Vec2i cells) {
    auto mesh = std::make_shared<primitives::Grid>(size, cells);
    put(assets::MESHES, name, mesh);
    return mesh;
}

std::shared_ptr<hg::graphics::Mesh> hg::loadPlaneMesh(std::string name, hg::Vec2 size, hg::Vec2i cells) {
    auto mesh = std::make_shared<primitives::Plane>(size, cells);
    put(assets::MESHES, name, mesh);
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
    put(assets::MATERIALS, name, materials);
    remember(path, Kind::Materials, name);
    return materials;
}

hg::graphics::Materials *hg::getMaterials(std::string name) {
    return assets::MATERIALS.get(name).get();
}

std::shared_ptr<hg::graphics::Model> hg::loadModel(std::string name, std::string path) {
    auto model = std::make_shared<Model>(path);
    put(assets::MODELS, name, model);
    remember(path, Kind::Model, name);
    return model;
}

bool hg::hasModel(std::string name) {
    return assets::MODELS.has(name);
}

hg::graphics::Model *hg::getModel(std::string name) {
    return assets::MODELS.get(name).get();
}

std::shared_ptr<hg::Prefab> hg::loadPrefab(std::string name, std::string path) {
    auto prefab = std::make_shared<Prefab>(path);
    put(assets::PREFABS, name, prefab);
    remember(path, Kind::Prefab, name);
    return prefab;
}

std::shared_ptr<hg::Prefab> hg::createPrefab(std::string name) {
    auto prefab = std::make_shared<Prefab>();
    put(assets::PREFABS, name, prefab);
    return prefab;
}

bool hg::hasPrefab(std::string name) {
    return assets::PREFABS.has(name);
}

hg::Prefab *hg::getPrefab(std::string name) {
    return assets::PREFABS.get(name).get();
}
namespace {

    // Reloads one thing that was loaded from path. False when nothing changed
    bool reloadSource(Source const& source, std::string const& path) {
        using namespace hg;
        switch (source.kind) {
            case Kind::Texture:
                // In place: sprites, sheets and the batch renderer hold the Texture itself
                if (assets::TEXTURES.has(source.name)) {
                    assets::TEXTURES.get(source.name)->load(path);
                } else {
                    loadTexture(source.name, path);
                }
                return true;
            case Kind::Prefab:
                // In place too: instances point at the Prefab and rebuild when it changes
                if (assets::PREFABS.has(source.name)) {
                    return assets::PREFABS.get(source.name)->reload(path);
                }
                loadPrefab(source.name, path);
                return true;
            case Kind::Shader:
                recompileShader(source.name);
                return true;
            case Kind::Font: {
                // The same file is loaded under several names at different sizes
                int size = assets::FONTS.has(source.name) ? assets::FONTS.get(source.name)->fontSize() : -1;
                auto font = loadFont(source.name, path);
                if (size > 0) {
                    font->fontSize(size);
                }
                return true;
            }
            case Kind::SpriteSheet: loadSpriteSheet(source.name, path); return true;
            case Kind::RigAtlas: loadRigAtlas(source.name, path); return true;
            case Kind::Skeleton: loadSkeleton(source.name, path); return true;
            case Kind::RigAnimations: loadRigAnimationSet(source.name, path); return true;
            case Kind::Audio: loadAudioStream(source.name, path); return true;
            case Kind::Mesh: loadMesh(source.name, path); return true;
            case Kind::Materials: loadMaterials(source.name, path); return true;
            case Kind::Model: loadModel(source.name, path); return true;
        }
        return false;
    }

    // A file that wasn't loaded before, named the way the loading scene names it: its path under ASSET_DIR without
    // the extension, e.g. "textures/props/crate"
    bool loadNew(std::string const& path) {
        using namespace hg;
        std::filesystem::path file(path);
        auto ext = file.extension().string();
        if (!ext.empty()) {
            ext = ext.substr(1);
        }
        auto name = path.starts_with(ASSET_DIR) ? path.substr(ASSET_DIR.size()) : path;
        name = name.substr(0, name.size() - file.extension().string().size());

        if (ext == "png") {
            loadTexture(name, path);
        } else if (ext == "hgss") {
            loadSpriteSheet(name, path);
        } else if (ext == "hgra") {
            loadRigAtlas(name, path);
        } else if (ext == "hg_skel") {
            loadSkeleton(name, path);
        } else if (ext == "hg_anims") {
            loadRigAnimationSet(name, path);
        } else if (ext == "hg_pfb") {
            loadPrefab(name, path);
        } else if (ext == "ttf") {
            loadFont(name, path);
        } else if (ext == "wav" || ext == "ogg" || ext == "mp3" || ext == "flac") {
            loadAudioStream(name, path);
        } else if (ext == "obj") {
            loadModel(name, path);
            loadMesh(name, path);
        } else if (ext == "mtl") {
            loadMaterials(name, path);
        } else if (ext == "vert" || ext == "frag") {
            // A shader is a pair: load it once both halves are there
            auto base = path.substr(0, path.size() - file.extension().string().size());
            if (!utils::f_exists(base + ".vert") || !utils::f_exists(base + ".frag")) {
                return false;
            }
            loadShader(name, base + ".vert", base + ".frag");
        } else {
            return false;
        }
        return true;
    }
}

bool hg::reloadAsset(std::string path) {
    bool changed = false;
    auto it = sources().find(fileKey(path));
    if (it != sources().end() && !it->second.empty()) {
        auto list = it->second; // Loading adds to sources(), so not by reference
        for (auto const& source : list) {
            try {
                changed = reloadSource(source, path) || changed;
            } catch (std::exception const& e) {
                log::error("Couldn't reload {} from {}: {}", source.name, path, e.what());
            }
        }
    } else {
        try {
            changed = loadNew(path);
        } catch (std::exception const& e) {
            log::error("Couldn't load {}: {}", path, e.what());
        }
    }

    if (changed) {
        ASSET_GENERATION++;
    }
    return changed;
}
