// Copyright 2016 Dan Ristic

#ifndef SRC_COMPONENT_H_
#define SRC_COMPONENT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <experimental/optional>

#include "src/element.h"

#define UNUSED(expr) (void)(expr)

namespace Nova {

class Document;
class Context;

};  // namespace Nova

namespace Cosmonaut {

namespace Api {

enum class E {
    Img,
    Div,
    Text,
    Input,
    Button,
    Component
};

class Attribute {
using EventCallback = std::function<void(Nova::Event*)>;

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

    Attribute(std::string key, EventCallback value)
        : mKey{key}
        , mCallbackValue{value}
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

    EventCallback asCallback() {
        if (mCallbackValue) {
            return *mCallbackValue;
        } else {
            return nullptr;
        }
    }

private:
    std::string mKey;
    std::experimental::optional<std::string> mStringValue;
    std::experimental::optional<Nova::Style> mStyleValue;
    std::experimental::optional<EventCallback> mCallbackValue;
};

class CElement {
public:
    CElement(E type, std::vector<Attribute> attributes, std::vector<std::shared_ptr<CElement>>& children)
        : mType{type}
        , mChildren{std::move(children)}
    {
        for (auto& attribute : attributes) {
            std::string key = attribute.getKey();
            mAttributes[key] = attribute;
        }
    }

    CElement(E type, std::vector<Attribute> attributes)
        : mType{type}
        { }

    E getType();
    std::experimental::optional<Attribute> getAttribute(const std::string& key);
    std::vector<std::shared_ptr<CElement>> const& getChildren() const;
    void setChildren(std::vector<std::shared_ptr<CElement>>& children);
    virtual std::shared_ptr<Api::CElement> render(Nova::Context& context);

private:
    E mType;
    std::unordered_map<std::string, Attribute> mAttributes{};
    std::vector<std::shared_ptr<CElement>> mChildren;
};

static inline std::shared_ptr<CElement> C(E type, std::vector<Attribute> attributes, std::vector<std::shared_ptr<CElement>>&& children) {
    return std::make_shared<CElement>(type, attributes, children);
}

static inline std::shared_ptr<CElement> C(E type, std::vector<Attribute> attributes) {
    std::vector<std::shared_ptr<CElement>> children{};
    return std::make_shared<CElement>(type, attributes, children);
}

}  // namespace Api

class ComponentManager {
public:
    ComponentManager(Nova::Context* document);

    void patch(Nova::Element* element, Api::CElement* other);
    void walk(Nova::Element* currentNode, Api::CElement* currentElement);
    void render(Nova::Element* root, std::shared_ptr<Api::CElement> element);

private:
    Nova::Context* mContext;
    std::shared_ptr<Api::CElement> mTreeRoot;
};

class Component: public Api::CElement {
public:
    Component()
        : Api::CElement(Api::E::Component, {})
        { }

    virtual std::shared_ptr<Api::CElement> render(Nova::Context& context) override {
        return Api::CElement::render(context);
    }

    void invalidate() {
        parentElement->remove(elementRef);
        componentManager->walk(parentElement, this);
    }

    Nova::Element* parentElement;
    Nova::Element* elementRef;
    ComponentManager* componentManager;
};
    
}  // namespace Cosmonaut

#endif  // SRC_COMPONENT_H_
