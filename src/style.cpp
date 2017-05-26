// Copyright 2017 Dan Ristic

#include <algorithm>
#include <stack>

#include "src/style.h"
#include "src/element.h"

namespace Cosmonaut {

void Style::update(Element* element, AnimationController& controller) {
    // Check for changed animation
    auto result = std::find_if(animations.begin(), animations.end(), [this](Animation& anim) {
        return anim.name != animationName;
    });

    if (result != animations.end()) {
        animations.erase(result);
    }

    // Check for new animation
    if (animationName != "" && animations.size() == 0) {
        auto func = controller.getFunction(animationName);

        if (func) {
            animations.push_back({animationName, element, func});
        }
    }

    // Now update
    for (auto& animation : animations) {
        animation.update(16.0f);
    }
}

StyleRule::StyleRule()
    : tagName{}
    , pseudoClass{PseudoClass::None}
    { }

StyleRule::~StyleRule() { }

void StyleRule::addId(std::string name) {
    idName = name;
}

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
    case StyleProp::Margin:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<LayoutProperty>(
                value,
                [](Style& style, LayoutProperty& layout) {
                    style.margin = layout;
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
    case StyleProp::FontSize:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<int>(
                value,
                [](Style& style, int size) {
                    style.fontSize = size;
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

void StyleRule::addValue(StyleProp property, std::string value) {
    switch(property) {
    case StyleProp::BackgroundImage:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<std::string>(
                value,
                [](Style& style, std::string value) {
                    style.backgroundImage = value;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    case StyleProp::AnimationName:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<std::string>(
                value,
                [](Style& style, std::string value) {
                    style.animationName = value;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
    default: break;
    }
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
        && (element->id == idName || element->tagName == tagName || classFound != std::string::npos);
    
    return matches;
}

StyleManager::StyleManager()
    : mRules{}
    { }

StyleManager::~StyleManager() { }

void StyleManager::addRule(StyleRule&& rule) {
    mRules.push_back(std::move(rule));
}

void StyleManager::addRule(std::string selector, std::vector<StyleAttribute> attributes) {
    StyleRule rule{};

    auto& type = selector.at(0);
    if (type == '.') {
        rule.addClass(selector.substr(1));
    } else if (type == '#') {
        rule.addId(selector.substr(1));
    } else {
        rule.addTag(selector);
    }

    for (auto& attribute : attributes) {
        switch (attribute.getProp()) {
            case StyleProp::Width:
            case StyleProp::Height:
            case StyleProp::FontSize:
                rule.addValue(attribute.getProp(), attribute.asInt());
                break;
            case StyleProp::BackgroundImage:
                rule.addValue(attribute.getProp(), attribute.asString());
                break;
            case StyleProp::AnimationName:
                rule.addValue(attribute.getProp(), attribute.asString());
                break;
            case StyleProp::FlexDirection:
                rule.addValue(attribute.getProp(), attribute.asDirection());
                break;
            case StyleProp::TextAlign:
            case StyleProp::JustifyContent:
                rule.addValue(attribute.getProp(), attribute.asAlign());
                break;
            case StyleProp::Color:
            case StyleProp::BackgroundColor:
                rule.addValue(attribute.getProp(), attribute.asColor());
                break;
            case StyleProp::Margin:
            case StyleProp::Padding:
                rule.addValue(attribute.getProp(), attribute.asLayoutProperty());
                break;
            default:
                break;
        }
    }

    addRule(std::move(rule));
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

}  // namespace Cosmonaut
