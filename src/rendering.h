// Copyright 2017 Dan Ristic

#ifndef SRC_RENDERING_H_
#define SRC_RENDERING_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <cstdlib>

#include "src/types.h"
#include "src/render/font.h"

namespace Chimera {

class Context;
class Adaptor;

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

class Renderer {
 public:
    Renderer(Adaptor* adaptor);

    Adaptor* getAdaptor();
    void setAdaptor(Adaptor* adaptor);
    FontManager* getFontManager();

 private:
    Adaptor* mAdaptor;
    FontManager mFontManager;
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

}  // namespace Chimera

#endif  // SRC_RENDERING_H_
