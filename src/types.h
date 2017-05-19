// Copyright 2017 Dan Ristic

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <string>

namespace Cosmonaut {

enum class CursorType {
    Arrow,
    Hand,
    IBeam
};

enum class EventType {
    Input,
    Change,
    MouseMove,
    MouseDown,
    Scroll,
    Key,
    Custom
};
    
struct EventTypeHash {
    template <typename T>
    std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

class Event {
 public:
    explicit Event(EventType type)
        : type{type}
        { }
    virtual ~Event() { }

    EventType type;
};

class MouseScrollEvent: public Event {
 public:
    explicit MouseScrollEvent(int direction, int x, int y)
        : Event(EventType::Scroll)
        , direction{direction}
        , x{x}
        , y{y}
        { }

    int direction, x, y;
};

class MouseDownEvent: public Event {
 public:
    explicit MouseDownEvent(int x, int y)
        : Event(EventType::MouseDown)
        , x{x}
        , y{y}
        { }

    int x, y;
};

class MouseMoveEvent: public Event {
 public:
    explicit MouseMoveEvent(int x, int y)
        : Event(EventType::MouseMove)
        , x{x}
        , y{y}
        { }

    int x, y;
};

class TextInputEvent: public Event {
 public:
    explicit TextInputEvent(const std::string& value)
        : Event(EventType::Input)
        , value{value}
        { }

    std::string value;
};

enum KeyType {
    Backspace,
    Enter,
    Spacebar
};

class KeyEvent: public Event {
 public:
    explicit KeyEvent(KeyType key)
        : Event(EventType::Key)
        , key{key}
        { }

    KeyType key;
};

class ImageRef {
 public:
    ImageRef(int w, int h, unsigned int texId)
        : width{w}
        , height{h}
        , textureId{texId}
        { }

    ~ImageRef() { }

    virtual void render() { }

    int width;
    int height;
    unsigned int textureId;
};

class Color {
 public:
    float r;
    float g;
    float b;
    float a;

    Color() : r(0), g(0), b(0), a(0) {}
    Color(float _r, float _g, float _b, float _a)
        : r(_r)
        , g(_g)
        , b(_b)
        , a(_a)
        { }

    Color multiplyAlpha(float _a) const {
        return {r, g, b, a * _a};
    }

    static Color fromRGBA(float r, float g, float b, float a) {
        return {r/255.0f, g/255.0f, b/255.0f, a};
    }
};

struct Rect {
    float x;
    float y;
    float width;
    float height;

    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(float _x, float _y, float _width, float _height)
        : x(_x)
        , y(_y)
        , width(_width)
        , height(_height)
        { }

    bool contains(float _x, float _y) {
        if (_x > x && _y > y && _x < x + width && _y < y + height) {
            return true;
        } else {
            return false;
        }
    }
};

}  // namespace Cosmonaut

#endif  // SRC_TYPES_H_
