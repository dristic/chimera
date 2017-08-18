// Copyright 2017 Dan Ristic

#ifndef SRC_DOM_BODY_H_
#define SRC_DOM_BODY_H_

#include "src/element.h"

namespace Chimera {

class Body: public Element {
 public:
    Body(Document& document);
    ~Body();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // SRC_DOM_BODY_H_