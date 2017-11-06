// Copyright 2017 Dan Ristic

#ifndef CHIMERA_DOM_BUTTON_H_
#define CHIMERA_DOM_BUTTON_H_

#include "chimera/element.h"

namespace Chimera {

class Button: public Element {
 public:
    explicit Button(Document& document);
    ~Button();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // CHIMERA_DOM_BUTTON_H_