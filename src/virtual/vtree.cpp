// Copyright 2017 Dan Ristic

#include "src/virtual/vtree.h"

namespace Chimera {

namespace Virtual {

VirtualElement::VirtualElement(
    std::string _name,
    std::vector<Attribute> _attributes)
    : name{_name}
{
    for (auto& attribute : _attributes) {
        std::string key = attribute.getKey();
        attributes[key] = attribute;
    }
}

Element* VirtualElement::create(Document& document)
{
    auto element = document.createElement(name);

    if (attributes.count("id") == 1)
    {
        auto id = attributes.at("id");
        element->id = id.asString();
    }

    if (attributes.count("src") == 1)
    {
        auto src = attributes.at("src");
        auto img = dynamic_cast<Img*>(element);
        if (img)
        {
            img->setSrc(src.asString());
        }
    }

    element->textContent = textContent;

    DEBUG(
        printf("[VElement] %s %s\n",
            element->tagName.c_str(),
            element->id.c_str());
    )

    for (auto& child : children)
    {
        element->append(child.create(document));
    }

    return element;
}

}  // namespace Virtual

}  // namespace Chimera
