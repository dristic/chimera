// Copyright 2017 Dan Ristic

#include "src/dom/button.h"

#include "src/document.h"
#include "src/rendering.h"

namespace Chimera {

Button::Button(Document& document)
    : Element("button", document)
    { }

Button::~Button() { }

bool Button::handleEvent(Event* event) {
    if (event->type == EventType::MouseMove) {
        mPseudoClass = PseudoClass::Hover;
        mDocument->setCursorType(CursorType::Hand);
        mDocument->styleManager.applyRules(this);
    }
    else if (event->type == EventType::MouseOut) {
        mPseudoClass = PseudoClass::None;
        mDocument->styleManager.applyRules(this);
    }
    else if (event->type == EventType::MouseDown) {
        MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(event);
        if (layout.rect.contains(float(mouseDownEvent->x), float(mouseDownEvent->y))) {
            event->target = this;
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