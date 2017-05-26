// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include <Cosmonaut/Cosmonaut.h>

class AppElement : public Cosmonaut::Element {
public:
    AppElement(Cosmonaut::Document& document)
        : Cosmonaut::Element("app-element", document)
    {
        addStyles(document);
        createTree(document);
    }

    void addStyles(Cosmonaut::Document& document)
    {
        int width = document.getWidth();
        int height = document.getHeight();

        document.animationController.addAnimation("slide-in",
            [](float time, Cosmonaut::Element* element) {
                float pos = -50 * time * (time-2);
                element->style.margin.top = 50 - pos;
                element->style.opacity = -1 * time * (time - 2);
            });

        document.animationController.addAnimation("slide-out",
            [](float time, Cosmonaut::Element* element) {
                float pos = 200 * time * (time-2);
                element->style.margin.top = pos;
                element->style.opacity = 1 - (-1 * time * (time - 2));
            });

        document.animationController.addAnimation("fade-in",
            [](float time, Cosmonaut::Element* element) {
                element->style.opacity = -1 * time * (time - 2);
            });

        document.styleManager.addRule("#background", {
            {Cosmonaut::StyleProp::Width, width},
            {Cosmonaut::StyleProp::Height, height},
            {Cosmonaut::StyleProp::BackgroundImage, "assets/bg.png"},
            {Cosmonaut::StyleProp::AnimationName, "fade-in"}
        });
    }

    void createTree(Cosmonaut::Document& document)
    {
        append(Cosmonaut::Api::CreateTree(document,
            Cosmonaut::Api::VElement<Cosmonaut::Div>({{"id", "background"}})
        ));

        /*
            {{"id", "background"}}
        */
    }

private:
    bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
