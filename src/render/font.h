// Copyright 2017 Dan Ristic

#ifndef SRC_RENDER_FONT_H_
#define SRC_RENDER_FONT_H_

#include <string>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

//#include <harfbuzz/hb.h>
//#include <harfbuzz/hb-ft.h>

namespace Chimera {

class Renderer;

struct Character {
    unsigned int width;
    unsigned int rows;
    int left, top;
    int64_t advance;
    unsigned int textureId;
};

class Font {
 public:
    Font()
        : characters{}
        { }

    std::unordered_map<unsigned long, Character> characters;
    FT_Face face;
};

class FontManager {
 public:
    FontManager(Renderer* renderer);
    ~FontManager();

    void loadFont(std::string name, std::string location);
    Font* getFont(std::string name);

    void loadGlyph(std::string name, unsigned long glyph);
    Character* getGlyph(std::string fontName, unsigned long glyph);

 private:
    Renderer* mRenderer;
    FT_Library mFTLibrary;
    std::unordered_map<std::string, Font> mFonts{};
};

}  // namespace Chimera

#endif  // SRC_RENDER_FONT_H_