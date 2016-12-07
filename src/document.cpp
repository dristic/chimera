// Copyright 2016 Dan Ristic

#include "src/document.h"

#include <algorithm>
#include <stack>
#include <utility>

#include "src/context.h"

namespace Nova {

Document::Document(Context& context)
    : body{nullptr}
    , styleManager{}
    , focusManager{}
    , mElements{}
    , mContext{&context}
{
    body = createElement<Div>();
}

Document::~Document() { }

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

}  // namespace Nova
