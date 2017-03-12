// Copyright 2016 Dan Ristic

#ifndef SRC_RENDERING_H_
#define SRC_RENDERING_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <hb.h>
#include <hb-ft.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "src/types.h"

namespace Nova {

class Context;

struct Vec2 {
    float x, y;
    Vec2() { x = y = 0.0f; }
    Vec2(float _x, float _y)
        : x(_x)
        , y(_y)
        { }
};

struct Vec4 {
    float x, y, z, w;
    Vec4() { x = y = z = w = 0.0f; }
    Vec4(float _x, float _y, float _z, float _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
        { }
};

struct DrawVert {
    Vec2 pos;
    Vec2 uv;
    Vec4 color;

    DrawVert(float x, float y, float u, float v,
             float r, float g, float b, float a)
        : pos(x, y)
        , uv(u, v)
        , color(r, g, b, a)
        { }
};

static unsigned int TEXTURE_ZERO = 0;

class DrawCommand {
 public:
    unsigned int textureId;
    Rect scissor;
    unsigned int elements;

    DrawCommand()
        : textureId{TEXTURE_ZERO}
        , elements{0}
        { }

    DrawCommand(const DrawCommand&) = delete;
    DrawCommand& operator=(const DrawCommand&) = delete;
    DrawCommand(DrawCommand&& other) noexcept = default;
    DrawCommand& operator=(DrawCommand&& other) noexcept = default;

    ~DrawCommand() {}
};

struct Character {
    // unsigned char* buffer;
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

    ~Font() {
        // for (auto &character : characters) {
        //     free(character.second.buffer);
        // }
    }

    std::unordered_map<unsigned char, Character> characters;
    FT_Face face;
};

class FontManager {
 public:
    FontManager()
        : fonts{}
        { }

    ~FontManager()  { }

    std::unordered_map<std::string, Font> fonts;
};

class Renderer {
 public:
    Renderer();
    ~Renderer();

    void loadFont(Context& context, std::string name, std::string location);

    FT_Library ft;
    FontManager fontManager;
};

class DrawData {
 public:
    DrawData();
    ~DrawData();

    void addRectFilled(const Rect& rect, const Color& color);
    void addRectFilled(
        const Rect& rect,
        const Color& color,
        const Rect& scissor);
    void addImage(const Rect& rect, unsigned int textureId);
    void addImage(const Rect& rect, unsigned int textureId, float opacity);
    void addText(
        Rect position,
        const std::string& text,
        std::string name,
        int size,
        const Rect& scissor,
        const Color& color);
    float measureText(const std::string& text, std::string name, int size);

    std::vector<DrawVert> vertices;
    std::vector<unsigned int> indicies;
    std::vector<DrawCommand> commands;

    float globalAlpha = 1.0f;

    Renderer* renderer;

    int width;
    int height;
    int screenWidth;
    int screenHeight;

 private:
    unsigned int mCurIndex;
};

}  // namespace Nova

#endif  // SRC_RENDERING_H_
