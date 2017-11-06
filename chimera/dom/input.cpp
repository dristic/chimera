// Copyright 2017 Dan Ristic

#include "chimera/dom/input.h"

#include "chimera/core.h"
#include "chimera/document.h"
#include "chimera/rendering.h"

namespace Chimera {

// const char ICN_BULLET[] = "\u2022";
const char ICN_CIRCLE[] = "\u25cf";
const char ICN_CHECK[] = "\u2713";

Input::Input(Document& document)
    : Element("input", document)
    , value{""}
    , mFrames{0}
    { }

Input::~Input() { }

void Input::attributeChangedCallback(
        std::string name, std::string oldValue, std::string newValue)
{
    CHIMERA_UNUSED(oldValue);

    if (name == "type")
    {
        mType = newValue;
    }
}

bool Input::handleEvent(Event* event) {
    if (event->type == EventType::Input)
    {
        if (mDocument->focusManager.focusedElement == this) {
            auto textInputEvent = dynamic_cast<TextInputEvent*>(event);
            value += textInputEvent->value;

            TextInputEvent dispatchEvent{value};
            dispatchEvent.type = EventType::Change;
            dispatch(dynamic_cast<Event*>(&dispatchEvent));
        }
    }
    else if (event->type == EventType::Key)
    {
        if (mDocument->focusManager.focusedElement == this) {
            auto keyEvent = dynamic_cast<KeyEvent*>(event);

            switch (keyEvent->key) {
            case KeyType::Backspace:
            {
                if (value != "")
                {
                    value.pop_back();
                }

                TextInputEvent dispatchEvent{value};
                dispatchEvent.type = EventType::Change;
                dispatch(dynamic_cast<Event*>(&dispatchEvent));

                break;
            }
            default:
                break;
            }
        }
    }
    else if (event->type == EventType::MouseDown)
    {
        auto mouseDownEvent = dynamic_cast<MouseDownEvent*>(event);

        if (layout.rect.contains(float(mouseDownEvent->x), float(mouseDownEvent->y)))
        {
            mDocument->focusManager.focusElement(this);

            if (mType == "checkbox")
            {
                value = value == "checked" ? "" : "checked";
            }

            return false;
        }
        else
        {
            mSelectionStart = mSelectionEnd = 0;
        }
    }
    else if (event->type == EventType::MouseMove)
    {
        MouseMoveEvent* mouseMoveEvent = dynamic_cast<MouseMoveEvent*>(event);
        if (layout.rect.contains(float(mouseMoveEvent->x), float(mouseMoveEvent->y)))
        {
            if (mType == "text" || mType == "password")
            {
                mDocument->setCursorType(CursorType::IBeam);
            }
        }
    }
    else if (event->type == EventType::DoubleClick)
    {
        if (mDocument->focusManager.focusedElement == this)
        {
            mSelectionStart = 0;
            mSelectionEnd = value.length();
            return false;
        }
    }

    return true;
}

std::string Input::filterText(std::string inValue)
{
    if (mType == "password")
    {
        std::string filtered{""};

        for (size_t i = 0; i < inValue.length(); i++)
        {
            filtered += std::string(ICN_CIRCLE);
        }

        return filtered;
    }

    return inValue;
}

void Input::render(DrawData* data)
{
    style.update(this, mDocument->animationController);

    if (layout.intrinsicHeight < style.fontSize)
    {
        layout.intrinsicHeight = style.fontSize;
    }

    float defaultCharacter = data->measureText("W", "system", style.fontSize);

    if (mType == "text" || mType == "password")
    {
        layout.intrinsicWidth = defaultCharacter * 20; // default "size"
    }
    else if (mType == "checkbox")
    {
        layout.intrinsicWidth = defaultCharacter;
    }

    // Do layout
    layout.layout();

    scissor.x = layout.rect.x;
    scissor.y = layout.rect.y;
    scissor.width = layout.rect.width;
    scissor.height = layout.rect.height;

    // Draw border rectangle
    if (style.borderWidth > 0) {
        Rect borderRect;
        borderRect.x = layout.rect.x - style.borderWidth;
        borderRect.y = layout.rect.y - style.borderWidth;
        borderRect.width = layout.rect.width + (style.borderWidth * 2);
        borderRect.height = layout.rect.height + (style.borderWidth * 2);

        data->addRectFilled(borderRect, style.borderColor);
    }

    // Draw normal background
    data->addRectFilled(layout.rect, style.backgroundColor);

    if (mType == "text" || mType == "password")
    {
        renderAsText(data);
    }
    else if (mType == "checkbox")
    {
        renderAsCheckbox(data);
    }
}

void Input::renderAsText(DrawData* data)
{
    bool isFocused = mDocument->focusManager.focusedElement == this;

    auto renderText = value != "" ?
        filterText(value) : placeholder;
    auto fontFamily = (value != "" && mType == "password") ?
        "system" : style.fontFamily;
    float textWidth = data->measureText(
        renderText, fontFamily, style.fontSize);

    // Draw current text
    if (renderText != "") {
        auto color = value != "" ?
            style.color : Color::fromRGBA(150, 150, 150, 0.6f);
        float pad = textWidth > layout.rect.width ?
            layout.rect.width - textWidth : 0;

        Rect position{
            layout.rect.x + pad,
            layout.rect.y + ((layout.rect.height - style.fontSize) / 2),
            layout.rect.width,
            layout.rect.height
        };

        data->addText(
            position,
            renderText,
            fontFamily,
            style.fontSize,
            layout.rect,
            color);
    }

    // Draw input line
    if (isFocused) {
        mFrames++;
        if (mFrames < 15) {
            Rect inputLine{
                layout.rect.x + textWidth,
                layout.rect.y + ((layout.rect.height - style.fontSize) / 2),
                1,
                float(style.fontSize)
            };

            data->addRectFilled(inputLine, style.color);
        } else {
            if (mFrames > 30) {
                mFrames = 0;
            }
        }
    }

    // Draw selection box
    if (mSelectionStart != mSelectionEnd)
    {
        float deltaWidth = 0;
        if (mSelectionStart > 0)
        {
            auto prefix = filterText(value).substr(0, mSelectionStart);
            deltaWidth = data->measureText(
                prefix, style.fontFamily, style.fontSize);
        }

        auto content = filterText(value).substr(mSelectionStart, mSelectionEnd);
        float contentWidth = data->measureText(
            content, style.fontFamily, style.fontSize);

        Rect highlight{
            layout.rect.x + deltaWidth,
            layout.rect.y + ((layout.rect.height - style.fontSize) / 2),
            contentWidth,
            static_cast<float>(style.fontSize)
        };

        data->addRectFilled(highlight, Color::fromRGBA(255, 255, 255, 0.5));
    }
}

void Input::renderAsCheckbox(DrawData* data)
{
    // bool isFocused = mDocument->focusManager.focusedElement == this;

    auto renderText = value == "checked" ? std::string(ICN_CHECK) : "";
    // float textWidth = data->measureText(
    //     renderText, "system", style.fontSize);

    if (renderText != "") {
        Rect position{
            layout.rect.x,
            layout.rect.y + ((layout.rect.height - style.fontSize) / 2),
            layout.rect.width,
            layout.rect.height
        };

        // Draw normal background
        data->addRectFilled(layout.rect, Color::fromRGBA(255, 255, 255, 0.1));

        data->addText(
            position,
            renderText,
            "system",
            style.fontSize,
            layout.rect,
            style.color);
    }
}

}  // namespace Chimera
