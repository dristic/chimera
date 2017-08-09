// Copyright 2017 Dan Ristic

#include "src/element.h"

namespace Chimera {

class Input: public Element {
 public:
    Input(Document& document);
    ~Input();

    void attributeChangedCallback(
        std::string name, std::string oldValue, std::string newValue) override;
    bool handleEvent(Event* event) override;
    std::string filterText(std::string value);
    void render(DrawData* data) override;

    std::string value;
    std::string placeholder;

 private:
    std::string mType{"text"};
    int mFrames;
    int mSelectionStart{0};
    int mSelectionEnd{0};
};

}  // namespace Chimera
