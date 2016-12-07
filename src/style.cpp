// Copyright 2016 Dan Ristic

#include <algorithm>
#include <stack>

#include "src/style.h"
#include "src/element.h"

namespace Nova {

StyleRule::StyleRule()
    : tagName{}
    , pseudoClass{PseudoClass::None}
    { }

StyleRule::~StyleRule() { }

void StyleRule::addTag(std::string name) {
    tagName = name;
}

void StyleRule::addClass(std::string name) {
    className = name;
}

void StyleRule::addPseudoClass(PseudoClass pseudo) {
    pseudoClass = pseudo;
}

void StyleRule::addValue(StyleProp property, Color value) {
    switch (property) {
    case StyleProp::BackgroundColor:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<Color>(
                value,
                [](Style& style, Color& color) {
                    style.backgroundColor = color;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    case StyleProp::Color:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<Color>(
                value,
                [](Style& style, Color& color) {
                    style.color = color;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    default: break;
    }
}

void StyleRule::addValue(StyleProp property, LayoutProperty value) {
    switch (property) {
    case StyleProp::Padding:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<LayoutProperty>(
                value,
                [](Style& style, LayoutProperty& layout) {
                    style.padding = layout;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    default: break;
    }
}

void StyleRule::addValue(StyleProp property, Align value) {
    switch (property) {
    case StyleProp::TextAlign:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<Align>(
                value,
                [](Style& style, Align align) {
                    style.textAlign = align;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    case StyleProp::JustifyContent:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<Align>(
                value,
                [](Style& style, Align align) {
                    style.justifyContent = align;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    default: break;
    }
}

void StyleRule::addValue(StyleProp property, int value) {
    switch (property) {
    case StyleProp::Width:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<int>(
                value,
                [](Style& style, int width) {
                    style.width = width;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    case StyleProp::Height:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<int>(
                value,
                [](Style& style, int height) {
                    style.height = height;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    case StyleProp::Top:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<int>(
                value,
                [](Style& style, int top) {
                    style.top = top;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    case StyleProp::Left:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<int>(
                value,
                [](Style& style, int left) {
                    style.left = left;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    default: break;
    }
}

void StyleRule::addValue(StyleProp property, float value) {
    switch (property) {
    case StyleProp::Opacity:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<float>(
                value,
                [](Style& style, float opacity) {
                    style.opacity = opacity;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    default: break;
    }
}

void StyleRule::addValue(StyleProp property, Position value) {
    std::unique_ptr<StyleValueInternal> styleValue{
        new StyleValue<Position>(
            value,
            [](Style& style, Position position) {
                style.position = position;
            })
    };

    mProperties[property] = std::move(styleValue);
}

void StyleRule::addValue(StyleProp property, Direction value) {
    std::unique_ptr<StyleValueInternal> styleValue{
        new StyleValue<Direction>(
            value,
            [](Style& style, Direction direction) {
                style.flexDirection = direction;
            })
    };

    mProperties[property] = std::move(styleValue);
}

void StyleRule::applyRule(Style& style) {
    std::for_each(mProperties.begin(), mProperties.end(), [&style](auto& pair) {
        pair.second->apply(style);
    });
}

bool StyleRule::matches(Element* element) {
    size_t classFound = className == "" ?
        std::string::npos :
        element->className.find(className);

    bool matches = element->mPseudoClass == pseudoClass
        && (element->tagName == tagName || classFound != std::string::npos);
    
    return matches;
}

StyleManager::StyleManager()
    : mRules{}
    { }

StyleManager::~StyleManager() { }

void StyleManager::addRule(StyleRule&& rule) {
    mRules.push_back(std::move(rule));
}

void StyleManager::addRule(std::function<void(StyleRule&)> factory) {
    StyleRule rule;
    factory(rule);
    addRule(std::move(rule));
}

void StyleManager::applyRules(Element* root) {
    std::stack<Element*> stack;

    stack.push(root);

    while (!stack.empty()) {
        Element* el = stack.top();
        stack.pop();

        std::for_each(mRules.begin(), mRules.end(), [&el](auto& rule) {
            if (rule.matches(el)) {
                rule.applyRule(el->style);
            }
        });

        for(auto& child : el->getChildren()) {
            stack.push(child);
        }
    }
}

}  // namespace Nova
