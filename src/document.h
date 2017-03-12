// Copyright 2016 Dan Ristic

#ifndef SRC_DOCUMENT_H_
#define SRC_DOCUMENT_H_

#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "src/element.h"
#include "src/style.h"
#include "src/focus.h"
#include "src/animation.h"

namespace Nova {

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

        return dynamic_cast<E*>(mElements.back().get());
    }

    void setDimensions(int width, int height);
    int getWidth();
    int getHeight();
    void setScreenDimensions(int width, int height);
    int getScreenWidth();
    int getScreenHeight();
    std::unique_ptr<ImageRef> loadImage(std::string src, int& width, int& height);
    void notifyRemoval(Element* element);

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
};

}  // namespace Nova

#endif  // SRC_DOCUMENT_H_
