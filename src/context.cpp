// Copyright 2016 Dan Ristic

#include "src/context.h"

#include <string>

#include "src/element.h"
#include "src/document.h"
#include "src/rendering.h"

namespace Nova {
    Context::Context()
        : document{*this}
        , renderer{}
        , component{this}
        , mAdaptor{nullptr}
        , mMouseX{0}
        , mMouseY{0}
        { }

    Context::~Context() { }

    void Context::update(double dt) {

    }

    void Context::addInputCharacters(std::string characters) {
        TextInputEvent event{characters};
        document.body->handleEvent(dynamic_cast<Event*>(&event));
    }

    void Context::setKeyPress(KeyType key) {
        KeyEvent event{key};
        document.body->handleEvent(dynamic_cast<Event*>(&event));
    }

    void Context::setMousePosition(int x, int y) {
        if (x != mMouseX || y != mMouseY) {
            MouseMoveEvent event{x, y};
            document.body->handleEvent(dynamic_cast<Event*>(&event));
        }

        mMouseX = x;
        mMouseY = y;
    }

    void Context::setScrollDirection(int direction) {
        mScrollDirection = direction;

        MouseScrollEvent event{direction, mMouseX, mMouseY};
        document.body->handleEvent(dynamic_cast<Event*>(&event));
    }

    Vec2 Context::getMousePosition() {
        Vec2 position{static_cast<float>(mMouseX), static_cast<float>(mMouseY)};
        return position;
    }

    void Context::setMouseDown() {
        MouseDownEvent event{mMouseX, mMouseY};
        bool bubbled = document.body->handleEvent(dynamic_cast<Event*>(&event));

        if (bubbled) {
            document.focusManager.focusedElement = nullptr;
        }
    }

    void Context::useAdaptor(std::shared_ptr<Adaptor> adaptor) {
        mAdaptor = adaptor;
    }

    std::unique_ptr<ImageRef> Context::loadImage(std::string name, int& width, int& height) {
        return mAdaptor->loadImage(name, width, height);
    }

    unsigned int Context::loadTexture(unsigned int width, unsigned int height, unsigned char* buffer) {
        return mAdaptor->loadTexture(width, height, buffer);
    }

    void Context::render(float dt) {
        // TODO: Make this not happen every frame
        document.styleManager.applyRules(document.body);

        DrawData data;

        data.width = document.getWidth();
        data.height = document.getHeight();
        data.screenWidth = document.getScreenWidth();
        data.screenHeight = document.getScreenHeight();

        data.renderer = &renderer;

        if (document.body != nullptr) {
            document.body->render(&data, &document.globalStyle);
        }

        if (mAdaptor) {
            mAdaptor->renderCallback(&data);
        }
    }
}  // namespace Nova
