// Copyright 2016 Dan Ristic

#include "src/component/component.h"

#include "src/document.h"

namespace Cosmonaut {

    namespace Api {

        E CElement::getType() {
            return mType;
        }

        std::string CElement::getAttribute(const std::string& key) {
            if (mAttributes.count(key) == 1) {
                return mAttributes.at(key);
            } else {
                return "";
            }
        }

        std::vector<CElement> CElement::getChildren() {
            return mChildren;
        }

        CElement CElement::children(std::vector<CElement> children) {
            mChildren.insert(mChildren.end(), children.begin(), children.end());

            return *this;
        }


    }  // namespace Api

    ComponentManager::ComponentManager(Nova::Document* document)
    : mDocument{document}
    { }

    void ComponentManager::patch(Nova::Element *element, Api::CElement other) {
        if (element->id() != other.getAttribute("id")) {
            element->id(other.getAttribute("id"));
        }
    }

    void ComponentManager::walk(Nova::Element *currentNode, Api::CElement currentElement) {
        switch (currentElement.getType()) {
            case Api::E::Div:
            {
                auto newElement = mDocument->createElement<Nova::Div>();
                patch(newElement, currentElement);
                currentNode->append(newElement);

                for (auto& child : currentElement.getChildren()) {
                    walk(newElement, child);
                }

                break;
            }
            case Api::E::Img:
            {
                auto newElement = mDocument->createElement<Nova::Img>();
                patch(newElement, currentElement);
                newElement->src(currentElement.getAttribute("src"));
                currentNode->append(newElement);

                break;
            }
            case Api::E::Text:
            {
                std::string content = currentElement.getAttribute("textContent");
                currentNode->textContent = content;
                break;
            }
        }
    }

    void ComponentManager::render(Nova::Element *root, Api::CElement element) {
        walk(root, element);
    }
    
}  // namespace Cosmonaut
