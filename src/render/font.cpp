// Copyright 2017 Dan Ristic

#include "src/render/font.h"

#include "src/rendering.h"
#include "src/adaptor/adaptor.h"

namespace Chimera {

FontManager::FontManager(Renderer* renderer)
    : mRenderer{renderer}
{
    if (FT_Init_FreeType(&mFTLibrary)) {
        printf("Could not init FreeType library\n");
    }
}

void FontManager::loadFont(std::string name, std::string location)
{
    FT_Face face;
    if (FT_New_Face(mFTLibrary, location.c_str(), 0, &face)) {
        printf("FreeType could not load the font: %s\n", name.c_str());
        return;
    }

    FT_Select_Charmap(face , ft_encoding_unicode);

#if WIN32
    FT_Set_Char_Size(
        face,    /* handle to face object           */
        0,       /* char_width in 1/64th of points  */
        24 * 64,   /* char_height in 1/64th of points */
        96,     /* horizontal device resolution    */
        96);   /* vertical device resolution      */
#else
    FT_Set_Char_Size(
        face,    /* handle to face object           */
        0,       /* char_width in 1/64th of points  */
        24 * 64,   /* char_height in 1/64th of points */
        72,     /* horizontal device resolution    */
        72);   /* vertical device resolution      */
#endif

    Font newFont;
    newFont.face = face;

    mFonts[name] = newFont;

    // unsigned long filter = (unsigned long)0x2022;
    // auto index = FT_Get_Char_Index(face, filter);
    // printf("Got character index %u\n", index);

    // Cache a preset of glyphs
    for (unsigned long c = 32; c < 128; c++) {
        loadGlyph(name, c);
    }
}

void FontManager::loadGlyph(std::string name, unsigned long glyph)
{
    auto font = getFont(name);
    auto adaptor = mRenderer->getAdaptor();
    auto face = font->face;

    unsigned long index = FT_Get_Char_Index(face, glyph);

    if (FT_Load_Char(face, glyph, FT_LOAD_RENDER)) {
        printf("FreeType failed to load glyph %lu\n", glyph);
        return;
    }

    size_t size =
        sizeof(unsigned char)
        * face->glyph->bitmap.width
        * face->glyph->bitmap.rows;

    font->characters.insert(std::pair<unsigned long, Character>(index, {
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        face->glyph->bitmap_left,
        face->glyph->bitmap_top,
        face->glyph->advance.x,
        0,
    }));

    auto buffer = static_cast<unsigned char*>(malloc(size * 4));

    for (size_t i = 0; i < size; i++) {
        int p = int(i) * 4;
        buffer[p] = 255;
        buffer[p+1] = 255;
        buffer[p+2] = 255;
        buffer[p+3] = face->glyph->bitmap.buffer[i];
    }

    font->characters[index].textureId = adaptor->loadTexture(
        font->characters[index].width,
        font->characters[index].rows,
        buffer);

    free(buffer);
}

Font* FontManager::getFont(std::string name)
{
    if (mFonts.count(name) == 1)
    {
        return &mFonts[name];
    }
    
    return nullptr;
}

Character* FontManager::getGlyph(std::string fontName, unsigned long glyph)
{
    auto font = getFont(fontName);

    if (!font)
    {
        return nullptr;
    }

    auto index = FT_Get_Char_Index(font->face, glyph);

    if (font->characters.count(index) == 0)
    {
        loadGlyph(fontName, glyph);
    }

    return &font->characters[index];
}

FontManager::~FontManager()
{
    FT_Done_FreeType(mFTLibrary);
}

}  // namespace Chimera