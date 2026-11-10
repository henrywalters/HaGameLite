//
// Created by henry on 10/26/26.
//

#ifndef HAGAME2_FONTASSET_H
#define HAGAME2_FONTASSET_H

#include "../core/asset.h"

namespace hg::graphics {
    class Font;

    // A font by its asset path, e.g. fonts/8bit. Fonts loaded under a bare name (the loading screen's own font is
    // "8bit") are found by the path's file name too. Apart from font.h so the variant can hold one without
    // including Font, which includes the variant through Object
    class FontAsset : public Asset<Font> {
    public:
        using Asset::Asset;
    protected:
        void reload() override;
    };
}

#endif //HAGAME2_FONTASSET_H
