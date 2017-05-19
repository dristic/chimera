// // Copyright 2016 Dan Ristic

// #include "src/component/component.h"

// #include "src/document.h"
// #include "src/context.h"

// namespace Cosmonaut {

//     namespace Api {

//         E CElement::getType() {
//             return mType;
//         }

//         Attribute* CElement::getAttribute(const std::string& key) {
//             if (mAttributes.count(key) == 1) {
//                 return &mAttributes.at(key);
//             } else {
//                 return nullptr;
//             }
//         }

//         std::vector<std::shared_ptr<CElement>> const& CElement::getChildren() const {
//             return mChildren;
//         }

//         void CElement::setChildren(std::vector<std::shared_ptr<CElement>>& children) {
//             mChildren = std::move(children);
//         }

//         std::shared_ptr<Api::CElement> CElement::render(Cosmonaut::Context& context) {
//             return nullptr;
//         }


//     }  // namespace Api

//     ComponentManager::ComponentManager(Cosmonaut::Context* context)
//         : mContext{context}
//         { }

//     void ComponentManager::update(double dt) {
//         size_t length = tasks.size();
//         for (int i = 0; i < length; i++) {
//             tasks[i]();
//         }

//         tasks.erase(tasks.begin(), tasks.begin() + (length - 1));
//     }

//     void ComponentManager::patch(
//         PatchMode mode,
//         Api::CElement* currentNode,
//         Api::CElement* newNode,
//         Cosmonaut::Element* element
//     ) {
//         Cosmonaut::Document& document = mContext->document;

//         if (mode == PatchMode::Create) {
//             Cosmonaut::Element* newElement{};

//             switch(newNode->getType()) {
//                 case Api::E::Div:
//                 {
//                     newElement = document.createElement<Cosmonaut::Div>();
//                     break;
//                 }
//                 case Api::E::Img:
//                 {
//                     newElement = document.createElement<Cosmonaut::Img>();
//                     auto src = newNode->getAttribute("src");
//                     if (src) {
//                         static_cast<Cosmonaut::Img*>(newElement)->src((*src).asString());
//                     }
//                     break;
//                 }
//                 case Api::E::Text:
//                 {
//                     std::string content = (*newNode->getAttribute("textContent")).asString();
//                     element->textContent = content;
//                     break;
//                 }
//                 case Api::E::Input:
//                 {
//                     newElement = document.createElement<Cosmonaut::Input>();
//                     auto placeholder = newNode->getAttribute("placeholder");
//                     if (placeholder) {
//                         static_cast<Cosmonaut::Input*>(newElement)->placeholder = (*placeholder).asString();
//                     }
//                     break;
//                 }
//                 case Api::E::Button:
//                 {
//                     newElement = document.createElement<Cosmonaut::Button>();
//                     break;
//                 }
//                 case Api::E::Component:
//                 {
//                     newElement = document.createElement<Cosmonaut::Div>();
//                     auto subtree = newNode->render(*mContext);

//                     std::vector<std::shared_ptr<Api::CElement>> children{};
//                     children.push_back(subtree);
//                     newNode->setChildren(children);

//                     patch(PatchMode::Create, nullptr, subtree.get(), newElement);

//                     auto component = dynamic_cast<Component*>(newNode);
//                     component->parentElement = element;
//                     component->elementRef = newElement;
//                     component->componentManager = this;

//                     if (!component->mounted) {
//                         component->componentDidMount();
//                         component->mounted = true;
//                     }

//                     break;
//                 }
//             }

//             auto id = newNode->getAttribute("id");
//             if (id) {
//                 newElement->id((*id).asString());
//             }

//             auto style = newNode->getAttribute("style");
//             if (style) {
//                 newElement->style = (*style).asStyle();
//             }

//             auto clickHandler = newNode->getAttribute("onClick");
//             if (clickHandler) {
//                 newElement->on(Cosmonaut::EventType::MouseDown, (*clickHandler).asCallback());
//             }

//             if (newElement) {
//                 element->append(newElement);

//                 if (newNode->getType() != Api::E::Component) {
//                     for (auto& child : newNode->getChildren()) {
//                         patch(PatchMode::Create, nullptr, child.get(), newElement);
//                     }
//                 }
//             }
//         } else if (mode == PatchMode::Diff) {
//             if (newNode->getType() == Api::E::Text) {
//                 auto newContent = (*newNode->getAttribute("textContent")).asString();
//                 auto oldContent = (*currentNode->getAttribute("textContent")).asString();
//                 if (newContent != oldContent) {
//                     element->textContent = newContent;
//                 }
//             } else {
//                 auto newId = (*newNode->getAttribute("id")).asString();
//                 auto oldId = (*currentNode->getAttribute("id")).asString();
//                 if (newId != oldId) {
//                     element->id(newId);
//                 }

//                 auto style = newNode->getAttribute("style");
//                 if (style) {
//                     element->style.animationName = (*style).asStyle().animationName;
//                     element->style.opacity = (*style).asStyle().opacity;
//                 }
//             }
//         }
//     }

//     void ComponentManager::walk(
//         Api::CElement* currentNode,
//         Api::CElement* newNode,
//         Cosmonaut::Element *rootElement
//     ) {
//         Cosmonaut::Document& document = mContext->document;

//         if (newNode) {
//             if (!currentNode) {
//                 patch(PatchMode::Create, currentNode, newNode, rootElement);
//             } else {
//                 patch(PatchMode::Diff, currentNode, newNode, rootElement);

//                 auto currentChildren = currentNode->getChildren();
//                 for(size_t i = 0; i < currentChildren.size(); i++) {
//                     auto currentChild = currentChildren[i];
//                     auto newChild = newNode->getChildren()[i];

//                     if (newChild->getType() == Api::E::Text) {
//                         patch(PatchMode::Diff, currentChild.get(), newChild.get(), rootElement);
//                     } else {
//                         auto childElement = rootElement->getChildren()[i];
//                         walk(currentChild.get(), newChild.get(), childElement);
//                     }
//                 }
//             }
//         } else {
//             if (currentNode) {
//                 // delete
//             } else {
//                 // ?
//             }
//         }
//     }

//     void ComponentManager::invalidate(Component* node) {
//         auto subtree = node->render(*mContext);

//         // Assumme there is only one child
//         auto currentSubtree = node->getChildren()[0];
//         walk(currentSubtree.get(), subtree.get(), node->elementRef->getChildren()[0]);
//     }

//     void ComponentManager::render(Cosmonaut::Element *root, std::shared_ptr<Api::CElement> element) {
//         mTreeRoot = element;

//         walk(nullptr, mTreeRoot.get(), root);
//     }
    
// }  // namespace Cosmonaut
