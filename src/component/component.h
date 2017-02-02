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

    class Document;

    using ElementAttributes = std::unordered_map<std::string, std::string>;

    enum class E {
        Img,
        Div,
        Text
    };

    class CElement {
    public:
        CElement(E type, ElementAttributes attributes)
        : mType{type}
        , mAttributes{attributes}
        { }

        E getType();
        std::string getAttribute(const std::string& key);
        std::vector<CElement> getChildren();
        CElement children(std::vector<CElement> children);

    private:
        E mType;
        ElementAttributes mAttributes{};
        std::vector<CElement> mChildren{};
    };

    static inline CElement C(E type, ElementAttributes attributes) {
        return {type, attributes};
    }

}  // namespace Api

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
