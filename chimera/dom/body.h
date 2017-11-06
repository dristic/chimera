// Copyright 2017 Dan Ristic

#ifndef CHIMERA_DOM_BODY_H_
#define CHIMERA_DOM_BODY_H_

#include "chimera/element.h"

namespace Chimera {

class Body: public Element {
 public:
    Body(Document& document);
    ~Body();

    bool handleEvent(Event* event) override;
    void render(DrawData* data) override;
};

}  // namespace Chimera

#endif  // CHIMERA_DOM_BODY_H_