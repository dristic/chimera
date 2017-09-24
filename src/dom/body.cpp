// Copyright 2017 Dan Ristic

#include "src/dom/body.h"

#include "src/document.h"
#include "src/rendering.h"

namespace Chimera {

Body::Body(Document& document)
    : Element("body", document)
    { }

Body::~Body() { }

bool Body::handleEvent(Event* event) {
    if (event->type == EventType::Key)
    {
        auto keyEvent = dynamic_cast<KeyEvent*>(event);

        if (keyEvent->key == KeyType::Tab)
        {
            std::stack<Element*> stack;

            stack.push(this);

            while (!stack.empty()) {
                Element* el = stack.top();
                stack.pop();

                if (el->tagName == "input")
                {
                    mDocument->focusManager.focusElement(el);
                    break;
                }

                auto children = el->getChildren();
                for (auto it = children.rbegin(); it != children.rend(); ++it)
                {
                    stack.push(*it);
                }
            }
        }
    }

    if (!Element::handleEvent(event)) {
        return false;
    }

    return true;
}

void Body::render(DrawData* data) {
    style.update(this, mDocument->animationController);

    float textWidth = data->measureText(textContent, style.fontFamily, style.fontSize);

    layout.calculateDimensions(mChildren);

    if (textWidth > layout.intrinsicWidth)
    {
        layout.intrinsicWidth = int(textWidth);
    }

    if (layout.intrinsicHeight < style.fontSize && textContent != "")
    {
        layout.intrinsicHeight = style.fontSize + 10;
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

#define CHIMERA_DEBUG_RENDER 0
#if CHIMERA_DEBUG_RENDER
    if (style.backgroundColor.a == 0)
    {
        style.backgroundColor = Color::fromRGBA(rand() % 255, rand() % 255, rand() % 255, 1);
    }
#endif

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