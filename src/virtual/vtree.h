// Copyright 2016 Dan Ristic

#ifndef SRC_VTREE_H_
#define SRC_VTREE_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "src/core.h"
#include "src/element.h"

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
    AttributeType mType;

    std::string mStringValue;
    Chimera::Style mStyleValue;
    EventCallback mCallbackValue;
};

template<class E>
class VirtualElement {
public:
    VirtualElement(std::vector<Attribute> _attributes)
    {
        for (auto& attribute : _attributes) {
            std::string key = attribute.getKey();
            attributes[key] = attribute;
        }
    }

    Element* create(Document& document)
    {
        auto element = document.createElement<E>();

        if (attributes.count("id") == 1)
        {
            auto id = attributes.at("id");
            element->id = id.asString();
        }

        element->textContent = textContent;

        DEBUG(
            printf("[VElement] %s %s\n",
                element->tagName.c_str(),
                element->textContent.c_str());
        )

        for (auto& child : children)
        {
            element->append(child.create(document));
        }

        return element;
    }

    std::string textContent;
    std::vector<VirtualElement<E>> children{};
    std::unordered_map<std::string, Attribute> attributes{};
};

template<class E>
static inline VirtualElement<E> VElement(std::vector<Attribute> attributes)
{
    return {attributes};
}

template<class E>
static inline VirtualElement<E> VElement(
    std::vector<Attribute> attributes,
    std::string textContent)
{
    VirtualElement<E> element{attributes};
    element.textContent = textContent;
    return element;
}

template<class E>
static inline VirtualElement<E> VElement(
    std::vector<Attribute> attributes,
    std::vector<VirtualElement<E>> children)
{
    VirtualElement<E> element{attributes};
    element.children = children;
    return element;
}

template<class E>
static inline Element* CreateTree(Document& document, VirtualElement<E> element)
{
    return element.create(document);
}

}  // namespace Virtual
    
}  // namespace Chimera

#endif  // SRC_VTREE_H_
