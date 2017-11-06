// Copyright 2016 Dan Ristic

#ifndef CHIMERA_DOCUMENT_H_
#define CHIMERA_DOCUMENT_H_

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "chimera/core.h"
#include "chimera/element.h"
#include "chimera/style/style.h"
#include "chimera/focus.h"
#include "chimera/animation.h"

namespace Chimera {

class Context;

class Document {
 public:
    Document(Context& context);
    ~Document();

    template<class E, typename... Args>
    E* createElement(Args&&... args) {
        static_assert(
            std::is_base_of<Element, E>::value,
            "Type must be a base class of Element.");

        std::unique_ptr<E> el{new E(*this, std::forward<Args...>(args)...)};

        mElements.push_back(std::move(el));

        return static_cast<E*>(mElements.back().get());
    }

    using ElementFactory = std::function<Element*(Document&)>;
    std::unordered_map<std::string, ElementFactory> elementTypes{};

    template<class E>
    void registerElement(std::string name)
    {
        elementTypes[name] = [](Document& document) {
            return static_cast<Element*>(document.createElement<E>());
        };
    }

    Element* createElement(std::string name)
    {
        if (elementTypes.count(name) > 0)
        {
            return elementTypes[name](*this);
        }
        else
        {
            CHIMERA_DEBUG(printf("[ERROR] Element not found: %s\n", name.c_str()));
            return nullptr;
        }
    }

    void setDimensions(int width, int height);
    int getWidth();
    int getHeight();
    void setScreenDimensions(int width, int height);
    int getScreenWidth();
    int getScreenHeight();

    // https://developer.mozilla.org/en-US/docs/Web/API/Document/elementFromPoint
    Element* elementFromPoint(int x, int y);

    std::unique_ptr<ImageRef> loadImage(std::string src, int& width, int& height);
    void notifyRemoval(Element* element);
    void setCursorType(CursorType type);
    CursorType getCursorType();

    Style globalStyle;
    Element* body;
    StyleManager styleManager;
    FocusManager focusManager;
    AnimationController animationController;

 private:
    std::vector<std::unique_ptr<Element>> mElements;
    Context* mContext;

    int mWidth;
    int mHeight;
    int mScreenWidth;
    int mScreenHeight;

    CursorType mCursorType;
};

}  // namespace Chimera

#endif  // CHIMERA_DOCUMENT_H_
