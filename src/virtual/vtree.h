// Copyright 2016 Dan Ristic

#ifndef SRC_VTREE_H_
#define SRC_VTREE_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "src/core.h"
#include "src/element.h"
#include "src/dom/img.h"
#include "src/document.h"

namespace Chimera {

namespace Virtual {

enum class AttributeType {
    String,
    Style,
    EventCallback
};

class Attribute {
using EventCallback = std::function<void(Chimera::Event*)>;

public:
    Attribute() { }

    Attribute(std::string key, std::string value)
        : mKey{key}
        , mStringValue{value}
        , mType{AttributeType::String}
        { }

    Attribute(std::string key, Chimera::Style value)
        : mKey{key}
        , mType{AttributeType::Style}
    {
        mStyleValue = value;
    }

    Attribute(std::string key, EventCallback value)
        : mKey{key}
        , mCallbackValue{value}
        , mType{AttributeType::EventCallback}
        { }

    std::string getKey() {
        return mKey;
    }

    std::string asString() {
        if (mType == AttributeType::String) {
            return mStringValue;
        } else {
            return "";
        }
    }

    Chimera::Style asStyle() {
        if (mType == AttributeType::Style) {
            return mStyleValue;
        } else {
            return {};
        }
    }

    EventCallback asCallback() {
        if (mType == AttributeType::EventCallback) {
            return mCallbackValue;
        } else {
            return nullptr;
        }
    }

private:
    std::string mKey;

    std::string mStringValue;
    Chimera::Style mStyleValue;
    EventCallback mCallbackValue;

    AttributeType mType;
};

class VirtualElement {
public:
    VirtualElement(std::string _name, std::vector<Attribute> _attributes);

    Element* create(Document& document);

    std::string name{"div"};
    std::string textContent;
    std::vector<VirtualElement> children{};
    std::unordered_map<std::string, Attribute> attributes{};
};

static inline VirtualElement VElement(
    std::string name,
    std::vector<Attribute> attributes)
{
    return {name, attributes};
}

static inline VirtualElement VElement(
    std::string name,
    std::vector<Attribute> attributes,
    std::string textContent)
{
    VirtualElement element{name, attributes};
    element.textContent = textContent;
    return element;
}

static inline VirtualElement VElement(
    std::string name,
    std::vector<Attribute> attributes,
    std::vector<VirtualElement> children)
{
    VirtualElement element{name, attributes};
    element.children = children;
    return element;
}

static inline Element* CreateTree(Document& document, VirtualElement element)
{
    return element.create(document);
}

}  // namespace Virtual
    
}  // namespace Chimera

#endif  // SRC_VTREE_H_
