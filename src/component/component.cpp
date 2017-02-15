// Copyright 2016 Dan Ristic

#include "src/component/component.h"

#include "src/document.h"

namespace Cosmonaut {

    namespace Api {

        E CElement::getType() {
            return mType;
        }

        std::experimental::optional<Attribute> CElement::getAttribute(const std::string& key) {
            if (mAttributes.count(key) == 1) {
                return mAttributes.at(key);
            } else {
                return {};
            }
        }

        std::vector<CElement> CElement::getChildren() {
            return mChildren;
        }


    }  // namespace Api

    ComponentManager::ComponentManager(Nova::Document* document)
    : mDocument{document}
    { }

    void ComponentManager::patch(Nova::Element *element, Api::CElement other) {
        auto id = other.getAttribute("id");
        if (id) {
            if (element->id() != (*id).asString()) {
                element->id((*id).asString());
            }
        }

        auto style = other.getAttribute("style");
        if (style) {
            element->style = (*style).asStyle();
        }

        auto clickHandler = other.getAttribute("onClick");
        if (clickHandler) {
            element->on(Nova::EventType::MouseDown, (*clickHandler).asCallback());
        }
    }

    void ComponentManager::walk(Nova::Element *currentNode, Api::CElement currentElement) {
        Nova::Element* newElement{};

        switch (currentElement.getType()) {
            case Api::E::Div:
            {
                auto element = mDocument->createElement<Nova::Div>();
                patch(element, currentElement);
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Img:
            {
                auto element = mDocument->createElement<Nova::Img>();
                patch(element, currentElement);
                auto src = currentElement.getAttribute("src");
                element->src((*src).asString());
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Text:
            {
                std::string content = (*currentElement.getAttribute("textContent")).asString();
                currentNode->textContent = content;
                break;
            }
            case Api::E::Input:
            {
                auto element = mDocument->createElement<Nova::Input>();
                patch(element, currentElement);
                auto placeholder = currentElement.getAttribute("placeholder");
                element->placeholder = ((*placeholder).asString());
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Button:
            {
                auto element = mDocument->createElement<Nova::Button>();
                patch(element, currentElement);
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
        }

        if (newElement) {
            for (auto& child : currentElement.getChildren()) {
                walk(newElement, child);
            }
        }
    }

    void ComponentManager::render(Nova::Element *root, Api::CElement element) {
        walk(root, element);
    }
    
}  // namespace Cosmonaut
