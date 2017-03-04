// Copyright 2016 Dan Ristic

#ifndef SRC_CONTEXT_H_
#define SRC_CONTEXT_H_

#include <string>

#include "src/animation.h"
#include "src/document.h"
#include "src/rendering.h"
#include "src/types.h"
#include "src/component/component.h"

namespace Nova {

using namespace Cosmonaut;

class Context {
 public:
    Context();
    ~Context();

    void update(double dt);
    void render(float dt);
    void addInputCharacters(std::string characters);
    void setKeyPress(KeyType key);
    void setMousePosition(int x, int y);
    void setScrollDirection(int direction);
    Vec2 getMousePosition();
    void setMouseDown();

    Document document;
    Renderer renderer;
    ComponentManager component;

    std::function<void(DrawData*)> renderCallback;
    std::function<std::unique_ptr<ImageRef>(std::string, int&, int&)> loadImage;

 private:
    int mMouseX, mMouseY, mScrollDirection;
};

}  // namespace Nova

#endif  // SRC_CONTEXT_H_
