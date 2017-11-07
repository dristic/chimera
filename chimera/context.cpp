// Copyright 2017 Dan Ristic

#include "chimera/context.h"

#include <string>

#include "chimera/core.h"
#include "chimera/adaptor/adaptor.h"
#include "chimera/element.h"
#include "chimera/document.h"
#include "chimera/rendering.h"

namespace Chimera {
    Context::Context()
        : document{*this}
        , mMouseX{0}
        , mMouseY{0}
        , mAdaptor{}
    {
        mAdaptor = std::make_shared<Adaptor>();
        mRenderer = std::make_unique<Renderer>(mAdaptor.get());
    }

    Context::~Context() { }

    void Context::update(double dt) {
        CHIMERA_UNUSED(dt);
    }

    void Context::addInputCharacters(std::string characters) {
        TextInputEvent event{characters};

        if (document.focusManager.focusedElement)
        {
            document.focusManager.focusedElement->handleEvent(
                static_cast<Event*>(&event));
        }
    }

    void Context::setKeyPress(KeyType key) {
        KeyEvent event{key};

        if (document.focusManager.focusedElement)
        {
            document.focusManager.focusedElement->handleEvent(
                static_cast<Event*>(&event));
        }
    }

    void Context::setMousePosition(int x, int y) {
        document.setCursorType(CursorType::Arrow);

        MouseMoveEvent event{x, y};
        event.target = document.elementFromPoint(x, y);

        if (event.target)
        {
            if (mLastMouseOverTarget && mLastMouseOverTarget != event.target)
            {
                MouseEvent outEvent{x, y, EventType::MouseOut};
                mLastMouseOverTarget->handleEvent(
                    static_cast<Event*>(&outEvent));
            }

            mLastMouseOverTarget = event.target;
            event.target->handleEvent(dynamic_cast<Event*>(&event));
        }

        mMouseX = x;
        mMouseY = y;
    }

    void Context::setScrollDirection(int direction) {
        mScrollDirection = direction;

        MouseScrollEvent event{direction, mMouseX, mMouseY};
        event.target = document.elementFromPoint(mMouseX, mMouseY);
        event.target->handleEvent(static_cast<Event*>(&event));
    }

    Vec2 Context::getMousePosition() {
        Vec2 position{static_cast<float>(mMouseX), static_cast<float>(mMouseY)};
        return position;
    }

    void Context::setMouseDown() {
        MouseDownEvent event{mMouseX, mMouseY};
        event.target = document.elementFromPoint(mMouseX, mMouseY);

        document.focusManager.focusedElement = event.target;

        event.target->handleEvent(dynamic_cast<Event*>(&event));
    }

    void Context::setDoubleClick()
    {
        Event event{EventType::DoubleClick};
        event.target = document.elementFromPoint(mMouseX, mMouseY);

        event.target->handleEvent(&event);
    }

    void Context::useAdaptor(std::shared_ptr<Adaptor> adaptor) {
        mAdaptor = adaptor;
        mRenderer->setAdaptor(mAdaptor.get());
    }

    std::unique_ptr<ImageRef> Context::loadImage(
        std::string name, int& width, int& height)
    {
        return mAdaptor->loadImage(name, width, height);
    }

    unsigned int Context::loadTexture(
        unsigned int width, unsigned int height, unsigned char* buffer)
    {
        return mAdaptor->loadTexture(width, height, buffer);
    }

    void Context::loadFont(std::string name, std::string location)
    {
        mRenderer->getFontManager()->loadFont(name, location);
    }

    void Context::notifyRemoval(Element* element)
    {
        if (mLastMouseOverTarget == element)
        {
            mLastMouseOverTarget = nullptr;
        }
    }

    void Context::render(float dt) {
        CHIMERA_UNUSED(dt);

        // TODO: Push this into renderer and cache it
        DrawData data;

        data.width = document.getWidth();
        data.height = document.getHeight();
        data.screenWidth = document.getScreenWidth();
        data.screenHeight = document.getScreenHeight();

        data.renderer = mRenderer.get();

        if (document.body != nullptr) {
            document.body->render(&data);
        }

        if (mAdaptor) {
            mAdaptor->renderCallback(&data);
        }
    }
}  // namespace Chimera
