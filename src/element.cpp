// Copyright 2016 Dan Ristic

#include "src/element.h"

#include <algorithm>

#include "src/core.h"
#include "src/document.h"
#include "src/rendering.h"

namespace Chimera {

Element::Element(std::string name, Document& document)
    : scissor{}
    , scroll{}
    , style{}
    , layout{&style}
    , tagName{name}
    , textContent{""}
    , mEventObservers{}
    , mChildren{}
    , mPseudoClass{PseudoClass::None}
    , mDocument{&document}
    , mBackgroundImage{nullptr}
    { }

void Element::append(Element* element) {
    mChildren.push_back(element);

    mDocument->styleManager.applyRules(element);

    std::stack<Element*> stack;
    stack.push(element);

    while (!stack.empty()) {
        Element* el = stack.top();
        stack.pop();

        for(auto& child : el->getChildren()) {
            stack.push(child);
        }

        style.inheritValues(el->style);
    }
}

void Element::remove(Element* element) {
    mChildren.erase(
        std::remove(mChildren.begin(), mChildren.end(), element),
        mChildren.end());

    mDocument->notifyRemoval(element);
}

const std::vector<Element*>& Element::getChildren() const {
    return mChildren;
}

void Element::on(EventType type, EventCallback func) {
    mEventObservers[type] = func;
}

void Element::dispatch(Event* event) {
    auto& type = event->type;
    if (mEventObservers.find(type) != mEventObservers.end()) {
        mEventObservers[type](event);
    }
}

bool Element::handleEvent(Event* event) {
    for (auto &element : mChildren) {
        if (!element->handleEvent(event)) {
            return false;
        }
    }

    return true;
}

void Element::render(DrawData* data) {
    style.update(this, mDocument->animationController);

    layout.calculateDimensions(mChildren);
    layout.layout();

    layout.updateChildren(mChildren);

    float previousAlpha = data->globalAlpha;
    data->globalAlpha *= style.opacity;

    for (auto &element : mChildren) {
        element->render(data);
    }

    data->globalAlpha = previousAlpha;
}

std::string Element::toHTML()
{
    std::string result = "<";

    result += tagName;

    if (id != "")
    {
        result += " id=" + id;
    }

    result += ">";

    return result;
}

Img::Img(Document& document)
    : Element("img", document)
    , mSrc{""}
    , mImageRef{nullptr}
    { }

Img::~Img() { }

void Img::render(DrawData* data) {
    layout.intrinsicWidth = style.width;
    layout.intrinsicHeight = style.height;

    layout.layout();

    if (mImageRef != nullptr) {
        mImageRef->render();
        data->addImage(layout.rect, mImageRef->textureId, style.opacity);
    }

    Element::render(data);
}

void Img::setSrc(std::string const& newSrc) {
    mSrc = newSrc;
    mImageRef = mDocument->loadImage(mSrc, style.width, style.height);
}

}  // namespace Chimera
