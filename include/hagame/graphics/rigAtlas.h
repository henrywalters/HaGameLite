//
// Created by henry on 10/16/26.
//

#ifndef HAGAME2_RIGATLAS_H
#define HAGAME2_RIGATLAS_H

#include <optional>
#include <unordered_map>
#include "texture.h"
#include "../core/asset.h"
#include "../math/aliases.h"
#include "nlohmann/json.hpp"

namespace hg::graphics {

    // One piece of a rig, cut from the atlas texture
    struct RigPart {
        std::string name;
        Rect uv; // Normalized rect in the texture, image space (top-left origin, y down)
        Vec2 size; // In pixels
        Vec2 origin; // Bottom-left corner on the source canvas, in pixels, y up
    };

    // The parts and joints of a rig, generated from an Aseprite file by tools/aseprite_to_rig_atlas.py (.hgra).
    // Everything is in canvas pixels with y up; the rig decides how many pixels make a world unit
    class RigAtlas {
    public:

        RigAtlas(nlohmann::json const& json);

        std::string texturePath;
        Vec2i textureSize;
        Vec2i canvasSize;

        // In draw order: the first part is furthest back
        std::vector<RigPart> parts;

        // Points the parts rotate around, keyed by name
        std::unordered_map<std::string, Vec2> joints;

        Texture* texture() const;

        const RigPart* getPart(std::string const& name) const;
        std::optional<Vec2> getJoint(std::string const& name) const;

        void load(nlohmann::json const& json);

    private:

        std::unordered_map<std::string, size_t> m_partIndex;
    };

    class RigAtlasAsset : public Asset<RigAtlas> {
    public:
        using Asset::Asset;
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_RIGATLAS_H
