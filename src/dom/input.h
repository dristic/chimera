// Copyright 2017 Dan Ristic

#include "src/element.h"

namespace Chimera {

class Input: public Element {
 public:
    Input(Document& document);
    ~Input();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;

    std::string value;
    std::string placeholder;

 private:
    int mFrames;
    int mSelectionStart{0};
    int mSelectionEnd{0};
};

}  // namespace Chimera
