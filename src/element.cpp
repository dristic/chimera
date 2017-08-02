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

Button::Button(Document& document)
    : Element("button", document)
    { }

Button::~Button() { }

bool Button::handleEvent(Event* event) {
    if (event->type == EventType::MouseMove) {
        MouseMoveEvent* mouseMoveEvent = dynamic_cast<MouseMoveEvent*>(event);
        if (layout.rect.contains(float(mouseMoveEvent->x), float(mouseMoveEvent->y))) {
            mPseudoClass = PseudoClass::Hover;
            mDocument->setCursorType(CursorType::Hand);
        } else {
            mPseudoClass = PseudoClass::None;
        }
    } else if (event->type == EventType::MouseDown) {
        MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(event);
        if (layout.rect.contains(float(mouseDownEvent->x), float(mouseDownEvent->y))) {
            dispatch(event);
            return false;
        }
    }

    return true;
}

void Button::render(DrawData* data) {
    style.update(this, mDocument->animationController);

    float textWidth = data->measureText(textContent, style.fontFamily, style.fontSize);

    layout.calculateDimensions(mChildren);

    if (textWidth > layout.intrinsicWidth)
    {
        layout.intrinsicWidth = int(textWidth);
    }

    if (layout.intrinsicHeight < style.fontSize && textContent != "")
    {
        layout.intrinsicHeight = style.fontSize;
    }

    // Layout
    layout.layout();

    scissor.x = layout.rect.x;
    scissor.y = layout.rect.y;
    scissor.width = layout.rect.width;
    scissor.height = layout.rect.height;

    // Rendering
    if (style.backgroundImage != "") {
        if (mBackgroundImage == nullptr) {
            mBackgroundImage = mDocument->loadImage(style.backgroundImage,
                style.width, style.height);
        }
        data->addImage(layout.rect, mBackgroundImage->textureId);
    } else {
        data->addRectFilled(layout.rect, style.backgroundColor);
    }

    if (textContent != "") {
        Rect position{
            layout.rect.x + style.padding.left,
            layout.rect.y + style.padding.top,
            layout.rect.width,
            layout.rect.height
        };

        if (style.textAlign == Align::Center) {
            position.x = layout.rect.x + (layout.rect.width / 2) - (textWidth / 2);
        }

        auto newColor = style.color;
        newColor.a *= style.opacity;
        data->addText(position, textContent, style.fontFamily, style.fontSize, scissor, newColor);

        if (style.textDecoration == TextDecoration::Underline) {
            Rect decorationRect{position.x, position.y + style.fontSize, textWidth, 2};
            data->addRectFilled(decorationRect, style.color, scissor);
        }
    }

    layout.updateChildren(mChildren);

    float previousAlpha = data->globalAlpha;
    data->globalAlpha *= style.opacity;

    for (auto& element : mChildren)
    {
        element->render(data);
    }

    data->globalAlpha = previousAlpha;
}

}  // namespace Chimera
