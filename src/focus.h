// Copyright 2017 Dan Ristic

#ifndef SRC_FOCUS_H_
#define SRC_FOCUS_H_

#include "src/element.h"

namespace Chimera {

class FocusManager {
 public:
    FocusManager();
    ~FocusManager();

    void focusElement(Element* element);

    Element* focusedElement;
};

}  // namespace Chimera

#endif  // SRC_FOCUS_H_
