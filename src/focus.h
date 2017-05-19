// Copyright 2017 Dan Ristic

#ifndef SRC_FOCUS_H_
#define SRC_FOCUS_H_

#include "src/element.h"

namespace Cosmonaut {

class FocusManager {
 public:
    FocusManager();
    ~FocusManager();

    void focusElement(Element* element);

    Element* focusedElement;
};

}  // namespace Cosmonaut

#endif  // SRC_FOCUS_H_
