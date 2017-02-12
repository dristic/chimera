// Copyright 2016 Dan Ristic

#ifndef SRC_ELEMENT_H_
#define SRC_ELEMENT_H_

#include <string>
#include <stack>
#include <vector>
#include <unordered_map>

#include "src/style.h"
#include "src/types.h"
#include "src/layout.h"

namespace Nova {

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
    void dispatch(EventType type, Event* event);

    virtual bool handleEvent(Event* event);
    virtual void render(DrawData* data, Style* parentStyle);

    // Declarative methods
    Element* id(const std::string& id) { mId = id; return this; }
    std::string id() { return mId; }
    Element* text(const std::string& text) { textContent = text; return this; }
    std::string text() { return textContent; }
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
                if (el->id() == query) {
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

 protected:
    std::unordered_map<EventType, EventCallback, EventTypeHash> mEventObservers;
    std::vector<Element*> mChildren;
    PseudoClass mPseudoClass;
    Document* mDocument;
    std::unique_ptr<ImageRef> mBackgroundImage;

    std::string mId;
};

class Div: public Element {
 public:
    Div(Document& document);
    ~Div();

    bool handleEvent(Event* event) override;
    void render(DrawData* data, Style* parentStyle) override;
};

class Img: public Element {
 public:
    Img(Document& document);
    ~Img();

    void render(DrawData* data, Style* parentStyle);

    std::string const& src() const { return mSrc; }
    void src(std::string const& newSrc);

 private:
    std::string mSrc;
    std::unique_ptr<ImageRef> mImageRef;
};

class Input: public Element {
 public:
    Input(Document& document);
    ~Input();

    bool handleEvent(Event* event) override;
    void render(DrawData* data, Style* parentStyle) override;

    std::string value;
    std::string placeholder;

 private:
    int mFrames;
};

class Button: public Element {
 public:
    Button(Document& document);
    ~Button();

    bool handleEvent(Event* event) override;
    void render(DrawData* data, Style* parentStyle) override;
};

}  // namespace Nova

#endif  // SRC_ELEMENT_H_
