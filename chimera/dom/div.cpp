// Copyright 2017 Dan Ristic

#include "chimera/dom/div.h"

#include "chimera/document.h"
#include "chimera/rendering.h"

namespace Chimera {

Div::Div(Document& document)
    : Element("div", document)
    { }

Div::~Div() { }

bool Div::handleEvent(Event* event) {
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

    // Layout
    layout.calculateDimensions(mChildren);

    if (textWidth > layout.intrinsicWidth)
    {
        layout.intrinsicWidth = int(textWidth);
    }

    if (layout.intrinsicHeight < style.fontSize && textContent != "")
    {
        layout.intrinsicHeight = style.fontSize + 10;
    }

    layout.layout();

    // Rendering
    scissor.x = layout.rect.x;
    scissor.y = layout.rect.y;
    scissor.width = layout.rect.width;
    scissor.height = layout.rect.height;

    if (dirty > 0)
    {
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

        // dirty--;
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

    // Updating and propagation
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