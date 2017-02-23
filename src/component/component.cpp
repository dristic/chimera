// Copyright 2016 Dan Ristic

#include "src/component/component.h"

#include "src/document.h"
#include "src/context.h"

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

        std::vector<std::shared_ptr<CElement>> const& CElement::getChildren() const {
            return mChildren;
        }

        void CElement::setChildren(std::vector<std::shared_ptr<CElement>>& children) {
            mChildren = std::move(children);
        }

        std::shared_ptr<Api::CElement> CElement::render(Nova::Context& context) {
            return nullptr;
        }


    }  // namespace Api

    ComponentManager::ComponentManager(Nova::Context* context)
    : mContext{context}
    { }

    void ComponentManager::patch(Nova::Element *element, Api::CElement* other) {
        auto id = other->getAttribute("id");
        if (id) {
            if (element->id() != (*id).asString()) {
                element->id((*id).asString());
            }
        }

        auto style = other->getAttribute("style");
        if (style) {
            element->style = (*style).asStyle();
        }

        auto clickHandler = other->getAttribute("onClick");
        if (clickHandler) {
            element->on(Nova::EventType::MouseDown, (*clickHandler).asCallback());
        }
    }

    void ComponentManager::walk(Nova::Element *currentNode, Api::CElement* currentElement) {
        Nova::Element* newElement{};
        Nova::Document& document = mContext->document;

        switch (currentElement->getType()) {
            case Api::E::Div:
            {
                auto element = document.createElement<Nova::Div>();
                patch(element, currentElement);
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Img:
            {
                auto element = document.createElement<Nova::Img>();
                patch(element, currentElement);
                auto src = currentElement->getAttribute("src");
                element->src((*src).asString());
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Text:
            {
                std::string content = (*currentElement->getAttribute("textContent")).asString();
                currentNode->textContent = content;
                break;
            }
            case Api::E::Input:
            {
                auto element = document.createElement<Nova::Input>();
                patch(element, currentElement);
                auto placeholder = currentElement->getAttribute("placeholder");
                element->placeholder = ((*placeholder).asString());
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Button:
            {
                auto element = document.createElement<Nova::Button>();
                patch(element, currentElement);
                currentNode->append(element);
                newElement = static_cast<Nova::Element*>(element);

                break;
            }
            case Api::E::Component:
            {
                auto element = document.createElement<Nova::Div>();
                auto subtree = currentElement->render(*mContext);

                std::vector<std::shared_ptr<Api::CElement>> children{};
                children.push_back(subtree);
                currentElement->setChildren(children);

                walk(element, subtree.get());
                currentNode->append(element);

                auto component = dynamic_cast<Component*>(currentElement);
                component->parentElement = currentNode;
                component->elementRef = element;
                component->componentManager = this;

                break;
            }
        }

        if (newElement) {
            for (auto& child : currentElement->getChildren()) {
                walk(newElement, child.get());
            }
        }
    }

    void ComponentManager::render(Nova::Element *root, std::shared_ptr<Api::CElement> element) {
        mTreeRoot = element;
        walk(root, mTreeRoot.get());
    }
    
}  // namespace Cosmonaut
