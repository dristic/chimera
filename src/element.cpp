// Copyright 2016 Dan Ristic

#include "src/element.h"

#include <algorithm>

#include "src/document.h"

namespace Nova {

Element::Element(std::string name, Document& document)
    : layout{}
    , scissor{}
    , scroll{}
    , textContent{""}
    , style{}
    , tagName{name}
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

std::vector<Element*>& Element::getChildren() {
    return mChildren;
}

void Element::on(EventType type, EventCallback func) {
    mEventObservers[type] = func;
}

void Element::dispatch(EventType type, Event* event) {
    mEventObservers[type](event);
}

bool Element::handleEvent(Event* event) {
    for (auto &element : mChildren) {
        if (!element->handleEvent(event)) {
            return false;
        }
    }

    return true;
}

void Element::render(DrawData* data, Style* parentStyle) {
    float previousAlpha = data->globalAlpha;
    data->globalAlpha *= style.opacity;

    for (auto &element : mChildren) {
        element->render(data, &style);
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

        if (rect.contains(mouseScrollEvent->x, mouseScrollEvent->y)) {
            int internalHeight = 0;
            for (auto& child : mChildren) {
                internalHeight += child->rect.height;
            }

            if (rect.height < internalHeight) {
                scroll.y += mouseScrollEvent->direction * 5;
                return false;
            }
        }
    }

    return true;
}

void Div::render(DrawData* data, Style* parentStyle) {
    rect.width = style.width;
    rect.height = style.height;

    if (style.position == Position::Absolute) {
        rect.x = style.left;
        rect.y = style.top;
        scissor.x = rect.x;
        scissor.y = rect.y;
        scissor.width = rect.width;
        scissor.height = rect.height;
    }

    if (style.backgroundImage != "") {
        if (mBackgroundImage == nullptr) {
            mBackgroundImage = mDocument->loadImage(style.backgroundImage,
                style.width, style.height);
        }
        data->addImage(rect, mBackgroundImage->textureId);
    } else {
        data->addRectFilled(rect, style.backgroundColor);
    }

    if (textContent != "") {
        Rect position{
            rect.x + style.padding.left,
            rect.y + style.padding.top,
            rect.width,
            rect.height
        };

        if (style.textAlign == Align::Center) {
            float width = data->measureText(textContent, style.fontFamily, style.fontSize);
            position.x = rect.x + (rect.width / 2) - (width / 2);
        }

        data->addText(position, textContent, style.fontFamily, style.fontSize, scissor, style.color);

        if (style.textDecoration == TextDecoration::Underline) {
            float textWidth = data->measureText(textContent, style.fontFamily, style.fontSize);
            Rect decorationRect{position.x, position.y + style.fontSize, textWidth, 2};
            data->addRectFilled(decorationRect, style.color, scissor);
        }
    }

    // Render scrollbars
    int internalHeight = 0;
    for (auto& child : mChildren) {
        internalHeight += child->rect.height;
    }
    if (rect.height < internalHeight) {
        Rect scrollbar{
            rect.x + rect.width,
            rect.y - scroll.y,
            20,
            50
        };
        data->addRectFilled(scrollbar, Color::fromRGBA(150, 150, 150, 0.7), scrollbar);
    }

    float xValue = rect.x;
    float yValue = rect.y + style.padding.top + scroll.y;
    float previousAlpha = data->globalAlpha;
    data->globalAlpha *= style.opacity;
    for (auto &element : mChildren) {
        if (element->style.position != Position::Absolute) {
            if (style.justifyContent == Align::Center) {
                float midpoint = (rect.width - element->style.width) / 2;
                element->rect.x = xValue + midpoint;
            } else if (style.justifyContent == Align::Right) {
                element->rect.x = xValue + rect.width - element->rect.width;
            } else {
                element->rect.x = xValue + style.padding.left;
            }

            if (style.alignItems == Align::Center) {
                float midpoint = (rect.height - element->style.height) / 2;
                element->rect.y = yValue + midpoint;
            } else {
                element->rect.y = yValue + element->style.margin.top;
            }

            element->scissor.x = rect.x;
            element->scissor.y = rect.y;
            element->scissor.width = rect.width;
            element->scissor.height = rect.height;
        }

        element->render(data, &style);

        if (element->style.position != Position::Absolute) {
            if (style.flexDirection == Direction::Row) {
                xValue += element->rect.width + element->style.margin.right;
                xValue += element->style.margin.left;
            } else {
                yValue += element->rect.height + element->style.margin.bottom;
                yValue += element->style.margin.top;
            }
        }
    }

    data->globalAlpha = previousAlpha;
}

Img::Img(Document& document)
    : Element("img", document)
    , mSrc{""}
    , mImageRef{nullptr}
    { }

Img::~Img() { }

void Img::render(DrawData* data, Style* parentStyle) {
    rect.width = style.width;
    rect.height = style.height;

    if (mImageRef != nullptr) {
        mImageRef->render();
        data->addImage(rect, mImageRef->textureId);
    }

    Element::render(data, &style);
}

void Img::src(std::string const& newSrc) {
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
        }
    } else if (event->type == EventType::Key) {
        if (mDocument->focusManager.focusedElement == this) {
            auto keyEvent = dynamic_cast<KeyEvent*>(event);

            switch (keyEvent->key) {
            case KeyType::Backspace:
            {
                value.pop_back();
                break;
            }
            default:
                break;
            }
        }
    } else if (event->type == EventType::MouseDown) {
        auto mouseDownEvent = dynamic_cast<MouseDownEvent*>(event);

        if (rect.contains(mouseDownEvent->x, mouseDownEvent->y)) {
            mDocument->focusManager.focusElement(this);
        }
    }

    return true;
}

