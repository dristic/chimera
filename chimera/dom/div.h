// Copyright 2017 Dan Ristic

#ifndef CHIMERA_DOM_DIV_H_
#define CHIMERA_DOM_DIV_H_

#include "chimera/element.h"

namespace Chimera {

class Div: public Element {
 public:
    Div(Document& document);
    ~Div();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // CHIMERA_DOM_DIV_H_