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

void Element::dispatch(EventType type, Event* event) {
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

Div::Div(Document& document)
    : Element("div", document)
    { }

Div::~Div() { }

bool Div::handleEvent(Event* event) {
    if (!Element::handleEvent(event)) {
        return false;
    }

    if (event->type == EventType::Scroll) {
        auto mouseScrollEvent = dynamic_cast<MouseScrollEvent*>(event);

        if (rect.contains(float(mouseScrollEvent->x), float(mouseScrollEvent->y))) {
            int internalHeight = 0;
            for (auto& child : mChildren) {
                internalHeight += int(child->rect.height);
            }

            if (rect.height < internalHeight) {
                scroll.y += mouseScrollEvent->direction * 5;
                return false;
            }
        }
    }

    return true;
}

void Div::render(DrawData* data) {
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

    // Render scrollbars
    // if (rect.height < internalHeight) {
    //     Rect scrollbar{
    //         rect.x + rect.width,
    //         rect.y - scroll.y,
    //         20,
    //         50
    //     };
    //     //data->addRectFilled(scrollbar, Color::fromRGBA(150, 150, 150, 0.7), scrollbar);
    // }

    layout.updateChildren(mChildren);

    float previousAlpha = data->globalAlpha;
    data->globalAlpha *= style.opacity;

    for (auto& element : mChildren)
    {
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

Input::Input(Document& document)
    : Element("input", document)
    , value{""}
    , mFrames{0}
    { }

Input::~Input() { }

bool Input::handleEvent(Event* event) {
    if (event->type == EventType::Input) {
        if (mDocument->focusManager.focusedElement == this) {
            auto textInputEvent = dynamic_cast<TextInputEvent*>(event);
            value += textInputEvent->value;

            TextInputEvent dispatchEvent{value};
            dispatch(EventType::Change, dynamic_cast<Event*>(&dispatchEvent));
        }
    } else if (event->type == EventType::Key) {
        if (mDocument->focusManager.focusedElement == this) {
            auto keyEvent = dynamic_cast<KeyEvent*>(event);

            switch (keyEvent->key) {
            case KeyType::Backspace:
            {
                value.pop_back();

                TextInputEvent dispatchEvent{value};
                dispatch(EventType::Change, dynamic_cast<Event*>(&dispatchEvent));

                break;
            }
            default:
                break;
            }
        }
    } else if (event->type == EventType::MouseDown) {
        auto mouseDownEvent = dynamic_cast<MouseDownEvent*>(event);

        if (layout.rect.contains(float(mouseDownEvent->x), float(mouseDownEvent->y))) {
            mDocument->focusManager.focusElement(this);
            return false;
        }
    } else if (event->type == EventType::MouseMove) {
        MouseMoveEvent* mouseMoveEvent = dynamic_cast<MouseMoveEvent*>(event);
        if (layout.rect.contains(float(mouseMoveEvent->x), float(mouseMoveEvent->y))) {
            mDocument->setCursorType(CursorType::IBeam);
        }
    }

    return true;
}

void Input::render(DrawData* data) {
    style.update(this, mDocument->animationController);

    if (layout.intrinsicHeight < style.fontSize)
    {
        layout.intrinsicHeight = style.fontSize;
    }

    // Layout
    layout.layout();

    scissor.x = layout.rect.x;
    scissor.y = layout.rect.y;
    scissor.width = layout.rect.width;
    scissor.height = layout.rect.height;

    Rect borderRect;
    borderRect.x = layout.rect.x;
    borderRect.y = layout.rect.y;
    borderRect.width = layout.rect.width;
    borderRect.height = layout.rect.height;

    if (borderRect.height < 30) borderRect.height = 30;

    if (style.borderWidth > 0) {
        data->addRectFilled(borderRect, style.borderColor);

        borderRect.width -= style.borderWidth * 2;
        borderRect.height -= style.borderWidth * 2;
        borderRect.x += style.borderWidth;
        borderRect.y += style.borderWidth;
    }

    data->addRectFilled(borderRect, style.backgroundColor);

    float padTop = (borderRect.height - style.fontSize) / 2;
    padTop = style.padding.top == 0 ? padTop : style.padding.top;
    bool isFocused = mDocument->focusManager.focusedElement == this;

    if (value != "") {
        float textWidth = data->measureText(value, style.fontFamily, style.fontSize);
        float pad = textWidth > borderRect.width - 30
            ? textWidth - borderRect.width + 30
            : 0;

        Rect position{
            borderRect.x + 5 + style.padding.left - pad,
            borderRect.y + padTop,
            borderRect.width,
            borderRect.height
        };

        data->addText(position, value, style.fontFamily, style.fontSize, borderRect, style.color);
    } else if (placeholder != "" && !isFocused) {
        float textWidth = data->measureText(value, style.fontFamily, style.fontSize);
        float pad = textWidth > borderRect.width - 30
            ? textWidth - borderRect.width + 30
            : 0;

        Rect position{
            borderRect.x + 5 + style.padding.left - pad,
            borderRect.y + padTop,
            borderRect.width,
            borderRect.height
        };

        data->addText(position, placeholder, style.fontFamily, style.fontSize, borderRect, Color::fromRGBA(150, 150, 150, 0.6f));
    }

    // Draw input line
    if (isFocused) {
        mFrames++;
        if (mFrames < 15) {
            float textWidth = data->measureText(value, style.fontFamily, style.fontSize);
            Rect inputLine{layout.rect.x + 2 + textWidth, layout.rect.y + 2, 1, layout.rect.height - 4};
            data->addRectFilled(inputLine, style.color);
        } else {
            if (mFrames > 30) {
                mFrames = 0;
            }
        }
    }
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
            dispatch(EventType::MouseDown, event);
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
