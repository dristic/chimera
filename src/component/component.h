// Copyright 2016 Dan Ristic

#ifndef SRC_COMPONENT_H_
#define SRC_COMPONENT_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "src/element.h"

#define UNUSED(expr) (void)(expr)

namespace Cosmonaut {

namespace Api {

enum class AttributeType {
    String,
    Style,
    EventCallback
};

class Attribute {
using EventCallback = std::function<void(Cosmonaut::Event*)>;

public:
    Attribute() { }

    Attribute(std::string key, std::string value)
        : mKey{key}
        , mStringValue{value}
        , mType{AttributeType::String}
        { }

    Attribute(std::string key, Cosmonaut::Style value)
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

    Cosmonaut::Style asStyle() {
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
    Cosmonaut::Style mStyleValue;
    EventCallback mCallbackValue;
};

// class CElement {
// public:
//     CElement(E type, std::vector<Attribute> attributes, std::vector<std::shared_ptr<CElement>>& children)
//         : mType{type}
//         , mChildren{std::move(children)}
//     {
//         for (auto& attribute : attributes) {
//             std::string key = attribute.getKey();
//             mAttributes[key] = attribute;
//         }
//     }

//     CElement(E type, std::vector<Attribute> attributes)
//         : mType{type}
//         { }

//     E getType();
//     Attribute* getAttribute(const std::string& key);
//     std::vector<std::shared_ptr<CElement>> const& getChildren() const;
//     void setChildren(std::vector<std::shared_ptr<CElement>>& children);
//     virtual std::shared_ptr<Api::CElement> render(Cosmonaut::Context& context);

// private:
//     E mType;
//     std::unordered_map<std::string, Attribute> mAttributes{};
//     std::vector<std::shared_ptr<CElement>> mChildren;
// };

// template<class E>
// static inline Element* Create<E>(std::vector<Attribute> attributes, std::vector<Element*> children)
// {
// }

}  // namespace Api

// enum class PatchMode {
//     Create,
//     Delete,
//     Diff
// };

// class Component;

// class ComponentManager {
// public:
//     ComponentManager(Cosmonaut::Context* document);

//     void update(double dt);
//     void patch(
//         PatchMode mode,
//         Api::CElement* currentNode,
//         Api::CElement* newNode,
//         Cosmonaut::Element* element);
//     void walk(
//         Api::CElement* currentNode,
//         Api::CElement* newNode,
//         Cosmonaut::Element* elementRoot);
//     void invalidate(Component* node);
//     void render(Cosmonaut::Element* root, std::shared_ptr<Api::CElement> element);

//     std::vector<std::function<void(void)>> tasks;

// private:
//     Cosmonaut::Context* mContext;
//     std::shared_ptr<Api::CElement> mTreeRoot;
// };
    
}  // namespace Cosmonaut

#endif  // SRC_COMPONENT_H_
