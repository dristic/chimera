// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include <Chimera/Chimera.h>

class AppElement : public Chimera::Element {
public:
    AppElement(Chimera::Document& document)
        : Chimera::Element("app-element", document)
    {
        addStyles(document);
        createTree(document);
    }

    void addStyles(Chimera::Document& document)
    {
        int width = document.getWidth();
        int height = document.getHeight();

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

        document.styleManager.addRule("#background", {
            {Chimera::StyleProp::Width, width},
            {Chimera::StyleProp::Height, height},
            {Chimera::StyleProp::BackgroundImage, "assets/bg.png"},
            {Chimera::StyleProp::AnimationName, "fade-in"},
            {Chimera::StyleProp::Color, Chimera::Color::fromRGBA(255, 255, 255, 1)}
        });
    }

    void createTree(Chimera::Document& document)
    {
        append(Chimera::Virtual::CreateTree(document,
            Chimera::Virtual::VElement<Chimera::Div>({{"id", "background"}}, "Hello World")
        ));
    }

private:
    bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
