// Copyright 2016 Dan Ristic

#ifndef SRC_CONTEXT_H_
#define SRC_CONTEXT_H_

#include <string>
#include <memory>

#include "src/animation.h"
#include "src/document.h"
#include "src/rendering.h"
#include "src/types.h"
#include "src/component/component.h"
#include "src/adaptor/adaptor.h"

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
    void useAdaptor(std::shared_ptr<Adaptor> adaptor);
    std::unique_ptr<ImageRef> loadImage(std::string name, int& width, int& height);
    unsigned int loadTexture(unsigned int width, unsigned int height, unsigned char* buffer);

    Document document;
    Renderer renderer;
    ComponentManager component;

 private:
    int mMouseX, mMouseY, mScrollDirection;
    std::shared_ptr<Adaptor> mAdaptor;
};

}  // namespace Nova

#endif  // SRC_CONTEXT_H_
