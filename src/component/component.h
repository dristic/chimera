// Copyright 2016 Dan Ristic

#ifndef SRC_COMPONENT_H_
#define SRC_COMPONENT_H_

#include <string>
#include <vector>
#include <unordered_map>

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

enum class AttributeType {
    String,
    Style,
    EventCallback
};

class Attribute {
using EventCallback = std::function<void(Nova::Event*)>;

public:
    Attribute() { }

    Attribute(std::string key, std::string value)
        : mKey{key}
        , mStringValue{value}
        , mType{AttributeType::String}
        { }

    Attribute(std::string key, Nova::Style value)
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

    Nova::Style asStyle() {
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
    Nova::Style mStyleValue;
    EventCallback mCallbackValue;
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
    Attribute* getAttribute(const std::string& key);
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

enum class PatchMode {
    Create,
    Delete,
    Diff
};

class Component;

class ComponentManager {
public:
    ComponentManager(Nova::Context* document);

    void update(double dt);
    void patch(
        PatchMode mode,
        Api::CElement* currentNode,
        Api::CElement* newNode,
        Nova::Element* element);
    void walk(
        Api::CElement* currentNode,
        Api::CElement* newNode,
        Nova::Element* elementRoot);
    void invalidate(Component* node);
    void render(Nova::Element* root, std::shared_ptr<Api::CElement> element);

    std::vector<std::function<void(void)>> tasks;

private:
    Nova::Context* mContext;
    std::shared_ptr<Api::CElement> mTreeRoot;
};

class Component: public Api::CElement {
public:
    Component()
        : Api::CElement(Api::E::Component, {})
        { }

    virtual void componentDidMount() { }
    virtual std::shared_ptr<Api::CElement> render(Nova::Context& context) override {
        return Api::CElement::render(context);
    }

    void invalidate() {
        componentManager->invalidate(this);
    }

    Nova::Element* parentElement;
    Nova::Element* elementRef;
    ComponentManager* componentManager;
    bool mounted{false};
};
    
}  // namespace Cosmonaut

#endif  // SRC_COMPONENT_H_
