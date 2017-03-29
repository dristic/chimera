// Copyright 2016 Dan Ristic

#include "Cosmonaut/Cosmonaut.h"

#include <thread>
#include <chrono>
#include <atomic>

class LoadingComponent : public Cosmonaut::Component {
public:
    LoadingComponent()
        : Cosmonaut::Component()
        { }

    void componentDidMount() override {
        componentManager->tasks.push_back(
            std::bind(&LoadingComponent::update, this)
        );
    }

    void update() {
        if (loadedPercentage < 1000) {
            loadedPercentage++;
            if (loadedPercentage % 60 == 0) {
                invalidate();
            }
        }

        componentManager->tasks.push_back(
            std::bind(&LoadingComponent::update, this)
        );
    }

    std::string getLoadingPercent() {
        return std::to_string(loadedPercentage) + "%";
    }

    std::shared_ptr<Cosmonaut::Api::CElement> render(Nova::Context& context) override {
        using namespace Nova;
        using namespace Cosmonaut::Api;
        using namespace std::placeholders;

        Nova::Document& document = context.document;

        int width = document.getWidth();
        int height = document.getHeight();

        document.styleManager.addRule("#background", {
            {StyleProp::Width, width},
            {StyleProp::Height, height},
            {StyleProp::BackgroundImage, "assets/bg.png"},
            {StyleProp::AnimationName, "fade-in"}
        });

        document.styleManager.addRule("#loading-container", {
            {StyleProp::Width, width},
            {StyleProp::Height, 100},
            {StyleProp::Margin, LayoutProperty(300, 0, 0, 0)},
            {StyleProp::FlexDirection, Direction::Column},
            {StyleProp::JustifyContent, Align::Center},
            {StyleProp::AnimationName, "slide-in"}
        });

        document.styleManager.addRule("#loading-text", {
            {StyleProp::Width, 300},
            {StyleProp::Height, 20},
            {StyleProp::Margin, LayoutProperty(10, 0, 0, 0)},
            {StyleProp::TextAlign, Align::Center},
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)}
        });

        document.styleManager.addRule("#loading-percent", {
            {StyleProp::Width, 300},
            {StyleProp::Height, 32},
            {StyleProp::FontSize, 32},
            {StyleProp::TextAlign, Align::Center},
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)}
        });

        return C(E::Div, {{"id", "background"}}, {
                C(E::Div, {{"id", "loading-container"}}, {
                    C(E::Div, {{"id", "loading-percent"}}, {
                        C(E::Text, {{"textContent", getLoadingPercent()}})
                    }),
                    C(E::Div, {{"id", "loading-text"}}, {
                        C(E::Text, {{"textContent", "Loading..."}})
                    })
                })
            });
    }

    int loadedPercentage{0};
};
