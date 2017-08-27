// Copyright 2017 Dan Ristic

#ifndef SRC_DOM_BUTTON_H_
#define SRC_DOM_BUTTON_H_

#include "src/element.h"

namespace Chimera {

class Button: public Element {
 public:
    explicit Button(Document& document);
    ~Button();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // SRC_DOM_BUTTON_H_