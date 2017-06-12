// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include <Chimera/Chimera.h>

/*

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

struct UserData {
    std::string name{""};
    std::string status{""};
};

struct SocialData {
    UserData currentUser{"Validus", "Online"};
    std::vector<UserData> friends{
        {"Validus", "Online"},
        {"Validus", "Online"},
        {"Validus", "Online"},
        {"Validus", "Online"},
        {"Validus", "Online"}
    };
};

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
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)}
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

class ChatWindow : public Chimera::Element {
public:
    ChatWindow(Chimera::Document& document)
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
        document.registerElement<ChatWindow>("chat-window");

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
            {StyleProp::AnimationName, "fade-in"}
        });

        document.styleManager.addRule("#sidebar", {
            {StyleProp::Width, 300},
            {StyleProp::Height, height},
            {StyleProp::FlexDirection, Direction::Column},
            {StyleProp::BackgroundColor, Color::fromRGBA(11, 30, 50, 1)}
        });
    }

    void createTree(Chimera::Document& document)
    {
        append(Chimera::Virtual::CreateTree(document,
            Chimera::Virtual::VElement("div", {{"id", "background"}},
                std::vector<Chimera::Virtual::VirtualElement>({
                    Chimera::Virtual::VElement("div", {{"id", "sidebar"}},
                        std::vector<Chimera::Virtual::VirtualElement>({
                            Chimera::Virtual::VElement("heading", {}),
                            Chimera::Virtual::VElement("spacer", {}),
                            Chimera::Virtual::VElement("heading", {{"size", "small"}}),
                            Chimera::Virtual::VElement("heading", {{"size", "small"}}),
                            Chimera::Virtual::VElement("heading", {{"size", "small"}}),
                            Chimera::Virtual::VElement("heading", {{"size", "small"}})
                        })),
                    Chimera::Virtual::VElement("chat-window", {})
                }))
        ));
    }

private:
    bool loaded{false};

    SocialData data;
};

#endif  // EXAMPLES_APP_ROOT_H_
