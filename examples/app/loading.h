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
        lastTime = std::chrono::system_clock::now();

        componentManager->tasks.push_back(
            std::bind(&LoadingComponent::update, this)
        );
    }

    void update() {
        if (loadedPercentage < 100) {
            auto now = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = now - lastTime;

            if (diff > std::chrono::milliseconds(50)) {
                loadedPercentage++;
                invalidate();

                lastTime = now;
            }

            componentManager->tasks.push_back(
                std::bind(&LoadingComponent::update, this)
            );
        } else if (!loaded) {
            auto now = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = now - lastTime;

            if (diff > std::chrono::seconds(3)) {
                loaded = true;
                invalidate();
            }
        }
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

        if (!mounted) {
            document.styleManager.addRule("#loading-container", {
                {StyleProp::Width, width},
                {StyleProp::Height, 100},
                {StyleProp::Margin, LayoutProperty(300, 0, 0, 0)},
                {StyleProp::FlexDirection, Direction::Column},
                {StyleProp::JustifyContent, Align::Center}
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
        }

        Style textStyle{};
        textStyle.animationName = loaded ? "slide-out" : "slide-in";

        return C(E::Div, {{"id", "loading-container"}}, {
                    C(E::Div, {{"id", "loading-percent"}, {"style", textStyle}}, {
                        C(E::Text, {{"textContent", getLoadingPercent()}})
                    }),
                    C(E::Div, {{"id", "loading-text"}, {"style", textStyle}}, {
                        C(E::Text, {{"textContent", "Loading..."}})
                    })
                });
    }

    int loadedPercentage{0};
    bool loaded{false};
    std::chrono::time_point<std::chrono::system_clock> lastTime;
};
