// Copyright 2017 Dan Ristic

#ifndef SRC_STYLE_H_
#define SRC_STYLE_H_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "src/types.h"
#include "src/animation.h"

namespace Chimera {

class Style;
class Element;

enum class PseudoClass { None, Hover };
enum class TextDecoration { None, Underline };
enum class Align { Left, Right, Center, Top, Bottom };
enum class Position { Absolute, Relative };
enum class Direction { Row, Column };

enum class StyleProp {
    Padding,
    Margin,
    TextAlign,
    TextDecoration,
    FontSize,
    FontFamily,
    AlignItems,
    JustifyContent,
    FlexDirection,
    BackgroundImage,
    BackgroundColor,
    BorderColor,
    Color,
    Opacity,
    BorderWidth,
    Width,
    Height,
    Position,
    Top,
    Left,
    AnimationName
};

enum class NumericType {
    Percentage,
    Pixels
};
    
struct StylePropHash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

class LayoutProperty {
 public:
    float top;
    float left;
    float right;
    float bottom;

    LayoutProperty() { }

    LayoutProperty(float _t, float _l, float _r, float _b)
        : top{_t}
        , left{_l}
        , right{_r}
        , bottom{_b}
        { }

    void set(float value) {
        top = left = right = bottom = value;
    }
};

class StyleValueInternal {
 public:
    StyleValueInternal() { };

    virtual void apply(Style& style) = 0;
};

template <typename T>
class StyleValue : public StyleValueInternal {
    using ApplyFunction = std::function<void(Style&, T&)>;

 public:
    StyleValue(T value, ApplyFunction func)
        : StyleValueInternal()
        , mValue{value}
        , mApply{func}
        { };

    void apply(Style& style) override {
        mApply(style, mValue);
    }

 private:
    T mValue;
    ApplyFunction mApply;
};

class Style {
 public:
    static const int AUTO = -1;

    static const char SET_COLOR = 1;
    static const char SET_FONT_FAMILY = 2;

    unsigned char setMap{0};

    void inheritValues(Style& other);

    // Layout
    int width = 0;
    NumericType widthType{NumericType::Pixels};
    int height = 0;
    LayoutProperty padding{0, 0, 0, 0};
    LayoutProperty margin{0, 0, 0, 0};
    Align alignItems = Align::Top;
    Align justifyContent = Align::Left;
    Direction flexDirection = Direction::Row;
    Position position = Position::Relative;
    int top = 0;
    int left = 0;

    // Font / Text
    std::string fontFamily = "Roboto";
    int fontSize = 24;
    Align textAlign = Align::Left;
    TextDecoration textDecoration = TextDecoration::None;
    Color color{Color::fromRGBA(0, 0, 0, 1)};

    // Visual
    std::string backgroundImage = "";
    Color backgroundColor;
    Color borderColor;
    int borderWidth;
    float opacity{1.0f};

    // Fidelity
    std::string animationName{""};

    std::vector<Animation> animations{};

    void update(Element* element, AnimationController& controller);

    void setWidth(std::string value) {
        if (value[value.length() - 1] == '%') {
            widthType = NumericType::Percentage;

            width = std::stoi(value);
        } else {
            widthType = NumericType::Pixels;

            width = std::stoi(value);
        }
    }
};

class StyleRule {
 public:
    StyleRule();
    ~StyleRule();

    void addId(std::string name);
    void addTag(std::string name);
    void addClass(std::string name);
    void addPseudoClass(PseudoClass pseudo);

    void addValue(StyleProp property, Color value);
    void addValue(StyleProp property, LayoutProperty value);
    void addValue(StyleProp property, Align value);
    void addValue(StyleProp property, int value);
    void addValue(StyleProp property, float value);
    void addValue(StyleProp property, Position value);
    void addValue(StyleProp property, Direction value);
    void addValue(StyleProp property, std::string value);

    void applyRule(Style& style);

    bool matches(Element* element);

    StyleRule(const StyleRule &) = delete;
    StyleRule(StyleRule &&) = default;

    std::string idName{""};
    std::string tagName{""};
    std::string className{""};
    PseudoClass pseudoClass;

 private:
    std::unordered_map<StyleProp, std::unique_ptr<StyleValueInternal>, StylePropHash> mProperties;
};

enum class StyleAttributeType {
    Int,
    String,
    Direction,
    Align,
    Color,
    LayoutProperty,
    Position
};

class StyleAttribute {
 public:
    StyleAttribute(StyleProp prop, int value)
        : mProp{prop}
        , mIntValue{value}
        , mType{StyleAttributeType::Int}
        { }

    StyleAttribute(StyleProp prop, std::string value)
        : mProp{prop}
        , mStringValue{value}
        , mType{StyleAttributeType::String}
        { }

    StyleAttribute(StyleProp prop, Direction value)
        : mProp{prop}
        , mDirectionValue{value}
        , mType{StyleAttributeType::Direction}
        { }

    StyleAttribute(StyleProp prop, Align value)
        : mProp{prop}
        , mAlignValue{value}
        , mType{StyleAttributeType::Align}
        { }

    StyleAttribute(StyleProp prop, Color value)
        : mProp{prop}
        , mColorValue{value}
        , mType{StyleAttributeType::Color}
        { }

    StyleAttribute(StyleProp prop, LayoutProperty value)
        : mProp{prop}
        , mLayoutPropertyValue{value}
        , mType{StyleAttributeType::LayoutProperty}
        { }

    StyleAttribute(StyleProp prop, Position value)
        : mProp{prop}
        , mPositionValue{value}
        , mType{StyleAttributeType::Position}
        { }

    StyleProp getProp() {
        return mProp;
    }

    std::string asString() {
        if (mType == StyleAttributeType::String) {
            return mStringValue;
        } else {
            return "";
        }
    }

    int asInt() {
        if (mType == StyleAttributeType::Int) {
            return mIntValue;
        } else {
            return -1;
        }
    }

    Direction asDirection() {
        return mDirectionValue;
    }

    Align asAlign() {
        return mAlignValue;
    }

    Color asColor() {
        return mColorValue;
    }

    LayoutProperty asLayoutProperty() {
        return mLayoutPropertyValue;
    }

    Position asPosition() {
        return mPositionValue;
    }

private:
    StyleProp mProp;

    std::string mStringValue;
    int mIntValue;
    Direction mDirectionValue{Direction::Row};
    Align mAlignValue{Align::Left};
    Color mColorValue{};
    LayoutProperty mLayoutPropertyValue{0, 0, 0, 0};
    Position mPositionValue{Position::Relative};

    StyleAttributeType mType;
};

class StyleManager {
 public:
    explicit StyleManager(Element* root);
    ~StyleManager();

    void parseSelector(std::string selector, StyleRule& rule);

    void addRule(StyleRule&& rule);
    void addRule(std::string selector, std::vector<StyleAttribute> attributes);
    void applyRules(Element* root);

    void setGlobalRoot(Element* element);

 private:
    PseudoClass getPseudoFromString(std::string value);

    Element* mGlobalRoot{nullptr};
    std::vector<StyleRule> mRules{};
};

}  // namespace Chimera

#endif  // SRC_STYLE_H_
