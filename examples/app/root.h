// Copyright 2017 Dan Ristic

#ifndef EXAMPLES_APP_ROOT_H_
#define EXAMPLES_APP_ROOT_H_

#include "chimera/chimera.h"

#include "chimera/dom/div.h"

#include <string>
#include <algorithm>

std::string trim(const std::string& str)
{
    std::size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    std::size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

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
        using namespace Chimera;

        int width = document.getWidth();
        int height = document.getHeight();

        document.styleManager.addRule("body", {
            {StyleProp::Color, Color::fromRGBA(255, 255, 255, 1)},
            {StyleProp::FontFamily, "LucidaGrande"}
        });

        document.styleManager.addRule("#background", {
            {StyleProp::Width, width},
            {StyleProp::Height, height},
            {StyleProp::BackgroundImage, "examples/assets/bg.png"},
            {StyleProp::FlexDirection, Direction::Column}
        });

        document.styleManager.addRule("#title", {
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
            {StyleProp::FontSize, 18},
            {StyleProp::BackgroundColor, Color::fromRGBA(15, 40, 67, 1)},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#input-checkbox", {
            {StyleProp::FontSize, 18},
            {StyleProp::BackgroundColor, Color::fromRGBA(15, 40, 67, 1)},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#button", {
            {StyleProp::FontSize, 18},
            {StyleProp::BackgroundColor, Color::fromRGBA(15, 40, 67, 1)},
            {StyleProp::Padding, LayoutProperty({10, 10, 10, 10})},
            {StyleProp::Margin, LayoutProperty({10, 10, 10, 10})}
        });

        document.styleManager.addRule("#button:hover", {
            {StyleProp::BackgroundColor, Color::fromRGBA(25, 47, 71, 1)}
        });
    }

    void onClick(Chimera::Event* event)
    {
        CHIMERA_UNUSED(event);
        printf("Got event!\n");
    }

    #define CHIMERA_REGISTER_EVENT(c, n) \
        if (name == #n) \
        { return std::bind(&c::n, this, std::placeholders::_1); }

    std::function<void(Chimera::Event*)> connect(std::string name)
    {
        CHIMERA_REGISTER_EVENT(AppElement, onClick)

        return [](Chimera::Event*) {};
    }

    // Chimera::Virtual::VirtualElement createNodes(GumboNode* node) {
    //     using namespace Chimera::Virtual;

    //     VirtualElement element = V("div", {});

    //     if (node->v.element.original_tag.data)
    //     {
    //         std::string elementText{node->v.element.original_tag.data};
    //         std::size_t spaceIndex = elementText.find_first_of(" ");
    //         std::size_t caretIndex = elementText.find_first_of(">");
    //         std::string tagName = elementText.substr(
    //             1, std::min(spaceIndex, caretIndex) - 1);
    //         std::cout << tagName << std::endl;
    //     }

    //     GumboAttribute* id{nullptr};
    //     GumboAttribute* src{nullptr};
    //     if (node->v.element.tag == GUMBO_TAG_DIV) {
    //         id = gumbo_get_attribute(&node->v.element.attributes, "id");

    //         element.attributes["id"] = {"id", id->value};
    //     }

    //     if (node->v.element.tag == GUMBO_TAG_IMG) {
    //         id = gumbo_get_attribute(&node->v.element.attributes, "id");
    //         src = gumbo_get_attribute(&node->v.element.attributes, "src");

    //         element = V("img", {
    //             {"id", id->value},
    //             {"src", src->value}
    //         });
    //     }

    //     GumboVector* children = &node->v.element.children;
    //     for (unsigned int i = 0; i < children->length; ++i) {
    //         GumboNode* childNode = static_cast<GumboNode*>(children->data[i]);
    //         if (childNode->type == GUMBO_NODE_TEXT)
    //         {
    //             std::string str{childNode->v.text.text};
    //             str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    //             element.textContent = trim(str);
    //         }
    //         else if (childNode->type == GUMBO_NODE_ELEMENT)
    //         {
    //             if (childNode->v.element.tag != GUMBO_TAG_STYLE)
    //             {
    //                 element.children.push_back(createNodes(childNode));
    //             }
    //         }
    //     }

    //     return element;
    // }

    void createTree(Chimera::Document& document)
    {
        using namespace Chimera::Virtual;

        // std::string title = "Hello world!";

        // auto source = absl::Substitute(R"html(
        //     <div id="background">
        //         <style>
        //             body {
        //                 color: rgba(255, 255, 255, 1);
        //                 font-family: "LucidaGrande";
        //             }

        //             #background {
        //                 width: 1280;
        //                 height: 720;
        //             }
        //         </style>

        //         <div id="title">$0</div>

        //         <my-custom-element>
        //             Hello
        //         </my-custom-element>

        //         <img src="assets/logo.png" id="image" />
        //         <input id="input-box" type="password" />
        //         <input id="input-checkbox" type="checkbox" />
        //         <button id="button" onMouseDown="onClick">
        //             Accept
        //         </button>
        //     </div>
        // )html", title);

        // CHIMERA_UNUSED(source);

        // GumboOutput* output = gumbo_parse(source.c_str());

        // auto virtualRoot = createNodes(output->root);

        // append(CreateTree(document, virtualRoot));

        // gumbo_destroy_output(&kGumboDefaultOptions, output);

        append(CreateTree(document,
            V("div", {{"id", "background"}},
                std::vector<VirtualElement>({
                    V("div", {{"id", "title"}}, "Hello World"),
                    V("img", {
                        {"src", "examples/assets/logo.png"},
                        {"id", "image"}
                    }),
                    V("input", {{"id", "input-box"}}),
                    V("input", {
                        {"id", "input-box"},
                        {"type", "password"}
                    }),
                    V("input", {
                        {"id", "input-checkbox"},
                        {"type", "checkbox"}
                    }),
                    V("button", {
                        {"id", "button"},
                        {"onMouseDown", connect("onClick")}
                    }, "Accept")
                }))
        ));
    }

// private:
//     bool loaded{false};
};

#endif  // EXAMPLES_APP_ROOT_H_
