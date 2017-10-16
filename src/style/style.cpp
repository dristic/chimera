// Copyright 2017 Dan Ristic

#include "src/style/style.h"

#include <algorithm>
#include <stack>

#include "src/element.h"

namespace Chimera {

void Style::inheritValues(Style& other)
{
    if ((setMap & SET_COLOR) == SET_COLOR
            && (other.setMap & SET_COLOR) != SET_COLOR)
    {
        other.color = color;
        other.setMap |= SET_COLOR;
    }

    if ((setMap & SET_FONT_FAMILY) == SET_FONT_FAMILY
            && (other.setMap & SET_FONT_FAMILY) != SET_FONT_FAMILY)
    {
        other.fontFamily = fontFamily;
        other.setMap |= SET_FONT_FAMILY;
    }
}

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
                    style.setMap |= Style::SET_COLOR;
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
    case StyleProp::AlignItems:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<Align>(
                value,
                [](Style& style, Align align) {
                    style.alignItems = align;
                })
        };

        mProperties[property] = std::move(styleValue);
        break;
    }
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
    case StyleProp::FontFamily:
    {
        std::unique_ptr<StyleValueInternal> styleValue{
            new StyleValue<std::string>(
                value,
                [](Style& style, std::string value) {
                    style.fontFamily = value;
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
    bool matches = element->mPseudoClass == pseudoClass;

    if (idName != "")
    {
        matches = matches && element->id == idName;
    }

    if (tagName != "")
    {
        matches = matches && element->tagName == tagName;
    }

    if (className != "")
    {
        size_t classFound = element->className.find(className);
        matches = matches && classFound != std::string::npos;
    }
    
    return matches;
}

StyleManager::StyleManager(Element* root)
    : mGlobalRoot{root}
    , mRules{}
    { }

StyleManager::~StyleManager() { }

void StyleManager::addRule(StyleRule&& rule) {
    mRules.push_back(std::move(rule));

    if (mGlobalRoot)
    {
        applyRules(mGlobalRoot);
    }
}

PseudoClass StyleManager::getPseudoFromString(std::string value)
{
    if (value == "hover")
    {
        return PseudoClass::Hover;
    }

    return PseudoClass::None;
}

enum class ParseMode
{
    Class, Id, Tag, Pseudo
};

void StyleManager::parseSelector(std::string selector, StyleRule& rule)
{
    std::string result;
    ParseMode mode{ParseMode::Tag};

    auto ParseResult = [&result, &mode, &rule, this]()
    {
        if (result != "")
        {
            switch (mode)
            {
                case ParseMode::Class: rule.addClass(result); break;
                case ParseMode::Id: rule.addId(result); break;
                case ParseMode::Tag: rule.addTag(result); break;
                case ParseMode::Pseudo:
                    rule.addPseudoClass(getPseudoFromString(result));
                    break;
            }

            result = "";
        }
    };

    for (auto& c : selector)
    {
        if (c == '.')
        {
            ParseResult();
            mode = ParseMode::Class;
        }
        else if (c == '#')
        {
            ParseResult();
            mode = ParseMode::Id;
        }
        else if (c == ':')
        {
            ParseResult();
            mode = ParseMode::Pseudo;
        }
        else
        {
            result += c;
        }
    }

    ParseResult();
}

void StyleManager::addRule(std::string selector, std::vector<StyleAttribute> attributes) {
    StyleRule rule{};

    parseSelector(selector, rule);

    for (auto& attribute : attributes) {
        switch (attribute.getProp()) {
            case StyleProp::Width:
            case StyleProp::Height:
            case StyleProp::FontSize:
            case StyleProp::Top:
            case StyleProp::Left:
                rule.addValue(attribute.getProp(), attribute.asInt());
                break;
            case StyleProp::BackgroundImage:
            case StyleProp::AnimationName:
            case StyleProp::FontFamily:
                rule.addValue(attribute.getProp(), attribute.asString());
                break;
            case StyleProp::FlexDirection:
                rule.addValue(attribute.getProp(), attribute.asDirection());
                break;
            case StyleProp::AlignItems:
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
            case StyleProp::Position:
                rule.addValue(attribute.getProp(), attribute.asPosition());
                break;
            default:
                CHIMERA_DEBUG(printf("Unrecognized style attribute property.\n"));
                break;
        }
    }

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

void StyleManager::setGlobalRoot(Element* element)
{
    mGlobalRoot = element;
}

}  // namespace Chimera
