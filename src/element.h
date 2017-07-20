// Copyright 2017 Dan Ristic

#ifndef SRC_ELEMENT_H_
#define SRC_ELEMENT_H_

#include <string>
#include <stack>
#include <vector>
#include <unordered_map>

#include "src/core.h"
#include "src/style/style.h"
#include "src/types.h"
#include "src/layout.h"

namespace Chimera {

class Document;
class DrawData;

enum class State { None, Hover };

class Element {
    using EventCallback = std::function<void(Event*)>;
    friend class StyleRule;

 public:
    Element(std::string name, Document& document);
    virtual ~Element() { }

    void append(Element* element);
    void remove(Element* element);
    const std::vector<Element*>& getChildren() const;

    void on(EventType type, EventCallback func);
    void dispatch(Event* event);

    virtual bool handleEvent(Event* event);
    virtual void render(DrawData* data);

    virtual void attributeChangedCallback(
        std::string name, std::string oldValue, std::string newValue)
    {
        CHIMERA_UNUSED(name);
        CHIMERA_UNUSED(oldValue);
        CHIMERA_UNUSED(newValue);
    };

    // Declarative methods
    Element* children(std::vector<Element*> children) {
        mChildren.insert(mChildren.end(), children.begin(), children.end());

        return this;
    }
    Element* onEvent(EventType type, EventCallback func) {
        on(type, std::forward<EventCallback>(func));
        return this;
    }

    Element* getElementById(std::string query) {
        std::stack<Element*> stack;
        Element* found = nullptr;

        for(auto child : mChildren) {
            stack.push(child);
        }

        while (!stack.empty()) {
            Element* el = stack.top();
            stack.pop();

            if (el) {
                if (el->id == query) {
                    found = el;
                    break;
                }
            }

            for(auto& child : el->getChildren()) {
                stack.push(child);
            }
        }

        return found;
    }

    Rect rect;
    Rect scissor;
    Rect scroll;
    Rect scrollTarget;
    Style style;
    Layout layout;
    std::string tagName;
    std::string className;
    std::string textContent;

    std::string id;

 protected:
    std::unordered_map<EventType, EventCallback, EventTypeHash> mEventObservers;
    std::vector<Element*> mChildren;
    PseudoClass mPseudoClass;
    Document* mDocument;
    std::unique_ptr<ImageRef> mBackgroundImage;
};

class Div: public Element {
 public:
    Div(Document& document);
    ~Div();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

class Img: public Element {
 public:
    Img(Document& document);
    ~Img();

    void render(DrawData* data);

    std::string const& getSrc() const { return mSrc; }
    void setSrc(std::string const& newSrc);

 private:
    std::string mSrc;
    std::unique_ptr<ImageRef> mImageRef;
};

class Button: public Element {
 public:
    Button(Document& document);
    ~Button();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // SRC_ELEMENT_H_
