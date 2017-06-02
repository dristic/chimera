// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include <Chimera/Chimera.h>

/*
root
    header
        img
        name
        status
    list
        title
        user
            img
            name
            status
        user2...
    list2...

document.animationController.addAnimation("slide-in",
    [](float time, Chimera::Element* element) {
        float pos = -50 * time * (time-2);
        element->style.margin.top = 50 - pos;
        element->style.opacity = -1 * time * (time - 2);
    });

document.animationController.addAnimation("slide-out",
    [](float time, Chimera::Element* element) {
        float pos = 200 * time * (time-2);
        element->style.margin.top = pos;
        element->style.opacity = 1 - (-1 * time * (time - 2));
    });

document.animationController.addAnimation("fade-in",
    [](float time, Chimera::Element* element) {
        element->style.opacity = -1 * time * (time - 2);
    });
*/

class Heading : public Chimera::Element {
public:
    Heading(Chimera::Document& document)
        : Chimera::Element("heading", document)
    {
        addStyles(document);
        createTree(document);
    }

    void attributeChangedCallback(
        std::string name, std::string oldValue, std::string newValue) override
    {
        if (name == "size")
        {
            mSize = newValue;

            auto element = getElementById("heading");
            if (element)
            {
                element->className = mSize;
            }
        }
    }

    void addStyles(Chimera::Document& document)
    {
        using namespace Chimera;

        document.styleManager.addRule("#heading", {
            {StyleProp::Margin, LayoutProperty({5, 5, 5, 5})}
        });

        document.styleManager.addRule("#username", {
            {StyleProp::Margin, LayoutProperty({5, 5, 5, 0})},
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)}
        });

        document.styleManager.addRule("#status", {
            {StyleProp::Margin, LayoutProperty({5, 5, 5, 5})},
            {StyleProp::Color, Color::fromRGBA(150, 150, 150, 1)},
            {StyleProp::FontSize, 20}
        });

        document.styleManager.addRule("#user-icon", {
            {StyleProp::Width, 50},
            {StyleProp::Height, 50},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#heading-right", {
            {StyleProp::FlexDirection, Chimera::Direction::Column}
        });
    }

    void createTree(Chimera::Document& document)
    {
        append(Chimera::Virtual::CreateTree(document,
            Chimera::Virtual::VElement("div", {{"id", "heading"}},
                std::vector<Chimera::Virtual::VirtualElement>({
                    Chimera::Virtual::VElement("img", {
                        {"src", "assets/logo.png"},
                        {"id", "user-icon"}
                    }),
                    Chimera::Virtual::VElement("div", {{"id", "heading-right"}},
                        std::vector<Chimera::Virtual::VirtualElement>({
                            Chimera::Virtual::VElement("div", {{"id", "username"}}, "Username"),
                            Chimera::Virtual::VElement("div", {{"id", "status"}}, "Online")
                        }))
                }))
        ));
    }

 private:
    std::string mSize{"large"};
};

class Spacer : public Chimera::Element {
public:
    Spacer(Chimera::Document& document)
        : Chimera::Element("spacer", document)
    {
        addStyles(document);
        createTree(document);
    }

    void addStyles(Chimera::Document& document)
    {
        using namespace Chimera;

        document.styleManager.addRule("#spacer-bar", {
            {StyleProp::Width, 300},
            {StyleProp::Height, 5},
            {StyleProp::BackgroundColor, Color::fromRGBA(255, 255, 255, 1)}
        });
    }

    void createTree(Chimera::Document& document)
    {
        append(Chimera::Virtual::CreateTree(document,
            Chimera::Virtual::VElement("div", {{"id", "spacer-bar"}})
        ));
    }
};

class AppElement : public Chimera::Element {
public:
    AppElement(Chimera::Document& document)
        : Chimera::Element("app-element", document)
    {
        document.registerElement<Heading>("heading");
        document.registerElement<Spacer>("spacer");

        addStyles(document);
        createTree(document);
    }

    void addStyles(Chimera::Document& document)
    {
        using namespace Chimera;

        int width = document.getWidth();
        int height = document.getHeight();

        document.styleManager.addRule("#background", {
            {StyleProp::Width, width},
            {StyleProp::Height, height},
            {StyleProp::BackgroundImage, "assets/bg.png"},
            {StyleProp::FlexDirection, Direction::Column},
            {StyleProp::AnimationName, "fade-in"}
        });
    }

    void createTree(Chimera::Document& document)
    {
        append(Chimera::Virtual::CreateTree(document,
            Chimera::Virtual::VElement("div", {{"id", "background"}},
                std::vector<Chimera::Virtual::VirtualElement>({
                    Chimera::Virtual::VElement("heading", {}),
                    Chimera::Virtual::VElement("spacer", {}),
                    Chimera::Virtual::VElement("heading", {{"size", "small"}}),
                    Chimera::Virtual::VElement("heading", {}),
                    Chimera::Virtual::VElement("heading", {}),
                    Chimera::Virtual::VElement("heading", {})
                }))
        ));
    }

private:
    bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
