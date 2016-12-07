// Copyright 2016 Dan Ristic

#ifndef SRC_ELEMENT_H_
#define SRC_ELEMENT_H_

#include <string>
#include <vector>
#include <unordered_map>

#include "src/rendering.h"
#include "src/style.h"
#include "src/types.h"
#include "src/layout.h"

namespace Nova {

class Document;

enum class State { None, Hover };

class Element {
    using EventCallback = std::function<void(Event*)>;
    friend class StyleRule;

 public:
    Element(std::string name, Document& document);
    virtual ~Element() { }

    void append(Element* element);
    void remove(Element* element);
    std::vector<Element*>& getChildren();

    void on(EventType type, EventCallback func);
    void dispatch(EventType type, Event* event);

    virtual bool handleEvent(Event* event);
    virtual void render(DrawData* data, Style* parentStyle);

    Rect rect;
    Rect scissor;
    Rect scroll;
    Rect scrollTarget;
    Style style;
    Layout layout;
    std::string id;
    std::string tagName;
    std::string className;
    std::string textContent;

 protected:
    std::unordered_map<EventType, EventCallback> mEventObservers;
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