void Input::render(DrawData* data, Style* parentStyle) {
    Rect borderRect;
    borderRect.x = rect.x;
    borderRect.y = rect.y;
    borderRect.width = rect.width = style.width;
    borderRect.height = rect.height = style.height;

    if (borderRect.height < 30) borderRect.height = 30;

    if (style.borderWidth > 0) {
        data->addRectFilled(borderRect, style.borderColor);

        borderRect.width -= style.borderWidth * 2;
        borderRect.height -= style.borderWidth * 2;
        borderRect.x += style.borderWidth;
        borderRect.y += style.borderWidth;
    }

    data->addRectFilled(borderRect, style.backgroundColor);

    float padTop = (borderRect.height - 24/*px*/) / 2;
    padTop = style.padding.top == 0 ? padTop : style.padding.top;

    if (value != "") {
        float textWidth = data->measureText(value, style.fontFamily, 24);
        float pad = textWidth > borderRect.width - 30
            ? textWidth - borderRect.width + 30
            : 0;

        Rect position{
            borderRect.x + 5 + style.padding.left - pad,
            borderRect.y + padTop,
            borderRect.width,
            borderRect.height
        };

        data->addText(position, value, style.fontFamily, 24, borderRect, style.color);
    }

    // Draw input line
    if (mDocument->focusManager.focusedElement == this) {
        mFrames++;
        if (mFrames < 60) {
            float textWidth = data->measureText(value, style.fontFamily, 24);
            Rect inputLine{rect.x + 5 + textWidth, rect.y + 5, 2, rect.height - 10};
            data->addRectFilled(inputLine, style.borderColor);
        } else {
            if (mFrames > 120) {
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
        if (rect.contains(mouseMoveEvent->x, mouseMoveEvent->y)) {
            mPseudoClass = PseudoClass::Hover;
        } else {
            mPseudoClass = PseudoClass::None;
        }
    } else if (event->type == EventType::MouseDown) {
        MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(event);
        if (rect.contains(mouseDownEvent->x, mouseDownEvent->y)) {
            dispatch(EventType::MouseDown, event);
            return false;
        }
    }

    return true;
}

void Button::render(DrawData* data, Style* parentStyle) {
    rect.width = style.width;
    rect.height = style.height;

    if (style.backgroundImage != "") {
        if (mBackgroundImage == nullptr) {
            mBackgroundImage = mDocument->loadImage(style.backgroundImage,
                style.width, style.height);
        }
        data->addImage(rect, mBackgroundImage->textureId);
    } else {
        data->addRectFilled(rect, style.backgroundColor);
    }

    if (textContent != "") {
        Rect position{
            rect.x + style.padding.left,
            rect.y + style.padding.top,
            rect.width,
            rect.height
        };

        if (style.textAlign == Align::Center) {
            float width = data->measureText(textContent, style.fontFamily, 24);
            position.x = rect.x + (rect.width / 2) - (width / 2);
        }

        data->addText(position, textContent, style.fontFamily, 24, rect, style.color);
    }

    float yValue = rect.y + style.padding.top;
    for (auto &element : mChildren) {
        element->rect.x = rect.x + style.padding.left;
        element->rect.y = yValue;
        element->render(data, &style);

        yValue += element->rect.height;
    }
}

}  // namespace Nova
