// Copyright 2016 Dan Ristic

#ifndef SRC_COMPONENT_H_
#define SRC_COMPONENT_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "src/core.h"
#include "src/element.h"

namespace Chimera {

namespace Api {

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

        return element;
    }

    std::unordered_map<std::string, Attribute> attributes{};
};

template<class E>
static inline VirtualElement<E> VElement(std::vector<Attribute> attributes)
{
    return {attributes};
}

template<class E>
static inline Element* CreateTree(Document& document, VirtualElement<E> element)
{
    return element.create(document);
}

}  // namespace Api

// class Component;

// class ComponentManager {
// public:
//     ComponentManager(Chimera::Context* document);

//     void update(double dt);
//     void patch(
//         PatchMode mode,
//         Api::CElement* currentNode,
//         Api::CElement* newNode,
//         Chimera::Element* element);
//     void walk(
//         Api::CElement* currentNode,
//         Api::CElement* newNode,
//         Chimera::Element* elementRoot);
//     void invalidate(Component* node);
//     void render(Chimera::Element* root, std::shared_ptr<Api::CElement> element);

//     std::vector<std::function<void(void)>> tasks;

// private:
//     Chimera::Context* mContext;
//     std::shared_ptr<Api::CElement> mTreeRoot;
// };
    
}  // namespace Chimera

#endif  // SRC_COMPONENT_H_
