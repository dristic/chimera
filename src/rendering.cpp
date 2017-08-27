// Copyright 2017 Dan Ristic

#include "src/rendering.h"

#include <utility>
#include <codecvt>
#include <locale>

#include "src/context.h"
#include "src/adaptor/adaptor.h"

namespace Chimera {

DrawData::DrawData()
    : vertices{}
    , indicies{}
    , mCurIndex{0}
    { }

DrawData::~DrawData() { }

void DrawData::addRectFilled(const Rect& rect, const Color& color) {
    addRectFilled(rect, color, rect);
}

void DrawData::addRectFilled(
        const Rect& rect,
        const Color& color,
        const Rect& scissor) {
    float right = rect.x + rect.width;
    float bottom = rect.y + rect.height;

    Color finalColor = color.multiplyAlpha(globalAlpha);

    std::vector<DrawVert> verts = {
        // top-left
        { rect.x, rect.y, 0.0f, 0.0f, finalColor.r, finalColor.g, finalColor.b, finalColor.a},
        // top-right
        { right,  rect.y, 1.0f, 0.0f, finalColor.r, finalColor.g, finalColor.b, finalColor.a},
        // bottom-right
        { right,  bottom, 1.0f, 1.0f, finalColor.r, finalColor.g, finalColor.b, finalColor.a},
        // bottom-left
        { rect.x, bottom, 0.0f, 1.0f, finalColor.r, finalColor.g, finalColor.b, finalColor.a}
    };

    vertices.insert(vertices.end(), verts.begin(), verts.end());

    std::vector<unsigned int> inds = {
        mCurIndex, mCurIndex + 1, mCurIndex + 2,
        mCurIndex + 2, mCurIndex + 3, mCurIndex
    };

    indicies.insert(indicies.end(), inds.begin(), inds.end());

    commands.emplace_back();
    DrawCommand& command = commands.back();
    command.elements = 6;

    command.scissor = scissor;

    mCurIndex += 4;
}

void DrawData::addImage(const Chimera::Rect &rect, unsigned int textureId) {
    addImage(rect, textureId, 1.0f);
}

void DrawData::addImage(const Rect& rect, unsigned int textureId, float opacity) {
    Color white{1.0f, 1.0f, 1.0f, opacity};
    addRectFilled(rect, white);

    commands.back().textureId = textureId;
}

void DrawData::addText(
        Rect position,
        const std::string& text,
        std::string name,
        int size,
        const Rect& scissor,
        const Color& color) {
    float scale = (float)size / (float)24;

    // Add the font size to the baseline
    position.y += 24 * scale;

    float xPosition = 0;

#if WIN32
    std::wstring_convert<std::codecvt_utf8<unsigned int>, unsigned int> converter{};
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter{};
#endif

    auto utf32str = converter.from_bytes(text);

    for (auto c : utf32str)
    {
        Rect rect;

        Character* ch = renderer->getFontManager()->getGlyph(name, c);

        rect.x = position.x + xPosition;
        rect.y = position.y - (ch->top * scale);

        rect.width = ch->width * scale;
        rect.height = ch->rows * scale;

        addRectFilled(rect, color);

        commands.back().textureId = ch->textureId;

        commands.back().scissor = scissor;

        xPosition += (ch->advance >> 6) * scale;
    }

    /* Create hb-ft font. */
    // hb_font_t *hb_font;
    // hb_font = hb_ft_font_create(renderer->fontManager.fonts[name].face, NULL);

    // /* Create hb-buffer and populate. */
    // hb_buffer_t *hb_buffer;
    // hb_buffer = hb_buffer_create();
    // hb_buffer_add_utf8(hb_buffer, text.c_str(), -1, 0, -1);
    // hb_buffer_guess_segment_properties(hb_buffer);

    // /* Shape it! */
    // hb_shape(hb_font, hb_buffer, nullptr, 0);

    // /* Get glyph information and positions out of the buffer. */
    // unsigned int len = hb_buffer_get_length(hb_buffer);
    // hb_glyph_info_t *info = hb_buffer_get_glyph_infos(hb_buffer, NULL);
    // hb_glyph_position_t *pos = hb_buffer_get_glyph_positions(hb_buffer, NULL);

    // /* And converted to absolute positions. */
    // {
    //     double current_x = 0;
    //     double current_y = 0;
    //     for (unsigned int i = 0; i < len; i++) {
    //         hb_codepoint_t gid   = info[i].codepoint;
    //         // unsigned int cluster = info[i].cluster;
    //         double x_position = current_x + pos[i].x_offset / 64.;
    //         double y_position = current_y + pos[i].y_offset / 64.;
    //         x_position *= scale;
    //         y_position *= scale;

    //         char glyphname[32];
    //         hb_font_get_glyph_name(hb_font, gid, glyphname, sizeof(glyphname));

    //         if (strncmp(glyphname, "space", 5) != 0) {
    //             Rect rect;

    //             Character& ch = renderer->fontManager.fonts[name].characters[text[i]];

    //             rect.x = position.x + x_position;
    //             rect.y = position.y + y_position - (ch.top * scale);

    //             rect.width = ch.width * scale;
    //             rect.height = ch.rows * scale;

    //             addRectFilled(rect, color);

    //             commands.back().textureId = ch.textureId;

    //             commands.back().scissor = scissor;
    //         } else {
    //             double length_x = current_x + (pos[i].x_advance / 64.);
    //             for (unsigned int j = i + 1; j < len; j++) {
    //                 hb_codepoint_t nGid = info[j].codepoint;
    //                 char glyphname[32];
    //                 hb_font_get_glyph_name(hb_font, nGid, glyphname, sizeof(glyphname));

    //                 if (strncmp(glyphname, "space", 5) != 0) {
    //                     if (length_x * scale > position.width) {
    //                         current_x = -(pos[i].x_advance / 64.);
    //                         position.y += 24 * scale;
    //                     }

    //                     break;
    //                 } else {
    //                     length_x += pos[j].x_advance / 64.;
    //                 }
    //             }
    //         }

    //         current_x += pos[i].x_advance / 64.;
    //         current_y += pos[i].y_advance / 64.;
    //     }
    // }
}

float DrawData::measureText(const std::string& text, std::string name, int size) {
    float scale = (float)size / (float)24;
    float result = 0;

#if WIN32
    std::wstring_convert<std::codecvt_utf8<unsigned int>, unsigned int> converter{};
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter{};
#endif

    auto utf32str = converter.from_bytes(text);

    for (auto &character : utf32str) {
        Character* ch = renderer->getFontManager()->getGlyph(name, character);

        if (ch)
        {
            result += (ch->advance >> 6) * scale;
        }
    }

    return result;
}

Renderer::Renderer(Adaptor* adaptor)
    : mAdaptor{adaptor}
    , mFontManager{this}
    { }

Adaptor* Renderer::getAdaptor()
{
    return mAdaptor;
}

void Renderer::setAdaptor(Adaptor* adaptor)
{
    mAdaptor = adaptor;
}

FontManager* Renderer::getFontManager()
{
    return &mFontManager;
}

}  // namespace Chimera
