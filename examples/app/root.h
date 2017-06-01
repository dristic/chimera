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

    void addStyles(Chimera::Document& document)
    {
        document.styleManager.addRule("#username", {
            {Chimera::StyleProp::Color, Chimera::Color::fromRGBA(255, 255, 255, 1)}
        });

        document.styleManager.addRule("#user-icon", {
            {Chimera::StyleProp::Width, 50},
            {Chimera::StyleProp::Height, 50}
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
                    Chimera::Virtual::VElement("div", {{"id", "username"}}, "Lionheart"),
                    Chimera::Virtual::VElement("div", {{"id", "username"}}, "Online")
                }))
        ));
    }
};

class AppElement : public Chimera::Element {
public:
    AppElement(Chimera::Document& document)
        : Chimera::Element("app-element", document)
    {
        document.registerElement<Heading>("heading");

        addStyles(document);
        createTree(document);
    }

    void addStyles(Chimera::Document& document)
    {
        int width = document.getWidth();
        int height = document.getHeight();

        document.styleManager.addRule("#background", {
            {Chimera::StyleProp::Width, width},
            {Chimera::StyleProp::Height, height},
            {Chimera::StyleProp::BackgroundImage, "assets/bg.png"},
            {Chimera::StyleProp::FlexDirection, Chimera::Direction::Column},
            {Chimera::StyleProp::AnimationName, "fade-in"}
        });

        document.styleManager.addRule("#title", {
            {Chimera::StyleProp::Color, Chimera::Color::fromRGBA(255, 255, 255, 1)}
        });
    }

    void createTree(Chimera::Document& document)
    {
        append(Chimera::Virtual::CreateTree(document,
            Chimera::Virtual::VElement("div", {{"id", "background"}},
                std::vector<Chimera::Virtual::VirtualElement>({
                    Chimera::Virtual::VElement("heading", {{"id", "title"}}),
                    Chimera::Virtual::VElement("heading", {{"id", "title"}}),
                    Chimera::Virtual::VElement("heading", {{"id", "title"}})
                }))
        ));
    }

private:
    bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
