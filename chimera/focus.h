// Copyright 2017 Dan Ristic

#ifndef CHIMERA_FOCUS_H_
#define CHIMERA_FOCUS_H_

#include "chimera/element.h"

namespace Chimera {

class FocusManager {
 public:
    FocusManager();
    ~FocusManager();

    void focusElement(Element* element);

    Element* focusedElement;
};

}  // namespace Chimera

#endif  // CHIMERA_FOCUS_H_
