// Copyright 2016 Dan Ristic

#ifndef SRC_COMPONENT_H_
#define SRC_COMPONENT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <experimental/optional>

#include "src/element.h"

#define UNUSED(expr) (void)(expr)

namespace Cosmonaut {

namespace Api {

class Document;

enum class E {
    Img,
    Div,
    Text,
    Input,
    Button
};

class Attribute {
public:
    Attribute() { }

    Attribute(std::string key, std::string value)
        : mKey{key}
        , mStringValue{value}
        { }

    Attribute(std::string key, Nova::Style value)
        : mKey{key}
        , mStyleValue{value}
        { }

    std::string getKey() {
        return mKey;
    }

    std::string asString() {
        if (mStringValue) {
            return *mStringValue;
        } else {
            return "";
        }
    }

    Nova::Style asStyle() {
        if (mStyleValue) {
            return *mStyleValue;
        } else {
            return {};
        }
    }

private:
    std::string mKey;
    std::experimental::optional<std::string> mStringValue;
    std::experimental::optional<Nova::Style> mStyleValue;
};

class CElement {
public:
    CElement(E type, std::vector<Attribute> attributes, std::vector<CElement> children)
        : mType{type}
        , mChildren{children}
    {
        for (auto& attribute : attributes) {
            std::string key = attribute.getKey();
            mAttributes[key] = attribute;
        }
    }

    E getType();
    std::experimental::optional<Attribute> getAttribute(const std::string& key);
    std::vector<CElement> getChildren();

private:
    E mType;
    std::unordered_map<std::string, Attribute> mAttributes{};
    std::vector<CElement> mChildren{};
};

static inline CElement C(E type, std::vector<Attribute> attributes, std::vector<CElement> children) {
    return {type, attributes, children};
}

static inline CElement C(E type, std::vector<Attribute> attributes) {
    return {type, attributes, {}};
}

}  // namespace Api

class Component {
public:
    Component() { }
};

class ComponentManager {
public:
    ComponentManager(Nova::Document* document);

    void patch(Nova::Element* element, Api::CElement other);
    void walk(Nova::Element* currentNode, Api::CElement currentElement);
    void render(Nova::Element* root, Api::CElement element);

private:
    Nova::Document* mDocument;
};
    
}  // namespace Cosmonaut

#endif  // SRC_COMPONENT_H_
