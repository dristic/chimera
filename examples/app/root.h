// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include "Cosmonaut/Cosmonaut.h"

#include "app/loading.h"

class RootComponent : public Cosmonaut::Component {
public:
    RootComponent()
        : Cosmonaut::Component()
        { }

    void componentDidMount() override {
    }

    std::shared_ptr<Cosmonaut::Api::CElement> render(Nova::Context& context) override {
        using namespace Nova;
        using namespace Cosmonaut::Api;
        using namespace std::placeholders;

        Nova::Document& document = context.document;

        int width = document.getWidth();
        int height = document.getHeight();

        if (!mounted) {
            document.animationController.addAnimation("slide-in",
                [](float time, Nova::Element* element) {
                    float pos = -50 * time * (time-2);
                    element->style.margin.top = 50 - pos;
                    element->style.opacity = -1 * time * (time - 2);
                });

            document.animationController.addAnimation("slide-out",
                [](float time, Nova::Element* element) {
                    float pos = -50 * time * (time-2);
                    element->style.margin.top = pos;
                    element->style.opacity = 1 - (-1 * time * (time - 2));
                });

            document.styleManager.addRule("#background", {
                {Nova::StyleProp::Width, width},
                {Nova::StyleProp::Height, height},
                {Nova::StyleProp::BackgroundImage, "assets/bg.png"},
                {Nova::StyleProp::AnimationName, "fade-in"}
            });
        }

        printf("Loaded %s\n", loaded ? "true" : "false");

        return C(E::Div, {{"id", "background"}}, {
                    std::make_shared<LoadingComponent>([this]() {
                        loaded = true;
                        invalidate();
                    })
                });
    }

private:
    bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
