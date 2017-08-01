// Copyright 2017 Dan Ristic

#ifndef SRC_DOM_DIV_H_
#define SRC_DOM_DIV_H_

#include "src/element.h"

namespace Chimera {

class Div: public Element {
 public:
    Div(Document& document);
    ~Div();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // SRC_DOM_DIV_H_