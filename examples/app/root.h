// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include <Chimera/Chimera.h>

class CustomComponent : public Chimera::Element {
public:
    CustomComponent(Chimera::Document& document)
        : Chimera::Element("heading", document)
    {
        addStyles(document);
        createTree(document);
    }

    void attributeChangedCallback(
        std::string name, std::string oldValue, std::string newValue) override
    {
        CHIMERA_UNUSED(oldValue);

        if (name == "size")
        {
            mSize = newValue;

            auto element = getElementById("user-icon");
            element->className = mSize;

            element = getElementById("username");
            element->className = mSize;
        }

        if (name == "name")
        {
            auto element = getElementById("username");
            element->textContent = newValue;
        }

        if (name == "status")
        {
            auto element = getElementById("status");
            element->textContent = newValue;
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
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)},
            {StyleProp::FontSize, 24}
        });

        document.styleManager.addRule("#username.small", {
            {StyleProp::FontSize, 18}
        });

        document.styleManager.addRule("#status", {
            {StyleProp::Margin, LayoutProperty({2, 5, 5, 5})},
            {StyleProp::Color, Color::fromRGBA(150, 150, 150, 1)},
            {StyleProp::FontSize, 16}
        });

        document.styleManager.addRule("#user-icon", {
            {StyleProp::Width, 50},
            {StyleProp::Height, 50},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#user-icon.small", {
            {StyleProp::Width, 30},
            {StyleProp::Height, 30}
        });

        document.styleManager.addRule("#heading-right", {
            {StyleProp::FlexDirection, Chimera::Direction::Column}
        });
    }

    void createTree(Chimera::Document& document)
    {
        using namespace Chimera;

        append(Virtual::CreateTree(document,
            Virtual::VElement("div", {{"id", "heading"}},
                std::vector<Virtual::VirtualElement>({
                    Virtual::VElement("img", {
                        {"src", "assets/logo.png"},
                        {"id", "user-icon"}
                    }),
                    Virtual::VElement("div", {{"id", "heading-right"}},
                        std::vector<Virtual::VirtualElement>({
                            Virtual::VElement("div", {{"id", "username"}}, "Username"),
                            Virtual::VElement("div", {{"id", "status"}}, "Online")
                        }))
                }))
        ));
    }

 private:
    std::string mSize{"large"};
};

class AppElement : public Chimera::Element {
public:
    AppElement(Chimera::Document& document)
        : Chimera::Element("app-element", document)
    {
        document.registerElement<CustomComponent>("custom-component");

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
            {StyleProp::FlexDirection, Direction::Column}
        });

        document.styleManager.addRule("#title", {
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#image", {
            {StyleProp::Width, 50},
            {StyleProp::Height, 50},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#input-box", {
            {StyleProp::Width, 380},
            {StyleProp::Height, 24},
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)},
            {StyleProp::FontSize, 18},
            {StyleProp::BackgroundColor, Color::fromRGBA(15, 40, 67, 1)},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#button", {
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)},
            {StyleProp::FontSize, 18},
            {StyleProp::BackgroundColor, Color::fromRGBA(15, 40, 67, 1)},
            {StyleProp::Padding, LayoutProperty({10, 10, 10, 10})},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#button:hover", {
            {StyleProp::BackgroundColor, Color::fromRGBA(25, 47, 71, 1)}
        });
    }

    void createTree(Chimera::Document& document)
    {
        using namespace Chimera;

        append(Virtual::CreateTree(document,
            Virtual::VElement("div", {{"id", "background"}},
                std::vector<Virtual::VirtualElement>({
                    Virtual::VElement("div", {{"id", "title"}}, "Hello World"),
                    Virtual::VElement("img", {
                        {"src", "assets/logo.png"},
                        {"id", "image"}
                    }),
                    Virtual::VElement("input", {{"id", "input-box"}}),
                    Virtual::VElement("button", {{"id", "button"}}, "Accept")
                }))
        ));
    }

private:
    bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
