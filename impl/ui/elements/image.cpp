//
// Created by henry on 8/23/24.
//
#include "../../../../include/hagame/ui/elements/image.h"

hg::ui::Image::Image(hg::graphics::Texture *texture, hg::Rect texRect):
    m_texture(texture),
    m_texRect(texRect),
    m_quad(Vec2::Identity(), Vec2::Zero()),
    m_mesh(&m_quad)
{
    m_quad.texSize(m_texRect.size);
    m_quad.texOffset(m_texRect.pos);
    m_quad.centered(false);
    m_mesh.update(&m_quad);
}

void hg::ui::Image::onRender(hg::ui::GraphicsContext *context, hg::Rect rootRect, double dt) {
    auto rect = getRect(rootRect);
    auto size = m_texRect.size.prod(m_texture->image->size.cast<float>());
    auto rectAR = rect.size[0] / rect.size[1];
    auto sizeAR = size[0] / size[1];
    float scale;
    if (sizeAR > rectAR) {
        scale = rect.size[0] / size[0];
    } else {
        scale = rect.size[1] / size[1];
    }
    auto finalSize = size * scale;
    auto offset = (rect.size - finalSize) * 0.5;

    context->textureShader.use();
    //context->textureShader.setVec4("color", hg::graphics::Color::white());
    context->textureShader.setMat4("model", Mat4::Translation(Vec3(0, 0, depth())));

    m_quad.setSizeAndOffset(finalSize, offset + rect.pos);
    m_quad.texSize(m_texRect.size);
    m_quad.texOffset(m_texRect.pos);
    m_mesh.update(&m_quad);
    m_texture->bind();
    m_mesh.render();
}
