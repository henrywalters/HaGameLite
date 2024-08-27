//
// Created by henry on 8/23/24.
//

#ifndef HAGAME2_UI_IMAGE_H
#define HAGAME2_UI_IMAGE_H

#include "../element.h"
#include "../../graphics/primitives/quad.h"
#include "../../graphics/mesh.h"

namespace hg::ui {
    class Image : public Element {
    public:

        Image(hg::graphics::Texture* texture, hg::Rect texRect = hg::Rect(hg::Vec2::Zero(), hg::Vec2::Identity()));

    protected:

        OBJECT_NAME(Image)

        void onRender(GraphicsContext* context, Rect rootRect, double dt) override;

    private:

        int m_index = 0;
        hg::Rect m_texRect;
        hg::graphics::Texture* m_texture;
        graphics::primitives::Quad m_quad;
        graphics::MeshInstance m_mesh;
    };
}

#endif //HAGAME2_IMAGE_H
