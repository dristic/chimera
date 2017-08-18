// Copyright 2016 Dan Ristic

#include "src/document.h"

#include <algorithm>
#include <stack>
#include <utility>

#include "src/context.h"
#include "src/dom/body.h"
#include "src/dom/div.h"
#include "src/dom/input.h"

namespace Chimera {

Document::Document(Context& context)
    : body{nullptr}
    , styleManager{body}
    , focusManager{}
    , animationController{}
    , mElements{}
    , mContext{&context}
    , mCursorType{CursorType::Arrow}
{
    // Default element set
    registerElement<Body>("body");
    registerElement<Div>("div");
    registerElement<Img>("img");
    registerElement<Input>("input");
    registerElement<Button>("button");

    body = createElement<Body>();

    styleManager.setGlobalRoot(body);
}

Document::~Document() { }

void Document::setDimensions(int width, int height) {
    mWidth = width;
    mHeight = height;
}

int Document::getWidth() {
    return mWidth;
}

int Document::getHeight() {
    return mHeight;
}

void Document::setScreenDimensions(int width, int height) {
    mScreenWidth = width;
    mScreenHeight = height;
}

int Document::getScreenWidth() {
    return mScreenWidth;
}

int Document::getScreenHeight() {
    return mScreenHeight;
}

std::unique_ptr<ImageRef> Document::loadImage(std::string src, int& width, int& height) {
    return mContext->loadImage(src, width, height);
}

void Document::notifyRemoval(Element* element) {
    std::stack<Element*> stack;

    stack.push(element);

    while (!stack.empty()) {
        Element* el = stack.top();
        stack.pop();

        for(auto& child : el->getChildren()) {
            stack.push(child);
        }

        mElements.erase(
            std::remove_if(mElements.begin(), mElements.end(),
                [&el](auto& other) {
                    return other.get() == el;
                }),
            mElements.end());
    }
}

void Document::setCursorType(CursorType type) {
    mCursorType = type;
}

CursorType Document::getCursorType() {
    return mCursorType;
}

}  // namespace Chimera
