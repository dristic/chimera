// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_LOADING_H_
#define EXAMPLES_APP_LOADING_H_

#include <Cosmonaut/Cosmonaut.h>

#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class LoadingComponent : public Cosmonaut::Element {
public:
    LoadingComponent(std::string name, Cosmonaut::Document& document)
        : Cosmonaut::Element(name, document)
    {
        addStyles(document);
        createTree(document);
    }

    void addStyles(Cosmonaut::Document& document)
    {
        int width = document.getWidth();
        int height = document.getHeight();

        document.styleManager.addRule("#loading-container", {
            {StyleProp::Width, width},
            {StyleProp::Height, 100},
            {StyleProp::Margin, LayoutProperty(300, 0, 0, 0)},
            {StyleProp::FlexDirection, Direction::Column},
            {StyleProp::JustifyContent, Align::Center}
        });

        document.styleManager.addRule("#text-container", {
            {StyleProp::Width, width},
            {StyleProp::Height, 100},
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

    void createTree(Cosmonaut::Document& document)
    {
        Style textStyle{};
        textStyle.animationName = loaded ? "slide-out" : "slide-in";

        using C = Cosmonaut::Create;
        using Dic = Cosmonaut::Div;

        mRoot = Cosmonaut::CreateTree(
            C<Div>({{"id", "loading-container"}}, {
                C<Div>({{"id", "text-container"}, {"style", textStyle}}, {
                    C<Div>({{"id", "loading-percent"}}, {
                        C(E::Text, {{"textContent", getLoadingPercent()}})
                    }),
                    C<Div>({{"id", "loading-text"}}, {
                        C(E::Text, {{"textContent", mMessage}})
                    })
                })
            })
        );
    }

    void connectedCallback() override
    {
        lastTime = std::chrono::system_clock::now();
    }

    std::string getLoadingPercent()
    {
        return std::to_string(loadedPercentage) + "%";
    }

    void render(DrawData* data, Style* parentStyle) override
    {
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = now - lastTime;

        if (loadedPercentage < 100) {
            if (diff > std::chrono::milliseconds(50)) {
                loadedPercentage++;
                lastTime = now;
            }
        } else if (!loaded) {
            if (diff > std::chrono::seconds(3)) {
                loaded = true;
                mOnLoaded();
            }
        }

        auto text = getElementById("loading-percent");
        text.textContent = getLoadingPercent();

        mRoot->render(data, parentStyle);
    }

    int loadedPercentage{0};
    bool loaded{false};
    std::chrono::time_point<std::chrono::system_clock> lastTime;
    std::string mMessage;
    std::function<void(void)> mOnLoaded;

    Cosmonaut::Element* mRoot;
};

#endif  // EXAMPLES_APP_LOADING_H_
